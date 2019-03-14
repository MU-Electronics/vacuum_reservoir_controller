#include "TransitionsBuilder.h"

// Include external libs
#include <QObject>
#include <QDebug>
#include <QState>

// Include validator state type
#include "CommandValidatorState.h"

// Include functions


namespace App { namespace Experiment { namespace Machines { namespace Helpers
{

    TransitionsBuilder::TransitionsBuilder(QObject *parent, Settings::Container *settings, Hardware::Access &hardware)
        :   QObject(parent)

            // Access the global referances
        ,   m_settings(settings)
        ,   m_hardware(hardware)

            // Functions

    {

    }

}}}}
