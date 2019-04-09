#include "HalContainer.h"

#include <QMap>
#include <QList>

namespace App { namespace Hardware { namespace HAL
{

        HalContainer::HalContainer(QObject *parent, Settings::Container *settings)
            :   QObject(parent)
            ,   m_parent(parent)
            ,   m_settings(settings)
        {
            //auto HAL::Presenters::GuagesPresenter::m_vacuum = {};

            setup();
        }

        HalContainer::~HalContainer()
        {

        }


        /**
         * Setup all the hals & presenters
         *
         * @brief HalContainer::setup
         */
        void HalContainer::setup()
        {
            setupGuages();
            setupPumps();
            setupValves();
            setupEmergancyStop();
            setuptemperatureSensor();
            setupRemote();
        }


        void HalContainer::setuptemperatureSensor()
        {
            auto setting = m_settings->hardware()->temperatureSensor();

            // Device settings for temperature sensor ADC
            QutiPi::Drivers::I2C::Device sensor;
            sensor.location = setting["bus"].toString().toStdString();
            sensor.address = char(setting["qutipi"].toInt());

            // Setup objects
            m_temperatureSensor = QSharedPointer<TemperatureSensor>(new HAL::TemperatureSensor(m_parent, sensor));

            // Setup presenter
            m_temperatureSensorPresenter = QSharedPointer<Presenters::TemperatureSensorPresenter>(new HAL::Presenters::TemperatureSensorPresenter(m_parent));
        }


        void HalContainer::setupRemote()
        {
            auto setting = m_settings->hardware()->remote();

            // GPIO pins
            PinName pin = static_cast<PinName>(setting["gpio"].toInt());

            // Setup objects
            m_remote = QSharedPointer<Remote>(new HAL::Remote(m_parent, pin));

            // Setup presenter
            m_remotePresenter = QSharedPointer<Presenters::RemotePresenter>(new HAL::Presenters::RemotePresenter(m_parent));
        }


        void HalContainer::setupEmergancyStop()
        {
            auto setting = m_settings->hardware()->emergencyStop();

            // GPIO pins
            PinName pin = static_cast<PinName>(setting["gpio"].toInt());

            // Setup objects
            m_emergancyStop = QSharedPointer<EmergancyStop>(new HAL::EmergancyStop(m_parent, pin));

            // Setup presenter
            m_emergancyStopPresenter = QSharedPointer<Presenters::EmergancyStopPresenter>(new HAL::Presenters::EmergancyStopPresenter(m_parent));
        }


        void HalContainer::setupValves()
        {
            // Valves settings
            auto settingsValve = m_settings->hardware()->valves();
            auto mappings = settingsValve["mappings"].toMap();

            // GPIO pins
            PinName valve_1 = static_cast<PinName>(mappings["1"].toInt());
            PinName valve_2 = static_cast<PinName>(mappings["2"].toInt());
            PinName valve_3 = static_cast<PinName>(mappings["3"].toInt());
            PinName valve_4 = static_cast<PinName>(mappings["4"].toInt());
            PinName valve_5 = static_cast<PinName>(mappings["5"].toInt());
            PinName valve_6 = static_cast<PinName>(mappings["6"].toInt());
            PinName valve_7 = static_cast<PinName>(mappings["7"].toInt());
            PinName valve_8 = static_cast<PinName>(mappings["8"].toInt());

            // Setup objects
            m_valves = QSharedPointer<Valves>(new HAL::Valves(m_parent, valve_1, valve_2, valve_3, valve_4, valve_5,
                                                              valve_6, valve_7, valve_8));

            // Setup presenter
            m_valvesPresenter = QSharedPointer<Presenters::ValvesPresenter>(new HAL::Presenters::ValvesPresenter(m_parent));
        }


        void HalContainer::setupPumps()
        {
            // Pump settings
            auto settingsPump = m_settings->hardware()->pumps();

            // GPIO pins
            PinName pump_1 = static_cast<PinName>(settingsPump["gpio_1"].toInt());
            PinName pump_2 = static_cast<PinName>(settingsPump["gpio_2"].toInt());

            // Setup objects
            m_pumps = QSharedPointer<Pumps>(new HAL::Pumps(m_parent, pump_1, pump_2));

            // Setup presenter
            m_pumpsPresenter = QSharedPointer<Presenters::PumpsPresenter>(new HAL::Presenters::PumpsPresenter(m_parent));
        }


        /**
         * Setup the guage hal object
         *
         * @brief HalContainer::setupGuages
         */
        void HalContainer::setupGuages()
        {
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

            try {
                // Setup guage
                m_guages = QSharedPointer<Guages>(new HAL::Guages(m_parent, guageReadings_1, guageReadings_2, guageGPIOExpander_1,
                                                                  guageGPIOExpander_2, guageADC_1, guageADC_2, guageTrip_1, guageTrip_2));
            } catch (...) {
                qDebug() << "Failed to create m_guages";
            }

            // Setup presenter
            m_guagesPresenter = QSharedPointer<Presenters::GuagesPresenter>(new HAL::Presenters::GuagesPresenter(m_parent));
        }


}}}
