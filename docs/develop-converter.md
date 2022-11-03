# 转换插件

## 前言

+ 作为一个歌声合成前端，与其他歌声合成工具的交互是必不可少的，因此需要支持主流的歌声合成工程格式。

+ 转换插件用于支持QSynthesis导入与导出Midi以及其他歌声合成工具的工程文件，如Synthesizer V的`svp`、X Studio的`svip`等格式。

+ 本项目中的转换插件基于OpenSVIP中间模型，可以按照OpenSVIP插件的开发思路开发本工程的转换插件。欲了解有关中间模型的更多信息，请参阅[OpenSVIP开发手册]((https://openvpi.github.io/docs/dev.html))。


## 插件系统

+ Qt官方提供的插件系统基于操作系统动态链接库机制实现，当插件需要被使用时，从文件系统动态加载`dll/so/dylib`库文件到内存，使用完毕后释放。

### 接口

+ 转换插件基于Qt插件系统，`ISVSConverter`是格式转换的基类，该基类包括两个虚函数。
    + `load`：从文件系统中读取源格式的文件，转换为OpenSVIP中间模型
        + `filename`：源格式文件绝对路径
        + `out`：输出模型的引用
        + `args`：转换参数字典
    + `save`：将OpenSVIP中间模型转换为目标格式，并写入文件系统中
        + `filename`：目标文件绝对路径
        + `in`：待转换模型
        + `args`：转换参数字典

### 协议

+ 转换参数字典默认为空，插件所支持的参数键值对由插件开发者指定，调用者应参考插件开发者的文档来设置转换参数。

+ `load`与`save`成功返回`true`，失败返回`false`；失败原因可以保存在成员变量`err`中。

## 插件开发方法

+ 假设你要开发的插件支持的格式扩展名为`acep`。

+ 进入`src/QsPlugin/SVSConverter`，将`sample`文件夹复制一份，改名为`qacep`，并进入此文件夹。

+ 将`CMakeLists.txt`第一行`project`名`sample`改为`qacep`，可修改`VERSION`后的版本号。后面涉及Windows资源文件的配置，将`WIN32_AUTHOR_NAME`变量的值更改为你的名字，`WIN32_COPYRIGHT_START_YEAR`与`WIN32_COPYRIGHT_END_YEAR`可以按需修改。

+ 将`plugin.json`按照同样的方式修改，需要指定开源协议、目标工程格式名字、目标工程格式扩展名（字符串数组）。

+ 回到上一层目录，将此目录下`CMakeLists.txt`中新增一行

````
add_subdirectory(qacep)
````

+ 至此，构建系统准备完毕。

+ 在此目录新建一个类，如`QAcepConverter`，头文件模板为
````
class Q_DECL_EXPORT QAcepConverter : public ISVSConverter {
    Q_OBJECT
    Q_INTERFACES(ISVSConverter)
    Q_PLUGIN_METADATA(IID ISVSConverter_IID FILE "plugin.json")
public:
    explicit QAcepConverter(QObject *parent = nullptr);
    ~QAcepConverter();

    bool load(const QString &filename, QSvipFile *out,
              const QMap<QString, QVariant> &args = {}) override;

    bool save(const QString &filename, const QSvipFile &in,
              const QMap<QString, QVariant> &args = {}) override;
};
````

+ 重新执行`CMake Configure`即可。

## 可扩展性

+ 默认的`CMakeLists.txt`只包含插件工程当前目录的所有`h`与`cpp`文件，如果源文件数量较多，可以自行将其分散到子目录并修改`CMakeLists.txt`将其包含进目标。

+ `ISVSPlugin`是`ISVSConverter`的父类，该基类提供两个公有成员变量，在插件的接口被调用前会被主程序初始化，在插件内部可以合理使用。
    + `path`: 插件库文件绝对路径
    + `locale`：插件调用时主程序的区域和语言

+ 插件自带的`qm`翻译文件建议嵌入二进制库文件。

+ 主程序依赖的所有动态库（包括`QtCore`、`QtWidgets`、`QtSvg`...）都可以被插件链接，如果插件有自己独有依赖的动态库，需要将其放入主程序所在目录或该目录下`lib`文件夹。因此插件所依赖的库最好使用静态链接，如果因为协议等因素不得已请在分发的文档中说明使用方式。

+ QSynthesis默认部署时所带的所有插件均为公共插件，可以被包括您开发的插件在内的其他任何插件使用，如`qzlib`提供了压缩与解压缩方法，可以使用`QPluginLoader`加载`compressengines/qzlib`。

+ 转换插件可以链接`QtWidgets`库，在读取或保存时可以弹出`QMessageBox`提示信息或`QDialog`进行用户交互（如多轨转单轨时进行选择），只要保证插件调用前后程序内存空间不变，不存在内存泄漏情况即可，开发者可以自行选择更人性化的处理方式。