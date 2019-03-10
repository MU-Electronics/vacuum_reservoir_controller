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
    void General::save()
    {
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
    QVariantMap General::pumps(int id)
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

    }

}}
