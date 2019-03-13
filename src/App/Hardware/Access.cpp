#include "Access.h"

// External deps
#include <QDebug>


namespace App { namespace Hardware
{

    /**
     * Class constructure performs:
     *      - Set parent thread class
     *      - Set settings member variable
     *
     * @brief Monitor::Monitor
     * @param parent
     */
    Access::Access(QObject *parent, Settings::Container *settings)
        :   Thread(parent, false, false)

           // System settings container
        ,    m_settings(settings)

           // Timer for time based events
        ,    m_timer(*new QTimer(parent))

            // Hal container
        ,   m_halContainer(*new HAL::HalContainer(parent, settings))

           // HAL Presenters

    {
        // Set possable methods to be ran within this class via the queue
        // None atm set like this: m_avaliableMethods.append("<method name>");
    }


    Access::~Access()
    {
        // Close connectors on hals
    }



    /**
     * Allows for any configurations to be ran before the worker is set going
     *
     * @brief Access::configure
     */
    void Access::configure(QThread &thread)
    {
        // Connect EmergancyStop HAL connections

        // Connect Guages HAL connections
        connect(m_halContainer.guages().data(), &HAL::Guages::emit_guageData, this, &Access::proccessDataFromHals);

        // Connect Pumps HAL connections

        // Connect Remote HAL connections

        // Connect TemperatureSensor HAL connections

        // Connect Valvs HAL connections


        // Open com port for all devices
        connectDevices();
    }



    /**
     * Initial work required before using devices
     *
     * @brief Access::connectDevices
     */
    void Access::connectDevices()
    {


    }


    /**
     * Takes data from the HALs converts it to useable data via the HAL presenters and
     * then inkokes the correct slot+signal for the rest of the application to connect to
     *
     * @brief Access::proccessDataFromHals
     * @param responable
     * @param method
     * @param halData
     */
    void Access::proccessDataFromHals(QString responable, QString method, QVariantMap commands, QStringList halData)
    {
        // Hold the presenters data
        QVariantMap package;

        // Format the data from the HAL package to useable data for the rest of the application
        // EmergancyStop presenter
        //if(responable == "EmergancyStop")
            // package = m_emergancyStop.proccess(method, commands, halData);

        // Guages presenter
        if(responable == "Guages")
            package = m_halContainer.guagesPresenter()->proccess(method, commands, halData);

        // Pumps presenter
        //if(responable == "Pumps")
            // package = m_pumps.proccess(method, commands, halData);

        // Remote presenter
        //if(responable == "Remote")
            // package = m_remote.proccess(method, commands, halData);

        // Temperature Sensor presenter
        //if(responable == "TemperatureSensor")
            // package = m_temperatureSensor.proccess(method, commands, halData);

        // Valves presenter
        //if(responable == "Valves")
            // package = m_valves.proccess(method, commands, halData);

        // Once the data is formatted run the correct signal
        if(!package.isEmpty() && !package["method"].isNull())
            // Trigger the correct access class signal
            QMetaObject::invokeMethod(this, package["method"].toString().toLatin1().data(), Qt::DirectConnection, Q_ARG( QVariantMap, package ));
    }


    /**
     * This slot allows other proccess to add commands to be ran into the queue
     *
     * @brief Access::hardwareAccess
     * @param method
     * @param commands
     */
    void Access::hardwareAccess(QVariantMap command)
    {
        // Check the require information is provided
        if(command.contains("hardware") && command.contains("method"))
        {
            // Add to the queue
            m_queue.enqueue(command);
        }
    }


    /**
     * The thread worker that will be in a forever loop untill stopped
     * via the thread_abort parent member variable
     *
     * @brief Monitor::worker
     */
    void Access::worker()
    {
       // Check if any commands need to be ran
       if(!m_queue.isEmpty())
       {
           // Run the command
           bool needsPause = executeHalMethods(m_queue.head());

           // Remove item from queue
           m_queue.dequeue();

           // Cycle immediate
           if(!needsPause)
               return;
       }

       // Sleep the thread for a while; this reduces cpu usage massivly 15-20%!
       thread()->usleep(500);
   }


    /**
     * Runs the correct method in the correct HAL object for a given command object
     *
     * @brief Access::executeHalMethods
     * @param command
     */
    bool Access::executeHalMethods(QVariantMap command)
    {
        // Get the hardware and method
        QString hardware = command.value("hardware").toString();
        QString method = command.value("method").toString();

        // Status package
        QVariantMap status;

        // Attach current commands to status package
        QMapIterator<QString, QVariant> i(command);

        // While iterator has data loop
        while (i.hasNext())
        {
            // Move to next record
            i.next();

            // Store the current record data
            status[i.key()] = i.value();
        }

        // Set results to be false as we have none yet
        status["resulting_status"] = false;

        // Find the correct HAL
        if(hardware == "EmergancyStop")
        {

        }
        else if(hardware == "Guages")
        {
            // If the bus is not free we cant procceed
            if(/*(!m_guages.busFree() || !m_guages.isOpen()) && method != "resetConnection"*/ false)
            {
                // Re add the method to the queue as this one will be removed
                m_queue.enqueue(command);

                // Log the issue only to console
                //qCDebug(halAccess) << "Command was relisted due to bus issue: " << command;

                // Return back to worker for next method
                return true;
            }

            // Set the method params
            m_halContainer.guages().data()->setParams(command);

            // Run the method in the HAL and cache the status
            status["resulting_status"] = (QMetaObject::invokeMethod(m_halContainer.guages().data(), method.toLatin1().data(), Qt::DirectConnection)) ? true : false;

        }
        else if(hardware == "Pumps")
        {

        }
        else if(hardware == "Remote")
        {

        }
        else if(hardware == "TemperatureSensor")
        {

        }
        else if(hardware == "Valves")
        {

        }
        else if (hardware == "AccessLayer") // Call internal method of Access
        {
            // If command is allowed to be ran
            if(m_avaliableMethods.contains(method))
            {
                // Store command data
                m_lastcommands = command;

                // Allow certain methods in this call to be called, main for management of USB connections
                status["resulting_status"] = (QMetaObject::invokeMethod(this, method.toLatin1().data(), Qt::DirectConnection)) ? true : false;
            }
            else
            {
                // Error
                //qCWarning(halAccess) << "Method not allow or does not exsist in Access class: " << command;

                // Failed to run method not allowed
                status["resulting_status"]  = false;

                return true;
            }
        }

        // Emit the result
        emit_methodAttemptResults(status);

        // No need for time out
        return false;
    }


}}
