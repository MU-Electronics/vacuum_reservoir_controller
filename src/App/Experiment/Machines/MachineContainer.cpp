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
        connect(&m_readPressure, &ReadPressure::emit_machineStopping, this, &MachineContainer::readVacuumStopping);
        connect(&m_readPressure, &ReadPressure::emit_machineFinished, this, &MachineContainer::readVacuumFinished);
        connect(&m_readPressure, &ReadPressure::emit_machineFailed, this, &MachineContainer::readVacuumFailed);
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


    void MachineContainer::startReadingVacuumGuages()
    {
        m_readPressure.setParams();
        m_readPressure.start();
        // emit
    }

    void MachineContainer::readVacuumStopping()
    {

    }

    void MachineContainer::readVacuumFinished()
    {

    }

    void MachineContainer::readVacuumFailed()
    {

    }

}}}
