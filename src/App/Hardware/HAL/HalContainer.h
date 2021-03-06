#pragma once
#include <QObject>
#include <QSharedPointer>

// Include settings container
#include "../../Settings/Container.h"

// Include HALS
#include "EmergancyStop.h"
#include "Guages.h"
#include "Pumps.h"
#include "Remote.h"
#include "TemperatureSensor.h"
#include "Valves.h"

// Include Presenters
#include "Presenters/GuagesPresenter.h"
#include "Presenters/PumpsPresenter.h"
#include "Presenters/ValvesPresenter.h"
#include "Presenters/EmergancyStopPresenter.h"
#include "Presenters/RemotePresenter.h"
#include "Presenters/TemperatureSensorPresenter.h"

namespace App { namespace Hardware { namespace HAL
{
    class HalContainer: public QObject
    {
        Q_OBJECT

        public:
            HalContainer(QObject *parent, Settings::Container *settings);
            ~HalContainer();

            void setup();


            QSharedPointer<Guages> guages()
            {
                return m_guages;
            }

            QSharedPointer<Presenters::GuagesPresenter> guagesPresenter()
            {
                return m_guagesPresenter;
            }


            QSharedPointer<Pumps> pumps()
            {
                return m_pumps;
            }

            QSharedPointer<Presenters::PumpsPresenter> pumpsPresenter()
            {
                return m_pumpsPresenter;
            }


            QSharedPointer<Valves> valves()
            {
                return m_valves;
            }

            QSharedPointer<Presenters::ValvesPresenter> valvesPresenter()
            {
                return m_valvesPresenter;
            }


            QSharedPointer<EmergancyStop> emergancyStop()
            {
                return m_emergancyStop;
            }

            QSharedPointer<Presenters::EmergancyStopPresenter> emergancyStopPresenter()
            {
                return m_emergancyStopPresenter;
            }


            QSharedPointer<Remote> remote()
            {
                return m_remote;
            }

            QSharedPointer<Presenters::RemotePresenter> remotePresenter()
            {
                return m_remotePresenter;
            }


            QSharedPointer<TemperatureSensor> temperatureSensor()
            {
                return m_temperatureSensor;
            }

            QSharedPointer<Presenters::TemperatureSensorPresenter> temperatureSensorPresenter()
            {
                return m_temperatureSensorPresenter;
            }

        private:
            // QObject pointer
            QObject* m_parent;

            // Contains for settings container
            Settings::Container* m_settings;

            // Guage hal and presenter
            QSharedPointer<Guages> m_guages;
            QSharedPointer<Presenters::GuagesPresenter> m_guagesPresenter;

            // Emergancy stop hal and presenter
            QSharedPointer<EmergancyStop> m_emergancyStop;
            QSharedPointer<Presenters::EmergancyStopPresenter> m_emergancyStopPresenter;

            // Pumps hal and presenter
            QSharedPointer<Pumps> m_pumps;
            QSharedPointer<Presenters::PumpsPresenter> m_pumpsPresenter;

            // Remote hal and presenter
            QSharedPointer<Remote> m_remote;
            QSharedPointer<Presenters::RemotePresenter> m_remotePresenter;

            // Temperature sensor hal and presenter
            QSharedPointer<TemperatureSensor> m_temperatureSensor;
            QSharedPointer<Presenters::TemperatureSensorPresenter> m_temperatureSensorPresenter;

            // Valves hal and presenter
            QSharedPointer<Valves> m_valves;
            QSharedPointer<Presenters::ValvesPresenter> m_valvesPresenter;

            // Setup objects
            void setupGuages();
            void setupPumps();
            void setupValves();
            void setupEmergancyStop();
            void setupRemote();
            void setuptemperatureSensor();
    };
}}}
