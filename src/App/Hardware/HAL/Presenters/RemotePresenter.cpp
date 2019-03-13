#include "RemotePresenter.h"

#include <QDebug>
#include "../../../Services/Debuging.h"


namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    RemotePresenter::RemotePresenter(QObject *parent)
    {

    }

    QVariantMap RemotePresenter::proccess(QString method, QVariantMap commands, QStringList package)
    {
        // Select the correct presenter
        if(method == "isConnected")
        {
            return extend(commands, isConnected(commands, package));
        }

        // No method could be found generate error package
        QVariantMap error;
        error["error_id"] = "RemotePresenter_NoMethodFound";
        error["level"] = "critical";
        error["message"] = "The method " + method + " does not exist in the remote presenter class.";

        // Log error
        qCCritical(halAccessGuagesPresenter) << "Could not find the correct remote presenter method. " << error;

        // Return the package
        return error;
    }


    QVariantMap RemotePresenter::isConnected(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_remoteIsConnected";

        // Voltage
        presented["status"] = package.at(0);

        // Return package
        return presented;
    }

}}}}
