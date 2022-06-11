# QSynthesis

## 介绍

歌声合成工具，兼容“歌声合成ツールUTAU”的所有工程文件、音源库、引擎，以及几乎所有的插件。

使用Qt开发，含有部分独立的跨平台代码，支持Windows、MacOS与Linux系统。

支持English、简体中文、繁體中文、日本語。

支持用户自定义颜色主题，默认提供浅色、深色两个主题。

## 软件部署

### Windows

从发布页面下载后，解压，将整个文件夹拷贝至您的个人目录，并手动创建桌面快捷方式即可。

建议赋予管理员权限。

### MacOS

从发布页面下载后，解压，将整个文件夹拷贝至您的个人目录，使用“xattr -cr”对文件夹赋予权限，然后可以手动创建桌面替身。

### Linux

从发布页面下载后，解压，将整个文件夹拷贝至您的个人目录，使用“chmod -R +x”对文件夹赋予权限，然后执行“./QSynthesis -d”自动创建.desktop快捷方式，剪切至桌面即可。

## 使用说明

1. 在Windows平台，本软件是64位程序，某些32位的引擎与插件可能无法正常调用
2.  请勿在两个进程中打开同一个音源文件夹，会有竞争问题

## 依赖

### Entropia File System Watcher

跨平台文件系统监视器，以静态库的形式链接到本程序的可执行文件中。

链接：https://github.com/SpartanJ/efsw

+ Windows：此仓库已准备了使用 MSVC2019 与 MinGW 8.1.0 编译的 efsw 静态库(.lib)，如果您用来编译本项目的编译器是其中之一，那么不用做任何改动；如果不是，那么请根据使用您的编译器编译此库，并修改.pro文件。

+ Linux64：此仓库已准备了使用 g++ 构建的 efsw 静态库(.a)。其他发行版的Linux请自行编译此库，并修改.pro文件。

+ MacOS：此仓库已准备了使用 Clang 64 构建的 efsw 静态库(.a)。如果使用其他编译器编译本项目，请自行编译此库，并修改.pro文件。

## 源代码构建

### Qt 5.15.2

在您的设备上安装 Qt 5.15.2， 执行qmake可直接编译。

### 运行

在工作目录下创建voice、plugins、config三个目录，继续在config中创建translations、themes两个目录，即可正常运行。

### 启动器

在Windows与Linux平台使用额外的启动器，只调用平台API，用以设置工作目录与创建本体进程，代码已开源。

## 获取引擎

### Windows

可用的引擎较多，直接拷贝UTAU自带的resampler.exe与wavtool.exe至QSynthesis目录并调用即可

### MacOS & Linux

可从以下页面获取 macres(tn_fnds) 的发布版本，已做了跨平台适配。

可从以下页面获取 world4utau 的发布版本，已做了跨平台适配。

可从以下页面获取 wavtool-yawu 的发布版本。

## 参与贡献

1.  Fork 本仓库
2.  新建 Feat_xxx 分支
3.  提交代码
4.  新建 Pull Request