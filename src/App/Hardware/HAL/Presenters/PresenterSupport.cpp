#include "PresenterSupport.h"


namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    PresenterSupport::PresenterSupport()
    {

    }


    /**
     * Checks that the package is the expected size
     *
     * @brief PresenterSupport::validatePackageSize
     * @param package
     * @param size
     * @return
     */
    bool PresenterSupport::validatePackageSize(QStringList package, int size)
    {
        if(!package.empty() && package.size() == size)
            return true;

        return false;
    }


    /**
     * Entry point to run any universal actions to the returning package
     *
     * @brief PresenterSupport::extend
     * @param commands
     * @param package
     * @return
     */
    QVariantMap PresenterSupport::extend(QVariantMap commands, QVariantMap package)
    {
        return methodOverride(commands, commandId(commands, package));
    }


    /**
     * Inserts the command id into the package if it exsits
     *
     * @brief PresenterSupport::commandId
     * @param commands
     * @param package
     * @return
     */
    QVariantMap PresenterSupport::commandId(QVariantMap commands, QVariantMap package)
    {
        QString id = commands.value("command_identifier").toString();

        if(id != "")
            package.insert("command_identifier", id);

        return package;
    }


    /**
     * In some cases a return method should be replaced with another alias
     *
     * @brief PresenterSupport::methodOverride
     * @param commands
     * @param package
     * @return
     */
    QVariantMap PresenterSupport::methodOverride(QVariantMap commands, QVariantMap package)
    {
        QString override = commands.value("emit_override").toString();

        if(override != "")
            package.insert("method", override);

        return package;
    }

}}}}
