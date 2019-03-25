# Vacuum Reservoir Controller

Control program for a vacuum reservoir controller to be used as a backing system for turbo pumps on vacuum rigs.

This program is developed using Qt 5.10 and runs on the QutiPi hardware and OS.


## Setup

To setup the program first open a cmd / terminal and navigate to the folder where you wish your new project to exist.

```bash
cd C:/development/qt/
```

Next we are going to clone the project project into a folder called 'my_project'. You can replace 'vacuum_reservoir_controller' with a name of you choice.

```bash
git clone --recurse-submodules https://github.com/MU-Electronics/vacuum_reservoir_controller.git vacuum_reservoir_controller
```

You can now open the project.pro file in Qt Creator to start your new project.

### Update submodules

In the future if you want to update the fluid and quitpi-cpp packages you can run the below

```bash
git submodule update --remote --recursive
```

## Requirements

This project was intended to be used with Qt >= 5.10

The compiler for this program and unlimitly the QutiPi hardware can be found on the QutiPi meta-qutipi repo.

## License

Licensed under the [GNU Lesser General Public License (LGPL)](http://doc.qt.io/qt-5/lgpl.html) version 3.

All rights reserved to change at any time.
