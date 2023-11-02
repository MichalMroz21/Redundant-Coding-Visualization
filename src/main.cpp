#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "CMakeConfig.hpp"
#include "debug_interceptor.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    auto debugInterceptor = Debug_Interceptor::getInstance();

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("ROOT_PATH", ROOT_PATH);

    const QUrl url(u"qrc:/RedundantCoding/src_gui/Main.qml"_qs);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
