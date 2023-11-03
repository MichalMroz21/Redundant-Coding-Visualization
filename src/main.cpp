#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "CMakeConfig.hpp"
#include "debug_interceptor.hpp"
#include "hammingcode.hpp"

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    auto debugInterceptor = Debug_Interceptor::getInstance();
    auto hammingCode = QSharedPointer<HammingCode>(new HammingCode(QBitArray(4, 1)));

    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("ROOT_PATH", ROOT_PATH);

    const QUrl url(u"qrc:/RedundantCoding/src_gui/Main.qml"_qs);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);



    return app.exec();
}
