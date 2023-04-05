# SettingCatalog

## ISettingPage

+ 负责单个设置页面，可对其进行`addPage`、`removePage`操作。

+ `sortKeyword`返回一个字符串常量，默认为`id`，建议将其重载为语言为英语时的`title`，这样英语时视觉上是关于名字有序的，切换为别的语言此排序方式依然保持。


## SettingCatalog
+ 负责管理树根上的所有`ISettingPage`，可对其进行`addPage`、`removePage`操作。由`ICore`或`SettingCatalog`负责提供`SettingDialog`显示的API。（QtCreator中为`Core::ICore::showOptionsDialog`）

+ `SettingDialog`使用懒加载方式，第一次调用时才创建，其生命周期由`SettingCatalog`负责管理。

+ `SettingDialog`创建时，按照`SettingCatalog`以及每个`ISettingPage`自己的`pages`进行递归创建`QTreeView`，同层的各`page`使用`sortKeyword`进行排序。

+ `SettingDialog`打开时，调用所有`ISettingPage`的`load`方法，在切换到某一个页面时，会调用此页面的`widget`函数，此函数中应创建一个页面并返回（之后调用返回以前创建的）。点击`Apply`后，会调用所有`ISettingPage`的`apply`函数，此函数中应自行判断是否进行了更改，如果更改了执行保存数据的操作。关闭时，会调用所有`ISettingPage`的`finish`函数，此函数中应当释放页面。