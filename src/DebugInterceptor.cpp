#include "DebugInterceptor.hpp"
#include "CMakeConfig.hpp"

QSharedPointer<DebugInterceptor> DebugInterceptor::getInstance(){
    static QSharedPointer<DebugInterceptor> instance{new DebugInterceptor(true, true, QString(ROOT_PATH) + "/Logs.txt")};
    return instance;
}

DebugInterceptor::DebugInterceptor(bool displayToConsole, bool saveToFile, QString logsPath, QObject *parent) :
    QObject{parent}, displayToConsole(displayToConsole), saveToFile(saveToFile){

    logFile = QSharedPointer<QFile>(new QFile(logsPath));

    if (logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)){

        QTextStream out(logFile.data());

        QString execTitle = "[Execution " + getCurrDate() + "]";

        out << QString("%1\n%2\n%1\n").arg(QString(execTitle.size(), '-'), execTitle);

        qInstallMessageHandler(&DebugInterceptor::myMessageOutputHandler);
    }

    else{
        qCritical() << "Failed to open log file!";
    }
}

DebugInterceptor::~DebugInterceptor(){
    if(logFile && logFile->isOpen()) logFile->close();
}

QString DebugInterceptor::getCurrDate(){
    QString currentDateStr { QDateTime::currentDateTime().toString(Qt::ISODate) };
    currentDateStr.replace('T', ' ');
    return currentDateStr;
}

void DebugInterceptor::disableDebug(){
    this->enabled = false;
}

void DebugInterceptor::enableDebug(){
    this->enabled = true;
}

void DebugInterceptor::myMessageOutputHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QSharedPointer<DebugInterceptor>instance{ getInstance() };
    if (instance) instance->myMessageOutput(type, context, msg);
}

void DebugInterceptor::myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if(this->enabled == false) return;

    QString msgType{}, contextFile = context.file;

    contextFile = contextFile.right(contextFile.length() - contextFile.lastIndexOf('/') - 1);

    QByteArray localMsg{ msg.toLocal8Bit() },
        contextFileBA = contextFile.toLocal8Bit();

    const char* contextFileConst = contextFileBA.data();
    const char* function{ context.function ? context.function : "" };

    QTextStream toLogFile(logFile.data());

    switch(type) {
        case QtDebugMsg:
            msgType = "Debug";
            break;
        case QtInfoMsg:
            msgType = "Info";
            break;
        case QtWarningMsg:
            msgType = "Warning";
            break;
        case QtCriticalMsg:
            msgType = "Critical";
            break;
        case QtFatalMsg:
            msgType = "Fatal";
            break;
    }

    if(saveToFile){
        toLogFile << QString("[%1 %2]: %3\n%4:%5, %6\n\n").arg(msgType, getCurrDate(), localMsg.constData(), contextFile, QString::number(context.line), function);
    }

    if(displayToConsole){
        fprintf(stdout, "[%s]: %s \n%s:%u, %s\n\n", msgType.toUtf8().constData(), localMsg.constData(), contextFileConst, context.line, function);
    }

}

