# LabelVoice

## Intro

LabelVoice is an open-source software and a labeling tool for vocal synthesis database to provide the community with user-friendly experience.

## Compatibility

LabelVoice is able to run on **Windows, MacOS and Linux**.


## Build from source

### Requirements

| Component  | Requirement | Detailed |
| :----:     | :----:      | :----:   |
| Qt         | >=5.15    | Core, Gui, Svg, Network, Multimedia    |
| Compiler      | >=C++17       | MSVC 2019, MinGW 0.8.1, GCC, Clang   |
| CMake      |  >=3.5      | >=3.20 is recommended  |


On Windows, compiling with Microsoft Visual C++ is strongly recommended.

For detailed information, see [Build Instructions](./docs/build-instructions.md).


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

See [Plugins Brief](./docs/plugins-brief.md) for dependencies for each plugin.

## Contributors

+ OpenVPI Team

Welcome to commit pull requests.

For Qt/C++ beginners, see [Setup Environment](./docs/setup-env.md) for instructions.

## License

LabelVoice is licensed under Apache 2.0 License.