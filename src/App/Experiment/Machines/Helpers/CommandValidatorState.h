#pragma once

#include <QObject>
#include <QState>
#include <QStateMachine>
#include <QVariantMap>

// Debugging catergories
#include "../../../Services/Debuging.h"

namespace App { namespace Experiment { namespace Machines { namespace Helpers
{

    class CommandValidatorState: public QState
    {
        public:
            CommandValidatorState(QState *machine);

            QVariantMap package;

        private:
            void onEntry(QEvent* e);
    };

}}}}

