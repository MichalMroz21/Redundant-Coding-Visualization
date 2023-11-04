#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QLoggingCategory>

#include "CMakeConfig.hpp"
#include "DebugInterceptor.hpp"

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    auto debugInterceptor = DebugInterceptor::getInstance();

    testing::InitGoogleTest(&argc, argv);

    debugInterceptor.data()->disableDebug();

    bool testsResult = RUN_ALL_TESTS();

    debugInterceptor.data()->enableDebug();

    if(testsResult != 0){
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
