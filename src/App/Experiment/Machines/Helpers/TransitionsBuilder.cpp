#include "TransitionsBuilder.h"

// Include external libs
#include <QObject>
#include <QDebug>
#include <QState>

namespace App { namespace Experiment { namespace Machines { namespace Helpers
{

    TransitionsBuilder::TransitionsBuilder(QObject *parent, Settings::Container *settings, Hardware::Access &hardware,
                                           Functions::EmergancyStopFunctions* emergancy, Functions::ValveFunctions* valve,
                                           Functions::PumpFunctions* pump, Functions::GuageFunctions* guage,
                                           Functions::RemoteFunctions* remote, Functions::TemperatureSensorFunctions* temperature)
        :   QObject(parent)

            // Access the global referances
        ,   m_settings(settings)
        ,   m_hardware(hardware)

            // Functions
        ,   m_emergancyStop(emergancy)
        ,   m_valve(valve)
        ,   m_pump(pump)
        ,   m_guage(guage)
        ,   m_remote(remote)
        ,   m_temperature(temperature)
    {

    }


    /**
     * Open a valve and validate the repsonce
     *
     * @brief TransitionsBuilder::openValve
     * @param open
     * @param openValidate
     * @param finished
     * @param failed
     */
    void TransitionsBuilder::openValve(QState* open,
                                       CommandValidatorState* openValidate,
                                       QState* finished,
                                       QState* failed)
    {
        // Open valve 2
        open->addTransition(&m_hardware, &Hardware::Access::emit_valveOpened, openValidate);
            // Wrong signal was picked up
            openValidate->addTransition(m_valve, &Functions::ValveFunctions::emit_validationWrongId, open);
            // Valve closed successfully
            openValidate->addTransition(m_valve, &Functions::ValveFunctions::emit_validationSuccess, finished);
            // Valve failed to close
            openValidate->addTransition(m_valve, &Functions::ValveFunctions::emit_validationFailed, failed);
    }

    /**
     * Close a valve and validate the repsonce
     *
     * @brief TransitionsBuilder::openValve
     * @param close
     * @param closeValidate
     * @param finished
     * @param failed
     */
    void TransitionsBuilder::closeValve(QState* close,
                                       CommandValidatorState* closeValidate,
                                       QState* finished,
                                       QState* failed)
    {
        // Open valve 2
        close->addTransition(&m_hardware, &Hardware::Access::emit_valveClosed, closeValidate);
            // Wrong signal was picked up
            closeValidate->addTransition(m_valve, &Functions::ValveFunctions::emit_validationWrongId, close);
            // Valve closed successfully
            closeValidate->addTransition(m_valve, &Functions::ValveFunctions::emit_validationSuccess, finished);
            // Valve failed to close
            closeValidate->addTransition(m_valve, &Functions::ValveFunctions::emit_validationFailed, failed);
    }

}}}}
