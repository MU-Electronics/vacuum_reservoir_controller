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

            QMap<int, QList<double>> m_vacuum;

        signals:

        public slots:


        private:
            QVariantMap enabled(QVariantMap commands, QStringList package);
            QVariantMap disabled(QVariantMap commands, QStringList package);
            QVariantMap readTrip(QVariantMap commands, QStringList package);
            QVariantMap readVacuum(QVariantMap commands, QStringList package);

            double vacuumAverage(int id, double pressure)
            {
                // Only append
                if(m_vacuum[id].count() > 4)
                    m_vacuum[id].pop_front();

                // Append value
                m_vacuum[id].append(pressure);

                // Find average
                double value = 0;
                for(auto i : m_vacuum[id])
                {
                    value += i;
                }
                value = value / 4;

                // Return the average
                return value;
            }


    };

}}}}

