#include "ReadPressure.h"

// Include extenral deps
#include <QObject>


namespace App { namespace Experiment { namespace Machines
{
    ReadPressure::ReadPressure(QObject *parent, Settings::Container* settings, Hardware::Access& hardware)
        :   Helpers::MachineStates(parent, settings, hardware)

            // Settings container
        ,   m_settings(settings)

            // Timers
        ,   t_pressureMonitor(*new QTimer(parent))
    {
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        // Update params when settings change @TODO

        // Connect states to functions
        connect(state("startPressureMonitor", true), &QState::entered, this, &ReadPressure::startPressureMonitor);

        connect(state("whereTo", true), &QState::entered, this, &ReadPressure::nextGuage);

        connect(state("guage_1", true), &QState::entered, this->guages(), &Functions::GuageFunctions::readGroup1);
        connect(state("guage_2", true), &QState::entered, this->guages(), &Functions::GuageFunctions::readGroup2);
        connect(state("guage_3", true), &QState::entered, this->guages(), &Functions::GuageFunctions::readGroup3);
        connect(state("guage_4", true), &QState::entered, this->guages(), &Functions::GuageFunctions::readGroup4);
        connect(state("guage_5", true), &QState::entered, this->guages(), &Functions::GuageFunctions::readGroup5);
        connect(state("guage_6", true), &QState::entered, this->guages(), &Functions::GuageFunctions::readGroup6);
        connect(state("guage_7", true), &QState::entered, this->guages(), &Functions::GuageFunctions::readGroup7);
        connect(state("guage_8", true), &QState::entered, this->guages(), &Functions::GuageFunctions::readGroup8);

        // Connect validators to functions
        connect(validator("guage_1", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateReadGroup1);
        connect(validator("guage_2", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateReadGroup2);
        connect(validator("guage_3", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateReadGroup3);
        connect(validator("guage_4", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateReadGroup4);
        connect(validator("guage_5", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateReadGroup5);
        connect(validator("guage_6", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateReadGroup6);
        connect(validator("guage_7", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateReadGroup7);
        connect(validator("guage_8", true), &QState::entered, this->guages(), &Functions::GuageFunctions::validateReadGroup8);
    }

    ReadPressure::~ReadPressure()
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
    void ReadPressure::setParams(QString mode)
    {       
        // Time interval for pressure sensor
        params.insert("pressureSensorTimeInter", m_settings->hardware()->guages()["poll"].toInt());
        t_pressureMonitor.setInterval(params["pressureSensorTimeInter"].toInt());
        t_pressureMonitor.setTimerType(Qt::PreciseTimer);

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
    void ReadPressure::beforeStart()
    {

    }


    /**
     * Start the state machine
     *
     * @brief ReadPressure::stopped
     */
    void ReadPressure::stopped()
    {
    }


    /**
     * Builds the machine connections
     *
     * @brief ReadPressure::buildMachine
     */
    void ReadPressure::buildMachine()
    {
        // Where to start the machine
        sm_master.setInitialState(state("startPressureMonitor", true));

        // Start the pressure monitor
        state("startPressureMonitor", true)->addTransition(&t_pressureMonitor, &QTimer::timeout, state("whereTo", true));

        state("whereTo", true)->addTransition(this, &ReadPressure::emit_goTo_1, state("guage_1", true));
        state("whereTo", true)->addTransition(this, &ReadPressure::emit_goTo_2, state("guage_2", true));
        state("whereTo", true)->addTransition(this, &ReadPressure::emit_goTo_3, state("guage_3", true));
        state("whereTo", true)->addTransition(this, &ReadPressure::emit_goTo_4, state("guage_4", true));
        state("whereTo", true)->addTransition(this, &ReadPressure::emit_goTo_5, state("guage_5", true));
        state("whereTo", true)->addTransition(this, &ReadPressure::emit_goTo_6, state("guage_6", true));
        state("whereTo", true)->addTransition(this, &ReadPressure::emit_goTo_7, state("guage_7", true));
        state("whereTo", true)->addTransition(this, &ReadPressure::emit_goTo_8, state("guage_8", true));


        // Read the pressure sensor
        state("guage_1", true)->addTransition(&m_hardware, &Hardware::Access::emit_guageReadVacuum, validator("guage_1", true));
            // Validate
            validator("guage_1", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationFailed, state("startPressureMonitor", true));
            validator("guage_1", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationSuccess, state("startPressureMonitor", true));
            //validator("guage_1", true)->addTransition(&t_pressureMonitor, &QTimer::timeout, state("startPressureMonitor", true));

        state("guage_2", true)->addTransition(&m_hardware, &Hardware::Access::emit_guageReadVacuum, validator("guage_2", true));
            // Validate
            validator("guage_2", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationFailed, state("startPressureMonitor", true));
            validator("guage_2", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationSuccess, state("startPressureMonitor", true));
            //validator("guage_2", true)->addTransition(&t_pressureMonitor, &QTimer::timeout, state("startPressureMonitor", true));

        state("guage_3", true)->addTransition(&m_hardware, &Hardware::Access::emit_guageReadVacuum, validator("guage_3", true));
            // Validate
            validator("guage_3", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationFailed, state("startPressureMonitor", true));
            validator("guage_3", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationSuccess, state("startPressureMonitor", true));
            //validator("guage_3", true)->addTransition(&t_pressureMonitor, &QTimer::timeout, state("startPressureMonitor", true));

        state("guage_4", true)->addTransition(&m_hardware, &Hardware::Access::emit_guageReadVacuum, validator("guage_4", true));
            // Validate
            validator("guage_4", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationFailed, state("startPressureMonitor", true));
            validator("guage_4", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationSuccess, state("startPressureMonitor", true));
            //validator("guage_4", true)->addTransition(&t_pressureMonitor, &QTimer::timeout, state("startPressureMonitor", true));


        state("guage_5", true)->addTransition(&m_hardware, &Hardware::Access::emit_guageReadVacuum, validator("guage_5", true));
            // Validate
            validator("guage_5", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationFailed, state("startPressureMonitor", true));
            validator("guage_5", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationSuccess, state("startPressureMonitor", true));
            //validator("guage_5", true)->addTransition(&t_pressureMonitor, &QTimer::timeout, state("startPressureMonitor", true));


        state("guage_6", true)->addTransition(&m_hardware, &Hardware::Access::emit_guageReadVacuum, validator("guage_6", true));
            // Validate
            validator("guage_6", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationFailed, state("startPressureMonitor", true));
            validator("guage_6", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationSuccess, state("startPressureMonitor", true));
            //validator("guage_6", true)->addTransition(&t_pressureMonitor, &QTimer::timeout, state("startPressureMonitor", true));


        state("guage_7", true)->addTransition(&m_hardware, &Hardware::Access::emit_guageReadVacuum, validator("guage_7", true));
            // Validate
            validator("guage_7", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationFailed, state("startPressureMonitor", true));
            validator("guage_7", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationSuccess, state("startPressureMonitor", true));
            //validator("guage_7", true)->addTransition(&t_pressureMonitor, &QTimer::timeout, state("startPressureMonitor", true));


        state("guage_8", true)->addTransition(&m_hardware, &Hardware::Access::emit_guageReadVacuum, validator("guage_8", true));
            // Validate
            validator("guage_8", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationFailed, state("startPressureMonitor", true));
            validator("guage_8", true)->addTransition(this->guages(), &Functions::GuageFunctions::emit_validationSuccess, state("startPressureMonitor", true));
            //validator("guage_8", true)->addTransition(&t_pressureMonitor, &QTimer::timeout, state("startPressureMonitor", true));



        // Account for com issues
        /*transitionsBuilder()->stateComErrors(state("systemPressure", true), state("startPressureMonitor", true));
        transitionsBuilder()->stateComErrors(validator("systemPressure", true), state("startPressureMonitor", true));*/



    }


    void ReadPressure::nextGuage()
    {
        // qDebug() << "Starting from: " << m_guageId;
        for(int a = m_guageId; a <= 8; a++)
        {
            // qDebug() << a << " = " << params["guage_" + QString::number(a)].toBool();
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
    void ReadPressure::startPressureMonitor()
    {
        // Setup timer
        t_pressureMonitor.setSingleShot(true);
        t_pressureMonitor.start();
    }
}}}






