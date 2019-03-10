#include "View.h"

// External deps
#include <QDebug>

namespace App { namespace Settings
{

    /**
     * Contructor
     *
     * @brief View::View
     * @param parent
     */
    View::View(QObject *parent, QString location)
        :   QObject(parent)
        ,   JsonFile()
    {
        // Load json data
        load(location, SaveFormat::Json);
    }


    /**
     * Extneral method to trigger an updating of json file
     *
     * @brief View::save
     */
    void View::save()
    {
        // Trigger private method update
        update();

        // Tell everyone we have saved
        emit emit_saved();
    }






    /**
     * Action to take on read of json data
     *
     * @brief View::read
     * @param json
     */
    void View::read(const QJsonObject &json)
    {

    }


    /**
     * Action to take on writing of json data
     *
     * @brief General::write
     * @param json
     */
    void View::write(QJsonObject &json) const
    {

    }

}}
