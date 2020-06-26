#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QtMessageHandler>

#include "myqmlapplicationengine.h"
#include "myfilewatcher.h"

QObject* consoleArea;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (consoleArea) {
        QString temp = msg;
        QMetaObject::invokeMethod(consoleArea, "append", Q_ARG(QString, temp));
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(messageHandler);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle("Fusion");

    MyQmlApplicationEngine engine;
    MyFileWatcher fileWatcher;
    engine.rootContext()->setContextProperty("$QmlEngine", &engine);
    engine.rootContext()->setContextProperty("$FileWatcher", &fileWatcher);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    consoleArea = engine.rootObjects().first()->findChild<QObject*>("consoleArea");

    return app.exec();
}
