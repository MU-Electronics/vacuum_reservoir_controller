#pragma once

// Include external deps
#include <QObject>
#include <QMap>
#include <QVariantMap>
#include <QString>

namespace App { namespace Settings
{
    class Container
    {

        public:
            Container(QObject *parent = 0);

       private:
            void loadSettings();
            QString getDir();
    };
}}

