#include "Hardware.h"

// External deps
#include <QDebug>

namespace App { namespace Settings
{

    /**
     * Contructor
     *
     * @brief Hardware::Hardware
     * @param parent
     */
    Hardware::Hardware(QObject *parent, QString location)
        :   QObject(parent)
        ,   JsonFile()
    {
        // Load json data
        load(location, SaveFormat::Json);
    }


    /**
     * Action to take on read of json data
     *
     * @brief Hardware::read
     * @param json
     */
    void Hardware::read(const QJsonObject &json)
    {
        // Get root to map
        QVariantMap rootMap = json.toVariantMap();


        // Get valves to map
        auto valves = rootMap["valves"].toMap();
            // @TODO maps values across


        // Get valves to map
        auto guages = rootMap["guages"].toMap();
            // @TODO maps values across


        // Get valves to map
        m_pumps = rootMap["pumps"].toMap();


        // Get valves to map
        m_remote = rootMap["remote"].toMap();
    }


    /**
     * Action to take on writing of json data
     *  - NOTES: hardware values are not updatable, they should never change unless by an engineer
     *
     * @brief Hardware::write
     * @param json
     */
    void Hardware::write(QJsonObject &json) const
    {

    }

}}
