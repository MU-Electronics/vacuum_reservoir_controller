#include "General.h"

// External deps
#include <QDebug>

#include <QJsonArray>
#include <QJsonObject>

namespace App { namespace Settings
{

    /**
     * Contructor
     *
     * @brief General::General
     * @param parent
     */
    General::General(QObject *parent, QString location)
        :   QObject(parent)
        ,   JsonFile()
    {
        // Load json data
        load(location, SaveFormat::Json);
    }


    /**
     * Extneral method to trigger an updating of json file
     *
     * @brief General::save
     */
    void General::save(Type type, QVariantMap data)
    {
        // Updated correct model
        switch(type)
        {
            case Type::chamber_1:
                m_chamber1 = data;
            break;
            case Type::chamber_2:
                m_chamber2 = data;
            break;
            case Type::chamber_3:
                m_chamber3 = data;
            break;
            case Type::chamber_4:
                m_chamber4 = data;
            break;
            case Type::chamber_5:
                m_chamber5 = data;
            break;
            case Type::chamber_6:
                m_chamber6 = data;
            break;
            case Type::valves:
                m_valves = data;
            break;
            case Type::pump_1:
                m_pump1 = data;
            break;
            case Type::pump_2:
                m_pump2 = data;
            break;
        }

        // Trigger private method update
        update();

        // Tell everyone we have saved
        emit emit_saved();
    }


    /**
     * Return chamber information
     *
     * @brief General::chamber
     * @param id
     * @return
     */
    QVariantMap General::chamber(int id)
    {
        switch(id)
        {
            case 1:
                return m_chamber1;
            case 2:
                return m_chamber2;
            case 3:
                return m_chamber3;
            case 4:
                return m_chamber4;
            case 5:
                return m_chamber5;
            case 6:
                return m_chamber6;
        }

        throw std::out_of_range ("Chambers id is from 1-6");
    }


    /**
     * Return valves information
     *
     * @brief General::valves
     * @return
     */
    QVariantMap General::valves()
    {
        return m_valves;
    }


    /**
     * Return pumps information
     *
     * @brief General::pumps
     * @param id
     * @return
     */
    QVariantMap General::pump(int id)
    {
        switch(id)
        {
            case 1:
                return m_pump1;
            case 2:
                return m_pump2;
        }

        throw std::out_of_range ("Pump id is from 1-2");
    }





    /**
     * Action to take on read of json data
     *
     * @brief General::read
     * @param json
     */
    void General::read(const QJsonObject &json)
    {
        // Get root to map
        QVariantMap rootMap = json.toVariantMap();

        // Get chambers to map
        auto chambers = rootMap["chambers"].toMap();
        m_chamber1 = chambers["1"].toMap();
        m_chamber2 = chambers["2"].toMap();
        m_chamber3 = chambers["3"].toMap();
        m_chamber4 = chambers["4"].toMap();
        m_chamber5 = chambers["5"].toMap();
        m_chamber6 = chambers["6"].toMap();

        // Valve data
        m_valves = rootMap["valves"].toMap();

        // Pump data
        auto pumps = rootMap["pumps"].toMap();
        m_pump1 = pumps["1"].toMap();
        m_pump2 = pumps["2"].toMap();
    }


    /**
     * Action to take on writing of json data
     *
     * @brief General::write
     * @param json
     */
    void General::write(QJsonObject &json) const
    {
        // Update chamber json
        QJsonObject chambers;

        // For each chamber
        QVariantList chamberList = {m_chamber1, m_chamber2, m_chamber3, m_chamber4, m_chamber5, m_chamber6};
        int c = 0;
        for(auto chamberItem : chamberList)
        {
            // Inc c
            c++;

            // Create single chamber
            QJsonObject chamber;

            // Convert current chamber to map
            auto item = chamberItem.toMap();

            // Add only the data we expect
            chamber["auto_control_enabled"] = item["auto_control_enabled"].toBool();
            chamber["manual_control_enabled"] = item["manual_control_enabled"].toBool();
            chamber["comment"] = item["comment"].toString();
            chamber["lower_set_point"] = item["manual_control_enabled"].toInt();
            chamber["upper_set_point"] = item["upper_set_point"].toInt();
            chamber["alarm_pressure"] = item["alarm_pressure"].toInt();
            chamber["alarm_time"] = item["alarm_time"].toInt();
            chamber["leak_detection"] = item["leak_detection"].toBool();
            chamber["leak_period"] = item["leak_period"].toInt();
            chamber["leak_max"] = item["leak_max"].toInt();

            // Add to main chamber object
            chambers[QString::number(c)] = chamber;
        }

        // Add chambers to json
        json["chambers"] = chambers;


        // Update valve json
        QJsonObject valves;
        valves["maxium_open"] = m_valves["maxium_open"].toInt();
        valves["interval_opening"] = m_valves["interval_opening"].toInt();
        json["valves"] = valves;


        // Update pump json
        QJsonObject pumps;

        // For each chamber
        QVariantList pumpList = {m_pump1, m_pump2};
        int p = 0;
        for(auto pumpItem : pumpList)
        {
            // Inc p
            p++;

            // Create single chamber
            QJsonObject pump;

            // Convert current chamber to map
            auto item = pumpItem.toMap();

            // Add only the data we expect
            pump["auto_control_enabled"] = item["auto_control_enabled"].toBool();
            pump["manual_control_enabled"] = item["manual_control_enabled"].toBool();
            pump["lower_set_point"] = item["manual_control_enabled"].toDouble();
            pump["upper_set_point"] = item["upper_set_point"].toDouble();
            pump["alarm_pressure"] = item["alarm_pressure"].toDouble();
            pump["alarm_time"] = item["alarm_time"].toInt();
            pump["warm_up"] = item["warm_up"].toInt();
            pump["pump_void"] = item["pump_void"].toInt();
            pump["pump_manifold_void"] = item["pump_manifold_void"].toInt();
            pump["leak_detection"] = item["leak_detection"].toBool();
            pump["leak_period"] = item["leak_period"].toInt();
            pump["leak_max"] = item["leak_max"].toDouble();

            // Add to main chamber object
            pumps[QString::number(p)] = pump;
        }

        // Add chambers to json
        json["pumps"] = pumps;
    }

}}
