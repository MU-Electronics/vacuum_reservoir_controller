#pragma once

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(general)

// Define logging catergoies for view managers
Q_DECLARE_LOGGING_CATEGORY(jsonFileService)
Q_DECLARE_LOGGING_CATEGORY(powerMain)
Q_DECLARE_LOGGING_CATEGORY(userAcknowledge)


// Hals & Presenters
Q_DECLARE_LOGGING_CATEGORY(halAccessGuagesPresenter)



// Define logging catergoies for experiment engine parts
Q_DECLARE_LOGGING_CATEGORY(machineStatesValidator)
Q_DECLARE_LOGGING_CATEGORY(machineStates)

// Define logging catergoies for services
Q_DECLARE_LOGGING_CATEGORY(serialService)

Q_DECLARE_LOGGING_CATEGORY(threadService)

