# Build Instructions

---

This project use Microsoft vcpkg to manage its dependencies, you should finish the pre-build work before open the project. The vcpkg works as a submodule.

## Get Pre-built Libraries

+ Find Qt Configuration path (The directory should contain `Qt5Config.cmake`)
    + For example, `C:\Qt\5.15.2\msvc2019\lib\cmake\Qt5`

<!-- + Tell vcpkg the overrided `triplet`
    + Create `scripts/vcpkg/triplets/paths/path_qt.txt`
    + Fill in the Qt Configuration path as single line, the `path_qt.cmake` will read it -->

+ Run `python setup-vcpkg.py` at repository root path.
    + The first run must failed and a default configuration file `setup-vcpkg.json` will be generated, you need to edit each `value` correctly with the help of `intro`, then run again.
    + The script will simply pull `vcpkg` from GitHub and run a series of `vcpkg install` commands.

<!-- ## Project Configuration

+ The LabelVoice Application uses a lot of QSynthesis Modules and the restare disbled(e.g. Utaplugin).
+ See `Config.cmake` for details. -->

## Notification

+ This project is heavily nested with the particular compiler and specified edition of Qt libraries. As a result, we recommend using exactly Qt 5.15.2 and MSVC, GCC, Clang on each platform.

<!-- ## Dependencies (No need to build)

### FFmpeg

You should build FFmpeg with autoconf because it doesn't support CMake building system, and install it after make.

You can either build FFmpeg yourself or download pre-built libraries. The configuration is the same as `QtAV`.

+ Windows (With Msys2)
```sh
./configure --prefix=ffbuild/release --enable-pic --enable-shared --enable-asm --enable-x86asm --disable-debug --enable-stripping --disable-doc --enable-runtime-cpudetect --disable-ptx-compression --enable-mediafoundation --disable-vulkan --disable-postproc --toolchain=msvc --arch=x86_64
````

+ Building Tutorials
    + https://ffmpeg.xianwaizhiyin.net/debug-ffmpeg/msys2-msvc.html
    + https://blog.csdn.net/dss875914213/article/details/120897704

+ Pre-built Binaries
    + https://github.com/wang-bin/avbuild -->

### Other Libraries
+ SDL2
+ QuaZip & zlib
+ FramelessHelper
+ yaml-cpp