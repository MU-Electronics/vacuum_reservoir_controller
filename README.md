# Qt Project

This Qt projects pulls together multiple Qt projects that's depend on each other. By using the below together it gives a seamless experience of using the Quti Pi with Qt.

## Setup

To setup a new project first open a cmd / terminal and navigate to the folder where you wish your new project to exist.

```bash
cd C:/development/qt/
```

 Next we are going to clone the Qutipi starter project into a folder called 'my_project'. You can replace 'my_project' with a name of you choice.

```bash
git clone --recurse-submodules https://github.com/QutiPi/qt-starter my_project
```

You can now open the project.pro file in Qt Creator to start your new project.

## Update submodules

In the future if you want to update the fluid and quitpi-cpp packages you can run the below

```bash
git submodule update --remote --recursive
```

## Requirements

This project was intended to be used with Qt >= 5.9.0 which is the current LTS version as of writing.

The project is also intended to be used with a QutiPi board however any board with a Raspberry pi CM3 should be fine also.

## Coding Standards

This project is no too strict on coding standard however i encourage you to have a scan through http://www.possibility.com/Cpp/CppCodingStandard.html

Noting the main difference betwen sytax for C vs C++, for example for function / method names

  * C: gpio_init()
  * C++: gpioInit()

## Folders

Below is an explaination of the main folder within this repo

### src

This is the folder that contains your program for the project. By default there is a setup Qt project which has a suggested strucure and core functionality.

Delete, modify, develop this Qt project to your requirments.

### vendor

This folder contains supporting libraries from Quti Pi and thridparty vendors.

This folder is managed by git's submodules feature and should not be edited.

#### fluid

This folder contains a Qt project called fluid developed by a [Liri](https://liri.io/).

Fluid contains a collection of cross-platform QtQuick components for building fluid and dynamic applications. You can use fluid's features in you main src project.

The version of Fluid used by QutiPi is V0.11.0 from the master branch. The latest / development version requires Qt >= 5.10.0 which QutiPi OS does not support (intentionally as Qt 5.9 is LTS).  


#### qutipi-c++

This folder contains a Qt project that is managed by Qutipi and is a set of C functions and C++ classes, which aids in using the functionality / control of QutiPi, additional ICs and modules.

Update to the lastest version by following the below

```bash
cd vendor/qutipi-cpp
git pull origin master
cd ../../
git add .
git commit -a -m "Updated to the latest release of QutiPi-Cpp"
```

## License

Licensed under the [GNU Lesser General Public License (LGPL)](http://doc.qt.io/qt-5/lgpl.html) version 3.

All rights reserved to change at any time.
