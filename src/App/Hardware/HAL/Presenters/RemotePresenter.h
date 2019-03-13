#pragma once

#include <QObject>


#include "PresenterSupport.h"

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{
    class RemotePresenter : public PresenterSupport
    {

        public:
            RemotePresenter(QObject *parent);

            // Select the correct presenter
            QVariantMap proccess(QString method, QVariantMap commands, QStringList package);

        signals:

        public slots:

        private:
            QVariantMap isConnected(QVariantMap commands, QStringList package);

    };

}}}}
