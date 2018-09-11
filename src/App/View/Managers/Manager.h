#pragma once

#include <QDebug>

// Debugging catergories
#include "../../Services/Debuging.h"

namespace App { namespace View { namespace Managers
{
    class Manager
    {
        public:

            Manager()
            {
            }

            ~Manager()
            {
            }


            Manager(const Manager&) = delete;

            virtual void makeConnections() = 0;

    };
}}}


