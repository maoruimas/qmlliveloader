#pragma once

#include <QQmlApplicationEngine>

class MyQmlApplicationEngine : public QQmlApplicationEngine
{
    Q_OBJECT
public:
    explicit MyQmlApplicationEngine(QObject *parent = nullptr);

    Q_INVOKABLE void clearCache();
};
