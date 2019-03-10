#include "Power.h"


// Include external libs
#include <QDebug>
#include <QProcess>


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
    Power::Power(QObject *parent, QQmlApplicationEngine *root, Settings::Container* settings)
        :   QObject(parent)
        ,   m_root(root)
        ,   m_settings(settings)

            // Interrupts
        //,   m_mainPowerDrop(GPIO_6)
        //,   m_shutDown(GPIO_5)
    {
        // Configure all interrupts
        // setupInterrupts();
    }


    Power::~Power()
    {

    }


    /**
     * Connect signals from and to this class and the hardware & safety thread
     *
     * @brief Power::makeConnections
     * @param hardware
     * @param safety
     */
    void Power::makeConnections()
    {
        // connect(&m_timer, &QTimer::timeout, this, &Global::togglePort);

    }


    /**
     * Setup interrupts
     *
     * @brief Power::setupInterrupts
     */
    void Power::setupInterrupts()
    {
        // Mains power drop out detection
       // m_mainPowerDrop.mode(PullUp);
       // m_mainPowerDrop.fall(callback(this, &Power::mainPowerDropIntr));

        // Shut down signal
       // m_shutDown.mode(PullUp);
       // m_shutDown.fall(callback(this, &Power::shutDownIntr));
    }


    /**
     * Shut down the processor
     *
     * @brief Power::shutDown
     */
    void Power::shutDownIntr()
    {
        // We are shutting down
        //qCInfo(general) << "Shut down has been requested via the front panel button.";

        // Shutdown
       // QProcess process;
       // process.startDetached("shutdown -P now");
    }



    /**
     * Drop in the main power supply
     *
     * @brief Power::mainPowerDrop
     */
    void Power::mainPowerDropIntr()
    {
        // Issue
        QString error = "Main 24V power supply has fluxuated below or above acceptable limits.";

        // Create an offical log
        //qCCritical(powerMain) << error;

        // @TODO Notify the assigned email address
    }

}}}

