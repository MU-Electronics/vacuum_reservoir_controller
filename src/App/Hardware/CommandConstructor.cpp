#include "CommandConstructor.h"

#include <QDateTime>

namespace App { namespace Hardware
{
    CommandConstructor::CommandConstructor(QObject *parent)
    {
        // Seed Qrand
        qsrand(QDateTime::currentMSecsSinceEpoch() / 1000);
    }


    /**
     * Class detructure performs:
     *      - Nothing atm
     *
     * @brief CommandConstructor::~CommandConstructor
     */
    CommandConstructor::~CommandConstructor()
    {

    }



    /**
     * Gives each command an id from 1-999999
     *
     * @brief CommandConstructor::generateCommandCount
     * @return
     */
    QString CommandConstructor::generateCommandCount()
    {
        // Allowed characters
        const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

        // Length of hash
        const int randomStringLength = 12;

        // Container for hash
        QString randomString;

        // Generate hash
        for(int i=0; i < randomStringLength; ++i)
        {
            // Get random index
            int index = qrand() % possibleCharacters.length();

            // Get correct character
            QChar nextChar = possibleCharacters.at(index);

            // Append character to hash container
            randomString.append(nextChar);
        }

        // Return the hash
        return randomString;
    }





    // Emergancy stop commands
    QVariantMap CommandConstructor::emergancyStopIsPressed()
    {
        // Create command for HAL
        QVariantMap command;

        // Give the command a unique id
        command.insert("command_identifier", generateCommandCount());

        // Return the package
        return command;
    }





    // Guage commands
    QVariantMap CommandConstructor::guageEnabled(int group)
    {
        // Create command for HAL
        QVariantMap command;

        // Give the command a unique id
        command.insert("command_identifier", generateCommandCount());

        // Return the package
        return command;
    }

    QVariantMap CommandConstructor::guageDisabled(int group)
    {
        // Create command for HAL
        QVariantMap command;

        // Give the command a unique id
        command.insert("command_identifier", generateCommandCount());

        // Return the package
        return command;
    }

    QVariantMap CommandConstructor::guageReadVacuum(int group)
    {
        // Create command for HAL
        QVariantMap command;

        // Give the command a unique id
        command.insert("command_identifier", generateCommandCount());

        // Return the package
        return command;
    }

    QVariantMap CommandConstructor::guageReadTrip(int group)
    {
        // Create command for HAL
        QVariantMap command;

        // Give the command a unique id
        command.insert("command_identifier", generateCommandCount());

        // Return the package
        return command;
    }





    // Pump commands
    QVariantMap CommandConstructor::pumpEnable(int pumpId)
    {
        // Create command for HAL
        QVariantMap command;

        // Give the command a unique id
        command.insert("command_identifier", generateCommandCount());

        // Return the package
        return command;
    }

    QVariantMap CommandConstructor::pumpDisable(int pumpId)
    {
        // Create command for HAL
        QVariantMap command;

        // Give the command a unique id
        command.insert("command_identifier", generateCommandCount());

        // Return the package
        return command;
    }


    // Remote commands
    QVariantMap CommandConstructor::remoteIsConnected()
    {
        // Create command for HAL
        QVariantMap command;

        // Give the command a unique id
        command.insert("command_identifier", generateCommandCount());

        // Return the package
        return command;
    }





    // Temperature sensor commands
    QVariantMap CommandConstructor::temperatureSensorGetTemperature(QString location)
    {
        // Create command for HAL
        QVariantMap command;

        // Give the command a unique id
        command.insert("command_identifier", generateCommandCount());

        // Return the package
        return command;
    }





    // Valve commands
    QVariantMap CommandConstructor::valveOpen()
    {
        // Create command for HAL
        QVariantMap command;

        // Give the command a unique id
        command.insert("command_identifier", generateCommandCount());

        // Return the package
        return command;
    }

    QVariantMap CommandConstructor::valveClose()
    {
        // Create command for HAL
        QVariantMap command;

        // Give the command a unique id
        command.insert("command_identifier", generateCommandCount());

        // Return the package
        return command;
    }


}}
