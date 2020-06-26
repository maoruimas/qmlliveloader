#include "myqmlapplicationengine.h"

MyQmlApplicationEngine::MyQmlApplicationEngine(QObject *parent)
    : QQmlApplicationEngine(parent)
{

}

void MyQmlApplicationEngine::clearCache()
{
    this->clearComponentCache();
}
