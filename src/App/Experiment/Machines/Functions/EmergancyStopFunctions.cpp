#include "EmergancyStopFunctions.h"

#include <QString>

#include "../Helpers/CommandValidatorState.h"

namespace App { namespace Experiment { namespace Machines { namespace Functions
{

    EmergancyStopFunctions::EmergancyStopFunctions(QObject *parent, Settings::Container* settings, Hardware::Access &hardware, QStateMachine& machine, QVariantMap &params, Hardware::CommandConstructor &commandConstructor)
        :   QObject(parent)

        ,   m_settings(settings)
        ,   m_hardware(hardware)
        ,   m_machine(machine)
        ,   m_params(params)
        ,   m_commandConstructor(commandConstructor)
    {
        // Connect object signals to hardware slots and visa versa
        connect(this, &EmergancyStopFunctions::hardwareRequest, &m_hardware, &Hardware::Access::hardwareAccess);

        // Connect the states to functions
        connectStatesToMethods();
    }

    EmergancyStopFunctions::~EmergancyStopFunctions()
    {

    }

    void EmergancyStopFunctions::connectStatesToMethods()
    {

    }


}}}}
