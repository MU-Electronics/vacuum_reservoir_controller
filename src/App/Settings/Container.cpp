#include "Container.h"

// External deps
#include <QDir>
#include <QCoreApplication>

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
        QString path = "/data/settings";

        // Return the path
        return path;
    }

}}
