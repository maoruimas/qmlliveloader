#include <QQmlFile>
#include <QFileInfo>
#include <QDir>
#include "myfilewatcher.h"

MyFileWatcher::MyFileWatcher(QObject *parent)
    : QFileSystemWatcher(parent)
{

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
