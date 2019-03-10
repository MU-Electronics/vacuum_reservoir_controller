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
     * Extneral method to trigger an updating of json file
     *
     * @brief Hardware::save
     */
    void Hardware::save()
    {
        // Trigger private method update
        update();

        // Tell everyone we have saved
        emit emit_saved();
    }






    /**
     * Action to take on read of json data
     *
     * @brief Hardware::read
     * @param json
     */
    void Hardware::read(const QJsonObject &json)
    {

    }


    /**
     * Action to take on writing of json data
     *
     * @brief Hardware::write
     * @param json
     */
    void Hardware::write(QJsonObject &json) const
    {

    }

}}
