#pragma once

// Include external libs
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QState>

// Include settings container
#include "../../../Settings/Container.h"

// Include threads
#include "../../../Hardware/Access.h"

// Include validator state
#include "CommandValidatorState.h"

// Include functions
#include "../Functions/ValveFunctions.h"
#include "../Functions/PumpFunctions.h"
#include "../Functions/EmergancyStopFunctions.h"
#include "../Functions/GuageFunctions.h"
#include "../Functions/RemoteFunctions.h"
#include "../Functions/TemperatureSensorFunctions.h"


namespace App { namespace Experiment { namespace Machines { namespace Helpers
{

    class TransitionsBuilder: public QObject
    {
        Q_OBJECT
        public:
            TransitionsBuilder(QObject *parent, Settings::Container *settings, Hardware::Access &hardware,
                               Functions::EmergancyStopFunctions* emergnacyStop, Functions::ValveFunctions* valve,
                               Functions::PumpFunctions* pump, Functions::GuageFunctions* guage,
                               Functions::RemoteFunctions* remote, Functions::TemperatureSensorFunctions* temperature);

            // Valve related transistions
            void openValve(QState* open, CommandValidatorState* openValidate,
                           QState* finished, QState* failed);
            void closeValve(QState* close, CommandValidatorState* closeValidate,
                           QState* finished, QState* failed);
            void closeAllChambers(QState* closeChamber1,
                                CommandValidatorState* closeChamber1Validate,
                                QState* closeChamber2,
                                CommandValidatorState* closeChamber2Validate,
                                QState* closeChamber3,
                                CommandValidatorState* closeChamber3Validate,
                                QState* closeChamber4,
                                CommandValidatorState* closeChamber4Validate,
                                QState* closeChamber5,
                                CommandValidatorState* closeChamber5Validate,
                                QState* closeChamber6,
                                CommandValidatorState* closeChamber6Validate,
                                QState* finished,
                                QState* failed);

        signals:


        public slots:


        private:
            // Holds the application settings
            Settings::Container* m_settings;

            // Hold the hardware gateway
            Hardware::Access &m_hardware;

            // Functions
            Functions::EmergancyStopFunctions* m_emergancyStop;
            Functions::ValveFunctions* m_valve;
            Functions::PumpFunctions* m_pump;
            Functions::GuageFunctions* m_guage;
            Functions::RemoteFunctions* m_remote;
            Functions::TemperatureSensorFunctions* m_temperature;
    };

}}}}
