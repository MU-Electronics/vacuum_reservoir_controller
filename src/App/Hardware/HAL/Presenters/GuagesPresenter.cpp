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

        //double voltage = (package.at(0).toDouble() / 0.201793722);
        double voltage = (package.at(0).toDouble() * 5.016666667);
        int id = presented["guage_id"].toInt();

        //if(id == 1)
        //    qDebug() << "Voltage: " << voltage << " Command: "<< commands << package;

        // Check drop gradient not too sharp (invalid reading)
        bool skipReading = false;
        if(abs(voltage - voltageAverage(id)) > 5 && largeDrop == 0)
        {
            skipReading = true;
            largeDrop++;
            qCWarning(halAccessGuagesPresenter) << "**Possible** invalid reading detected skipping this one but not the next:" << package << commands;
        }
        else
        {
            largeDrop = 0;
        }

        // Voltage
        presented["voltage"] = voltageAverage(id, voltage);

        // Convert voltage to pressure in mbar
        if(presented["voltage"] <= 3 && !skipReading)
        {
            presented["pressure_mbar"] = vacuumAverage(id, pow(10, -4));
        }
        else if(!skipReading)
        {
            presented["pressure_mbar"] = vacuumAverage(id, pow(10, (presented["voltage"].toDouble()-6)));
        }
        else if(skipReading)
        {
            presented["pressure_mbar"] = vacuumAverage(id);
        }

        // Define the guage tolerance
        auto calculatedPressure = presented["pressure_mbar"].toDouble();
        presented["tolerance_upper"] = toleranceUpper;
        presented["tolerance_upper"] = toleranceLower;
        presented["pressure_mbar_upper"] = calculatedPressure + (calculatedPressure * toleranceUpper);
        presented["pressure_mbar_lower"] = calculatedPressure- (calculatedPressure * toleranceLower);

        //if(commands["guage_id"] == 1)
         //   qDebug() <<commands["guage_id"]<< presented["pressure_mbar"]  << presented["voltage"];


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
