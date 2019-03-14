#pragma once

#include <QObject>
#include <QList>

namespace App { namespace Experiment { namespace Machines { namespace Helpers
{

    class FunctionsHelper
    {
        public:
            FunctionsHelper();

            QList<QString> getIds();
            QString stringOfIds();
            void registerId(QString id);
            bool isRegister(QString id);

        private:
            QList<QString> m_registeredId;

    };

}}}}
