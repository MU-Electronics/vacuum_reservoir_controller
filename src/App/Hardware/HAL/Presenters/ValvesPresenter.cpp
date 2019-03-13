#include "ValvesPresenter.h"

#include <QDebug>
#include "../../../Services/Debuging.h"


namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    ValvesPresenter::ValvesPresenter(QObject *parent)
    {

    }

    QVariantMap ValvesPresenter::proccess(QString method, QVariantMap commands, QStringList package)
    {
        // Select the correct presenter
        if(method == "opened")
        {
            return extend(commands, opened(commands, package));
        }
        else if(method == "closed")
        {
            return extend(commands, closed(commands, package));
        }

        // No method could be found generate error package
        QVariantMap error;
        error["error_id"] = "ValvesPresenter_NoMethodFound";
        error["level"] = "critical";
        error["message"] = "The method " + method + " does not exist in the valve presenter class.";

        // Log error
        qCCritical(halAccessGuagesPresenter) << "Could not find the correct valve presenter method. " << error;

        // Return the package
        return error;
    }

    QVariantMap ValvesPresenter::opened(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_valveOpened";

        // Voltage
        presented["status"] = package.at(0);

        // View status
        presented["view_status"] = "3"; // Error
        if(presented["status"] == "1") // On
        {
            presented["view_status"] = "1";
        }
        else if(presented["status"] == "0") // Off
        {
            presented["view_status"] = "2";
        }

        // Return package
        return presented;
    }


    QVariantMap ValvesPresenter::closed(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_valveClosed";

        // Voltage
        presented["status"] = package.at(0);

        // View status
        presented["view_status"] = "3"; // Error
        if(presented["status"] == "1") // On
        {
            presented["view_status"] = "1";
        }
        else if(presented["status"] == "0") // Off
        {
            presented["view_status"] = "2";
        }

        // Return package
        return presented;
    }



}}}}
