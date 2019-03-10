#pragma once

// Include external deps
#include <QObject>
#include <QMap>
#include <QVariantMap>
#include <QString>

#include "../Services/JsonFile.h"

namespace App { namespace Settings
{
    class General : public QObject, private Services::JsonFile
    {
        Q_OBJECT

        public:
            General(QObject *parent, QString location);

            void save();

            // Getters
            QVariantMap chamber(int id);
            QVariantMap valves();
            QVariantMap pump(int id);

        signals:
            void emit_saved();

        private:
            void read(const QJsonObject &json);
            void write(QJsonObject &json) const;

            // Chamber data
            QVariantMap m_chamber1;
            QVariantMap m_chamber2;
            QVariantMap m_chamber3;
            QVariantMap m_chamber4;
            QVariantMap m_chamber5;
            QVariantMap m_chamber6;

            // Valve data
            QVariantMap m_valves;

            // Pump data
            QVariantMap m_pump1;
            QVariantMap m_pump2;
    };
}}

