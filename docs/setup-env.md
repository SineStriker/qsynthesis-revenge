# 准备环境

## 写在前面

在Windows上进行Qt开发，可以选择下面任何一款IDE
+ Visual Studio（不需要任何配置）
+ Clion（几乎不需要任何配置）
+ Qt Creator（需要初始化编译器和LLVM）
+ VSCode（需要下载CMake、Clangd插件，编写settings.json）

### 注意事项
+ 除了Qt Creator以外，其他所有非官方IDE都无法使用图形界面给工程添加资源条目，因此Qt Creator是必须安装的，在需要改动资源时用QtCreator打开`qrc`文件单独修改
+ 只有Qt Creator和VS支持qmake项目，不过本工程是CMake项目，所以都可以

## 工具集合

### 自己安装

+ Microsoft VC++ 编译器
+ Qt 5.15.2 开发套件
+ LLVM 静态分析工具（使用Qt Creator时需要安装）

### 软件附带

+ CMake
+ Ninja Build

在Qt与MSVC装完以后，它们各自都会带一份自己的CMake和Ninja，选择其中一个，将它们所在路径添加到系统环境变量中。

### MSVC

+ 在Visual Studio Installer中增加`使用C+的桌面开发`模块，添加如下组件，建议都选，ATL与MFC可以不选。

![](./images/image1.png)

### Qt

+ 参考链接：https://blog.csdn.net/Qi_1337/article/details/121249717

+ 版本选择：5.15.2

+ 安装组件：
    + Preview
    + Qt 5.15.2
        + MSVC 2019 32-bit
        + MSVC 2019 64-bit
        + Sources（源码，便于调试与造控件，如果嫌太大可以不要）
        + Qt Debug Information Files（调试文件）
    + Developer and Designer Tools
        + Qt Creator
        + Debugging Tools
        + Qt Creator Debug Symbols
        + Qt Creator Plugin Development
        + Qt Design Studio（设计器，有需要可以下）
        + CMake
        + Ninja

MinGW是另一个编译套件，可以选，但本工程大概率不会用到。

### LLVM

+ 下载地址：https://github.com/llvm/llvm-project/releases

+ 初始化：
    + 打开Qt Creator
    + 帮助-关于插件-C++
    + 打开Beautifier
    + 重启Qt Creator
    + 工具-选项-Beautifier-Clang Format
    + 修改ClangForamtCommand为`安装路径\bin\clang-format.exe`
    + 环境-键盘
    + 命令ClangFormat-FormatFile，快捷键自定义，我用的`Ctrl+Alt+L`
    + 确定保存即可

### CMake

+ VS的cmake插件在类似下面路径
    + `C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe`
    + 也可以手动从官网下载最新版本的CMake全套工具（包括GUI虽然没什么用），然后添加环境变量
+ 下载地址：https://cmake.org/files

+ 添加环境变量
    + 将`cmake`命令所在目录添加到环境变量即可


## OS Preparation

### Mac OSX

+ 在 macOS 平台编译时，如果因为使用homebrew安装Qt等原因导致产生了诸如`Could not find a package configuration file provided by "Qt5" with any of the following names:...`之类的错误，请在configure前提供如下环境变量以使CMake能找到Qt库的位置（请视情况自行根据安装Qt库的位置改变命令中的路径）：
   ```sh
   export QT_DIR=/opt/homebrew/opt/qt@5
   export Qt5_DIR=/opt/homebrew/opt/qt@5
   ```

### Linux

````
sudo apt install mesa-common-dev libgtk-3-dev libxext-dev libasound2-dev libpulse-dev
````

## 本工程的配置

### 工程说明

+ 本工程使用CMake构建系统，在Windows上默认使用MSVC 2019 64bit进行开发，如果下载Visual Studio 2022则使用MSVC 2022 64bit。

+ 本工程使用了很多除了Qt以外的库，在Linux上可以直接`make install`，在Windows上我把它们的源码独立在`src`的外面，在开发之前先构建它们。

### 准备FFmpeg库

+ 下载地址：https://github.com/SineStriker/binary-res
    + 将`bin`、`include`、`lib`、`share`目录直接复制到`pkg/src/FFmpeg/usr`中。
+ 由于需要自己编译FFmpeg，过于复杂建议直接复制粘贴。

### 构建Pre-Built库

+ 在开始菜单搜索`x64 Native Tools Command Prompt for VS 2019`（也可能是2022），它会配置好MSVC编译器的一系列环境变量给此终端上下文。

+ 切换到`pkg`目录
````
cd pkg
````

+ 执行CMake Configure
````
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Release -S. -Bbuild -G Ninja
````

+ 执行CMake Build
````
cmake --build build --target all
````

+ 执行CMake Install
````
cmake --build build --target install
````

+ 这些库也可以放在工程里一起参与构建，但是可能会带来不必要的编译任务，所以提前全部编译好。Ninja是用来引入并行化加速构建过程的，因此Build过程可能出错，重新执行Build命令即可。

+ 此外，由于一些库Debug版本与Release版本是不兼容的，还需要构建一份Debug版本的，在CMake Configure中把参数`-DCMAKE_BUILD_TYPE:STRING=Release`改为`-DCMAKE_BUILD_TYPE:STRING=Debug`，然后再次执行三次命令，CMake有热更新机制不用执行Clean。

### 打开工程

+ 使用Qt Creator打开`src/CMakeLists.txt`，选择编译器为64位MSVC，构建目录放在上一层

+ 使用Visual Studio直接打开工程目录，VS会自动检测CMake工程，添加配置可以点击上方`x64-Debug`下拉选择管理配置

![](./images/image2.png)

+ 可以为本项目添加x64-Release目标
+ 当前目标选择为`LabelVoice.exe`
+ 由于插件目标与主目标没有隐式依赖关系，而VS又是不支持同时构建所有目标的，所以如果更改了插件，那么请先构建插件目标，比如`NativeWindow.dll`，如果不构建此插件，那么`LabelVoice`会找不到无边框窗体插件而使用系统边框。