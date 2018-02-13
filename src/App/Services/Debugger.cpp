#include "Debugger.h"

#include <QFile>
#include <QtDebug>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QLoggingCategory>
#include <QtGlobal>
#include <QObject>


namespace App { namespace Services
{

    /**
     * Constructor for debugger
     *    - sets up the class read for handling debugging messages
     *
     * @brief Debugger::Debugger
     */
    Debugger::Debugger()
    {
        // Always assum setup failed
        success = false;

        // Setup instance
        setup();
    }

    /**
     * Return instance of class (singlton)
     *
     * @brief Debugger::getInstance
     * @return
     */
    Debugger& Debugger::getInstance()
    {
        // Make thread safe
        static QMutex mutex;
        QMutexLocker lock(&mutex);

        static Debugger instance;
        return instance;
    }


    /**
     * Returns the current log file location
     *
     * @brief Debugger::log
     * @return
     */
    QString Debugger::log()
    {
        return Debugger::getInstance().logLocation + "/" +Debugger::getInstance().logFileName;
    }


    /**
     * Returns whether the setup was successfull or not
     *
     * @brief Debugger::wasSuccess
     * @return
     */
    bool Debugger::wasSuccess()
    {
        return Debugger::getInstance().success;
    }


    /**
     * Provides a static inteface for the debug handler
     *
     * @brief Debugger::handlerAccessor
     * @param type
     * @param context
     * @param msg
     */
    void Debugger::handlerAccessor(QtMsgType type, const QMessageLogContext &context, const QString &msg)
    {
        return Debugger::getInstance().handler(type, context, msg);
    }



    /**
     * Debug message handler
     *
     * @brief logOutputHandler
     * @param type
     * @param context
     * @param msg
     */
    void Debugger::handler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
    {
        // Define message container
        QString message = "";
        QMap<QString, QString> messageArray;

        // State debug message
        messageArray.insert("0", "Debugger Message");

        // Get the current date and time
        messageArray.insert("1_Timestamp", QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss"));

        // Append the catergory
        messageArray.insert("2_Category", QString::fromUtf8(context.category));

        // Append the version number
        messageArray.insert("3_Version", QString::number(context.version));

        // Append the file
        messageArray.insert("4_File", QString::fromUtf8(context.file));

        // Append the function
        messageArray.insert("5_Function", QString::fromUtf8(context.function));

        // Append the line number
        messageArray.insert("6_Line", QString::number(context.line));

        // Get error type
        switch (type)
        {
            case QtInfoMsg:
                messageArray.insert("7_Type", "Information");
                break;
            case QtDebugMsg:
                messageArray.insert("7_Type", "Debug");
                break;
            case QtWarningMsg:
                messageArray.insert("7_Type", "Warning");
                break;
            case QtCriticalMsg:
                messageArray.insert("7_Type", "Critical");
                break;
            case QtFatalMsg:
                messageArray.insert("7_Type", "Fatal");
        }

        // Append the message
        messageArray.insert("8_Message", msg);

        // Create message string
        QMapIterator<QString, QString> messageIterator(messageArray);
        while (messageIterator.hasNext())
        {
            // Get data
            messageIterator.next();

            // Create string
            if(messageIterator.key() != "0")
            {
                message += "[" + messageIterator.key() +"] " + messageIterator.value() + "\n";
            }
            else if(messageIterator.key() == "0")
            {
                message += "######## "+ messageIterator.value() + " ########\n";
            }
        }

        // Addtion line ending at end of string
        message += + "\n";

        // Make thread safe
        static QMutex mutex;
        QMutexLocker lock(&mutex);

        // Output to log file
        if(type != QtDebugMsg)
            (*file) << message << "\n\n" << endl;

        // Output to console
        (*console) << message << "\n\n" << endl;

        // If fatel error abort the application
        if (QtFatalMsg == type)
        {
            abort();
        }
        else
        {
            if(type != QtDebugMsg)
                emit Debugger::getInstance().emit_logChanged(messageArray);
        }
    }


    /**
     * Defines the path to store the log and what it's name should be
     *
     * @brief Debugger::logPaths
     */
    void Debugger::logPaths()
    {
        // Get the current date and time and but the log on the end
        logFileName = QDateTime::currentDateTime().toString("yyyy.MM.dd hh.mm.ss") + ".log";

        // Get the OS app data location
        logLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/logs";
    }


    /**
     * Setup the debugger class
     *
     * @brief Debugger::setup
     */
    void Debugger::setup()
    {
        // Setup log location
        logPaths();

        // Get dir info
        QDir dir(logLocation);

        // Create the app data location is it does not exist
        if(!dir.exists())
        {
            QDir().mkpath(logLocation);
        }

        // Open the file
        QFile *log = new QFile(logLocation + "/" + logFileName);

        // Was the file able to be opened with write access?
        if (log->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        {
            // Set filtering and sorting for file list
            dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
            dir.setSorting(QDir::Name | QDir::Reversed);

            // Get file info on log dir
            QFileInfoList list = dir.entryInfoList();

            // If there are more than 10 logs
            if(list.size() > 10)
            {
                // Loop through each log past 10
                for (int i = 10; i < list.size(); ++i)
                {
                    // Get the file info on perticular log
                    QFileInfo fileInfo = list.at(i);

                    // Delete the log file as its old
                    dir.remove(fileInfo.fileName());
                }
            }


            // Create log file streaming instance
            file = new QTextStream(log);

            // Create console streaming instance
            console = new QTextStream( stdout );

            // State success
            success = true;
        }
        else
        {
            // Log file could not be opened, log error to console if one open
            qDebug() << "Error opening log file '" << logFileName << "'" << " In location '" << logLocation << "' .All debug outputs will be redirected to console.";
        }
    }

}}
