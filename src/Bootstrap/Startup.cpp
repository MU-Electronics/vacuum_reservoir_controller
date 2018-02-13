#include "Startup.h"

// Qt libraries
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>
#include <QtGlobal>

// Include the template framework
#include "iconsimageprovider.h"
#include "iconthemeimageprovider.h"

// Application includes
#include "App/Application.h"

#include "App/Services/Debugger.h"

namespace Bootstrap
{

    /**
     * Boot up the applcation
     *
     * @brief Startup::Startup
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     */
    Startup::Startup()
        : QObject(nullptr),
          m_engine (*new QQmlApplicationEngine()),
          m_isValid(true),
          m_application(*new App::Application(this, &m_engine))
    {
        // Set debugger QObject parent
        App::Services::Debugger::getInstance().setParent(this);

        // Configure the theme being used
        configTheme();

        // Load main view
        loadMainView();
    }


    /**
     * Destroy anything still exsisiting
     *
     * @brief Startup::~Startup
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     */
    Startup::~Startup()
    {
        // Set debugger QObject parent
        App::Services::Debugger::getInstance().setParent(nullptr);
    }


    /**
     * Configure any theme related stuff
     *
     * @brief Startup::configTheme
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     */
    void Startup::configTheme()
    {
        // Setup the styling
        if (QQuickStyle::name().isEmpty())
            QQuickStyle::setStyle(QLatin1String("Material"));

        // Load the template framework
        m_engine.addImportPath(QLatin1String("qrc:/"));
        m_engine.addImageProvider(QLatin1String("fluidicons"), new IconsImageProvider());
        m_engine.addImageProvider(QLatin1String("fluidicontheme"), new IconThemeImageProvider());
    }


    /**
     * Load the main view of the app
     *
     * @brief Startup::loadMainView
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     */
    void Startup::loadMainView()
    {
        // Load the main application qml file
        m_engine.load(QUrl(QStringLiteral("qrc:/views/main.qml")));

        // Get root objects
        auto rootObjects = m_engine.rootObjects();

        // Validate
        if (rootObjects.isEmpty())
        {
            qDebug() << "There are no QML objects!";
            m_isValid = false;
            return;
        }

        // Save a pointer to the root objects
        m_root = rootObjects[0];
    }
}







