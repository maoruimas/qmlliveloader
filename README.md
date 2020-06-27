# Intro

以往怕麻烦的我一直在用Felgo自带的Felgo Live做QML实时预览, 不过Felgo虽好, 但强制增加~5秒的启动页面实在太恶心, 更何况Felgo更新慢 (似乎至今还是Qt 13? 不太清楚), 于是决定自己做一个, 本来也没有什么技术含量.

![](https://img2020.cnblogs.com/blog/1468726/202006/1468726-20200626171744251-897189811.gif)

[项目地址 - 码云](https://gitee.com/maoruimas/qmlliveloader)

注意, 发布程序时应链接所有Qml module, 以Windows为例
```cmd
windeployqt D:\QtRepository\qmlliveloader\win-deploy -qmldir D:\Qt\5.15.0\mingw81_64\qml
```

# TODO

File watcher理应递归地监视主文件夹下所有文件和文件夹, 不过方便起见我只监视了主文件夹. 经测试使用Qt Creator和Notepad++编辑文件效果良好, 使用Notepad和Vs code则检测不到更改.

# Reference

[Live Reloading or Hot Reloading with QML - qml.guide](https://qml.guide/live-reloading-hot-reloading-qml/)