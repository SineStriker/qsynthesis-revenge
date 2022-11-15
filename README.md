# QSynthesis 2.0

## Intro

QSynthesis is an open-source software and a remake of vocal synthesis UTAU to provide UTAU community with user-friendly experience.

The project is in its second iteration.

## Compatibility

LabelVoice is able to run on **Windows, MacOS and Linux**.


## Build from source

### Requirements

| Component | Requirement |              Detailed                |
|:---------:|:-----------:|:------------------------------------:|
|    Qt     |   >=5.15    |   Core, Gui, Widgets, Svg, Network   |
| Compiler  |   >=C++17   |        MSVC 2019, GCC, Clang         |
|   CMake   |   >=3.17    |        >=3.20 is recommended         |
|  Python   |   >=3.6     |             annotations              |


On Windows, compiling with MinGW is not tested.

For detailed information, see [Build Instructions](./docs/build-instructions.md).


### Modules

On Windows, you can use the default `wavtool.exe` and `resampler.exe` which are distributed with UTAU to synthesize the audio for QSynthesis.

On Mac OS and Linux, there are no ready-made concatenating or resampling tools. You need to compile them yourself.

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

See [Plugins Brief](./docs/plugins-brief.md) for dependencies for each plugin.

## Contributors

+ Sine Striker

Welcome to commit pull requests.

For Qt/C++ beginners, see [Setup Environment](./docs/setup-env.md) for instructions.

## License

QSynthesis 2.0 is licensed under Apache 2.0 License.
