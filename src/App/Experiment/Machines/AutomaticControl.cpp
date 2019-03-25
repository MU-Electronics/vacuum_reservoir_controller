#include "AutomaticControl.h"

// Include extenral deps
#include <QObject>


namespace App { namespace Experiment { namespace Machines
{
    AutomaticControl::AutomaticControl(QObject *parent, Settings::Container* settings, Hardware::Access& hardware,
                                       ReadGuageTrip& trips, PumpControl& pumpController)
        :   Helpers::MachineStates(parent, settings, hardware)

            // Settings container
        ,   m_settings(settings)
        ,   m_pumpController(pumpController)
    {
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        // Record when trips are triggered
        connect(&trips, &ReadGuageTrip::emit_guageTripped, this, &AutomaticControl::guageTripped);

        // State machine connections
        connect(state("selectPump", true), &QState::entered, this, &AutomaticControl::selectPump);

        connect(state("requestPump1", true), &QState::entered, this, &AutomaticControl::requestPump1);
        connect(state("requestPump2", true), &QState::entered, this, &AutomaticControl::requestPump2);

        connect(state("markPumpFailure", true), &QState::entered, this, &AutomaticControl::markPumpFailure);

        connect(state("selectBarrel", true), &QState::entered, this, &AutomaticControl::selectBarrel);

    }

    AutomaticControl::~AutomaticControl()
    {

    }


    /**
     * Set the commands to be used by the machine
     *
     */
    void AutomaticControl::setParams()
    {       
        // Set disabled valves
        for(int i : {1,2,3,4,5,6})
        {
            if(!m_settings->general()->chamber(i)["auto_control_enabled"].toBool())
                m_disabled.append(i);
        }

        // Set pump states
        m_disabledPump1 = (m_settings->general()->pump(1)["auto_control_enabled"].toBool()) ? 0 : 1;
        m_disabledPump2 = (m_settings->general()->pump(2)["auto_control_enabled"].toBool()) ? 0 : 1;
    }


    /**
     * Start the state machine
     *
     * @brief AutomaticControl::start
     */
    void AutomaticControl::beforeStart()
    {

    }


    /**
     * Start the state machine
     *
     * @brief AutomaticControl::stopped
     */
    void AutomaticControl::stopped()
    {
    }


    /**
     * Builds the machine connections
     *
     * @brief AutomaticControl::buildMachine
     */
    void AutomaticControl::buildMachine()
    {
        // Where to start the machine
        sm_master.setInitialState(state("selectPump", true));

        // Which pump should be turn on
        state("selectPump", true)->addTransition(this, &AutomaticControl::emit_usePump1, state("requestPump1", true));
        state("selectPump", true)->addTransition(this, &AutomaticControl::emit_usePump2, state("requestPump2", true));
        state("selectPump", true)->addTransition(this, &AutomaticControl::emit_noPumpsAvailable, &sm_stopAsFailed);

        state("requestPump1", true)->addTransition(&m_pumpController, &PumpControl::emit_machineFinished, state("selectBarrel", true));
        state("requestPump1", true)->addTransition(&m_pumpController, &PumpControl::emit_machineFailed, state("markPumpFailure", true));

        state("requestPump2", true)->addTransition(&m_pumpController, &PumpControl::emit_machineFinished, state("selectBarrel", true));
        state("requestPump2", true)->addTransition(&m_pumpController, &PumpControl::emit_machineFailed, state("markPumpFailure", true));

        state("markPumpFailure", true)->addTransition(this, &AutomaticControl::emit_pumpMarkedAsFailed, state("selectPump", true));


        // Select barrel
        // state("selectBarrel", true);
    }




    void AutomaticControl::selectBarrel()
    {
        qDebug() << "Selecting barrels";
    }




    /**
     * Selects which pump should be used
     *
     * @brief AutomaticControl::selectPump
     */
    void AutomaticControl::selectPump()
    {
        // Switch current pump
        int selectedPump = (m_currentPump == 1) ? 2 : 1;

        // Make sure pump allowed
        if(selectedPump == 1 && m_disabledPump1 == 0)
        {
            // Mark pump 1 as on
            m_currentPump = 1;

            // Emit pump 1 to be used
            emit emit_usePump1();

            // No more work here
            return;
        }
        else if(selectedPump == 2 && m_disabledPump2 == 0)
        {
            // Mark pump 2 as on
            m_currentPump = 2;

            // Emit pump 2 to be used
            emit emit_usePump2();

            // No more work here
            return;
        }

        // No pumps avaliable
        emit emit_noPumpsAvailable();
    }


    /**
     * Request pump 1 to be turned on
     *
     * @brief AutomaticControl::requestPump1
     */
    void AutomaticControl::requestPump1()
    {
        m_pumpController.setParams(1);
        m_pumpController.start();
    }


    /**
     * Request pump 2 to be turned on
     *
     * @brief AutomaticControl::requestPump2
     */
    void AutomaticControl::requestPump2()
    {
        m_pumpController.setParams(2);
        m_pumpController.start();
    }



    void AutomaticControl::markPumpFailure()
    {
        // Mark pump as leak detected
        if(m_currentPump == 1)
        {
            m_disabledPump1 = 2;
            emit emit_pumpMarkedAsFailed(1);
        }
        else if(m_currentPump == 2)
        {
            m_disabledPump2 = 2;
            emit emit_pumpMarkedAsFailed(1);
        }
    }




    /**
     * Keep track of tripped valves
     *
     * @brief AutomaticControl::guageTripped
     * @param group
     * @param state
     */
    void AutomaticControl::guageTripped(int group, bool state)
    {
        int i = m_tripped.indexOf(group);

        if(i == -1)
        {
            if(state)
                m_tripped.append(group);
        }
        else
        {
            if(!state)
                m_tripped.removeAt(i);
        }
    }



    /**
     * Keep track of pressures
     *
     * @brief AutomaticControl::guagePressures
     * @param package
     */
    void AutomaticControl::guagePressures(QVariantMap package)
    {
        int group = package.value("guage_id").toInt();
        double value = package.value("pressure_mbar").toDouble();

        m_pressures.insert(group, value);
    }



}}}






