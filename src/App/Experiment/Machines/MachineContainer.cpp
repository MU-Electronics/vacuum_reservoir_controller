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
    {

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


}}}
