# Chorus Kit

## Intro

<!-- QSynthesis is originally the name of the Qt-based UTAU editor, as a result of continuous iterations, its functionality has been abstracted into a library that can be used by multiple SVS frontend, so we called the library QSynthesis Library as well.

All applications in this toolset are free softwares and aim to provide the community with user-friendly experience. -->

ChorusKit is a Singing Voice Synthesis toolset that aims to provide the community with user-friendly experience.

## Applications

+ LabelVoice (Suspended)
    + Annnotation tool for vocal synthesis database.

+ DiffScope (Developing)
    + DiffSinger graphical editing tool.

<!-- + QSynthesis
    + Remade edition of UTAU in second iteration.

+ QVogenClient
    + Third-party edition of [Vogen.Client](https://github.com/aqtq314/Vogen.Client). -->

### Simple Tools

+ MinLabel
+ SlurCutter

## Supported Platforms

+ Microsoft Windows (Vista ~ 11)
+ Apple Mac OSX (11+)
+ Linux (Tested on Ubuntu)

## Build from source

### Requirements

| Component | Requirement |               Detailed               |
|:---------:|:-----------:|:------------------------------------:|
|    Qt     |   \>=5.15   |   Core, Gui, Widgets, Svg, Network   |
| Compiler  |  \>=C++17   |        MSVC 2019, GCC, Clang         |
|   CMake   |   \>=3.17   |        >=3.20 is recommended         |
|  Python   |   \>=3.8    |                  /                   |


On Windows, compiling with MinGW is not tested.

For detailed information, see [Build Instructions](./docs/build-instructions.md).


### Modules

For detailed information, see [Related Tools](./docs/related-tools.md).


## Open-source softwares used

+ [Qt 5.15.2](https://www.qt.io/) (Core, Widgets, Svg, Network)
    + GNU LGPL v2.1 or later
+ [FFmpeg](https://github.com/FFmpeg/FFmpeg) (libavcodec libavformat libavutil libswresample)
    + GNU LGPL v2.1 or later
+ [SDL](https://github.com/libsdl-org/SDL) (Audio)
    + Zlib License
+ [vcpkg](https://github.com/microsoft/vcpkg)
    + MIT License
+ [FramelessHelper](https://github.com/wangwenx190/framelesshelper) (Core, Widgets)
    + MIT License
+ [QuaZip](https://github.com/stachenov/quazip)
    + GNU LGPL v2.1
+ [Zlib](http://www.zlib.net)
    + Zlib License
+ [IntelliJ IDEA](https://github.com/JetBrains/intellij-community) (fsnotifier)
    + Apache 2.0 License
+ [OpenSVIP](https://github.com/yqzhishen/opensvip)
    + MIT License
+ [SingleApplication](https://github.com/itay-grudev/SingleApplication)
    + MIT License
+ [yaml-cpp](https://github.com/jbeder/yaml-cpp)
    + MIT License
+ [QNrbf](https://github.com/SineStriker/QNrbf)
    + GNU LGPL v2.1 or later
+ [qastool](https://github.com/SineStriker/qt-json-autogen)
    + Apache 2.0 License
+ [qBreakpad](https://github.com/buzzySmile/qBreakpad)
    + LGPL-3.0 license
+ [QtCreator 3.6](https://github.com/qt-creator/qt-creator/tree/3.6) (ExtensionSystem, Aggregation, Utils)
    + GNU LGPL v2.1 or later
<!-- # [LinuxDeployQt](https://github.com/probonopd/linuxdeployqt)
    + GPL v3 -->

See [Related Tools](./docs/related-tools.md) for dependencies for each plugin.

## Contribute to project

### Contributors

+ Sine Striker
+ [Arabaku](https://github.com/Arabaku)
+ [RigoLigoRLC](https://github.com/RigoLigoRLC)
+ [CrSjimo](https://github.com/CrSjimo)
+ [wolfgitpr](https://github.com/wolfgitpr)

Welcome to commit pull requests.

## License

ChorusKit is licensed under Apache 2.0 License.
