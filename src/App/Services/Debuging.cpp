#include "Debuging.h"

#include <QDebug>
#include <QLoggingCategory>


Q_LOGGING_CATEGORY(general, "general")

// Define logging catergoies for view managers
Q_LOGGING_CATEGORY(userAcknowledge, "user.acknowledge")
Q_LOGGING_CATEGORY(powerMain, "power.main")



// Define logging catergoies for experiment engine parts
Q_LOGGING_CATEGORY(machineStatesValidator, "experiment.machines.validatorstate")
Q_LOGGING_CATEGORY(machineStates, "experiment.machines.machinestates")


// Hals & Presenters
Q_LOGGING_CATEGORY(halAccessGuagesPresenter, "hal.presenters.guages")



// Define logging catergoies for state machines



// Define logging catergoies for services
Q_LOGGING_CATEGORY(serialService, "services.serial-bus")
Q_LOGGING_CATEGORY(jsonFileService, "services.json-file")
Q_LOGGING_CATEGORY(threadService, "services.thread")
