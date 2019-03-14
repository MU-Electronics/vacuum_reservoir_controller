#include "TemperatureSensorFunctions.h"

#include <QString>

#include "../Helpers/CommandValidatorState.h"

namespace App { namespace Experiment { namespace Machines { namespace Functions
{

    TemperatureSensorFunctions::TemperatureSensorFunctions(QObject *parent, Settings::Container* settings, Hardware::Access &hardware, QStateMachine& machine, QVariantMap &params, Hardware::CommandConstructor &commandConstructor)
        :   QObject(parent)

        ,   m_settings(settings)
        ,   m_hardware(hardware)
        ,   m_machine(machine)
        ,   m_params(params)
        ,   m_commandConstructor(commandConstructor)
    {
        // Connect object signals to hardware slots and visa versa
        connect(this, &TemperatureSensorFunctions::hardwareRequest, &m_hardware, &Hardware::Access::hardwareAccess);

        // Connect the states to functions
        connectStatesToMethods();
    }

    TemperatureSensorFunctions::~TemperatureSensorFunctions()
    {

    }

    void TemperatureSensorFunctions::connectStatesToMethods()
    {

    }


    void TemperatureSensorFunctions::getQutipiTemperature()
    {
        // Generate command
        QVariantMap command = m_commandConstructor.temperatureSensorGetTemperature("qutipi");

        // Register id
        registerId(command.value("command_identifier").toString());

        // Emit siganl to HAL
        emit hardwareRequest(command);
    }



    void TemperatureSensorFunctions::validateGetQutipiTemperature()
    {
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

                // Do nothing else
                return;
            }

            // Check guage is the same
            if(package.value("temperature").toDouble() > 0)
            {
                // Failed data to passon
                QVariantMap successPackage;

                // Data to pass on
                successPackage.insert("temperature", package.value("temperature").toDouble());

                // Emit safe to proceed
                emit emit_validationSuccess(successPackage);

                return;
            }

            // Failed data to passon
            errorDetails.clear();
            errorDetails.insert("message", "The temperature sensor failed to read correctly");
            errorDetails.insert("requested_location", "qutipi");
            errorDetails.insert("temperature", package.value("temperature").toDouble());
        }
        else
        {
            errorDetails.clear();
            errorDetails.insert("message", "Validation casting failed");
        }

        // Emit not safe to proceed
        emit emit_validationFailed(errorDetails);
    }

}}}}
