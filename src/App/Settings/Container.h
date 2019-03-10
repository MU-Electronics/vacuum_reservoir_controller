#pragma once

// Include external deps
#include <QObject>
#include <QMap>
#include <QVariantMap>
#include <QString>

namespace App { namespace Settings
{
    class Container: public QObject
    {
        Q_OBJECT

        public:
            Container(QObject *parent = nullptr);
            ~Container();

            void test()
            {

            }
       private:
            void loadSettings();
            QString getDir();
    };
}}

