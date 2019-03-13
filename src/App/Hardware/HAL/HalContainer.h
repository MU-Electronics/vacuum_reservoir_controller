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

        private:
            // QObject pointer
            QObject* m_parent;

            // Contains for settings container
            Settings::Container* m_settings;

            // Guage hal and presenter
            QSharedPointer<Guages> m_guages;
            QSharedPointer<Presenters::GuagesPresenter> m_guagesPresenter;

            // Emergancy stop hal and presenter

            // Pumps hal and presenter
            QSharedPointer<Guages> m_pumps;
            QSharedPointer<Presenters::GuagesPresenter> m_pumpsPresenter;

            // Remote hal and presenter

            // Temperature sensor hal and presenter

            // Valves hal and presenter

            // Setup objects
            void setupGuages();
            void setupPumps();
    };
}}}
