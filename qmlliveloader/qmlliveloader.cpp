#include "qmlliveloader.h"
#include <QFileInfo>
#include <QTimer>
#include <QFileSystemWatcher>
#include <QQmlContext>
#include <QDir>

QObject *QmlLiveLoader::m_console = nullptr;

QmlLiveLoader::QmlLiveLoader(QObject *parent)
    : QQmlApplicationEngine(parent)
    , m_watcher(nullptr)
    , m_timer(nullptr)
    , m_window(nullptr)
{
}

void QmlLiveLoader::watch(const QString &mainQml)
{
    if (mainQml == m_mainQml) return;

    m_mainQml = mainQml;
    m_dir = m_mainQml.left(m_mainQml.lastIndexOf('/'));

    unwatchAll();
    watchFileSystemRecursively(m_dir);
}

void QmlLiveLoader::hotLoad(const QUrl &url)
{
    m_watcher = new QFileSystemWatcher(this);
    m_timer = new QTimer(this);

    m_timer->setInterval(500);
    m_timer->setSingleShot(true);

    connect(m_watcher, &QFileSystemWatcher::directoryChanged, this
        , &QmlLiveLoader::onFileSystemChanged);
    connect(m_watcher, &QFileSystemWatcher::fileChanged, this
        , &QmlLiveLoader::onFileSystemChanged);
    connect(m_timer, &QTimer::timeout
        , this, &QmlLiveLoader::onReloadRequested);

    rootContext()->setContextProperty("$QmlLiveLoader", this);
    load(url);

    m_window = rootObjects().first();

    m_console = rootObjects().first()->findChild<QObject*>("_console");
    qInstallMessageHandler(QmlLiveLoader::messageHandler);
}

void QmlLiveLoader::onFileSystemChanged()
{
    if (!m_timer) return;
    if (!m_timer->isActive()) m_timer->start();
}

void QmlLiveLoader::onReloadRequested()
{
    unwatchAll();
    watchFileSystemRecursively(m_dir);

    if (m_window->property("_reloadOnChanges").toBool()) {
        QMetaObject::invokeMethod(m_window, "_reload");
    }
}

void QmlLiveLoader::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);
    if (!m_console)
        return;

    QString str = msg;
    if (type == QtWarningMsg || type == QtCriticalMsg || type == QtFatalMsg) {
        str = "<font color='red'>" + str + "</font>";
    }
    QMetaObject::invokeMethod(m_console, "append", Q_ARG(QString, str));
}

void QmlLiveLoader::watchFileSystemRecursively(const QString &dir)
{
    QDir d(dir);
    QStringList files = d.entryList(QStringList() << "*.qml", QDir::Files);
    QStringList dirs = d.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    m_watcher->addPath(dir);
    for (QString &file: files) {
        m_watcher->addPath(dir + '/' + file);
    }

    for (QString &subdir: dirs) {
        watchFileSystemRecursively(dir + '/' + subdir);
    }
}

void QmlLiveLoader::unwatchAll()
{
    QStringList dirs = m_watcher->directories();
    QStringList files = m_watcher->files();
    QStringList fails;

    for (QString &dir: dirs) {
        if (!m_watcher->removePath(dir)) {
            fails << dir;
        }
    }
    for (QString &file: files) {
        if (!m_watcher->removePath(file)) {
            fails << file;
        }
    }

    if (!fails.empty()) {
        qWarning() << tr("The following directories or files "
                 "cannot be removed from file system watcher:");
        for (QString &fail: fails) {
            qWarning() << "\t" << fail;
        }
    }
}
