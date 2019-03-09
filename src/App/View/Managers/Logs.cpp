#include "Logs.h"

// Include external libs
#include <QDebug>

#include "../../Services/Debugger.h"
#include <QQmlApplicationEngine>
#include <QObject>
#include <QQmlContext>
#include <QQmlEngine>
#include <QRegularExpression>
#include <QTimer>

namespace App { namespace View { namespace Managers
{

    /**
     * Configure the view manager
     *
     * @brief Global::Global
     * @param parent
     * @param root
     * @param settings
     * @param experimentEngine
     */
    Logs::Logs(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings)
        :   QObject(parent)
        ,   m_root(root)
        ,   m_settings(settings)

    {
        // Set current log
        currentLog(0);
    }


    Logs::~Logs()
    {

    }

    void Logs::selectLog(QString file)
    {
        // Get log
        QVector<QStringList> logInit = Services::Debugger::getInstance().getLog(file);

        // Define list
        QVariantList listModel;

        // Newest first
        QVector<QStringList> log;
        for (int i = 0; i < logInit.size(); ++i)
        {
            // Get log line
            QStringList data = logInit.at(i);

            // Format item into map
            QVariantMap item;
            for (auto line : data)
            {
                // Find name
                QStringList parts = line.split(QRegularExpression("(\\] )"));

                // Check for title
                if(parts.count() == 1)
                {
                    item["type"] = line;
                    continue;
                }

                // Remove characters
                parts[0].remove(0, 3);

                // Set title and info
                item[parts[0].toLower()] = parts[1].simplified();
            }

            listModel.insert(listModel.begin(), item);
        }

        qDebug() << listModel;

        // Update view
        logData(listModel);

        // Remove .log from file name
        file.chop(4);

        // Name the log
        QString currentInitLog = logList().first().toMap().value("filename").toString();
        currentInitLog.chop(4);
        viewingLog({(currentInitLog == file) ? "Current Instance" : file });
    }


    void Logs::getLogs()
    {
        // Define list
        QVariantList listModel;

        // Get logs
        QVector<QString> logs = Services::Debugger::getInstance().listLogs();

        // Format log
        for(auto log : logs)
        {
            QVariantMap logInfo;
            logInfo["filename"] = log;
            log.chop(4);
            logInfo["name"]  = (listModel.empty()) ? "Current Instance" : log;
            listModel.append(logInfo);
        }

        logList(listModel);
    }


    /**
     * Connect signals from and to this class and the hardware & safety thread
     *
     * @brief Logs::makeConnections
     * @param hardware
     * @param safety
     */
    void Logs::makeConnections()
    {
        // Get list of all log files
        getLogs();

        // After application has become stable retreive logs for current instance (1st log in list)
        QTimer::singleShot(1000, [this]() {
            QString firstLog = logList().first().toMap().value("filename").toString();
            selectLog(firstLog);
        });
    }

}}}

