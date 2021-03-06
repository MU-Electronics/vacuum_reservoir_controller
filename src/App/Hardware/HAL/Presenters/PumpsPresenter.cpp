#include "PumpsPresenter.h"

#include <QDebug>
#include "../../../Services/Debuging.h"


namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    PumpsPresenter::PumpsPresenter(QObject *parent)
    {

    }

    QVariantMap PumpsPresenter::proccess(QString method, QVariantMap commands, QStringList package)
    {
        // Select the correct presenter
        if(method == "enabled") //  && expectedPackage(commands, package, "48", 10)
        {
            return extend(commands, enable(commands, package));
        }
        else if(method == "disabled") //  && expectedPackage(commands, package, "48", 10)
        {
            return extend(commands, disable(commands, package));
        }

        // No method could be found generate error package
        QVariantMap error;
        error["error_id"] = "PumpsPresenter_NoMethodFound";
        error["level"] = "critical";
        error["message"] = "The method " + method + " does not exist in the pumps presenter class.";

        // Log error
        qCCritical(halAccessGuagesPresenter) << "Could not find the correct pumps presenter method. " << error;

        // Return the package
        return error;
    }

    QVariantMap PumpsPresenter::enable(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_pumpEnabled";
        presented["pump_id"] = commands["pump_id"];

        // Voltage
        presented["status"] = bool(package.at(0).toInt());

        // View status
        presented["view_status"] = "3"; // Error
        if(presented["status"] == true) // On
        {
            presented["view_status"] = "1";
        }
        else if(presented["status"] == false) // Off
        {
            presented["view_status"] = "2";
        }

        // Return package
        return presented;
    }


    QVariantMap PumpsPresenter::disable(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_pumpDisabled";
        presented["pump_id"] = commands["pump_id"];

        // Voltage
        presented["status"] = bool(package.at(0).toInt());

        // View status
        presented["view_status"] = "3"; // Error
        if(presented["status"] == true) // On
        {
            presented["view_status"] = "1";
        }
        else if(presented["status"] == false) // Off
        {
            presented["view_status"] = "2";
        }

        // Return package
        return presented;
    }



}}}}
