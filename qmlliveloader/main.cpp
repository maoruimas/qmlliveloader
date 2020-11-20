#include <QGuiApplication>
#include <QQuickStyle>
#include "qmlliveloader.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Fusion");

    QmlLiveLoader loader;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&loader, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    loader.hotLoad(url);

    return app.exec();
}
