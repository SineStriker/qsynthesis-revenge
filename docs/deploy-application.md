# 软件发布

## 构建说明

本软件采用CMake系统构建。

### 构建目标

+ 本软件的非共享代码一共构建两个目标
    + QsBlock（动态库）
    + QSynthesis（可执行）

+ 动态链接库`QsBlock`包含本软件的核心代码，可执行文件`QSynthesis`充当一个加载器。
+ 动态链接库`QsBlock`仅导出一个与`main`函数结构完全一样的函数，用来给`QSynthesis`调用，其他函数和类对外均不可见。

### 程序相关目录

+ 本软件的两个重要目录
    + lib
    + plugins
+ 目录`plugins`是Qt插件系统的检索目录，在`QApplication`实例化时使用`addLibraryPath`设定。同样，由于QSynthesis的插件也基于Qt插件系统，因此所有实现QSynthesis接口的插件（如ISvsConverter、IUtaPlugin等）也放置于这个目录。
+ 目录`lib`是程序会用到的其他动态库集中放置的路径。在Linux下，链接器默认会将此目录名设为动态库的检索路径；在Windows下，`QSynthesis`在入口点会调用`SetDllDirectoryW`将此目录加入动态库搜索路径集合；在Mac下，此目录不必存在。

### 开发模式

+ `src/CMakeLists.txt`中`APP_DEPLOY`变量设为关闭。

+ 在Windows下，`QSynthesis`隐式链接`QsBlock`，因此两个目标文件必须在同一目录。在开发状态下，两者在同一目录对于依赖关系比较友好，构建时两者会同时在`${CMAKE_BINARY_DIR}/bin`目录中生成。

+ `plugins`目录只包含QSynthesis的插件。QSynthesis插件在构建完成后会自动复制一份副本到`plugins`目录的相应子目录，主程序显式加载插件也会从里面搜索。

### 发布模式

+ `src/CMakeLists.txt`中`APP_DEPLOY`变量设为开启。

+ 在Windows下，`QSynthesis`在运行期显式加载`QsBlock`动态库。`QSynthesis`固定从`lib`目录寻找`QsBlock`，`QsBlock`构建完成后会自动复制一份副本到`lib`目录。

+ 在构建完成`QSynthesis`目标后，CMake会调用`deployqt`将Qt的动态库与自带插件部署在`lib`和`plugins`目录中。

## 其他

+ 在Linux和Mac下，不管是开发模式还是发布模式，`QSynthesis`均在编译期直接动态链接`QsBlock`。

+ 在Linux和Windows下，不管是开发模式还是发布模式，构建时会将除了Qt以外的外部库都复制一份到`lib`目录中。

+ 之所以要分成这两部分，主要是为了在Windows中能够把大部分动态库放置在`lib`目录中来保持根目录的简洁，这样的根目录结构也与Linux保持了统一。

+ 在发布模式下，您可以直接构建目标`deploy_app`（这个目标在开发模式是没有的），CMake会自动完成所有构建工作并将完整的发布版本输出在`CMAKE_BINARY_DIR`中。由于发布模式下各目标之间的依赖关系过于复杂不适合热加载，因此不应当反复修改源文件再中途构建。