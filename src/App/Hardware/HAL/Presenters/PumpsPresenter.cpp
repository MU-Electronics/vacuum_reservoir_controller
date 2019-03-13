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
        if(method == "enable") //  && expectedPackage(commands, package, "48", 10)
        {
            return extend(commands, enable(commands, package));
        }
        else if(method == "disable") //  && expectedPackage(commands, package, "48", 10)
        {
            return extend(commands, disable(commands, package));
        }

        // No method could be found generate error package
        QVariantMap error;
        error["error_id"] = "PumpsPresenter_NoMethodFound";
        error["level"] = "critical";
        error["message"] = "The method " + method + " does not exist in the flow controller presenter class.";

        // Log error
        qCCritical(halAccessGuagesPresenter) << "Could not find the correct flow controll presenter method. " << error;

        // Return the package
        return error;
    }

    QVariantMap PumpsPresenter::enable(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_pumpEnabled";

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


    QVariantMap PumpsPresenter::disable(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_pumpDisabled";

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
