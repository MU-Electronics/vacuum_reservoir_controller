#include "General.h"

// External deps
#include <QDebug>

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
     * Action to take on read of json data
     *
     * @brief General::read
     * @param json
     */
    void General::read(const QJsonObject &json)
    {

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
