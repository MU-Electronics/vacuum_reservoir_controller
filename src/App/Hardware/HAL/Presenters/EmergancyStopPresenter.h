#pragma once

#include <QObject>


#include "PresenterSupport.h"

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{
    class EmergancyStopPresenter : public PresenterSupport
    {

        public:
            EmergancyStopPresenter(QObject *parent);

            // Select the correct presenter
            QVariantMap proccess(QString method, QVariantMap commands, QStringList package);

        signals:

        public slots:

        private:
            QVariantMap isPressed(QVariantMap commands, QStringList package);

    };

}}}}
