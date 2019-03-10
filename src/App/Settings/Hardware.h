#pragma once

// Include external deps
#include <QObject>
#include <QMap>
#include <QVariantMap>
#include <QString>

#include "../Services/JsonFile.h"

namespace App { namespace Settings
{
    class Hardware : public QObject, private Services::JsonFile
    {
        Q_OBJECT

        public:
            Hardware(QObject *parent, QString location);

            void save();

        signals:
            void emit_saved();

        private:
            void read(const QJsonObject &json);
            void write(QJsonObject &json) const;
    };
}}

