#pragma once

// Include external deps
#include <QObject>
#include <QMap>
#include <QVariantMap>
#include <QString>

#include "../Services/JsonFile.h"

namespace App { namespace Settings
{
    class Hardware : public QObject, private Services::JsonFile
    {
        Q_OBJECT

        public:
            Hardware(QObject *parent, QString location);

            // Getters
            QVariantMap valves();
            QVariantMap guages();
            QVariantMap pumps();
            QVariantMap remote();

        signals:

        private:

            // Valves data
            QVariantMap m_valves;

            // Guage data
            QVariantMap m_guages;

            // Guage data
            QVariantMap m_pumps;

            // Guage data
            QVariantMap m_remote;

            void read(const QJsonObject &json);
            void write(QJsonObject &json) const;
    };
}}

