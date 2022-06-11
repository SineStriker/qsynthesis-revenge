# Build Instructions

## Preparation

### Windows

Windows SDK 10.0.19044 or newer is required since it ships the headers and the libraries required to build Windows Applications.

### Mac OS

To be implemented...

### Linux

#### Packages 

You need to install these packages to get the required C/C++ header files.

##### Midi

    sudo apt-get install libasound2-dev

##### OpenGL

    sudo apt-get install mesa-common-dev

##### X11

    sudo apt install libqt5x11extras5-dev

#### Local input method

Qt doesn't work well with input programs, you may need to move some shared libraries to the `lib` directory to enable it in Qt Application.

## Build from source

There's no other libraries you need to compile in advance.

You can compile the source code without any further compiling preparation and it should successfully build the executable file with a few minite. 


## Binary Modules

### Resamplers

The resampler is the basic tool to render audio from voice sample.

#### Usage

+ It should be spcified by the user in each project.
+ When real-time rendering mode is on, QSynthesis will automatically call the resampler in a number of background threads to generate lyric cache. Otherwise, an external wavtool should be specified as well.

#### Build

Check [Synthesizing Tools](./synthesizing-tools.md).

### Filesystem events processor

#### Usage

+ This is a component of JetBrains IntelliJ IDEA. Due to its strong performance and cross-platform feature, it's used to watch directories as a backend of QSynthesis. 

#### Build

Since it's coded with C, you can compile it from source code quickly. The source code is in `Modules/jetbrains-fsnotifier`, choose right version for your platform and build it.

## Directory Structure

If you enable `SETUP_WORKING_DIR` in `CMakeLists.txt` at root directory, the working directory `_workingDir_[Your OS Name]` will be automatically set up after CMake Configuration.

The following directory structure is required for the application to run.


+ Working Directory
    + config
        + docs
        + fonts
            + *xxx.ttf*
            + *xxx.ttc*
        + themes
        + tools
            + fsnotifier
    + plugins
        + *plugin1*
        + *...*
    + voice
        + *voice1*
        + *...*


## How to Run

QSynthesis checks its working directory when initializing, make sure the directory structure is consistent with above.

#### Woring directory

The working directory should be consitent with above,ou may need to set the working directory in your IDE manually.

#### Fsnotifier Module

The `fsnotifier` module is indispensable, on Windows, it's of the form like `fsnotifier.exe` or `fsnotifier64.exe`. On Mac and Linux, it's just named `fsnotifier` and should have the executable permission.

+ The `fsnotifier` serves as a child process of QSynthesis and uses IPC to communicate with host program, so you can choose other compiler to build it as long as it can run.

+ After build, you should rename it and move it to `config/tools` directory for QSynthesis to call.

+ You shouldn't change any directory structure related to it because QSynthesis call it by a fixed path.

+ You shouldn't make any change of this child process since any failure will cause bad feedback in QSynthesis, even abortion.