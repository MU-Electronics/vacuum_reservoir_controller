#include "ValvesVsTime.h"

#include <cmath>

#include <QVariantMap>
#include <QString>
#include <QtGlobal>
#include <QDateTime>

namespace App { namespace View { namespace Managers { namespace Graphs
{
    ValvesVsTime::ValvesVsTime(QObject *parent, QQmlApplicationEngine *root, Settings::Container *settings, Experiment::Engine* experimentEngine)
        : Graph(parent),
          m_root(root),
          m_settings(settings),
          m_experimentEngine(experimentEngine)
    {

    }


    /**
     * Connect method to signals and vis-versa
     *
     * @brief ValvesVsTime::makeConnections
     * @param hardware
     * @param safety
     */
    void ValvesVsTime::makeConnections(Hardware::Access& hardware)
    {
        // Pressure sensor
        connect(&hardware, &Hardware::Access::emit_valveClosed, this, &ValvesVsTime::data);
        connect(&hardware, &Hardware::Access::emit_valveOpened, this, &ValvesVsTime::data);

        // Get the current date time
        QDateTime current = QDateTime::currentDateTime();

        // Append correct x axis limits
        graphMinX(current);
        graphMaxX(current.addSecs(hold));

        // Set random Y axis limits
        graphMinY(0);
        graphMaxY(4);

        // No data currently
        hasData(false);
    }



    /**
     * Updates valve data
     *
     * @brief ValvesVsTime::updateValve
     * @param valve
     * @param series
     */
    void ValvesVsTime::update(int valve, QAbstractSeries *series)
    {
        // If series not null
        if (series)
        {
            if(valve == 1)
            {
                // Update graph
                updateGraph(series, currentValveOneId, m_valveOneData);
            }
            else if(valve == 2)
            {
                // Update graph
                updateGraph(series, currentValveTwoId, m_valveTwoData);
            }
            else if(valve == 3)
            {
                // Update graph
                updateGraph(series, currentValveThreeId, m_valveThreeData);
            }
            else if(valve == 4)
            {
                // Update graph
                updateGraph(series, currentValveFourId, m_valveFourData);
            }
            else if(valve == 5)
            {
                // Update graph
                updateGraph(series, currentValveFiveId, m_valveFiveData);
            }
            else if(valve == 6)
            {
                // Update graph
                updateGraph(series, currentValveSixId, m_valveSixData);
            }
            else if(valve == 7)
            {
                // Update graph
                updateGraph(series, currentValveSevenId, m_valveSevenData);
            }
            else if(valve == 8)
            {
                // Update graph
                updateGraph(series, currentValveEightId, m_valveEightData);
            }
        }
    }


    /**
     * Updates the valve data
     *
     * @brief ValvesVsTime::valveData
     * @param package
     */
    void ValvesVsTime::data(QVariantMap package)
    {
        // Get valve number
        int valve = package["group"].toInt();

        // Update valve value
        int value = package.value("value").toInt();

        if(valve == 1)
        {
            // Find graph value
            qreal graphValue = (value == 1) ? 1 : 0;

            // Update data
            updateTimedData(graphValue, m_valveOneData);

            // Emit we have new data
            emit emit_newValveOneGraphData();
        }
        else if(valve == 2)
        {
            // Find graph value
            qreal graphValue = (value == 1) ? 1.4 : 0;

            // Update data
            updateTimedData(graphValue, m_valveTwoData);

            // Emit we have new data
            emit emit_newValveTwoGraphData();
        }
        else if(valve == 3)
        {
            // Find graph value
            qreal graphValue = (value == 1) ? 1.8 : 0;

            // Update data
            updateTimedData(graphValue, m_valveThreeData);

            // Emit we have new data
            emit emit_newValveThreeGraphData();
        }
        else if(valve == 4)
        {
            // Find graph value
            qreal graphValue = (value == 1) ? 2.2 : 0;

            // Update data
            updateTimedData(graphValue, m_valveFourData);

            // Emit we have new data
            emit emit_newValveFourGraphData();
        }
        else if(valve == 5)
        {
            // Find graph value
            qreal graphValue = (value == 1) ? 2.6 : 0;

            // Update data
            updateTimedData(graphValue, m_valveFiveData);

            // Emit we have new data
            emit emit_newValveFiveGraphData();
        }
        else if(valve == 6)
        {
            // Find graph value
            qreal graphValue = (value == 1) ? 3 : 0;

            // Update data
            updateTimedData(graphValue, m_valveSixData);

            // Emit we have new data
            emit emit_newValveSixGraphData();
        }
        else if(valve == 7)
        {
            // Find graph value
            qreal graphValue = (value == 1) ? 3.4 : 0;

            // Update data
            updateTimedData(graphValue, m_valveSevenData);

            // Emit we have new data
            emit emit_newValveSevenGraphData();
        }
        else if(valve == 8)
        {
            // Find graph value
            qreal graphValue = (value == 1) ? 3.8 : 0;

            // Update data
            updateTimedData(graphValue, m_valveEightData);

            // Emit we have new data
            emit emit_newValveEightGraphData();
        }
    }

}}}}

