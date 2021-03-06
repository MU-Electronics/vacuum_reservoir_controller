#pragma once

#include <QObject>

#include <QVariantMap>

namespace App { namespace Hardware
{

    class CommandConstructor
    {

        public:
            CommandConstructor(QObject *parent = nullptr);

            ~CommandConstructor();

            QString generateCommandCount();

            // Emergancy stop commands
            QVariantMap emergancyStopIsPressed();

            // Guage commands
            QVariantMap guageSetState(int group, bool state);
            QVariantMap guageEnabled(int group);
            QVariantMap guageDisabled(int group);
            QVariantMap guageReadVacuum(int group);
            QVariantMap guageReadTrip(int group);

            // Pump commands
            QVariantMap pumpEnable(int pumpId);
            QVariantMap pumpDisable(int pumpId);

            // Remote commands
            QVariantMap remoteIsConnected();

            // Temperature sensor commands
            QVariantMap temperatureSensorGetTemperature(QString location = "qutipi");

            // Valve commands
            QVariantMap valveState(int group, bool state);
            QVariantMap valveOpen(int group);
            QVariantMap valveClose(int group);

    };
}}
