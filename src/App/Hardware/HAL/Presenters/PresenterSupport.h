#pragma once

#include <QStringList>
#include <QVariantMap>

// Debugging catergories
#include "../../../Services/Debuging.h"

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    class PresenterSupport
    {
        public:
            PresenterSupport();

            bool validatePackageSize(QStringList package, int size);

            QVariantMap extend(QVariantMap commands, QVariantMap package);
            QVariantMap commandId(QVariantMap commands, QVariantMap package);
            QVariantMap methodOverride(QVariantMap commands, QVariantMap package);

    };

}}}}
