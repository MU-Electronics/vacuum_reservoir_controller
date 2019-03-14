#pragma once

// Include extenral deps
#include <QObject>

// Debugging catergories
#include "../../Services/Debuging.h"

// Include settings container
#include "../../Settings/Container.h"

// Include hardware thread
#include "../../Hardware/Access.h"

namespace App { namespace Experiment { namespace Machines
{
    class MachineContainer    :   public QObject
    {
        Q_OBJECT

        public:
            MachineContainer(QObject *parent, Settings::Container* settings, Hardware::Access &hardware);

            ~MachineContainer();

            MachineContainer(const MachineContainer&) = delete;

            void makeConnections();

        private:
            // Holds the application settings
            Settings::Container* m_settings;

    };
}}}


