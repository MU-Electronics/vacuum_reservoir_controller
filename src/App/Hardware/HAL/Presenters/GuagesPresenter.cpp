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
        if(method == "enabled") //  && expectedPackage(commands, package, "48", 10)
        {
            return extend(commands, enabled(commands, package));
        }
        if(method == "disabled") //  && expectedPackage(commands, package, "48", 10)
        {
            return extend(commands, disabled(commands, package));
        }
        if(method == "readTrip") //  && expectedPackage(commands, package, "48", 10)
        {
            return extend(commands, readTrip(commands, package));
        }

        // No method could be found generate error package
        QVariantMap error;
        error["error_id"] = "GuagesPresenter_NoMethodFound";
        error["level"] = "critical";
        error["message"] = "The method " + method + " does not exist in the guages presenter class.";

        // Log error
        qCCritical(halAccessGuagesPresenter) << "Could not find the correct guages presenter method. " << error;

        // Return the package
        return error;
    }

    QVariantMap GuagesPresenter::enabled(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_guageEnabled";

        return presented;
    }

    QVariantMap GuagesPresenter::disabled(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_guageDisabled";

        return presented;
    }

    QVariantMap GuagesPresenter::readTrip(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_guageReadTrip";

        return presented;
    }

    QVariantMap GuagesPresenter::readVacuum(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_guageReadVacuum";

        // Voltage
        presented["voltage"] = package.at(0);

        // Convert voltage to pressure in mbar @TODO
        presented["pressure"] = package.at(0);

        // Return package
        return presented;
    }



}}}}
