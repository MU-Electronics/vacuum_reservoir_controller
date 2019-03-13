#include "GuagesPresenter.h"

#include <QDebug>
#include "../../../Services/Debuging.h"


namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    GuagesPresenter::GuagesPresenter(QObject *parent)
    {

    }

    QVariantMap GuagesPresenter::proccess(QString method, QVariantMap commands, QStringList package)
    {
        // Select the correct presenter
        if(method == "readVacuum") //  && expectedPackage(commands, package, "48", 10)
        {
            return extend(commands, readVacuum(commands, package));
        }

        // No method could be found generate error package
        QVariantMap error;
        error["error_id"] = "GuagesPresenter_NoMethodFound";
        error["level"] = "critical";
        error["message"] = "The method " + method + " does not exist in the flow controller presenter class.";

        // Log error
        qCCritical(halAccessGuagesPresenter) << "Could not find the correct flow controll presenter method. " << error;

        // Return the package
        return error;
    }

    QVariantMap GuagesPresenter::readVacuum(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_guagesReadVacuum";

        // Voltage
        presented["voltage"] = package.at(0);

        // Convert voltage to pressure in mbar @TODO
        presented["pressure"] = package.at(0);

        // Return package
        return presented;
    }



}}}}
