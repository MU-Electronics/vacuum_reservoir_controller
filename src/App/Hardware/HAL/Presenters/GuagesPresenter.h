#pragma once

#include <QObject>


#include "PresenterSupport.h"

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{
    class GuagesPresenter : public PresenterSupport
    {

        public:
            GuagesPresenter(QObject *parent);

            // Select the correct presenter
            QVariantMap proccess(QString method, QVariantMap commands, QStringList package);

            QMap<int, QList<double>> m_voltage;
            QMap<int, QList<double>> m_vacuum;

        signals:

        public slots:


        private:
            double toleranceUpper = 0.15;
            double toleranceLower = 0.15;

            int largeDrop = 0;

            QVariantMap enabled(QVariantMap commands, QStringList package);
            QVariantMap disabled(QVariantMap commands, QStringList package);
            QVariantMap readTrip(QVariantMap commands, QStringList package);
            QVariantMap readVacuum(QVariantMap commands, QStringList package);

            double vacuumAverage(int id, double pressure)
            {
                // Only append
                if(m_vacuum[id].count() >= 4)
                    m_vacuum[id].pop_front();

                // Append value
                m_vacuum[id].append(pressure);

                // Find average
                double value = 0;
                for(auto i : m_vacuum[id])
                {
                    value += i;
                }
                value = value / m_vacuum[id].count();

                // Return the average
                return value;
            }

            double vacuumAverage(int id)
            {
                // Find average
                double value = 0;
                for(auto i : m_vacuum[id])
                {
                    value += i;
                }
                value = value / m_vacuum[id].count();

                // Return the average
                return value;
            }

            double voltageAverage(int id, double pressure)
            {
                // Only append
                if(m_voltage[id].count() >= 4)
                    m_voltage[id].pop_front();

                // Append value
                m_voltage[id].append(pressure);

                // Find average
                double value = 0;
                for(auto i : m_voltage[id])
                {
                    value += i;
                }
                value = value / m_voltage[id].count();

                // Return the average
                return value;
            }


            double voltageAverage(int id)
            {
                // Find average
                double value = 0;
                for(auto i : m_voltage[id])
                {
                    value += i;
                }
                value = value / m_voltage[id].count();

                // Return the average
                return value;
            }


    };

}}}}

