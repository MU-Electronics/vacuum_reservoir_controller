#include "Application.h"

// External libs
#include <QQmlApplicationEngine>
#include <QObject>
#include <QQmlContext>
#include <QQmlEngine>
#include <QThread>
#include <QString>

// Include the setting contain
#include "Settings/Container.h"

// Include the expeirment engine
#include "Experiment/Engine.h"

// For debugging only to be removed


namespace App
{
    /**
     * Class constructure performs:
     *      - Set QObject
     *      - Sets the engine member variable
     *      - Create instant of the settings container to be past to other objects
     *      - Registers threads
     *      - Starts threads
     *      - Registers the main view manager which then registers sub managers
     *      - Connects the threads signals and slots together
     *      - For development only runs a debugging methods (to be removed before first release)
     *
     * @brief Application::Application
     * @param parent
     * @param engine
     */
    Application::Application(QObject* parent, QQmlApplicationEngine* engine)
        :   QObject(parent)
        ,   m_engine(engine)

            // Create instance of the settings container
        ,   settings_container(*new Settings::Container)

            // Create instances that are to be threaded

            // Include the expeirment engine
        ,    experiment_engine(*new Experiment::Engine(this, settings_container))

            // View manager factory
        ,   manager_factory(*new View::ManagerFactory())
    {
        // Register qml types with qml
        registerQmlTypes();

        // Create managers
        createManagers();

        // Load all managers
        registerManagers();

        // Connect view managers to threads
        connectViewToThreads();

        // Connect threads to one another
        connectThreads();

        // Register experiment engine threads
        connectEngineToThreads();

        // Register addtion threads
        registerAddtionalThreads();

        // Start addtional application threads
        startAddtionalThread();
    }


    /**
     * Class destructor performs:
     *    - Stops all running threads
     *
     * @brief Application::~Application
     */
    Application::~Application()
    {

    }


    /**
     * Create instance for each view manager
     *
     * @brief Application::createManagers
     */
    void Application::createManagers()
    {
        manager_factory.create<View::Managers::Global>("Global", this,  m_engine, settings_container);
    }


    /**
     * Register all the view manager instances
     *
     * @brief Application::registerManagers
     */
    void Application::registerManagers()
    {
        // Set global manger
        m_engine->rootContext()->setContextProperty("GlobalManager", manager_factory.get<View::Managers::Global>("Global"));
    }



    /**
     * Register qml types
     *
     * @brief Application::registerQmlTypes
     */
    void Application::registerQmlTypes()
    {

    }


    /**
     * Setups up any additional threads to be ran but does not start then
     *
     * @brief Application::registerAddtionalThreads
     */
    void Application::registerAddtionalThreads()
    {
    }


    /**
     * Loads the hardware and safety monitor thread
     *
     * @brief Application::startAddtionalThread
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     */
    void Application::startAddtionalThread()
    {

    }


    /**
     * Connect the view managers to threads
     *
     * @brief Application::connectViewToThreads
     */
    void Application::connectViewToThreads()
    {
        // Make connections for global view manager
        //manager_global.makeConnections();
    }


    /**
     * Connects the experiment engine to the threads
     *
     * @brief Application::connectEngineToThreads
     */
    void Application::connectEngineToThreads()
    {
        experiment_engine.makeConnections();
    }


    /**
     * Connect the safety thread to IO thread via the Qt connect method
     *
     * @brief Application::connectThreads
     */
    void Application::connectThreads()
    {


    }
}
