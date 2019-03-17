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
        presented["guage_id"] = commands["guage_id"];

        // Logic
        presented["status"] = bool(package.at(0).toInt());
        presented["status_int"] = package.at(0).toInt();

        // View status
        presented["view_status"] = "3"; // Error
        if(presented["status"] == true) // plugged in
        {
            presented["view_status"] = "1";
        }
        else if(presented["status"] == false) // not plugged in
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
        presented["guage_id"] = commands["guage_id"];

        // Logic
        presented["status"] = bool(package.at(0).toInt());
        presented["status_int"] = package.at(0).toInt();

        // View status
        presented["view_status"] = "3"; // Error
        if(presented["status"] == true) // plugged in
        {
            presented["view_status"] = "1";
        }
        else if(presented["status"] == false) // not plugged in
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
        presented["guage_id"] = commands["guage_id"];

        // Logic (Invert logic as tripped is pulled low)
        presented["status"] = !bool(package.at(0).toInt());
        presented["status_int"] = presented["status"].toInt();

        return presented;
    }

    QVariantMap GuagesPresenter::readVacuum(QVariantMap commands, QStringList package)
    {
        //qDebug() << "read vacuum presenter" << commands << package;
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_guageReadVacuum";
        presented["guage_id"] = commands["guage_id"];

        // Voltage
        presented["voltage"] = package.at(0).toDouble() / 0.201793722;

        // Convert voltage to pressure in mbar
        if(presented["voltage"] <= 3)
        {
            presented["pressure_mbar"] = vacuumAverage(presented["guage_id"].toInt(), pow(10, -4));
        }
        else
        {
            presented["pressure_mbar"] = vacuumAverage(presented["guage_id"].toInt(), pow(10, (presented["voltage"].toDouble()-6)));
        }

        // Find status of device
        presented["view_status"] = 3; // Assume error condition
        presented["error"] = "Unknown error"; // With Unknown eeeor
        if(presented["voltage"] >= 1.75 && presented["voltage"] < 9.25)
        {
            // Guage is ok
            presented["view_status"] = 1;
            presented["error"] = "No errors detected";
        }
        else if(presented["voltage"] >= 9.25 || presented["voltage"] < 1.75)
        {
            // Error condition
            presented["view_status"] = 3;

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
