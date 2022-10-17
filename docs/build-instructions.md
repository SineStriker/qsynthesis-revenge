# Build Instructions

---

## Dependencies

### FFmpeg

You should build FFmpeg with autoconf because it doesn't support CMake building system, and install it after make.

You can either build FFmpeg yourself or download pre-built libraries. The configuration is the same as `QtAV`.

+ Windows (With Msys2)
````
./configure --disable-doc --disable-debug --enable-shared --enable-runtime-cpudetect --enable-mediafoundation --disable-postproc --toolchain=msvc --enable-pic --prefix=ffbuild/build
````

Copy all the directories(`bin, include, lib...`) to `pkg/src/FFmpeg/usr`, make sure that all library declaration files `*.lib` are in `lib` directory instead of `bin` when you use MSVC.

You may need to change the directory structure, please refer to `pkg/src/FFmpeg/CMakeLists.txt`.

+ Building Tutorials
    + https://ffmpeg.xianwaizhiyin.net/debug-ffmpeg/msys2-msvc.html
    + https://blog.csdn.net/dss875914213/article/details/120897704

+ Pre-built
    + https://github.com/wang-bin/avbuild

### Prepare Pre-built Environment

Configure the CMake project in `pkg` directory and build virtual target `install_all`, then it should generate a directory called `libs` with all files which will be used in the main project.

+ SDL2
+ QuaZip & Zlib