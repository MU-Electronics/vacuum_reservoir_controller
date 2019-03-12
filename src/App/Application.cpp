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

// View managers
#include "View/Managers/Global.h"
#include "View/Managers/SystemValues.h"
#include "View/Managers/Power.h"
#include "View/Managers/Logs.h"
#include "View/Managers/Control.h"
#include "View/Managers/SettingsUpdater.h"

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
        ,   hardware(*new Hardware::Access(this, &settings_container))

            // Include the expeirment engine
        ,   experiment_engine(*new Experiment::Engine(this, &settings_container))

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
        // Close hardware thread
        hardware.abort();
        thread_hardware.wait();
    }


    /**
     * Create instance for each view manager
     *
     * @brief Application::createManagers
     */
    void Application::createManagers()
    {
        manager_factory.create<View::Managers::Global>("Global", this,  m_engine, &settings_container);
        manager_factory.create<View::Managers::SystemValues>("SystemValues", this,  m_engine, &settings_container);
        manager_factory.create<View::Managers::Power>("Power", this,  m_engine, &settings_container);
        manager_factory.create<View::Managers::Logs>("Logs", this,  m_engine, &settings_container);
        manager_factory.create<View::Managers::Control>("Control", this,  m_engine, &settings_container);
        manager_factory.create<View::Managers::SettingsUpdater>("SettingsUpdater", this,  m_engine, &settings_container);
    }


    /**
     * Register all the view manager instances
     *
     * @brief Application::registerManagers
     */
    void Application::registerManagers()
    {
        // Set qml context
        m_engine->rootContext()->setContextProperty("GlobalManager", manager_factory.get<View::Managers::Global>("Global"));
        m_engine->rootContext()->setContextProperty("SystemValuesManager", manager_factory.get<View::Managers::SystemValues>("SystemValues"));
        m_engine->rootContext()->setContextProperty("PowerManager", manager_factory.get<View::Managers::Power>("Power"));
        m_engine->rootContext()->setContextProperty("LogsManager", manager_factory.get<View::Managers::Logs>("Logs"));
        m_engine->rootContext()->setContextProperty("ControlManager", manager_factory.get<View::Managers::Control>("Control"));
        m_engine->rootContext()->setContextProperty("SettingsUpdaterManager", manager_factory.get<View::Managers::SettingsUpdater>("SettingsUpdater"));
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
        // Hardware thread
        hardware.setup(thread_hardware);
        hardware.moveToThread(&thread_hardware);
    }


    /**
     * Loads the hardware and safety monitor thread
     *
     * @brief Application::startAddtionalThread
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     */
    void Application::startAddtionalThread()
    {
        // Start the hardware thread
        hardware.abort();
        thread_hardware.wait();
        hardware.requestWork();
    }


    /**
     * Connect the view managers to threads
     *
     * @brief Application::connectViewToThreads
     */
    void Application::connectViewToThreads()
    {
        // Make connections for global view manager
        manager_factory.get<View::Managers::Global>("Global")->makeConnections();
        manager_factory.get<View::Managers::SystemValues>("SystemValues")->makeConnections();
        manager_factory.get<View::Managers::Power>("Power")->makeConnections();
        manager_factory.get<View::Managers::Logs>("Logs")->makeConnections();
        manager_factory.get<View::Managers::Control>("Control")->makeConnections();
        manager_factory.get<View::Managers::SettingsUpdater>("SettingsUpdater")->makeConnections();
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
