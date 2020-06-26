#include <QQmlFile>
#include <QFileInfo>
#include <QDir>
#include <QTimer>
#include "myfilewatcher.h"

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
    QString path = QQmlFile::urlToLocalFileOrQrc(url);
    QFileInfo info(path);
    if (!info.isFile()) {
        emit message(tr("Not a file"));
        return;
    }

    QString dirPath = path.left(path.lastIndexOf('/'));
    emit message(tr("Directory to be watched: ") + dirPath);

    QStringList dirsUnderWatch = directories();
    bool succeeded = true;

    if (dirsUnderWatch.isEmpty()) {
        succeeded = addPath(dirPath);
    } else if(dirPath != dirsUnderWatch[0]) {
        removePaths(dirsUnderWatch);
        succeeded = addPath(dirPath);
    }

    if (succeeded) {
        emit message(tr("Directory is under watch"));
    } else {
        emit message(tr("Fail to put the directory under watch"));
    }
}
