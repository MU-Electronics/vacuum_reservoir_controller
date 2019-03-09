#include "Container.h"

// External deps
#include <QDebug>
#include <QDir>
#include <QStandardPaths>

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
    {
        // Configure the setting
        loadSettings();
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

        qDebug() << pathDir;
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
        QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/vacuum_reservoir_controller/";

        // Check it exists and if not create it
        if(!QDir(path).exists())
            QDir().mkdir(path);

        // Return the path
        return path;
    }

}}
