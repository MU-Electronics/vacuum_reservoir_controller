# Qt Project

This Qt projects pulls together multiple Qt projects thats depend on each other. By using the below together it gives a seamless experiance of using the Quti Pi with Qt.

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


#### qutipi-c++

This folder contains a Qt project that is managed by Qutipi and is a set of C++ class that aids in using the functionallity of QutiPi, addtionial ICs and modules.

## License



