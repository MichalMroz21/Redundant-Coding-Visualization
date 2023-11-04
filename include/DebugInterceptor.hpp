#ifndef DEBUG_INTERCEPTOR_H
#define DEBUG_INTERCEPTOR_H

#include <QObject>
#include <QCoreApplication>
#include <QDate>
#include <QFile>

class DebugInterceptor : public QObject
{
    Q_OBJECT

public:

    explicit DebugInterceptor(bool displayToConsole, bool saveToFile, QString logsPath, QObject *parent = nullptr);
    ~DebugInterceptor();

    static void myMessageOutputHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static QSharedPointer<DebugInterceptor> getInstance();

    void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg), disableDebug(), enableDebug();

    QString getCurrDate();


signals:


private:

    QSharedPointer<QFile> logFile;

    bool displayToConsole{}, saveToFile{}, enabled{true};

};

#endif // DEBUG_INTERCEPTOR_H
