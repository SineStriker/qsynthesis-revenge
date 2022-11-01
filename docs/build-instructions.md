# Build Instructions

---

## Get Pre-built libraries

+ Find Qt Configuration path (The directory should contain `Qt5Config.cmake`)
    + For example, `C:\Qt\5.15.2\msvc2019\lib\cmake\Qt5`

+ Modify overrided vcpkg `triplet`
    + Edit `scripts/vcpkg/triplets/paths/path_qt.cmake`
    + Change the value of `_qt_dir` to your Qt Configuration path

+ Run `setup-vcpkg.bat` or `setup-vcpkg.sh` at repository root path

## Dependencies (No need to build)

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
    + https://github.com/wang-bin/avbuild

### Other Libraries
+ SDL2
+ QuaZip & zlib
+ FramelessHelper
+ yaml-cpp