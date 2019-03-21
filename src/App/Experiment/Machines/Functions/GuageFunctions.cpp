#include "GuageFunctions.h"

#include <QString>

#include "../Helpers/CommandValidatorState.h"

namespace App { namespace Experiment { namespace Machines { namespace Functions
{

    GuageFunctions::GuageFunctions(QObject *parent, Settings::Container* settings, Hardware::Access &hardware, QStateMachine& machine, QVariantMap &params, Hardware::CommandConstructor &commandConstructor)
        :   QObject(parent)

        ,   m_settings(settings)
        ,   m_hardware(hardware)
        ,   m_machine(machine)
        ,   m_params(params)
        ,   m_commandConstructor(commandConstructor)
    {
        // Connect object signals to hardware slots and visa versa
        connect(this, &GuageFunctions::hardwareRequest, &m_hardware, &Hardware::Access::hardwareAccess);

        // Connect the states to functions
        connectStatesToMethods();
    }

    GuageFunctions::~GuageFunctions()
    {

    }

    void GuageFunctions::connectStatesToMethods()
    {

    }

    void GuageFunctions::guageTripHelper(int group)
    {
        // Generate command
        QVariantMap command = m_commandConstructor.guageReadTrip(group);
        //qDebug() << command;

        // Register id
        registerId(command.value("command_identifier").toString());

        // Emit siganl to HAL
        emit hardwareRequest(command);
    }

    void GuageFunctions::validateGuageTripHelper(int group)
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
            if(package.value("guage_id").toInt() == group)
            {
                // Failed data to passon
                QVariantMap successPackage;

                // Data to pass on
                successPackage.insert("requested_gauge_group", group);
                successPackage.insert("status", package.value("status").toBool());

                // Emit safe to proceed
                emit emit_validationSuccess(successPackage);

                return;
            }

            // Failed data to passon
            errorDetails.clear();
            errorDetails.insert("message", "The guage trip failed to read correctly");
            errorDetails.insert("requested_guage_group", group);
            errorDetails.insert("group", package.value("group").toInt());
        }
        else
        {
            errorDetails.clear();
            errorDetails.insert("message", "Validation casting failed");
            errorDetails.insert("requested_valve_group", group);
        }

        // Emit not safe to proceed
        emit emit_validationFailed(errorDetails);
    }

    /**
     * ready guage helper
     *
     * @brief GuageFunctions::guagePresureHelper
     * @param number
     */
    void GuageFunctions::guagePresureHelper(int group)
    {
        // Generate command
        QVariantMap command = m_commandConstructor.guageReadVacuum(group);

        // Register id
        registerId(command.value("command_identifier").toString());

        // Emit siganl to HAL
        emit hardwareRequest(command);

        // qDebug() << "Requesting guage: " << command["guage_id"];
    }

    /**
     * Vavlidate read guage helper
     *
     * @brief GuageFunctions::validateGuagePressureHelper
     * @param number
     */
    void GuageFunctions::validateGuagePressureHelper(int group)
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

                qDebug () << errorDetails << package;

                // Tell everyone the signal was wrong
                emit emit_validationWrongId(errorDetails);

                // Do nothing else
                return;
            }

            // Check guage is the same
            if(package.value("guage_id").toInt() == group)
            {
                // Failed data to passon
                QVariantMap successPackage;

                // Data to pass on
                successPackage.insert("requested_gauge_group", group);
                successPackage.insert("pressure", package.value("pressure").toDouble());
                successPackage.insert("voltage", package.value("voltage").toDouble());
                successPackage.insert("status", package.value("status").toInt());
                successPackage.insert("error", package.value("error").toString());

                // Emit safe to proceed
                emit emit_validationPressureReading(successPackage);

                return;
            }

            // Failed data to passon
            errorDetails.clear();
            errorDetails.insert("message", "The guage failed to update correctly");
            errorDetails.insert("requested_guage_group", group);
            errorDetails.insert("group", package.value("group").toInt());
        }
        else
        {
            errorDetails.clear();
            errorDetails.insert("message", "Validation casting failed");
            errorDetails.insert("requested_valve_group", group);
        }

        // Emit not safe to proceed
        emit emit_validationFailed(errorDetails);
    }



    /**
     * Open guage functions
     *
     * @brief GuageFunctions::readGroupX
     */

    void GuageFunctions::readGroup1()
    {
        guagePresureHelper(1);
    }

    void GuageFunctions::readGroup2()
    {
        guagePresureHelper(2);
    }

    void GuageFunctions::readGroup3()
    {
        guagePresureHelper(3);
    }

    void GuageFunctions::readGroup4()
    {
        guagePresureHelper(4);
    }

    void GuageFunctions::readGroup5()
    {
        guagePresureHelper(5);
    }

    void GuageFunctions::readGroup6()
    {
        guagePresureHelper(6);
    }

    void GuageFunctions::readGroup7()
    {
        guagePresureHelper(7);
    }

    void GuageFunctions::readGroup8()
    {
        guagePresureHelper(8);
    }





    /**
     * Validate Read gauges
     *
     */

    void GuageFunctions::validateReadGroup1()
    {
        validateGuagePressureHelper(1);
    }

    void GuageFunctions::validateReadGroup2()
    {
        validateGuagePressureHelper(2);
    }

    void GuageFunctions::validateReadGroup3()
    {
        validateGuagePressureHelper(3);
    }

    void GuageFunctions::validateReadGroup4()
    {
        validateGuagePressureHelper(4);
    }

    void GuageFunctions::validateReadGroup5()
    {
        validateGuagePressureHelper(5);
    }

    void GuageFunctions::validateReadGroup6()
    {
        validateGuagePressureHelper(6);
    }

    void GuageFunctions::validateReadGroup7()
    {
        validateGuagePressureHelper(7);
    }

    void GuageFunctions::validateReadGroup8()
    {
        validateGuagePressureHelper(8);
    }




    // Read trips
    void GuageFunctions::isTripedGroup1()
    {
        guageTripHelper(1);
    }
    void GuageFunctions::isTripedGroup2()
    {
        guageTripHelper(2);
    }
    void GuageFunctions::isTripedGroup3()
    {
        guageTripHelper(3);
    }
    void GuageFunctions::isTripedGroup4()
    {
        guageTripHelper(4);
    }
    void GuageFunctions::isTripedGroup5()
    {
        guageTripHelper(5);
    }
    void GuageFunctions::isTripedGroup6()
    {
        guageTripHelper(6);
    }
    void GuageFunctions::isTripedGroup7()
    {
        guageTripHelper(7);
    }
    void GuageFunctions::isTripedGroup8()
    {
        guageTripHelper(8);
    }

    // Validate read trips
    void GuageFunctions::validateIsTripedGroup1()
    {
        validateGuageTripHelper(1);
    }
    void GuageFunctions::validateIsTripedGroup2()
    {
        validateGuageTripHelper(2);
    }
    void GuageFunctions::validateIsTripedGroup3()
    {
        validateGuageTripHelper(3);
    }
    void GuageFunctions::validateIsTripedGroup4()
    {
        validateGuageTripHelper(4);
    }
    void GuageFunctions::validateIsTripedGroup5()
    {
        validateGuageTripHelper(5);
    }
    void GuageFunctions::validateIsTripedGroup6()
    {
        validateGuageTripHelper(6);
    }
    void GuageFunctions::validateIsTripedGroup7()
    {
        validateGuageTripHelper(7);
    }
    void GuageFunctions::validateIsTripedGroup8()
    {
        validateGuageTripHelper(8);
    }




}}}}
