#include "Debuging.h"

#include <QDebug>
#include <QLoggingCategory>


Q_LOGGING_CATEGORY(general, "general")

// Define logging catergoies for view managers
Q_LOGGING_CATEGORY(powerMain, "power.main")



// Define logging catergoies for experiment engine parts



// Define logging catergoies for state machines



// Define logging catergoies for services
Q_LOGGING_CATEGORY(serialService, "services.serial-bus")
Q_LOGGING_CATEGORY(jsonFileService, "services.json-file")
Q_LOGGING_CATEGORY(threadService, "services.thread")
