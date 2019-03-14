#include "TemperatureSensorPresenter.h"

#include <QDebug>
#include "../../../Services/Debuging.h"


namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    TemperatureSensorPresenter::TemperatureSensorPresenter(QObject *parent)
    {

    }

    QVariantMap TemperatureSensorPresenter::proccess(QString method, QVariantMap commands, QStringList package)
    {
        // Select the correct presenter
        if(method == "getTemperature")
        {
            return extend(commands, getTemperature(commands, package));
        }

        // No method could be found generate error package
        QVariantMap error;
        error["error_id"] = "TemperatureSensorPresenter_NoMethodFound";
        error["level"] = "critical";
        error["message"] = "The method " + method + " does not exist in the temperature sensor presenter class.";

        // Log error
        qCCritical(halAccessGuagesPresenter) << "Could not find the correct temperature sensor presenter method. " << error;

        // Return the package
        return error;
    }


    QVariantMap TemperatureSensorPresenter::getTemperature(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_temperatureSensorTemperature";

        // Voltage
        presented["temperature"] = package.at(0);

        // Return package
        return presented;
    }

}}}}
