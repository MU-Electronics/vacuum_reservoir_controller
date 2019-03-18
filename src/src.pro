
# Set this project as the main app
TEMPLATE = app

# Icnlude some useful Qt libs by default
QT += qml quick quickcontrols2 widgets serialport charts svg

QTPLUGIN += qtvirtualkeyboardplugin



# Ensure we're using C11 standard
CONFIG += c++14 static exceptions

# Include the confiruation file
include($$PWD/../config.pri)

# Warnings for deprecated Qt features
DEFINES += QT_DEPRECATED_WARNINGS



# Disable unused warning parameter
QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter -Wall
QMAKE_CFLAGS_WARN_OFF -= -Wunused-parameter -Wall



# Include all the source files
SOURCES += \
    main.cpp \
    Bootstrap/Startup.cpp \
    App/Experiment/Engine.cpp \
    App/Services/Logger.cpp \
    App/Application.cpp \
    App/Services/Thread.cpp \
    App/Services/JsonFile.cpp \
    App/Settings/Container.cpp \
    App/Services/SerialController.cpp \
    App/View/Managers/Global.cpp \
    App/Services/Debugger.cpp \
    App/View/Managers/Graphs/Graph.cpp \
    App/View/Managers/SystemValues.cpp \
    App/View/Managers/Power.cpp \
    App/Services/Debuging.cpp \
    App/View/Managers/Logs.cpp \
    App/View/Managers/Control.cpp \
    App/Settings/General.cpp \
    App/View/Managers/SettingsUpdater.cpp \
    App/Settings/Hardware.cpp \
    App/Settings/View.cpp \
    App/Hardware/Access.cpp \
    App/Hardware/CommandConstructor.cpp \
    App/Hardware/HAL/EmergancyStop.cpp \
    App/Hardware/HAL/Guages.cpp \
    App/Hardware/HAL/Pumps.cpp \
    App/Hardware/HAL/Remote.cpp \
    App/Hardware/HAL/TemperatureSensor.cpp \
    App/Hardware/HAL/Valves.cpp \
    App/Hardware/HAL/HALSupport.cpp \
    App/Hardware/HAL/HalContainer.cpp \
    App/Hardware/HAL/Presenters/GuagesPresenter.cpp \
    App/Hardware/HAL/Presenters/PresenterSupport.cpp \
    App/Hardware/HAL/Presenters/PumpsPresenter.cpp \
    App/Hardware/HAL/Presenters/ValvesPresenter.cpp \
    App/Hardware/HAL/Presenters/EmergancyStopPresenter.cpp \
    App/Hardware/HAL/Presenters/RemotePresenter.cpp \
    App/Hardware/HAL/Presenters/TemperatureSensorPresenter.cpp \
    App/Experiment/Machines/MachineContainer.cpp \
    App/Experiment/Machines/Helpers/CommandValidatorState.cpp \
    App/Experiment/Machines/Helpers/FunctionsHelper.cpp \
    App/Experiment/Machines/Helpers/MachineStates.cpp \
    App/Experiment/Machines/Helpers/TransitionsBuilder.cpp \
    App/Experiment/Machines/Functions/ValveFunctions.cpp \
    App/Experiment/Machines/ReadPressure.cpp \
    App/Experiment/Machines/Functions/GuageFunctions.cpp \
    App/Experiment/Machines/Functions/EmergancyStopFunctions.cpp \
    App/Experiment/Machines/Functions/PumpFunctions.cpp \
    App/Experiment/Machines/Functions/RemoteFunctions.cpp \
    App/Experiment/Machines/Functions/TemperatureSensorFunctions.cpp \
    App/Experiment/Machines/ReadGuageTrip.cpp \
    App/Experiment/Machines/PumpControl.cpp

# Include all the header files
HEADERS += \
    Bootstrap/Startup.h \
    App/Experiment/Engine.h \
    App/Services/Logger.h \
    App/Application.h \
    App/Services/Thread.h \
    App/Services/JsonFile.h \
    App/Settings/Container.h \
    App/Services/SerialController.h \
    App/View/Managers/Global.h \
    App/View/Managers/Manager.h \
    App/Services/Debugger.h \
    App/View/Managers/Graphs/Graph.h \
    App/Services/Debuging.h \
    App/View/ManagerFactory.h \
    App/View/Managers/SystemValues.h \
    App/Utilities/Properties.h \
    App/View/Managers/Power.h \
    App/View/Managers/Logs.h \
    App/View/Managers/Control.h \
    App/Settings/General.h \
    App/View/Managers/SettingsUpdater.h \
    App/Settings/Hardware.h \
    App/Settings/View.h \
    App/Hardware/Access.h \
    App/Hardware/CommandConstructor.h \
    App/Hardware/HAL/EmergancyStop.h \
    App/Hardware/HAL/Guages.h \
    App/Hardware/HAL/Pumps.h \
    App/Hardware/HAL/Remote.h \
    App/Hardware/HAL/TemperatureSensor.h \
    App/Hardware/HAL/Valves.h \
    App/Hardware/HAL/HALSupport.h \
    App/Hardware/HAL/HalContainer.h \
    App/Hardware/HAL/Presenters/GuagesPresenter.h \
    App/Hardware/HAL/Presenters/PresenterSupport.h \
    App/Hardware/HAL/Presenters/PumpsPresenter.h \
    App/Hardware/HAL/Presenters/ValvesPresenter.h \
    App/Hardware/HAL/Presenters/EmergancyStopPresenter.h \
    App/Hardware/HAL/Presenters/RemotePresenter.h \
    App/Hardware/HAL/Presenters/TemperatureSensorPresenter.h \
    App/Experiment/Machines/MachineContainer.h \
    App/Experiment/Machines/Helpers/CommandValidatorState.h \
    App/Experiment/Machines/Helpers/FunctionsHelper.h \
    App/Experiment/Machines/Helpers/MachineStates.h \
    App/Experiment/Machines/Helpers/TransitionsBuilder.h \
    App/Experiment/Machines/Functions/ValveFunctions.h \
    App/Experiment/Machines/ReadPressure.h \
    App/Experiment/Machines/Functions/GuageFunctions.h \
    App/Experiment/Machines/Functions/EmergancyStopFunctions.h \
    App/Experiment/Machines/Functions/PumpFunctions.h \
    App/Experiment/Machines/Functions/RemoteFunctions.h \
    App/Experiment/Machines/Functions/TemperatureSensorFunctions.h \
    App/Experiment/Machines/ReadGuageTrip.h \
    App/Experiment/Machines/PumpControl.h


# Include the resource file
RESOURCES += \
    Resources/resources.qrc




# Additional import path used to resolve QML modules in Qt Creator's code model for Fluid GUI Library
QML_IMPORT_PATH = $$OUT_PWD/../vendor/fluid/qml
#QML2_IMPORT_PATH = $$OUT_PWD/../vendor/fluid/qml




# Android specific actions
android {
    # Bundle Fluid QML plugins with the application
    ANDROID_EXTRA_PLUGINS = $$OUT_PWD/../fluid/qml

    # Android package sources
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

# Mac os specific actions
macx {
    # Bundle Fluid QML plugins with the application
    APP_QML_FILES.files = $$OUT_PWD/../vendor/fluid/qml/Fluid
    APP_QML_FILES.path = Contents/MacOS
    QMAKE_BUNDLE_DATA += APP_QML_FILES
}

# Windows specific actions
win32 {
    # Bundle Fluid QML plugins with the application
    WINDEPLOYQT_OPTIONS = -qmldir $$OUT_PWD/../vendor/fluid/qml/Fluid
}




# Include the QutiPi project lib
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../vendor/qutipi-cpp/release/ -lqutipi-cpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../vendor/qutipi-cpp/debug/ -lqutipi-cpp
else:unix: LIBS += -L$$OUT_PWD/../vendor/qutipi-cpp/ -lqutipi-cpp
else:macx: LIBS += -L$$OUT_PWD/../vendor/qutipi-cpp/ -lqutipi-cpp

INCLUDEPATH +=  $$PWD/../vendor/qutipi-cpp \
                $$PWD/../vendor/qutipi-cpp/Targets/$${TARGETSOC} \
                $$PWD/../vendor/qutipi-cpp/Targets/$${TARGETSOC}/Boards/$${TARGETBOARD}
#DEPENDPATH += $$PWD/../vendor/qutipi-cpp

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../vendor/qutipi-cpp/release/libqutipi-cpp.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../vendor/qutipi-cpp/debug/libqutipi-cpp.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../vendor/qutipi-cpp/release/qutipi-cpp.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../vendor/qutipi-cpp/debug/qutipi-cpp.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../vendor/qutipi-cpp/libqutipi-cpp.a
else:macx:PRE_TARGETDEPS += $$OUT_PWD/../vendor/qutipi-cpp/libqutipi-cpp.a






# Configs from fluid framework
qtConfig(static) {
    QMAKE_LIBDIR += \
        $$OUT_PWD/../vendor/fluid/qml/Fluid/Core \
        $$OUT_PWD/../vendor/fluid/qml/Fluid/Controls \
        $$OUT_PWD/../vendor/fluid/qml/Fluid/Controls/Private \
        $$OUT_PWD/../vendor/fluid/qml/Fluid/Templates
    QTPLUGIN += \
        qsvg \
        fluidcoreplugin \
        fluidcontrolsplugin \
        fluidcontrolsprivateplugin \
        fluidtemplatesplugin
}



# Deploy fluid docs
fluiddocs.files = $$OUT_PWD/../vendor/fluid/
fluiddocs.path = /home/root/qutipi_remote/vendor

# Deploy setting files
settingsfiles.files = $$PWD/Resources/config
settingsfiles.path = /home/root/.config/vacuum_reservoir_controller/

# Deploy main program location
target.path = /home/root/qutipi_remote

# Register deployments
INSTALLS += target fluiddocs settingsfiles



# Include the read me files and licensing files
DISTFILES += \
    readme.md

