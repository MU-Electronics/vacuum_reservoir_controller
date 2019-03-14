#pragma once

// Include extenral deps
#include <QObject>

// Debugging catergories
#include "../Services/Debuging.h"

// Include settings container
#include "../Settings/Container.h"

// Include hardware thread
#include "../Hardware/Access.h"

// Include machine container
#include "Machines/MachineContainer.h"

namespace App { namespace Experiment
{
    class Engine    :   public QObject
    {
        Q_OBJECT

        public:
            Engine(QObject *parent, Settings::Container* settings, Hardware::Access &hardware);

            ~Engine();

            Engine(const Engine&) = delete;

            void makeConnections();

            Machines::MachineContainer& machines()
            {
                return m_machines;
            }

        private:
            // Holds the application settings
            Settings::Container* m_settings;

            // Machines container
            Machines::MachineContainer& m_machines;

    };
}}


