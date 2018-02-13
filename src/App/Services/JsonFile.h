#pragma once

#include <QObject>
#include <QString>
#include <QDebug>
#include <QJsonObject>

// Debugging catergories
#include "../Services/Debuging.h"

namespace App { namespace Services {
    class JsonFile
    {

    public:
        JsonFile();
        //virtual ~JsonFile() {};

        enum SaveFormat {
            Json, Binary
        };

        QString m_loadedFile;
        SaveFormat m_format;

        bool load(QString location, SaveFormat format);
        bool update();

        virtual void read(const QJsonObject &json) = 0;
        virtual void write(QJsonObject &json) const = 0;
    };
}}


