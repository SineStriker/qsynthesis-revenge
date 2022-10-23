# QSynthesis 2.0

## Intro

QSynthesis is an open-source software and a remake of vocal synthesis UTAU to provide UTAU community with user-friendly experience.

The project is in its second iteration.

## Compatibility

Compatible for all UTAU project files and voice database, as well as resamplers and wavtools which meet UTAU standards.

QSynthesis inherits UTAU's plug-in interface so that most original UTAU plugins for Windows are supported.

QSynthesis is able to run on **Windows, MacOS and Linux**.


## Build from source

### Requirements

| Component  | Requirement | Detailed |
| :----:     | :----:      | :----:   |
| Qt         | >=5.15    | Core, Gui, Svg, Network, Multimedia    |
| Compiler      | >=C++17       | MSVC 2019, MinGW 0.8.1, GCC, Clang   |
| CMake      |  >=3.5      | >=3.20 is recommended  |


On Windows, compiling with Microsoft Visual C++ is strongly recommended.

For detailed information, see [Build Instructions](./docs/build-instructions.md).

### Modules

On Windows, you can use the default `wavtool.exe` and `resampler.exe` which are distributed with UTAU to synthesize the audio for QSynthesis.

On Mac OS and Linux, there are no ready-made concatenating or resampling tools. You need to compile them yourself.

For detailed information, see [Synthesizing Tools](./docs/synthesizing-tools.md).


## Open-source softwares used

+ [Qt 5.15.2](https://www.qt.io/) (Core, Widgets, Svg, Network)
    + GNU LGPL v3
+ [FFmpeg](https://github.com/FFmpeg/FFmpeg) (libavcodec libavformat libavutil libswresample)
    + GNU LGPL v3
+ [SDL](https://github.com/libsdl-org/SDL) (Audio)
    + Zlib License
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
+ [Yaml CPP](https://github.com/jbeder/yaml-cpp)
    + MIT License

See [Plugins Brief](./docs/plugins-brief.md) for dependencies for each plugin.

## Contributors

+ Sine Striker (A.k.a VerySimple)
+ Toarumoe

Welcome to commit pull requests.

## License

QSynthesis 2.0 is licensed under Apache 2.0 License.