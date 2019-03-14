#include "GuagesPresenter.h"

#include <cmath>

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

        // Logic
        presented["logic"] = package.at(0);

        // View status
        presented["view_status"] = "3"; // Error
        if(presented["logic"] == "1") // plugged in
        {
            presented["view_status"] = "1";
        }
        else if(presented["logic"] == "0") // not plugged in
        {
            presented["view_status"] = "2";
        }

        return presented;
    }

    QVariantMap GuagesPresenter::disabled(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_guageDisabled";

        // Logic
        presented["logic"] = package.at(0);

        // View status
        presented["view_status"] = "3"; // Error
        if(presented["logic"] == "1") // plugged in
        {
            presented["view_status"] = "1";
        }
        else if(presented["logic"] == "0") // not plugged in
        {
            presented["view_status"] = "2";
        }

        return presented;
    }

    QVariantMap GuagesPresenter::readTrip(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_guageReadTrip";

        // Logic
        presented["state"] = bool(package.at(0).toInt());
        presented["state_int"] = package.at(0).toInt();

        return presented;
    }

    QVariantMap GuagesPresenter::readVacuum(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_guageReadVacuum";

        // Voltage
        presented["voltage"] = package.at(0).toDouble() / 0.201793722;

        // Convert voltage to pressure in mbar
        if(presented["voltage"] <= 3)
        {
            presented["pressure"] = pow(10, -4);
        }
        else
        {
            presented["pressure"] = pow(10, (presented["voltage"].toDouble()-6));
        }

        // Find status of device
        presented["status"] = 3; // Assume error condition
        presented["error"] = "Unknown error"; // With Unknown eeeor
        if(presented["voltage"] >= 1.75 && presented["voltage"] < 9.25)
        {
            // Guage is ok
            presented["status"] = 1;
            presented["error"] = "No errors detected";
        }
        else if(presented["voltage"] >= 9.25 || presented["voltage"] < 1.75)
        {
            // Error condition
            presented["status"] = 3;

            // Try to identify the error
            if(presented["voltage"] > 9.45 && presented["voltage"] < 9.55)
                presented["error"] = "Filament failure";
            if(presented["voltage"] >= 9.55)
                presented["error"] = "Calibration error";
            if(presented["voltage"] < 1.75)
                presented["error"] = "Unknown error";
        }

        // Return package
        return presented;
    }



}}}}
