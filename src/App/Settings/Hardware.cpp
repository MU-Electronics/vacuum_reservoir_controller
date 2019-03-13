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
     * Return emergancy stop settings
     *
     * @brief Hardware::emergencyStop
     * @return
     */
    QVariantMap Hardware::emergencyStop()
    {
        return m_emergencyStop;
    }


    /**
     * Return temperature sensor settings
     *
     * @brief temperatureSensor
     * @return
     */
    QVariantMap Hardware::temperatureSensor()
    {
        return m_temperatureSensor;
    }


    /**
     * Return guage information
     *
     * @brief Hardware::guages
     * @return
     */
    QVariantMap Hardware::guages()
    {
        return m_guages;
    }


    /**
     * Return valves information
     *
     * @brief Hardware::valves
     * @return
     */
    QVariantMap Hardware::valves()
    {
        return m_valves;
    }


    /**
     * Return pump information
     *
     * @brief Hardware::pumps
     * @return
     */
    QVariantMap Hardware::pumps()
    {
        return m_pumps;
    }


    /**
     * Return remote information
     *
     * @brief Hardware::remote
     * @return
     */
    QVariantMap Hardware::remote()
    {
        return m_remote;
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
        m_valves["mappings"] = valves["mappings"].toMap();


        // Get valves to map
        auto guages = rootMap["guages"].toMap();
        m_guages = guages;


        // Get valves to map
        m_pumps = rootMap["pumps"].toMap();


        // Get valves to map
        m_remote = rootMap["remote"].toMap();


        // Get emergancy stop to map
        m_emergencyStop = rootMap["emergancy_stop"].toMap();


        // Get temperature sensor to map
        m_temperatureSensor = rootMap["temperature_sensor"].toMap();
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
