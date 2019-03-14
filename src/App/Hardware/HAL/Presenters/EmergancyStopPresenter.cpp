#include "EmergancyStopPresenter.h"

#include <QDebug>
#include "../../../Services/Debuging.h"


namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    EmergancyStopPresenter::EmergancyStopPresenter(QObject *parent)
    {

    }

    QVariantMap EmergancyStopPresenter::proccess(QString method, QVariantMap commands, QStringList package)
    {
        // Select the correct presenter
        if(method == "opened")
        {
            return extend(commands, isPressed(commands, package));
        }

        // No method could be found generate error package
        QVariantMap error;
        error["error_id"] = "EmergancyStopPresenter_NoMethodFound";
        error["level"] = "critical";
        error["message"] = "The method " + method + " does not exist in the emergancy stop presenter class.";

        // Log error
        qCCritical(halAccessGuagesPresenter) << "Could not find the correct emergancy stop presenter method. " << error;

        // Return the package
        return error;
    }


    QVariantMap EmergancyStopPresenter::isPressed(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_emergancyStopIsPressed";

        // Voltage
        presented["status"] = bool(package.at(0).toInt());

        // Return package
        return presented;
    }

}}}}
