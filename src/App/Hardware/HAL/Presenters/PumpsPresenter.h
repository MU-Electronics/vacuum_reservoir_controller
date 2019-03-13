#pragma once

#include <QObject>


#include "PresenterSupport.h"

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{
    class PumpsPresenter : public PresenterSupport
    {

        public:
            PumpsPresenter(QObject *parent);

            // Select the correct presenter
            QVariantMap proccess(QString method, QVariantMap commands, QStringList package);

        signals:

        public slots:


        private:
            QVariantMap enable(QVariantMap commands, QStringList package);
            QVariantMap disable(QVariantMap commands, QStringList package);


    };

}}}}
