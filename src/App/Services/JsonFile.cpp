#include "JsonFile.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

namespace App { namespace Services
{
    JsonFile::JsonFile()
    {

    }

    /**
     * Load the settings from a file
     *
     * @brief Settings::load
     * @param location
     * @param format
     * @return bool
     */
    bool JsonFile::load(QString location, SaveFormat format)
    {
        // Cache data
        m_loadedFile = location;
        m_format = format;

        // Load the file
        QFile loadFile(m_loadedFile);

        // Open the file and return error if failed
        if (!loadFile.open(QIODevice::ReadOnly))
        {
            // Log error
            qCCritical(jsonFileService) << "Couldn't open file to read in " << location;

            return false;
        }

        // Get the file data
        QByteArray saveData = loadFile.readAll();

        // Load the document into the json object from json or binary stored format
        QJsonDocument loadDoc(m_format == Json
            ? QJsonDocument::fromJson(saveData)
            : QJsonDocument::fromBinaryData(saveData));

        // Get the object
        auto object = loadDoc.object();

        // Trigger the read method
        read(object);

        return true;
    }


    /**
     * Update the loaded file with the new data
     *
     * @brief Settings::update
     * @return bool
     */
    bool JsonFile::update()
    {
        // Load the file
        QFile saveFile(m_loadedFile);

        // Open the file and error if failed
        if (!saveFile.open(QIODevice::WriteOnly))
        {
            // Log error
            qCCritical(jsonFileService) << "Couldn't open file to save in " << m_loadedFile;

            return false;
        }

        // Create a json object
        QJsonObject object;

        // Add data to the json object
        write(object);

        // Create a json format document
        QJsonDocument saveDoc(object);

        // Save the file in the correct format
        saveFile.write(m_format == Json
            ? saveDoc.toJson(QJsonDocument::Indented)
            : saveDoc.toBinaryData());

        return true;
    }

}}
