#include "LeakDetection.h"

// Include extenral deps
#include <QObject>


namespace App { namespace Experiment { namespace Machines
{
    LeakDetection::LeakDetection(QObject *parent, Settings::Container* settings, Hardware::Access& hardware)
        :   Helpers::MachineStates(parent, settings, hardware)

            // Settings container
        ,   m_settings(settings)

            // Timers
        ,   t_leakPeriod(*new QTimer(parent))
    {
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        // Update params when settings change @TODO

        connect(state("startPressureMonitor", true), &QState::entered, this, &LeakDetection::startLeakPeriod);

        connect(validator("checkSamplePressure", true), &QState::entered, this, &LeakDetection::checkPressure);

        shutDownMachines = false;
    }

    LeakDetection::~LeakDetection()
    {

    }


    /**
     * Set the commands to be used by the machine
     *
     */
    void LeakDetection::setParams(int group, int period, int fall, int sample)
    {               
        // Group to monitor for leak
        params.insert("group", group);

        // Period of monitoring
        params.insert("period", period);

        // Maxium pressure fall over period
        params.insert("fall", fall);

        // Number of times to sample over set period
        params.insert("sample", sample);

        // Set timer for pressure monitoring
        t_leakPeriod.setInterval(int(period / sample));

        // Number of time pressure has been checked
        m_count = 0;

        // Set default pressure
        m_pressure = -1;
    }


    /**
     * Start the state machine
     *7leakDetecter
     * @brief LeakDetection::start
     */
    void LeakDetection::beforeStart()
    {

    }


    /**
     * Start the state machine
     *
     * @brief LeakDetection::stopped
     */
    void LeakDetection::stopped()
    {
    }




    /**
     * Builds the machine connections7
     *
     * @brief LeakDetection::buildMachine
     */
    void LeakDetection::buildMachine()
    {
        // Where to start the machine
        sm_master.setInitialState(state("startPressureMonitor", true));

        // Timer for checking pressure on timeout OR emit_samplesReached final decision
        state("startPressureMonitor", true)->addTransition(&t_leakPeriod, &QTimer::timeout, state("takePressureReading", true));


        // Wait for pressure reading
        state("takePressureReading", true)->addTransition(&m_hardware, &Hardware::Access::emit_guageReadVacuum, validator("checkSamplePressure", true));

        // Check total pressure fall from start
        validator("checkSamplePressure", true)->addTransition(this, &LeakDetection::emit_wrongGuage, state("takePressureReading", true));
        validator("checkSamplePressure", true)->addTransition(this, &LeakDetection::emit_leakCastingError, &sm_stopAsFailed);
        validator("checkSamplePressure", true)->addTransition(this, &LeakDetection::emit_leakDetected, &sm_stopAsFailed);
        validator("checkSamplePressure", true)->addTransition(this, &LeakDetection::emit_samplesReachedNoLeak, &sm_stop);
        validator("checkSamplePressure", true)->addTransition(this, &LeakDetection::emit_continue, state("startPressureMonitor", true));
   }




    void LeakDetection::checkPressure()
    {
        // Get the validator state instance
        Helpers::CommandValidatorState* command = dynamic_cast<Helpers::CommandValidatorState*>(sender());

        // If cast successfull
        if(command != nullptr && command->package.value("cast_status").toBool())
        {
            // Get the package data from the instance
            QVariantMap package = command->package;

            //qDebug() << "Checking pressure:" << package << " at fall:" << params["fall"].toDouble();

            // Guage id
            int guageId = package["guage_id"].toInt();

            // Guage state
            double pressure = package["pressure_mbar"].toDouble();

            // Check correct valve
            if(guageId != params["group"].toInt())
            {
                // qDebug() << "wrong valve:" << guageId << "wanted:"<<params["group"].toInt();
                emit emit_wrongGuage();
                return;
            }

            // First sample save pressure for later comparisions
            if(m_count == 1)
            {
                //qDebug() << "Setting m_pressure to:"<<pressure;
                m_pressure = pressure;
            }

            // Only update and alert if value has changed
            //qDebug() << "Checking for pressure drop:"<<(pressure - m_pressure)<<" is less than"<<params["fall"].toDouble();
            if((pressure - m_pressure) >= params["fall"].toDouble() && params["sample"].toInt() >= m_count)
            {
                // Guage tripped emit
                emit emit_leakDetected();
                //qDebug() << "Leak detected";
                return;
            }

            // Has we reached the end of sampling 4 < 1
            if(params["sample"].toInt() < m_count)
            {
                //qDebug() << "sample:" << params["sample"].toInt() << "count:" << m_count;
                // No leak detected
                emit emit_samplesReachedNoLeak();
                //qDebug() << "samples reached no leak detected";
                return;
            }

            // No leak detected
            emit emit_continue();
            //qDebug() << "continue";
            return;
        }

        // Some error occured
        //qDebug() << "Casting error";
        emit emit_leakCastingError();
    }



    /**
     * Timer to use to trigger pump warmup
     *
     * @brief LeakDetection::startWarmup
     */
    void LeakDetection::startLeakPeriod()
    {
        // Increment count
        m_count++;

        //qDebug() << "leaker timer started:" << t_leakPeriod.interval() << " At count:" << m_count;

        // Start timer for a single shot
        t_leakPeriod.setSingleShot(true);
        t_leakPeriod.start();
    }
}}}






