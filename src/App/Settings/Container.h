#pragma once

// Include external deps
#include <QObject>
#include <QMap>
#include <QVariantMap>
#include <QString>
#include <QSharedPointer>

// Setting objects
#include "General.h"

namespace App { namespace Settings
{
    class Container: public QObject
    {
        Q_OBJECT

        public:
            Container(QObject *parent = nullptr);
            ~Container();

            // Getters for setting objects
            QSharedPointer<General> general();

       private:
            void loadSettings();
            QString getDir();

            // Settings objects
             QSharedPointer<General> m_general;
    };
}}

