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

           // HAL objects

           // HAL Presenters

    {
        // Set possable methods to be ran within this class via the queue
        // None atm set like this: m_avaliableMethods.append("<method name>");

        // Guage settings
        auto settingsGuages = m_settings->hardware()->guages();

        // Device settings for guage 1 ADC
        QutiPi::Drivers::I2C::Device guageReadings_1;
        guageReadings_1.location = settingsGuages["bus"].toString().toStdString();
        guageReadings_1.address = char(settingsGuages["adc_id_1-4"].toInt());

        // Device settings for guage 2 ADC
        QutiPi::Drivers::I2C::Device guageReadings_2;
        guageReadings_2.location = settingsGuages["bus"].toString().toStdString();
        guageReadings_2.address = char(settingsGuages["adc_id_5-8"].toInt());

        // Device settings for guage 1 gpio expander
        QutiPi::Drivers::I2C::Device guageGPIOExpander_1;
        guageGPIOExpander_1.location = settingsGuages["bus"].toString().toStdString();
        guageGPIOExpander_1.address = char(settingsGuages["gpio_id_1-4"].toInt());

        // Device settings for guage 2 gpio expander
        QutiPi::Drivers::I2C::Device guageGPIOExpander_2;
        guageGPIOExpander_2.location = settingsGuages["bus"].toString().toStdString();
        guageGPIOExpander_2.address = char(settingsGuages["gpio_id_5-8"].toInt());

        // ADC mapping guage 1
        auto guageADCSettings_1 = settingsGuages["mappings_1-4"].toMap();
        QMap<QString, int> guageADC_1;
        guageADC_1["1"] =  guageADCSettings_1["1"].toInt();
        guageADC_1["2"] =  guageADCSettings_1["2"].toInt();
        guageADC_1["3"] =  guageADCSettings_1["3"].toInt();
        guageADC_1["4"] =  guageADCSettings_1["4"].toInt();

        // ADC mapping guage 2
        auto guageADCSettings_2 = settingsGuages["mappings_5-8"].toMap();
        QMap<QString, int> guageADC_2;
        guageADC_2["1"] =  guageADCSettings_2["1"].toInt();
        guageADC_2["2"] =  guageADCSettings_2["2"].toInt();
        guageADC_2["3"] =  guageADCSettings_2["3"].toInt();
        guageADC_2["4"] =  guageADCSettings_2["4"].toInt();


        // Guage 1 trip mappings
        auto guageTripeSettings_1 = settingsGuages["gpio_trips_1-4"].toMap();
        QMap<QString, PinName> guageTrip_1;
        guageTrip_1["1"] = static_cast<PinName>(guageTripeSettings_1["1"].toInt());
        guageTrip_1["2"] = static_cast<PinName>(guageTripeSettings_1["2"].toInt());
        guageTrip_1["3"] = static_cast<PinName>(guageTripeSettings_1["3"].toInt());
        guageTrip_1["4"] = static_cast<PinName>(guageTripeSettings_1["4"].toInt());

        // Guage 2 trip mappings
        auto guageTripeSettings_2 = settingsGuages["gpio_trips_5-8"].toMap();
        QMap<QString, PinName> guageTrip_2;
        guageTrip_2["1"] = static_cast<PinName>(guageTripeSettings_2["1"].toInt());
        guageTrip_2["2"] = static_cast<PinName>(guageTripeSettings_2["2"].toInt());
        guageTrip_2["3"] = static_cast<PinName>(guageTripeSettings_2["3"].toInt());
        guageTrip_2["4"] = static_cast<PinName>(guageTripeSettings_2["4"].toInt());


        // Setup guage
        m_guage = new HAL::Guages(parent, guageReadings_1, guageReadings_2, guageGPIOExpander_1, guageGPIOExpander_2,
                                    guageADC_1, guageADC_2, guageTrip_1, guageTrip_2);
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
        // connect(&m_guage, &HAL::Guages::emit_pressureData, this, &Access::proccessDataFromHals);

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
        if(responable == "EmergancyStop")
            // package = m_emergancyStop.proccess(method, commands, halData);

        // Guages presenter
        if(responable == "Guages")
            // package = m_guages.proccess(method, commands, halData);

        // Pumps presenter
        if(responable == "Pumps")
            // package = m_pumps.proccess(method, commands, halData);

        // Remote presenter
        if(responable == "Remote")
            // package = m_remote.proccess(method, commands, halData);

        // Temperature Sensor presenter
        if(responable == "TemperatureSensor")
            // package = m_temperatureSensor.proccess(method, commands, halData);

        // Valves presenter
        if(responable == "Valves")
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
            if(/*(!m_guages.busFree() || !m_guages.isOpen()) &&*/ method != "resetConnection")
            {
                // Re add the method to the queue as this one will be removed
                m_queue.enqueue(command);

                // Log the issue only to console
                //qCDebug(halAccess) << "Command was relisted due to bus issue: " << command;

                // Return back to worker for next method
                return true;
            }

            // Set the method params
            // m_guages.setParams(command);

            // Run the method in the HAL and cache the status
            // status["resulting_status"] = (QMetaObject::invokeMethod(&m_guages, method.toLatin1().data(), Qt::DirectConnection)) ? true : false;

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
