# Gas Rig

This program was developed to control a gas rig developed by the electronics section within The University of Manchester.

The program will work across platform, but is not reccomened for embeded devices as no resource or speed optimisation
will or have taken place (due to time limits).

![alt text](https://github.com/MU-Electronics/GasRig/raw/master/resources/assets/images/TestingControl.png "Testing and Maintanence View")


# Qt Framework

This project was designed to run on Qt5.9+.


# Install

```
git clone https://github.com/MU-Electronics/GasRig
git submodule init
git submodule update
```


# For Automated Builds

During development Jenkins was used to act as a quick and dirty remote deployment tool.

When a commit is made to the repo Jenkins would run the following script to

  * Pull the new code from git
  * Kill the running instance of the program
  * Delete the current build directory
  * Make a new build directory
  * Generate the make files using qtmake
  * Compile the program using mingw32
  * Copy the required dll files to the directory from qt
  * Run the new version of the software

```BASH
REM "Update the source code"
cd c:\Users\xray\Desktop\GasRig\GasRig
git pull
REM "Shut down the running instance of the program"
Taskkill /IM GasRig.exe /F
REM "Delete the old program folder"
RD /S /Q C:\Users\xray\Desktop\GasRig\build
REM "Make the folder again and cd into the directory"
cd C:\Users\xray\Desktop\GasRig
mkdir build
cd build
REM "Qt generate the make files"
"c:\Qt\5.9.1\mingw53_32\bin\qmake.exe" "c:\development\qt\GasRig\GasRig.pro" -r -spec win32-g++ CONFIG+=production
REM "Compile the release"
mingw32-make -f Makefile.Release
REM "Ensure all the required DLLs are avaiable to the program"
"c:\Qt\5.9.1\mingw53_32\bin\windeployqt" --qmldir "C:\development\qt\GasRig" "C:\development\qt\build-gasRig\release"
REM "Copy missing dlls that windeployqt seems to miss"
copy c:\Qt\5.9.1\mingw53_32\bin\libwinpthread-1.dll C:\development\qt\build-gasRig\release\libwinpthread-1.dll
copy c:\Qt\5.9.1\mingw53_32\bin\Qt5OpenGL.dll C:\development\qt\build-gasRig\release\Qt5OpenGL.dll
copy c:\Qt\5.9.1\mingw53_32\bin\Qt5MultimediaQuick_p.dll C:\development\qt\build-gasRig\release\Qt5MultimediaQuick_p.dll
copy c:\Qt\5.9.1\mingw53_32\bin\Qt5Multimedia.dll C:\development\qt\build-gasRig\release\Qt5Multimedia.dll
copy c:\Qt\5.9.1\mingw53_32\bin\Qt5Concurrent.dll C:\development\qt\build-gasRig\release\Qt5Concurrent.dll
copy c:\Qt\5.9.1\mingw53_32\bin\Qt5DesignerComponents.dll C:\development\qt\build-gasRig\release\Qt5DesignerComponents.dll
copy c:\Qt\5.9.1\mingw53_32\bin\Qt5Designer.dll C:\development\qt\build-gasRig\release\Qt5Designer.dll
REM "Run the new app"
"C:\PSTools\PsExec.exe" -u xray -p "PASSWORD" -accepteula "C:\Users\xray\Desktop\GasRig\build\release\GasRig.exe"
```
