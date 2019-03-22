#include "PumpFunctions.h"

#include <QString>

#include "../Helpers/CommandValidatorState.h"

namespace App { namespace Experiment { namespace Machines { namespace Functions
{

    PumpFunctions::PumpFunctions(QObject *parent, Settings::Container* settings, Hardware::Access &hardware, QStateMachine& machine, QVariantMap &params, Hardware::CommandConstructor &commandConstructor)
        :   QObject(parent)

        ,   m_settings(settings)
        ,   m_hardware(hardware)
        ,   m_machine(machine)
        ,   m_params(params)
        ,   m_commandConstructor(commandConstructor)
    {
        // Connect object signals to hardware slots and visa versa
        connect(this, &PumpFunctions::hardwareRequest, &m_hardware, &Hardware::Access::hardwareAccess);

        // Connect the states to functions
        connectStatesToMethods();
    }

    PumpFunctions::~PumpFunctions()
    {

    }

    void PumpFunctions::connectStatesToMethods()
    {

    }


    void PumpFunctions::validatePumpHelper(int pumpId, bool state)
    {
        qDebug() << "validateing pump";
        // Get the validator state instance
        Helpers::CommandValidatorState* command = dynamic_cast<Helpers::CommandValidatorState*>(sender());

        // If cast successfull
        if(command != nullptr && command->package.value("cast_status").toBool())
        {
            // Get the package data from the instance
            QVariantMap package = command->package;

            // If id was not registered then the signal name was correct but the signal is not
            if(!isRegister(package.value("command_identifier").toString()))
            {
                errorDetails.clear();
                errorDetails.insert("message", "ID was wrong in the package, not an error but skipping signal");
                errorDetails.insert("acutal_id", package.value("command_identifier").toInt());
                errorDetails.insert("ids", stringOfIds());

                // Tell everyone the signal was wrong
                emit emit_validationWrongId(errorDetails);
                qDebug() << "wrong id";
                // Do nothing else
                return;
            }

            // Check guage is the same
            if(package.value("pump_id").toInt() == pumpId && package.value("status").toBool() == state)
            {
                // Failed data to passon
                QVariantMap successPackage;

                // Data to pass on
                successPackage.insert("requested_pump_id", pumpId);
                successPackage.insert("status", package.value("status").toBool());
                successPackage.insert("requested_status", state);

                // Emit safe to proceed
                emit emit_validationSuccess(successPackage);
                qDebug() << "emit success";

                // More helpful emit
                if(pumpId == 1)
                {
                    emit emit_Pump1On(successPackage);
                }
                else if(pumpId == 2)
                {
                    emit emit_Pump2On(successPackage);
                }

                return;
            }

            // Failed data to passon
            errorDetails.clear();
            errorDetails.insert("message", "The pump failed to update correctly");
            errorDetails.insert("requested_guage_group", pumpId);
            errorDetails.insert("requested_status", state);
            errorDetails.insert("status", package.value("status").toBool());
        }
        else
        {
            errorDetails.clear();
            errorDetails.insert("message", "Validation casting failed");
            errorDetails.insert("requested_valve_group", pumpId);
            errorDetails.insert("requested_status", state);
        }

        // Emit not safe to proceed
        emit emit_validationFailed(errorDetails);

        qDebug() << "emit fail";
    }


    void PumpFunctions::enablePump1()
    {
        // Generate command
        QVariantMap command = m_commandConstructor.pumpEnable(1);

        // Register id
        registerId(command.value("command_identifier").toString());

        // Emit siganl to HAL
        emit hardwareRequest(command);
    }

    void PumpFunctions::enablePump2()
    {
        // Generate command
        QVariantMap command = m_commandConstructor.pumpEnable(2);

        // Register id
        registerId(command.value("command_identifier").toString());

        // Emit siganl to HAL
        emit hardwareRequest(command);
    }



    void PumpFunctions::disablePump1()
    {
        // Generate command
        QVariantMap command = m_commandConstructor.pumpDisable(1);

        // Register id
        registerId(command.value("command_identifier").toString());

        // Emit siganl to HAL
        emit hardwareRequest(command);
    }

    void PumpFunctions::disablePump2()
    {
        // Generate command
        QVariantMap command = m_commandConstructor.pumpDisable(2);

        // Register id
        registerId(command.value("command_identifier").toString());

        // Emit siganl to HAL
        emit hardwareRequest(command);
    }




    void PumpFunctions::validateEnablePump1()
    {
        validatePumpHelper(1, true);
    }

    void PumpFunctions::validateEnablePump2()
    {
        validatePumpHelper(2, true);
    }

    void PumpFunctions::validateDisablePump1()
    {
        validatePumpHelper(1, false);
    }

    void PumpFunctions::validateDisablePump2()
    {
        validatePumpHelper(2, false);
    }
}}}}
