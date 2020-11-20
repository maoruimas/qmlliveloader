#pragma once

#include <QQmlApplicationEngine>

class QFileSystemWatcher;
class QTimer;

class QmlLiveLoader : public QQmlApplicationEngine
{
    Q_OBJECT
public:
    explicit QmlLiveLoader(QObject *parent = nullptr);

    void hotLoad(const QUrl &url);
    Q_INVOKABLE void watch(const QString &mainQml);
    Q_INVOKABLE void clearCache() { clearComponentCache(); }

private slots:
    void onFileSystemChanged();
    void onReloadRequested();

private:
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static QObject *m_console;

    void watchFileSystemRecursively(const QString &dir);
    void unwatchAll();

    QFileSystemWatcher *m_watcher;
    QTimer *m_timer;
    QObject *m_window;

    QString m_mainQml;
    QString m_dir;
};
