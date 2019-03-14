#include "Engine.h"

// Include extenral deps
#include <QObject>

// Include settings container
#include "../Settings/Container.h"


namespace App { namespace Experiment
{
    Engine::Engine(QObject *parent, Settings::Container* settings, Hardware::Access &hardware)
        :   QObject(parent)
        ,   m_settings(settings)
        ,   m_machines(*new Machines::MachineContainer(parent, settings, hardware))
    {

    }

    Engine::~Engine(){}


    /**
     * Make connections to other parts of the application
     *
     * @brief Engine::makeConnections
     */
    void Engine::makeConnections()
    {

    }


}}
