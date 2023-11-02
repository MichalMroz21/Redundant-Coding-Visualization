#ifndef DEBUG_INTERCEPTOR_H
#define DEBUG_INTERCEPTOR_H

#include <QObject>
#include <QCoreApplication>
#include <QDate>
#include <QFile>

class Debug_Interceptor : public QObject
{
    Q_OBJECT
public:
    explicit Debug_Interceptor(bool displayToConsole, bool saveToFile, QString logsPath, QObject *parent = nullptr);
    ~Debug_Interceptor();
    static void myMessageOutputHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    static QSharedPointer<Debug_Interceptor> getInstance();
    void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

private:
    QSharedPointer<QFile> logFile;
    bool displayToConsole{}, saveToFile{};
    QString getCurrDate();

signals:

};

#endif // DEBUG_INTERCEPTOR_H
