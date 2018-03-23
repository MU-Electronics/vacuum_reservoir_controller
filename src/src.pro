
# Set this project as the main app
TEMPLATE = app

# Icnlude some useful Qt libs by default
QT += qml quick quickcontrols2 widgets serialport charts

# Ensure we're using C11 standard
CONFIG += c++11 static

# Include the confiruation file
include($$PWD/../config.pri)

# Warnings for deprecated Qt features
DEFINES += QT_DEPRECATED_WARNINGS




# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




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
    App/Services/Debugging.cpp

# Include all the header files
HEADERS += \
    bootstrap/Startup.h \
    Bootstrap/Startup.h \
    App/Experiment/Engine.h \
    App/Services/Logger.h \
    App/Application.h \
    App/Services/Thread.h \
    App/Services/JsonFile.h \
    App/Settings/Container.h \
    App/Services/SerialController.h \
    App/View/Managers/Global.h \
    App/Services/Debugger.h \
    App/View/Managers/Graphs/Graph.h \
    App/Services/Debuging.h


# Include the resource file
RESOURCES += \
    Resources/resources.qrc




# Additional import path used to resolve QML modules in Qt Creator's code model for Fluid GUI Library
QML_IMPORT_PATH = $$OUT_PWD/../vendor/fluid/qml




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




# Include the QutiPi prorject lubaray
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../vendor/qutipi-cpp/release/ -lqutipi-cpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../vendor/qutipi-cpp/debug/ -lqutipi-cpp
else:unix: LIBS += -L$$OUT_PWD/../vendor/qutipi-cpp -lqutipi-cpp
LIBS += -L$$OUT_PWD/../vendor/qutipi-cpp -lqutipi-cpp
INCLUDEPATH += $$PWD/../vendor/qutipi-cpp
DEPENDPATH += $$PWD/../vendor/qutipi-cpp

# Include the target board headers
INCLUDEPATH += $$PWD/../vendor/qutipi-cpp/Targets/$${TARGETBOARD} \




# Include the read me files and licensing files
DISTFILES += \
    readme.md
