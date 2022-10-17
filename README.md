# QSynthesis 2.0

## Intro

QSynthesis is an open-source software and a remake of vocal synthesis UTAU to provide UTAU community with user-friendly experience.

The project is in its second iteration.

## Compatibility

Compatible for all UTAU project files and voice database, as well as resamplers and wavtools which meet UTAU standards.

QSynthesis inherits UTAU's plug-in interface so that most original UTAU plugins for Windows are supported.

QSynthesis is able to run on **Windows, MacOS and Linux**.


## Build from source

The whole QSynthesis source code is written in C++ with Qt library. In order to facilitate code reuse and reduce compilation troubles, the project uses as few other libraries as possible.

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


## Features

+ Frameless Window
+ Customized Color And Themes
+ Real-time Rendering
+ Multilingual Environment
+ Effcient Voice Bank Management
+ Easy to draw pitch


## Open-source softwares used

#### Qt 5.15.2
+ Home: https://www.qt.io/
+ License: LGPL v3
+ Owner: Qt Company

#### Frameless Helper
+ Source: https://github.com/wangwenx190/framelesshelper
+ License: MIT License
+ Owner: wangwenx190

#### QMidi
+ Source: https://github.com/waddlesplash/QMidi
+ License: MIT License
+ Owner: waddlesplash

#### JetBrains IntelliJ IDEA
+ Source: https://github.com/JetBrains/intellij-community
+ License: Apache 2.0 License
+ Owner: JetBrains s.r.o.

## Contributors

+ Sine Striker (A.k.a VerySimple)
+ Toarumoe

Welcome to commit pull requests.

## License

QSynthesis 2.0 is licensed under Apache 2.0 License.