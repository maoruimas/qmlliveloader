[项目地址 - 码云](https://gitee.com/maoruimas/qmlliveloader)

# Qml Live Loader

简单的预览工具.

![](https://img2020.cnblogs.com/blog/1468726/202006/1468726-20200626171744251-897189811.gif)

发布程序时最好链接上所有 Qml module, 以 Windows 为例:
```cmd
windeployqt D:\QtRepository\qmlliveloader\win-deploy -qmldir D:\Qt\5.15.0\mingw81_64\qml
```

**注意:** 导入的 qml 文件根组件必须是 `Window` 或 `ApplicationWindow`, 并且所有 qml 文件都位于主 qml 文件所在的文件夹及其子文件夹中.

# Qml Live Engine

Qml live loader 一个很大的问题是只能预览纯 QML 文档, 不支持第三方项目中的 C++ 接口. 目前我的解决方案是将其修改后编译到第三方项目中, 需要做的修改如下:
1. 将下列文件导入项目
```
qmlliveengine.h
qmlliveengine.cpp
live.qml
```
2. 修改 main.cpp

Qt Creator 默认生成的 main.cpp 内容为
```cpp
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
```
`QmlLiveEngine` 继承自 `QQmlApplicationEngine`, 因此可以直接将 `QQmlApplicationEngine` 替换为 `QmlLiveEngine`.

要使用 Live, 需要启用 hot load, 并且必须用绝对路径而不是 qrc 链接:
```cpp
#include <QGuiApplication>
#include "qmlliveengine.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QmlLiveEngine engine;
    engine.hotLoad("/path/to/main.qml");

    return app.exec();
}
```
这里假定所有 qml 文件都在主 qml 文件所在的文件夹及其子文件夹中, 并且 live.qml 和主 qml 文件处于同级文件夹中.

另外, 由于 Qt Creator 自带的输出窗口显示效果不好, Live engine 默认将 console message 重定向至 live 窗口. 要使用自带的输出窗口, 只需要改为:
```cpp
engine.hotLoad("/path/to/main.qml", false);
```

# Reference

[Live Reloading or Hot Reloading with QML - qml.guide](https://qml.guide/live-reloading-hot-reloading-qml/)