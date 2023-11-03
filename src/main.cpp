#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "CMakeConfig.hpp"
#include "debug_interceptor.hpp"

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    auto debugInterceptor = Debug_Interceptor::getInstance();

    testing::InitGoogleTest(&argc, argv);

    if(RUN_ALL_TESTS() != 0){
        qWarning() << "Not all tests passed!";
    }

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("ROOT_PATH", ROOT_PATH);

    const QUrl url(u"qrc:/RedundantCoding/src_gui/Main.qml"_qs);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
