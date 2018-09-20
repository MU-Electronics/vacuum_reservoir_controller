#pragma once

#include <QDebug>
#include <QString>

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


            virtual void makeConnections()
            {

            }

    };
}}}


