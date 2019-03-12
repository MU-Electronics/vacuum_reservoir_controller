#include "HALSupport.h"

namespace App { namespace Hardware { namespace HAL
{

    /**
     * Set the param package to be used with the methods below
     *
     * @brief VacStation::setParams
     * @param command
     * @return
     */
    void HALSupport::setParams(QVariantMap command)
    {
        m_command = command;
    }

}}}
