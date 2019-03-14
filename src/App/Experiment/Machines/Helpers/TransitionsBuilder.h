#pragma once

// Include external libs
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QState>

// Include settings container
#include "../../../Settings/Container.h"

// Include threads
#include "../../../Hardware/Access.h"

// Include validator state
#include "CommandValidatorState.h"

// Include functions


namespace App { namespace Experiment { namespace Machines { namespace Helpers
{

    class TransitionsBuilder: public QObject
    {
        Q_OBJECT
        public:
            TransitionsBuilder(QObject *parent, Settings::Container* settings, Hardware::Access &hardware);

        signals:


        public slots:


        private:
            // Holds the application settings
            Settings::Container* m_settings;

            // Hold the hardware gateway
            Hardware::Access &m_hardware;

            // External states

    };

}}}}
