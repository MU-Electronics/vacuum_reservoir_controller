#pragma once

#include <QObject>
#include <QVariantMap>

// Debugging catergories
#include "../../Services/Debuging.h"

namespace App { namespace Hardware { namespace HAL
{

    class HALSupport : public QObject
    {
        Q_OBJECT
        public:
            HALSupport(QObject *parent = 0)
                : QObject(parent)
            {

            }

            QVariantMap m_command;

            void setParams(QVariantMap command);

        public slots:
            // virtual void testConnection() = 0;
            // virtual void resetConnection() = 0;
    };

}}}
