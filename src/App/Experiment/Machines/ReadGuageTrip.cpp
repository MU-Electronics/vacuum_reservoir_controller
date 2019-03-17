#include "ReadGuageTrip.h"

// Include extenral deps
#include <QObject>


namespace App { namespace Experiment { namespace Machines
{
    ReadGuageTrip::ReadGuageTrip(QObject *parent, Settings::Container* settings, Hardware::Access& hardware)
        :   Helpers::MachineStates(parent, settings, hardware)

            // Settings container
        ,   m_settings(settings)

            // Timers
        ,   t_tripMonitor(*new QTimer(parent))
    {
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        // Init
        m_trips[1] = false; m_trips[2] = false; m_trips[3] = false; m_trips[4] = false; m_trips[5] = false;
        m_trips[6] = false; m_trips[7] = false; m_trips[8] = false;

        // Update params when settings change @TODO

        // Connect states to functions
        connect(state("startTripMonitor", true), &QState::entered, this, &ReadGuageTrip::startTripMonitor);

        connect(state("whereTo", true), &QState::entered, this, &ReadGuageTrip::nextGuage);

        connect(validator("emitTripped", true), &QState::entered, this, &ReadGuageTrip::emitTripped);

        connect(state("guage_1", true), &QState::entered, this->guages(), &Functions::GuageFunctions::isTripedGroup1);
        connect(state("guage_2", true), &QState::entered, this->guages(), &Functions::GuageFunctions::isTripedGroup2);
        connect(state("guage_3", true), &QState::entered, this->guages(), &Functions::GuageFunctions::isTripedGroup3);
        connect(state("guage_4", true), &QState::entered, this->guages(), &Functions::GuageFunctions::isTripedGroup4);
        connect(state("guage_5", true), &QState::entered, this->guages(), &Functions::GuageFunctions::isTripedGroup5);
        connect(state("guage_6", true), &QState::entered, this->guages(), &Functions::GuageFunctions::isTripedGroup6);
        connect(state("guage_7", true), &QState::entered, this->guages(), &Functions::GuageFunctions::isTripedGroup7);
        connect(state("guage_8", true), &QState::entered, this->guages(), &Functions::GuageFunctions::isTripedGroup8);

        // Connect validators to functions
        connect(validator("guage_1", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateIsTripedGroup1);
        connect(validator("guage_2", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateIsTripedGroup2);
        connect(validator("guage_3", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateIsTripedGroup3);
        connect(validator("guage_4", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateIsTripedGroup4);
        connect(validator("guage_5", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateIsTripedGroup5);
        connect(validator("guage_6", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateIsTripedGroup6);
        connect(validator("guage_7", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateIsTripedGroup7);
        connect(validator("guage_8", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateIsTripedGroup8);
    }

    ReadGuageTrip::~ReadGuageTrip()
    {

    }


    /**
     * Set the commands to be used by the machine
     *
     * @brief setParams
     * @param mintues
     * @param turbo
     * @param gasMode
     */
    void ReadGuageTrip::setParams(QString mode)
    {       
        // Time interval for pressure sensor
        params.insert("tripTimeInter", m_settings->hardware()->guages()["trip_poll"].toInt());
        t_tripMonitor.setInterval(params["tripTimeInter"].toInt());
        t_tripMonitor.setTimerType(Qt::PreciseTimer);

        // Set which gauge to read
        params.insert("guage_1", m_settings->general()->chamber(1)[mode].toBool());
        params.insert("guage_2", m_settings->general()->chamber(2)[mode].toBool());
        params.insert("guage_3", m_settings->general()->chamber(3)[mode].toBool());
        params.insert("guage_4", m_settings->general()->chamber(4)[mode].toBool());
        params.insert("guage_5", m_settings->general()->chamber(5)[mode].toBool());
        params.insert("guage_6", m_settings->general()->chamber(6)[mode].toBool());
        params.insert("guage_7", m_settings->general()->pump(1)[mode].toBool());
        params.insert("guage_8", m_settings->general()->pump(2)[mode].toBool());

        // Set intial guage id
        m_guageId = 1;

        // Set mode
        m_mode = mode;
    }


    /**
     * Start the state machine
     *
     * @brief ReadPressure::start
     */
    void ReadGuageTrip::beforeStart()
    {

    }


    /**
     * Start the state machine
     *
     * @brief ReadPressure::stopped
     */
    void ReadGuageTrip::stopped()
    {
    }


    /**
     * Builds the machine connections
     *
     * @brief ReadPressure::buildMachine
     */
    void ReadGuageTrip::buildMachine()
    {
        // Where to start the machine
        sm_master.setInitialState(state("startTripMonitor", true));

        // Start the pressure monitor
        state("startTripMonitor", true)->addTransition(&t_tripMonitor, &QTimer::timeout, state("whereTo", true));

        state("whereTo", true)->addTransition(this, &ReadGuageTrip::emit_goTo_1, state("guage_1", true));
        state("whereTo", true)->addTransition(this, &ReadGuageTrip::emit_goTo_2, state("guage_2", true));
        state("whereTo", true)->addTransition(this, &ReadGuageTrip::emit_goTo_3, state("guage_3", true));
        state("whereTo", true)->addTransition(this, &ReadGuageTrip::emit_goTo_4, state("guage_4", true));
        state("whereTo", true)->addTransition(this, &ReadGuageTrip::emit_goTo_5, state("guage_5", true));
        state("whereTo", true)->addTransition(this, &ReadGuageTrip::emit_goTo_6, state("guage_6", true));
        state("whereTo", true)->addTransition(this, &ReadGuageTrip::emit_goTo_7, state("guage_7", true));
        state("whereTo", true)->addTransition(this, &ReadGuageTrip::emit_goTo_8, state("guage_8", true));


        // Read the pressure sensor
        state("guage_1", true)->addTransition(&m_hardware, &Hardware::Access::emit_guageReadTrip, validator("guage_1", true));
            // Validate
            validator("guage_1", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationFailed, state("startTripMonitor", true));
            validator("guage_1", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationSuccess, validator("emitTripped", true));

        state("guage_2", true)->addTransition(&m_hardware, &Hardware::Access::emit_guageReadTrip, validator("guage_2", true));
            // Validate
            validator("guage_2", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationFailed, state("startTripMonitor", true));
            validator("guage_2", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationSuccess, validator("emitTripped", true));

        state("guage_3", true)->addTransition(&m_hardware, &Hardware::Access::emit_guageReadTrip, validator("guage_3", true));
            // Validate
            validator("guage_3", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationFailed, state("startTripMonitor", true));
            validator("guage_3", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationSuccess, validator("emitTripped", true));

        state("guage_4", true)->addTransition(&m_hardware, &Hardware::Access::emit_guageReadTrip, validator("guage_4", true));
            // Validate
            validator("guage_4", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationFailed, state("startTripMonitor", true));
            validator("guage_4", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationSuccess, validator("emitTripped", true));

        state("guage_5", true)->addTransition(&m_hardware, &Hardware::Access::emit_guageReadTrip, validator("guage_5", true));
            // Validate
            validator("guage_5", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationFailed, state("startTripMonitor", true));
            validator("guage_5", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationSuccess, validator("emitTripped", true));

        state("guage_6", true)->addTransition(&m_hardware, &Hardware::Access::emit_guageReadTrip, validator("guage_6", true));
            // Validate
            validator("guage_6", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationFailed, state("startTripMonitor", true));
            validator("guage_6", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationSuccess, validator("emitTripped", true));

        state("guage_7", true)->addTransition(&m_hardware, &Hardware::Access::emit_guageReadTrip, validator("guage_7", true));
            // Validate
            validator("guage_7", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationFailed, state("startTripMonitor", true));
            validator("guage_7", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationSuccess, validator("emitTripped", true));

        state("guage_8", true)->addTransition(&m_hardware, &Hardware::Access::emit_guageReadTrip, validator("guage_8", true));
            // Validate
            validator("guage_8", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationFailed, state("startTripMonitor", true));
            validator("guage_8", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationSuccess, validator("emitTripped", true));


        validator("emitTripped", true)->addTransition(this, &ReadGuageTrip::emit_guageTripped, state("startTripMonitor", true));
        validator("emitTripped", true)->addTransition(this, &ReadGuageTrip::emit_guageTrippedFailed, state("startTripMonitor", true));
    }


    void ReadGuageTrip::emitTripped()
    {
        // Get the validator state instance
        Helpers::CommandValidatorState* command = dynamic_cast<Helpers::CommandValidatorState*>(sender());

        // If cast successfull
        if(command != nullptr && command->package.value("cast_status").toBool())
        {
            // Get the package data from the instance
            QVariantMap package = command->package;

            // Guage id
            int guage = package["requested_gauge_group"].toInt();

            // Guage state
            bool state = package["status"].toBool();

            // Only update and alert if value has changed
            if(m_trips[guage] != state)
            {
                // Save state of guage
                m_trips[guage] = state;

                // Guage tripped emit
                emit emit_guageTripped(guage, state);
            }
        }

        emit emit_guageTrippedFailed();
    }


    void ReadGuageTrip::nextGuage()
    {
        for(int a = m_guageId; a <= 8; a++)
        {
            if(params["guage_" + QString::number(a)].toBool())
            {
                m_guageId = a;
                break;
            }
        }

        // Save selected then increment current
        int selected = m_guageId;
        m_guageId++;

        if(selected == 9)
        {
            m_guageId = 1;
            nextGuage();
        }

        // Return the state to transition to
        switch(selected)
        {
            case 1:
                emit emit_goTo_1();
            break;
            case 2:
                emit emit_goTo_2();
            break;
            case 3:
                emit emit_goTo_3();
            break;
            case 4:
                emit emit_goTo_4();
            break;
            case 5:
                emit emit_goTo_5();
            break;
            case 6:
                emit emit_goTo_6();
            break;
            case 7:
                emit emit_goTo_7();
            break;
            default: // 8
                emit emit_goTo_8();
            break;
        }
    }


    /**
     * Timer to use to trigger reading the pressure sensor
     *
     * @brief ReadPressure::startPressureMonitor
     */
    void ReadGuageTrip::startTripMonitor()
    {
        // Setup timer
        t_tripMonitor.setSingleShot(true);
        t_tripMonitor.start();
    }
}}}






