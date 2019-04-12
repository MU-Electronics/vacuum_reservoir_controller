#include "ValveFunctions.h"

#include <QString>

#include "../Helpers/CommandValidatorState.h"

namespace App { namespace Experiment { namespace Machines { namespace Functions
{

    ValveFunctions::ValveFunctions(QObject *parent, Settings::Container* settings, Hardware::Access &hardware, QStateMachine& machine, QVariantMap &params, Hardware::CommandConstructor &commandConstructor)
        :   QObject(parent)

        ,   m_settings(settings)
        ,   m_hardware(hardware)
        ,   m_machine(machine)
        ,   m_params(params)
        ,   m_commandConstructor(commandConstructor)
    {
        // Connect object signals to hardware slots and visa versa
        connect(this, &ValveFunctions::hardwareRequest, &m_hardware, &Hardware::Access::hardwareAccess);

        // Connect the states to functions
        connectStatesToMethods();
    }

    ValveFunctions::~ValveFunctions()
    {

    }

    void ValveFunctions::connectStatesToMethods()
    {

    }

    /**
     * Open valve helper
     *
     * @brief ValveFunctions::openValveHelper
     * @param number
     */
    void ValveFunctions::valveHelper(int group, bool state)
    {
        // Generate command
        QVariantMap command = m_commandConstructor.valveState(group, state);

        // Register id
        registerId(command.value("command_identifier").toString());

        // Emit siganl to HAL
        emit hardwareRequest(command);
    }

    /**
     * Vavlidate valve helper
     *
     * @brief ValveFunctions::openValveHelper
     * @param number
     */
    void ValveFunctions::validateValveHelper(int group, bool state)
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

            // Check valve is the same
            if(package.value("group").toInt() == group && state == package.value("value").toBool())
            {
                // Failed data to passon
                QVariantMap successPackage;

                // Data to pass on
                successPackage.insert("requested_valve_group", group);
                successPackage.insert("valve_changed", package.value("group").toInt());
                successPackage.insert("requested_state", state);
                successPackage.insert("state", package.value("value").toBool());

                // Emit safe to proceed
                emit emit_validationSuccess(successPackage);

                return;
            }

            // Failed data to passon
            errorDetails.clear();
            errorDetails.insert("message", "The valve failed to update correctly");
            errorDetails.insert("requested_valve_group", group);
            errorDetails.insert("valve_changed", package.value("group").toInt());
            errorDetails.insert("requested_state", state);
            errorDetails.insert("state", package.value("value").toBool());
        }
        else
        {
            errorDetails.clear();
            errorDetails.insert("message", "Validation casting failed");
            errorDetails.insert("requested_valve_group", group);
            errorDetails.insert("requested_state", state);
        }

        // Emit not safe to proceed
        emit emit_validationFailed(errorDetails);
    }



    /**
     * Open valve functions
     *
     * @brief ValveFunctions::openGroupX
     */

    void ValveFunctions::openGroup1()
    {
        valveHelper(1, true);
    }

    void ValveFunctions::openGroup2()
    {
        valveHelper(2, true);
    }

    void ValveFunctions::openGroup3()
    {
        valveHelper(3, true);
    }

    void ValveFunctions::openGroup4()
    {
        valveHelper(4, true);
    }

    void ValveFunctions::openGroup5()
    {
        valveHelper(5, true);
    }

    void ValveFunctions::openGroup6()
    {
        valveHelper(6, true);
    }

    void ValveFunctions::openGroup7()
    {
        //qDebug() << "opening valve 7";
        valveHelper(7, true);
    }

    void ValveFunctions::openGroup8()
    {
        //qDebug() << "opening valve 8";
        valveHelper(8, true);
    }



    /**
     * Close valve functions
     *
     * @brief ValveFunctions::closeGroupX
     */

    void ValveFunctions::closeGroup1()
    {
        valveHelper(1, false);
    }

    void ValveFunctions::closeGroup2()
    {
        valveHelper(2, false);
    }

    void ValveFunctions::closeGroup3()
    {
        valveHelper(3, false);
    }

    void ValveFunctions::closeGroup4()
    {
        valveHelper(4, false);
    }

    void ValveFunctions::closeGroup5()
    {
        valveHelper(5, false);
    }

    void ValveFunctions::closeGroup6()
    {
        valveHelper(6, false);
    }

    void ValveFunctions::closeGroup7()
    {
        valveHelper(7, false);
    }

    void ValveFunctions::closeGroup8()
    {
        valveHelper(8, false);
    }




    /**
     * Validate open valves
     *
     */

    void ValveFunctions::validateOpenGroup1()
    {
        validateValveHelper(1, true);
    }

    void ValveFunctions::validateOpenGroup2()
    {
        validateValveHelper(2, true);
    }

    void ValveFunctions::validateOpenGroup3()
    {
        validateValveHelper(3, true);
    }

    void ValveFunctions::validateOpenGroup4()
    {
        validateValveHelper(4, true);
    }

    void ValveFunctions::validateOpenGroup5()
    {
        validateValveHelper(5, true);
    }

    void ValveFunctions::validateOpenGroup6()
    {
        validateValveHelper(6, true);
    }

    void ValveFunctions::validateOpenGroup7()
    {
        //qDebug() << "validating opne group 7";
        validateValveHelper(7, true);
    }

    void ValveFunctions::validateOpenGroup8()
    {
        //qDebug() << "validating opne group 8";
        validateValveHelper(8, true);
    }




    /**
     * Validate close valves
     *
     */

    void ValveFunctions::validateCloseGroup1()
    {
        validateValveHelper(1, false);
    }

    void ValveFunctions::validateCloseGroup2()
    {
        validateValveHelper(2, false);
    }

    void ValveFunctions::validateCloseGroup3()
    {
        validateValveHelper(3, false);
    }

    void ValveFunctions::validateCloseGroup4()
    {
        validateValveHelper(4, false);
    }

    void ValveFunctions::validateCloseGroup5()
    {
        validateValveHelper(5, false);
    }

    void ValveFunctions::validateCloseGroup6()
    {
        validateValveHelper(6, false);
    }

    void ValveFunctions::validateCloseGroup7()
    {
        validateValveHelper(7, false);
    }

    void ValveFunctions::validateCloseGroup8()
    {
        validateValveHelper(8, false);
    }

}}}}
