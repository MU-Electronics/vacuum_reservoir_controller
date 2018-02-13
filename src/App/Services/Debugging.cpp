#include "Debuging.h"

#include <QDebug>
#include <QLoggingCategory>

// Define logging catergoies for view managers
Q_LOGGING_CATEGORY(usbConnectionStatus, "usb.connection.status")



// Define logging catergoies for experiment engine parts



// Define logging catergoies for state machines



// Define logging catergoies for services
Q_LOGGING_CATEGORY(serialService, "services.serial-bus")
Q_LOGGING_CATEGORY(jsonFileService, "services.json-file")
Q_LOGGING_CATEGORY(threadService, "services.thread")
