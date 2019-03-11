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
     * Return designer information
     *
     * @brief View::designer
     */
    QVariantMap View::designer()
    {
        return m_designer;
    }


    /**
     * Return client information
     *
     * @brief View::client
     */
    QVariantMap View::client()
    {
        return m_client;
    }


    /**
     * Action to take on read of json data
     *
     * @brief View::read
     * @param json
     */
    void View::read(const QJsonObject &json)
    {
        // Get root to map
        QVariantMap rootMap = json.toVariantMap();

        // Designer
        m_designer = rootMap["designed_by"].toMap();

        // Client
        m_client = rootMap["designed_for"].toMap();
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
