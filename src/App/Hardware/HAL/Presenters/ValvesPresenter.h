#pragma once

#include <QObject>


#include "PresenterSupport.h"

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{
    class ValvesPresenter : public PresenterSupport
    {

        public:
            ValvesPresenter(QObject *parent);

            // Select the correct presenter
            QVariantMap proccess(QString method, QVariantMap commands, QStringList package);

        signals:

        public slots:


        private:
            QVariantMap opened(QVariantMap commands, QStringList package);
            QVariantMap closed(QVariantMap commands, QStringList package);


    };

}}}}
