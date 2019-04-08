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

            // Getters
            QVariantMap chamber(int id);
            QVariantMap valves();
            QVariantMap pump(int id);
            int defaultPump();
            bool manualSafetyValve();

            // Types
            enum class Type: int
            {
                chamber_1 = 1,
                chamber_2 = 2,
                chamber_3 = 3,
                chamber_4 = 4,
                chamber_5 = 5,
                chamber_6 = 6,
                valves = 7,
                pump_1 = 8,
                pump_2 = 9,
                default_pump = 10,
                manual_safety_valve = 11
            };

            void save(Type type, QVariantMap data);
            void save(Type type, bool data);
            void save(Type type, int data);

        signals:
            void emit_saved();

        private:
            void read(const QJsonObject &json);
            void write(QJsonObject &json) const;

            // Chamber data
            bool m_manualSafetyValve;
            QVariantMap m_chamber1;
            QVariantMap m_chamber2;
            QVariantMap m_chamber3;
            QVariantMap m_chamber4;
            QVariantMap m_chamber5;
            QVariantMap m_chamber6;

            // Valve data
            QVariantMap m_valves;

            // Pump data
            int m_defaultPump = 1;
            QVariantMap m_pump1;
            QVariantMap m_pump2;
    };
}}

