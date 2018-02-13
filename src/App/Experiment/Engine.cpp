#include "Engine.h"

// Include extenral deps
#include <QObject>

// Include settings container
#include "../Settings/Container.h"


namespace App { namespace Experiment
{
    Engine::Engine(QObject *parent, Settings::Container settings)
        :   QObject(parent)
        ,   m_settings(settings)
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
