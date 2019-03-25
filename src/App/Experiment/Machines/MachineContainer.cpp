#include "MachineContainer.h"


namespace App { namespace Experiment { namespace Machines
{
    MachineContainer::MachineContainer(QObject *parent, Settings::Container* settings, Hardware::Access& hardware)
        :   QObject(parent)
        ,   m_settings(settings)

            // State machines
        ,   m_readPressure(*new ReadPressure(parent, settings, hardware))
        ,   m_readGuageTrip(*new ReadGuageTrip(parent, settings, hardware))
        ,   m_pumpControl(*new PumpControl(parent, settings, hardware))
        ,   m_autoControl(*new AutomaticControl(parent, settings, hardware, m_readGuageTrip, m_pumpControl))
        ,   m_leakDetection(*new LeakDetection(parent, settings, hardware))
        ,   m_valveControl(*new ValveControl(parent, settings, hardware))
    {
        // Connect the finished signals for the machine set pressure emit_pressuriseStopped
        connect(&m_readPressure, &ReadPressure::emit_machineStopping, this, &MachineContainer::emit_vacuumMachineStopping);
        connect(&m_readPressure, &ReadPressure::emit_machineFinished, this, &MachineContainer::emit_vacuumMachineStopped);
        connect(&m_readPressure, &ReadPressure::emit_machineFailed, this, &MachineContainer::emit_vacuumMachineFailed);


        // Read trips
        connect(&m_readGuageTrip, &ReadGuageTrip::emit_machineStopping, this, &MachineContainer::emit_vacuumTripMachineStopping);
        connect(&m_readGuageTrip, &ReadGuageTrip::emit_machineFinished, this, &MachineContainer::emit_vacuumTripMachineStopped);
        connect(&m_readGuageTrip, &ReadGuageTrip::emit_machineFailed, this, &MachineContainer::emit_vacuumTripMachineFailed);
        connect(&m_readGuageTrip, &ReadGuageTrip::emit_guageTripped, this, &MachineContainer::emit_guageTripped);
        connect(&m_readGuageTrip, &ReadGuageTrip::emit_guageTrippedFailed, this, &MachineContainer::emit_guageTrippedFailed);


        // Valve control
        connect(&m_valveControl, &ValveControl::emit_machineStopping, this, &MachineContainer::emit_valveMachineStopping);
        connect(&m_valveControl, &ValveControl::emit_machineFinished, this, &MachineContainer::emit_valveMachineStopped);
        connect(&m_valveControl, &ValveControl::emit_machineFailed, this, &MachineContainer::emit_valveMachineFailed);


        // Leak detection
        connect(&m_leakDetection, &LeakDetection::emit_machineStopping, this, &MachineContainer::emit_leakMachineStopping);
        connect(&m_leakDetection, &LeakDetection::emit_machineFinished, this, &MachineContainer::emit_leakMachineStopped);
        connect(&m_leakDetection, &LeakDetection::emit_machineFailed, this, &MachineContainer::emit_leakMachineFailed);


        // Pump control
        connect(&m_pumpControl, &PumpControl::emit_machineStopping, this, &MachineContainer::emit_pumpMachineStopping);
        connect(&m_pumpControl, &PumpControl::emit_machineFinished, this, &MachineContainer::emit_pumpMachineStopped);
        connect(&m_pumpControl, &PumpControl::emit_machineFailed, this, &MachineContainer::emit_pumpMachineFailed);


        // Auto control
        connect(&m_autoControl, &AutomaticControl::emit_machineStopping, this, &MachineContainer::emit_autoMachineStopping);
        connect(&m_autoControl, &AutomaticControl::emit_machineFinished, this, &MachineContainer::emit_autoMachineStopped);
        connect(&m_autoControl, &AutomaticControl::emit_machineFailed, this, &MachineContainer::emit_autoMachineFailed);

    }

    MachineContainer::~MachineContainer(){}


    /**
     * Make connections to other parts of the application
     *
     * @brief MachineContainer::makeConnections
     */
    void MachineContainer::makeConnections()
    {

    }


    void MachineContainer::startReadingVacuumGuages(QString mode)
    {
        m_readPressure.setParams(mode);
        m_readPressure.start();

        // Emit machine started
        emit emit_vacuumMachineStarted(mode);
    }

    void MachineContainer::stopReadVacuum()
    {
        m_readPressure.cancelStateMachine();
    }





    void MachineContainer::startReadingTripVacuumGuages(QString mode)
    {
        m_readGuageTrip.setParams(mode);
        m_readGuageTrip.start();
    }

    void MachineContainer::stopReadTripVacuum()
    {
        m_readGuageTrip.cancelStateMachine();
    }





    // Pump state machine control
   void MachineContainer::startPumpsControl(int pump, QString mode)
    {
        m_pumpControl.setParams(pump, mode);
        m_pumpControl.start();
    }
    void MachineContainer::stopPumpsControl()
    {
        m_pumpControl.cancelStateMachine();
    }





    // Valve state machine control
    void MachineContainer::startValvesControl(int valveId, bool state)
    {
        m_valveControl.setParams(valveId, state);
        m_valveControl.start();
    }
    void MachineContainer::stopVavlesControl()
    {
        m_valveControl.cancelStateMachine();
    }





    // Leak state machine control
    void MachineContainer::startLeakDetection(int group, int period, int fall, int sample)
    {
        m_leakDetection.setParams(group, period, fall, sample);
        m_leakDetection.start();
    }
    void MachineContainer::stopLeakDetection()
    {
        m_leakDetection.cancelStateMachine();
    }





    // Auto control state machine control
    void MachineContainer::startAutoControl()
    {
        m_autoControl.setParams();
        m_autoControl.start();
    }
    void MachineContainer::stopAutoControl()
    {
        m_autoControl.cancelStateMachine();
    }


}}}
