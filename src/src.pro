TEMPLATE = app

QT += qml quick quickcontrols2 widgets serialport charts
CONFIG += c++11

#QMAKE_EXTRA_TARGETS += configfiles
#POST_TARGETDEPS += configfiles


# Application is very dynamic in nature to allow for easy expantion and since there is no worry of resource
# As such paramaters are passed to methods that they may not use but are required to keep a tidy format or for reflection
#QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter


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

RESOURCES += \
    Resources/resources.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
# QML_IMPORT_PATH = $$OUT_PWD/../fluid/qml
QML_IMPORT_PATH = $$OUT_PWD/../vendor/fluid/qml

# Additional import path used to resolve QML modules just for Qt Quick Designer
# QML_DESIGNER_IMPORT_PATH = $$OUT_PWD/../fluid/qml/Fluid/Controls/plugins.qmltypes
# QML_DESIGNER_IMPORT_PATH += $$OUT_PWD/../fluid/qml/Fluid/Controls

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

android {
    # Bundle Fluid QML plugins with the application
    ANDROID_EXTRA_PLUGINS = $$OUT_PWD/../fluid/qml

    # Android package sources
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
}

macx {
    # Bundle Fluid QML plugins with the application
    #APP_QML_FILES.files = $$OUT_PWD/../fluid/qml/Fluid
    APP_QML_FILES.files = $$OUT_PWD/../vendor/fluid/qml/Fluid
    APP_QML_FILES.path = Contents/MacOS
    QMAKE_BUNDLE_DATA += APP_QML_FILES
}

win32 {
    WINDEPLOYQT_OPTIONS = -qmldir $$OUT_PWD/../vendor/fluid/qml/Fluid
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    readme.md

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



win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../vendor/qutipi-cpp/release/ -lqutipi-cpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../vendor/qutipi-cpp/debug/ -lqutipi-cpp
else:unix: LIBS += -L$$OUT_PWD/../vendor/qutipi-cpp/ -lqutipi-cpp

INCLUDEPATH += $$PWD/../vendor/qutipi-cpp
DEPENDPATH += $$PWD/../vendor/qutipi-cpp

INCLUDEPATH += $$PWD/../vendor/qutipi-cpp/Targets/RaspberryPiCM3 \
