#pragma once

// include external libs
#include <QObject>
#include <QThread>
#include <QMutex>

// Debugging catergories
#include "../Services/Debuging.h"

namespace App { namespace Services{
    class Thread : public QObject
    {
        Q_OBJECT

    public:
        Thread(QObject *parent, bool working, bool abort);

        void setup(QThread &thread);
        void requestWork();
        void abort();

        bool thread_abort;
        bool thread_working;

        QMutex mutex;



    public slots:
        void foreverLoop();


    signals:
        void workRequested();
        void finished();

    private:
        virtual void worker() = 0;

        virtual void configure(QThread &thread) = 0;

    };
}}


