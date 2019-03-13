#pragma once

#include <QObject>

#include "PresenterSupport.h"

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{
    class TemperatureSensorPresenter : public PresenterSupport
    {

        public:
            TemperatureSensorPresenter(QObject *parent);

            // Select the correct presenter
            QVariantMap proccess(QString method, QVariantMap commands, QStringList package);

        signals:

        public slots:

        private:
            QVariantMap getTemperature(QVariantMap commands, QStringList package);

    };

}}}}
