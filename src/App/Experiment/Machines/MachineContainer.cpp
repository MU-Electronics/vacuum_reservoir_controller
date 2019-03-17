#include "MachineContainer.h"

// Include extenral deps
#include <QObject>

// Include settings container
#include "../../Settings/Container.h"


namespace App { namespace Experiment { namespace Machines
{
    MachineContainer::MachineContainer(QObject *parent, Settings::Container* settings, Hardware::Access& hardware)
        :   QObject(parent)
        ,   m_settings(settings)

            // State machines
        ,   m_readPressure(*new ReadPressure(parent, settings, hardware))
    {
        // Connect the finished signals for the machine set pressure emit_pressuriseStopped
        connect(&m_readPressure, &ReadPressure::emit_machineStopping, this, &MachineContainer::emit_vacuumMachineStopping);
        connect(&m_readPressure, &ReadPressure::emit_machineFinished, this, &MachineContainer::emit_vacuumMachineStopped);
        connect(&m_readPressure, &ReadPressure::emit_machineFailed, this, &MachineContainer::emit_vacuumMachineFailed);

        //connect(m_StartButton, SIGNAL(clicked()), this, SIGNAL(sig_StartClik()));

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


}}}
