# Intro

简单的预览.

![](https://img2020.cnblogs.com/blog/1468726/202006/1468726-20200626171744251-897189811.gif)

[项目地址 - 码云](https://gitee.com/maoruimas/qmlliveloader)

注意, 发布程序时应链接所有Qml module, 以Windows为例
```cmd
windeployqt D:\QtRepository\qmlliveloader\win-deploy -qmldir D:\Qt\5.15.0\mingw81_64\qml
```

# TODO

* File watcher理应递归地监视主文件夹下所有文件和文件夹, 不过方便起见我只监视了主文件夹. 经测试使用Qt Creator和Notepad++编辑文件效果良好, 使用Notepad和Vs code则检测不到更改.
* 目前导入的Qml文件根组件必须是Window或ApplicationWindow

# Reference

[Live Reloading or Hot Reloading with QML - qml.guide](https://qml.guide/live-reloading-hot-reloading-qml/)

# Update

此方法很大的问题是只能预览纯 QML 文档, 不支持第三方项目中的 C++ 接口. 目前我的解决方案是将 Qml live loader 修改后编译到第三方项目中, 需要做的修改如下:
1. 将下列文件导入项目
```
qmlliveengine.h
qmlliveengine.cpp
live.qml
```
2. 修改 main.cpp
```cpp
...
#include <QQmlApplicationEngine>
...
int main(int argc, char *argv[])
{
    ...
    QQmlApplicationEngine engine;
    ...
}
```
改为
```cpp
...
#include <QQmlContext>
#include "qmlliveengine.h"
...
int main(int argc, char *argv[])
{
    ...
    QQmlApplicationEngine engine;
    engine.watch("path/to/project/src", "main.qml");
    engine.rootContext()->setContextProperty("$QmlLiveEngine", &engine);
    ...
}
```
代码作为 Qml live loader 的发行版存于[码云](https://gitee.com/maoruimas/qmlliveloader/releases/0.0.1).