#include "Errors.h"

// Include external libs
#include <QDebug>
#include <QProcess>

#include "../../Services/Debugger.h"


namespace App { namespace View { namespace Managers
{

    /**
     * Configure the view manager
     *
     * @brief Errors::Errors
     * @param parent
     * @param root
     * @param settings
     * @param experimentEngine
     */
    Errors::Errors(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings)
        :   QObject(parent)
        ,   m_root(root)
        ,   m_settings(settings)
    {
        // Pad out error
        m_visualErrorSettings["colour"] = "#fff542";
        m_visualErrorSettings["title"] = "";
        m_visualErrorSettings["message"] = "";
        m_visualErrorSettings["status"] = false;


        // Errors to show on screen as popup
        m_visualErrors.insert("power.main", true);
    }


    Errors::~Errors()
    {

    }

    /**
     * Connect signals from and to this class and the hardware & safety thread
     *
     * @brief Errors::makeConnections
     * @param hardware
     * @param safety
     */
    void Errors::makeConnections()
    {
        // Listen for debugging messages
        connect(&Services::Debugger::getInstance(), &Services::Debugger::emit_logChanged, this, &Errors::logChanged);
    }


    /**
     * Triggered when a new debugging log is added
     *
     * @brief Errors::logChanged
     * @param message
     */
    void Errors::logChanged(QMap<QString, QString> message)
    {
        // Add message to list
        m_debugMessages.append(message);

        // Update every one
        emit emit_debugMessagesChanged(m_debugMessages);

        // Check for visual error popup required
        auto check = m_visualErrors.find(message["2_Category"]);
        if(check != m_visualErrors.end() && check.value() == true)
        {
            // Set the alert colour
            m_visualErrorSettings["bg_colour"] = "#fff542";
            m_visualErrorSettings["tx_colour"] = "#bab22e";

            // Set the title of the error
            m_visualErrorSettings["title"] = message["0"];

            // Set the message of the error
            auto formattedMessage = message["8_Message"];
            if(formattedMessage.at(0) == "\"" && formattedMessage.at(formattedMessage.size() - 1) == "\"")
                formattedMessage = formattedMessage.mid(1, formattedMessage.size() - 2);
            m_visualErrorSettings["message"] = formattedMessage;

            // Set status
            m_visualErrorSettings["status"] = true;

            // Update view
            emit emit_visualErrorSettingsChanged(m_visualErrorSettings);
        }
    }


    /**
     * Generates the debug message from the array of messages
     *
     * @brief Errors::debugMessages
     * @return
     */
    QString Errors::debugMessages()
    {
        QString message = "";
        for (int i = 0; i < m_debugMessages.size(); ++i)
        {
            QString color = ( i % 2 == 0)? "white" : "#9b9b9b" ;

            message += "<div>";
            // Create message string
            QMapIterator<QString, QString> messageIterator(m_debugMessages[i]);
            while (messageIterator.hasNext()) {
                // Get data
                messageIterator.next();

                // Create string
                if(messageIterator.key() != "0")
                {
                    message += "<font color='#ed6110' size='3'>[" + messageIterator.key() +"]</font> <font color='#0054fc' size='3'>" + messageIterator.value() + "</font><br>";
                }
                else if(messageIterator.key() == "0")
                {
                    message += "<b><font color='#fc4300' size='4'>" + QString::number(i + 1) + ": " + messageIterator.value() + "</font></b><br>";
                }
            }

            // Clsoe div and addtion break at end of string
            message += "</div>";
        }

        return message;
    }



    /**
     * Logs that the user acknowledged an visual error
     *
     * @brief Errors::acknowledgeError
     */
    void Errors::acknowledgeError()
    {
        // Set status
        m_visualErrorSettings["status"] = false;

        // Update view
        emit emit_visualErrorSettingsChanged(m_visualErrorSettings);

        // Log that the error was accepted
        auto message = "User acknowledge a '" + m_visualErrorSettings["title"].toString() + "' of '" + m_visualErrorSettings["message"].toString() + "'";
        qCInfo(userAcknowledge) << message;
    }


}}}

