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

        signals:

        public slots:


        private:
            QVariantMap enabled(QVariantMap commands, QStringList package);
            QVariantMap disabled(QVariantMap commands, QStringList package);
            QVariantMap readTrip(QVariantMap commands, QStringList package);
            QVariantMap readVacuum(QVariantMap commands, QStringList package);


    };

}}}}
