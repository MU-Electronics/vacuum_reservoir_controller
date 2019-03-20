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
        t_leakPeriod.setInterval(period / sample);

        // Connect correct guage function
        switch(params["group"].toInt())
        {
            case 1:
                connect(state("takePressureReading", true), &QState::entered, this->guages(), &Functions::GuageFunctions::readGroup1);
                connect(validator("validateTakePressureReading", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateReadGroup1);
            break;
            case 2:
                connect(state("takePressureReading", true), &QState::entered, this->guages(), &Functions::GuageFunctions::readGroup2);
                connect(validator("validateTakePressureReading", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateReadGroup2);
            break;
            case 3:
                connect(state("takePressureReading", true), &QState::entered, this->guages(), &Functions::GuageFunctions::readGroup3);
                connect(validator("validateTakePressureReading", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateReadGroup3);
            break;
            case 4:
                connect(state("takePressureReading", true), &QState::entered, this->guages(), &Functions::GuageFunctions::readGroup4);
                connect(validator("validateTakePressureReading", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateReadGroup4);
            break;
            case 5:
                connect(state("takePressureReading", true), &QState::entered, this->guages(), &Functions::GuageFunctions::readGroup5);
                connect(validator("validateTakePressureReading", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateReadGroup5);
            break;
            case 6:
                connect(state("takePressureReading", true), &QState::entered, this->guages(), &Functions::GuageFunctions::readGroup6);
                connect(validator("validateTakePressureReading", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateReadGroup6);
            break;
            case 7:
                connect(state("takePressureReading", true), &QState::entered, this->guages(), &Functions::GuageFunctions::readGroup7);
                connect(validator("validateTakePressureReading", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateReadGroup7);
            break;
            case 8:
                connect(state("takePressureReading", true), &QState::entered, this->guages(), &Functions::GuageFunctions::readGroup8);
                connect(validator("validateTakePressureReading", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateReadGroup8);
            break;
        }

        // Number of time pressure has been checked
        m_count = 0;

        // Set default pressure
        m_pressure = -1;
    }


    /**
     * Start the state machine
     *7
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
        sm_master.setInitialState(state("timer", true));

        // Timer for checking pressure on timeout OR emit_samplesReached final decision
        state("startPressureMonitor", true)->addTransition(&t_leakPeriod, &QTimer::timeout, state("takePressureReading", true));


        // Take pressure reading form group
        state("takePressureReading", true)->addTransition(&m_hardware, &Hardware::Access::emit_guageReadVacuum, validator("validateTakePressureReading", true));
            validator("validateTakePressureReading", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationPressureReading, validator("checkSamplePressure", true));
            validator("validateTakePressureReading", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationWrongId, validator("takePressureReading", true));
            validator("validateTakePressureReading", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationFailed, &sm_stopAsFailed);


        // Check total pressure fall from start
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

            // Guage id
            int guageId = package["requested_gauge_group"].toInt();

            // Guage state
            double pressure = package["pressure"].toDouble();

            // First sample save pressure for later comparisions
            if(m_count == 1)
                m_pressure = pressure;

            // Only update and alert if value has changed
            if(abs(pressure - m_pressure) >= params["fall"].toDouble() && params["samples"].toInt() < m_count)
            {
                // Guage tripped emit
                emit emit_leakDetected();
            }

            // Has we reached the end of sampling
            if(params["samples"].toInt() > m_count)
            {
                // No leak detected
                emit emit_samplesReachedNoLeak();
                return;
            }

            // No leak detected
            emit emit_continue();
            return;
        }

        // Some error occured
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

        // Start timer for a single shot
        t_leakPeriod.setSingleShot(true);
        t_leakPeriod.start();
    }
}}}






