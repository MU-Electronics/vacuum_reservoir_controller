#pragma once

// Include external deps
#include <QObject>
#include <QMap>
#include <QVariantMap>
#include <QString>
#include <QSharedPointer>

// Setting objects
#include "General.h"
#include "Hardware.h"
#include "View.h"

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
            QSharedPointer<Hardware> hardware();
            QSharedPointer<View> view();

       private:
            void loadSettings();
            QString getDir();

            // Settings objects
            QSharedPointer<General> m_general;
            QSharedPointer<Hardware> m_hardware;
            QSharedPointer<View> m_view;
    };
}}

