#pragma once

// Include extenral deps
#include <QObject>

// Debugging catergories
#include "../Services/Debuging.h"

// Include settings container
#include "../Settings/Container.h"

namespace App { namespace Experiment
{
    class Engine    :   public QObject
    {
        Q_OBJECT

        public:
            Engine(QObject *parent, Settings::Container settings);

            ~Engine();

            Engine(const Engine&) = delete;

            void makeConnections();

        private:
            // Holds the application settings
            Settings::Container m_settings;

    };
}}


