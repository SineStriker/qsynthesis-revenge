# QSynthesis Toolset

## Intro

QSynthesis is originally the name of the Qt-based UTAU editor, as a result of continuous iterations, its functionality has been abstracted into a class that can be used by multiple SVS frontend, so I called the library QSynthesis as well.

All applications in this toolset are free softwares and aim to provide the community with user-friendly experience.

## Compatibility

The application is able to run on **Windows, MacOS and Linux**.

## Toolset

### LabelVoice

LabelVoice is an annotation tool for vocal synthesis database.

### QSynthesis

QSynthesis is a remade edition of UTAU, and it is in its second iteration.

## Build from source

### Requirements

| Component | Requirement |              Detailed                |
|:---------:|:-----------:|:------------------------------------:|
|    Qt     |   >=5.15    |   Core, Gui, Widgets, Svg, Network   |
| Compiler  |   >=C++17   |        MSVC 2019, GCC, Clang         |
|   CMake   |   >=3.17    |        >=3.20 is recommended         |
|  Python   |   >=3.6     |                  /                   |


On Windows, compiling with MinGW is not tested.

For detailed information, see [Build Instructions](./docs/build-instructions.md).


### Modules

For detailed information, see [Related Tools](./docs/related-tools.md).


## Open-source softwares used

+ [Qt 5.15.2](https://www.qt.io/) (Core, Widgets, Svg, Network)
    + GNU LGPL v3
+ [FFmpeg](https://github.com/FFmpeg/FFmpeg) (libavcodec libavformat libavutil libswresample)
    + GNU LGPL v3
+ [SDL](https://github.com/libsdl-org/SDL) (Audio)
    + Zlib License
+ [vcpkg](https://github.com/microsoft/vcpkg)
    + MIT License
+ [FramelessHelper](https://github.com/wangwenx190/framelesshelper) (Core, Widgets)
    + MIT License
+ [QuaZip](https://github.com/stachenov/quazip)
    + GNU LGPL v2.1
+ [Zlib](http://www.zlib.net/)
    + Zlib License
+ [IntelliJ IDEA](https://github.com/JetBrains/intellij-community)
    + Apache 2.0 License
+ [OpenSVIP](https://github.com/yqzhishen/opensvip)
    + MIT License
+ [SingleApplication](https://github.com/itay-grudev/SingleApplication)
    + MIT License
+ [yaml-cpp](https://github.com/jbeder/yaml-cpp)
    + MIT License

See [Related Tools](./docs/related-tools.md) for dependencies for each plugin.

## Contributors

+ Sine Striker

Welcome to commit pull requests.

For Qt/C++ beginners, see [Setup Environment](./docs/setup-env.md) for instructions.

## License

QSynthesis Toolset is licensed under Apache 2.0 License.
