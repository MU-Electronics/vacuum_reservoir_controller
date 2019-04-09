#include "PressureVsTime.h"

#include <cmath>

#include <QVariantMap>
#include <QString>
#include <QtGlobal>
#include <QDateTime>


namespace App { namespace View { namespace Managers { namespace Graphs
{
    PressureVsTime::PressureVsTime(QObject *parent, QQmlApplicationEngine *root, Settings::Container* settings, Experiment::Engine* experimentEngine, int guageId)
        : Graph(parent),
          m_guageId(guageId),
          m_root(root),
          m_settings(settings),
          m_experimentEngine(experimentEngine)
    {

    }


    /**
     * Connect method to signals and vis-versa
     *
     * @brief PressureVsTime::makeConnections
     * @param hardware
     * @param safety
     */
    void PressureVsTime::makeConnections(Hardware::Access& hardware)
    {
        // Pressure sensor
        connect(&hardware, &Hardware::Access::emit_guageReadVacuum, this, &PressureVsTime::pressureData);

        // Get the current date time
        QDateTime current = QDateTime::currentDateTime();

        // Append correct x axis limits
        graphMinX(current);
        graphMaxX(current.addSecs(hold));

        // Set random Y axis limits
        graphMinY(-10);
        graphMaxY(1000);

        // No data currently
        hasData(false);
    }



    /**
     * Updates the graph data
     *
     * @brief PressureVsTime::updateGraph
     * @param series
     */
    void PressureVsTime::update(int i, QAbstractSeries *series)
    {
        // If series not null
        if (series)
        {
            // Update x axis if guage has been changed
            hasGraphChanged(i);

            // Update graph data
            switch(i)
            {
                case 1:
                    updateGraph(series, currentGuageOneId, m_GuageOneData);
                break;
                case 2:
                    updateGraph(series, currentGuageTwoId, m_GuageTwoData);
                break;
                case 3:
                    updateGraph(series, currentGuageThreeId, m_GuageThreeData);
                break;
                case 4:
                    updateGraph(series, currentGuageFourId, m_GuageFourData);
                break;
                case 5:
                    updateGraph(series, currentGuageFiveId, m_GuageFiveData);
                break;
                case 6:
                    updateGraph(series, currentGuageSixId, m_GuageSixData);
                break;
                case 7:
                    updateGraph(series, currentGuageSevenId, m_GuageSevenData);
                break;
                case 8:
                    updateGraph(series, currentGuageEightId, m_GuageEightData);
                break;
            }
        }
    }



    /**
     * Saves the pressure data recoreded
     *
     * @brief PressureVsTime::data
     * @param package
     */
    void PressureVsTime::pressureData(QVariantMap package)
    {
        //if(package["guage_id"].toInt() != m_guageId)
        //    return;

        switch(package["guage_id"].toInt())
        {
            case 1:
                updateTimedData(package["pressure_mbar"].toReal(), m_GuageOneData);
                updateAxis(m_GuageOneData);
                emit emit_newGuageOneGraphData();
            break;
            case 2:
                updateTimedData(package["pressure_mbar"].toReal(), m_GuageTwoData);
                updateAxis(m_GuageTwoData);
                emit emit_newGuageTwoGraphData();
            break;
            case 3:
                updateTimedData(package["pressure_mbar"].toReal(), m_GuageThreeData);
                updateAxis(m_GuageThreeData);
                emit emit_newGuageThreeGraphData();
            break;
            case 4:
                updateTimedData(package["pressure_mbar"].toReal(), m_GuageFourData);
                updateAxis(m_GuageFourData);
                emit emit_newGuageFourGraphData();
            break;
            case 5:
                updateTimedData(package["pressure_mbar"].toReal(), m_GuageFiveData);
                updateAxis(m_GuageFiveData);
                emit emit_newGuageFiveGraphData();
            break;
            case 6:
                updateTimedData(package["pressure_mbar"].toReal(), m_GuageSixData);
                updateAxis(m_GuageSixData);
                emit emit_newGuageSixGraphData();
            break;
            case 7:
                updateTimedData(package["pressure_mbar"].toReal(), m_GuageSevenData);
                updateAxis(m_GuageSevenData);
                emit emit_newGuageSevenGraphData();
            break;
            case 8:
                updateTimedData(package["pressure_mbar"].toReal(), m_GuageEightData);
                updateAxis(m_GuageEightData);
                emit emit_newGuageEightGraphData();
            break;
        }

        // has data
        if(!hasData())
            hasData(true);
    }

}}}}

