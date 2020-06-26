#pragma once

#include <QFileSystemWatcher>

class MyFileWatcher : public QFileSystemWatcher
{
    Q_OBJECT
public:
    explicit MyFileWatcher(QObject *parent = nullptr);

    Q_INVOKABLE void setPath(const QUrl& url);

signals:
    void message(QString msg);
};