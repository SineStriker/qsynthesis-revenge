# QSynthesis 2.0

## Intro

QSynthesis is an open-source software and a remake of vocal synthesis UTAU to provide UTAU community with user-friendly experience.

QSynthesis gains its name from Qt, a popular C++ GUI framework, and a part of the phrase "Vocal Synthesis".

This repository is the version of the second iteration.


## Compatibility

Compatible for all UTAU project files and voice database, as well as resamplers and wavtools which meet UTAU standards.

All 64bit UTAU plugins for Windows are supported on QSynthesis that inherits UTAU's plug-in interface.

QSynthesis is able to run on **Windows, MacOS and Linux**.


## Build from source

The whole QSynthesis source code is written in C++ with Qt library. In order to facilitate code reuse and reduce compile trouble, the project uses as few other static libraries as possible.

### Requirements

| Component  | Requirement | Detailed |
| :----:     | :----:      | :----:   |
| Qt         | >=5.15    | Core, Gui, Svg, Network, Multimedia    |
| Compiler      | >=C++17       | MSVC 2019, MinGW 0.8.1, GCC, Clang   |
| CMake      |  >=3.5      | >=3.20 is recommended  |


On Windows, compiling with Microsoft Visual C++ is strongly recommended.

For detailed information, see [Build Instructions](./docs/build-insturctions.md).

### Modules

On Windows, you can use the default `wavtool.exe` and `resamplers.exe` which are distributed with UTAU to synthesize the audio for QSynthesis.

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

#### Frq Reader
+ Source: https://github.com/titinko/frq_reader
+ License: MIT License
+ Owner: titinko

#### JetBrains IntelliJ IDEA
+ Source: https://github.com/JetBrains/intellij-community
+ License: Apache 2.0 License
+ Owner: JetBrains s.r.o.

## Libraries built for QSynthesis

#### FsNotifier
+ Source: https://github.com/SineStriker/fsnotifier
+ License: Apache-2.0 License

#### QScrollableTabWidget

+ Source: https://github.com/SineStriker/QScrollableTabWidget
+ License: Apache-2.0 License

## UTAU FRQ References

#### VocalShifter
+ Home: http://ackiesound.ifdef.jp/soko.html
+ License: *Unknown*

#### Mrq
+ Source: https://github.com/Sleepwalking/mrq
+ License: BSD
+ Owner: Kanru Hua

#### Pmk (TIPS frequency map file)
+ Origin: ScientistB
+ Owner: Masao
+ License: *Unknown*

#### World4Utau
+ Source: https://github.com/Luk3M/world4utau
+ License: GPL v3
+ Owner: Luk3M

## Contributors

+ Sine Striker (A.k.a VerySimple)
+ Toarumoe

Welcome to commit pull requests.

## License

QSynthesis 2.0 is licensed under Apache 2.0 License.