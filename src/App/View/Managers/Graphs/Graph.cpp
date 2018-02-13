#include "Graph.h"

#include <QObject>
#include <QVariantMap>
#include <QString>
#include <QtGlobal>
#include <QDateTime>

#include <QtCharts/QXYSeries>
#include <QtCharts/QAreaSeries>

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)


namespace App { namespace View { namespace Managers { namespace Graphs
{
    Graph::Graph(QObject *parent)
        : QObject(parent)
    {
        qRegisterMetaType<QAbstractSeries*>();
        qRegisterMetaType<QAbstractAxis*>();
    }


    /**
     * Updates graph data
     *
     * @brief Graph::updateGraph
     * @param series
     * @param currentId
     * @param data
     */
    void Graph::updateGraph(QAbstractSeries *series, int &currentId, QVector<QPointF> &data)
    {
        // Get the seriers info
        QXYSeries* xySeries = static_cast<QXYSeries *>(series);

        // Get total number of records
        int records = xySeries->count();

        // If there is no data then the graph was re-init so reset the current id index
        if(records == 0)
            currentId = 0;

        // Check buffer is not too large
        if(records == maxBuffer)
            xySeries->remove(0);

        if(data.count() == maxBuffer)
            data.removeAt(0);

        // Loop through new data currentId
        for (int i = currentId; i < data.size(); ++i)
        {
            // Append data
            xySeries->append(data.at(i));

            // Mark current id
            currentId = i;
        }
    }


    /**
     * Adds data to store with time stamp
     *
     * @brief Graph::updateTimedData
     * @param value
     * @param store
     */
    void Graph::updateTimedData(qreal value, QVector<QPointF> &store)
    {
        // Only store 1000 values
        if(store.count() >= 1000)
            store.pop_front();

        // Get the current time
        qreal time = qreal(QDateTime::currentMSecsSinceEpoch());

        // Add new data to container
        store.append(QPointF(time, value));
    }


     /**
      * Updates the axis limits
      */
    void Graph::updateAxis(QVector<QPointF> &data)
    {
        // Does the x axis need scaling?
        if(qRound64(data.last().x()) > graphMaxX().toMSecsSinceEpoch())
        {
            // Increase X axis
            graphMinX(graphMinX().addSecs(scrollBy));
            graphMaxX(graphMaxX().addSecs(scrollBy));
        }

        // Does the Y axis need scaling
        double value = data.last().y();
        if (value > graphMaxY())
        {
            // Increase Y axis
            graphMaxY(value + 1);
        }
        else if (value < graphMinY())
        {
            // Increase Y axis
            if(value > 1)
            {
               graphMinY(value - 1);
            }
            else
            {
                graphMinY(value - (value / 2));
            }
        }
    }

}}}}


