#include <QQmlFile>
#include <QFileInfo>
#include <QDir>
#include <QTimer>
#include "myfilewatcher.h"

#include <QDebug>

MyFileWatcher::MyFileWatcher(QObject *parent)
    : QFileSystemWatcher(parent), timer(new QTimer(this))
{
    timer->setInterval(500);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &MyFileWatcher::update);
    connect(this, &MyFileWatcher::directoryChanged, this, [=](const QString& path) {
        if (!timer->isActive()) {
            timer->start();
        }
    });
}

void MyFileWatcher::setPath(const QUrl &url)
{
    QStringList fileList = files();
    if (!fileList.isEmpty()) {
        fileList = removePaths(fileList);
        if (!fileList.isEmpty()) {
            qWarning() << "Fail to unwatch files:" << fileList;
        }
    }
    QStringList dirList = directories();
    if (!dirList.isEmpty()) {
        dirList = removePaths(dirList);
        if (!dirList.isEmpty()) {
            qWarning() << "Fail to unwatch directories:" << dirList;
        }
    }

    QString path = QQmlFile::urlToLocalFileOrQrc(url);
    QFileInfo info(path);
    if (!info.isFile()) {
        qWarning() << "Not a file or not local";
        return;
    }

    QString dirPath = path.left(path.lastIndexOf('/'));

    if (addPath(dirPath)) {
        qInfo() << dirPath << "is under watch";
    } else {
        qWarning() << "Fail to put" << dirPath << "under watch";
    }
}
