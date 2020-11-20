#pragma once

#include <QQmlApplicationEngine>

class QFileSystemWatcher;
class QTimer;

class QmlLiveEngine : public QQmlApplicationEngine
{
    Q_OBJECT
public:
    explicit QmlLiveEngine(QObject *parent = nullptr);

    void hotLoad(const QString &mainQml, bool integratedConsole = true);

    Q_INVOKABLE void clearCache() { clearComponentCache(); }
    Q_INVOKABLE QString getMainQml() { return m_mainQml; };

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
    QObject *m_checkBox;

    QString m_mainQml;
    QString m_dir;
};
