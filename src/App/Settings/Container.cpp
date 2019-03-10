#include "Container.h"

// External deps
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QScopedPointer>

namespace App { namespace Settings
{

    /**
     * Contruct the container performs:
     *      - Creation of all setting managers
     *
     * @brief Container::Container
     * @param parent
     */
    Container::Container(QObject *parent)
        :   QObject(parent)
    {
        // Configure the setting
        loadSettings();
    }

    Container::~Container()
    {

    }


    /**
     * Return instance of General setting object
     *
     * @brief Container::general
     * @return
     */
    QSharedPointer<General> Container::general()
    {
        QSharedPointer<General> pointer(m_general);
        return pointer;
    }


    /**
     * Return instance of Hardware setting object
     *
     * @brief Container::hardware
     * @return
     */
    QSharedPointer<Hardware> Container::hardware()
    {
        QSharedPointer<Hardware> pointer(m_hardware);
        return pointer;
    }


    /**
     * Return instance of View setting object
     *
     * @brief Container::view
     * @return
     */
    QSharedPointer<View> Container::view()
    {
        QSharedPointer<View> pointer(m_view);
        return pointer;
    }


    /**
     * Load the json setting files into the setting objects
     *
     * @brief Container::loadSettings
     */
    void Container::loadSettings()
    {
        // Get the current programs dir
        QString pathDir = getDir();

        // Load setting objects
            // General
            m_general = QSharedPointer<General>(new General(this, getDir() + "General.json"));
            // Hardware
            m_hardware = QSharedPointer<Hardware>(new Hardware(this, getDir() + "Hardware.json"));
            // View
            m_view = QSharedPointer<View>(new View(this, getDir() + "View.json"));
    }


    /**
     * Get the programs root path
     *
     * @brief Container::getDir
     * @return QString path
     */
    QString Container::getDir()
    {
        // This should work across platform, working windows need to check mac
        QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/vacuum_reservoir_controller/config/";

        // Check it exists and if not create it
        if(!QDir(path).exists())
            QDir().mkdir(path);

        // Return the path
        return path;
    }

}}
