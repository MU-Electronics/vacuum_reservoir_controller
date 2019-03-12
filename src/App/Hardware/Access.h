#pragma once

// Include external libs
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QVariantMap>
#include <QQueue>
#include <QTimer>


// Include settings container
#include "../Settings/Container.h"


// Include thread
#include "../Services/Thread.h"


// Include HALS
#include "HAL/EmergancyStop.h"
#include "HAL/Guages.h"
#include "HAL/Pumps.h"
#include "HAL/Remote.h"
#include "HAL/TemperatureSensor.h"
#include "HAL/Valves.h"


// Include HAL presenters


namespace App { namespace Hardware
{
    class Access: public App::Services::Thread
    {
        Q_OBJECT

        public:
            Access(QObject *parent, Settings::Container* settings);
            ~Access();

        public slots:
            void hardwareAccess(QVariantMap command);

        signals:
            void emit_methodAttemptResults(QVariantMap status);

        private:
            // Contains for settings container
            Settings::Container* m_settings;

            // Timer for time based events
            QTimer& m_timer;

            // Queue for commands to be ran
            QQueue<QVariantMap> m_queue;

            // HAL objects
            HAL::Guages* m_guage;

            // HAL presenters

            // Methods in this class that can be ran externally
            QList<QString> m_avaliableMethods;

            // Store command data for access invoked commands
            QVariantMap m_lastcommands;

            // Runs the correct HAL function
            bool executeHalMethods(QVariantMap command);

            void proccessDataFromHals(QString responable, QString method, QVariantMap commands, QStringList halData);

            // Check com port connections
            void connectDevices();

            // Ran when configuring thread
            void configure(QThread &thread);

            // The worker to run in forever loop
            void worker();

    };

}}
