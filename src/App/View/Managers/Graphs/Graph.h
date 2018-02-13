#pragma once

// Include external libs
#include <QObject>
#include <QQmlApplicationEngine>
#include <QString>
#include <QVariantMap>
#include <QVariant>
#include <QtGlobal>
#include <QDateTime>
#include <QString>

#include <QtCharts/QAbstractSeries>
QT_CHARTS_USE_NAMESPACE

// Include contract
#include "../Manager.h"

// Include app deps
#include "../../../Utilities/Properties.h"

// Include settings container
#include "../../../Settings/Container.h"

// Experiment Engine with state machine methods
#include "../../../Experiment/Engine.h"

namespace App { namespace View { namespace Managers { namespace Graphs
{


    class Graph : public QObject
    {
        Q_OBJECT

        AUTO_PROPERTY(double, graphMaxY)
        AUTO_PROPERTY(double, graphMinY)
        AUTO_PROPERTY(QDateTime, graphMaxX)
        AUTO_PROPERTY(QDateTime, graphMinX)
        AUTO_PROPERTY(bool, hasData)

        public:
            // constructure and destructor
            Graph(QObject *parent);
            virtual ~Graph(){}

            // Scroll by
            int scrollBy = 20;
            int hold = 300;

            // Limit the buffer size
            int maxBuffer = 4000;

            // Update axis
            void updateAxis(QVector<QPointF> &data);

            // Adds and removes data from a series
            void updateGraph(QAbstractSeries *series, int &currentId, QVector<QPointF> &data);

            // Adds and removes data from the data container
            void updateTimedData(qreal value, QVector<QPointF> &store);
    };
}}}}


