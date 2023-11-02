#include "debug_interceptor.hpp"
#include "CMakeConfig.hpp"

QSharedPointer<Debug_Interceptor> Debug_Interceptor::getInstance(){
    static QSharedPointer<Debug_Interceptor> instance{new Debug_Interceptor(true, true, QString(ROOT_PATH) + "/Logs.txt")};
    return instance;
}

Debug_Interceptor::Debug_Interceptor(bool displayToConsole, bool saveToFile, QString logsPath, QObject *parent) :
    QObject{parent}, displayToConsole(displayToConsole), saveToFile(saveToFile){

    logFile = QSharedPointer<QFile>(new QFile(logsPath));

    if (logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)){

        QTextStream out(logFile.data());

        QString execTitle = "[Execution " + getCurrDate() + "]";

        out << QString("%1\n%2\n%1\n").arg(QString(execTitle.size(), '-'), execTitle);

        qInstallMessageHandler(&Debug_Interceptor::myMessageOutputHandler);
    }

    else{
        qCritical() << "Failed to open log file!";
    }
}

Debug_Interceptor::~Debug_Interceptor(){
    if(logFile && logFile->isOpen()) logFile->close();
}

QString Debug_Interceptor::getCurrDate(){
    QString currentDateStr { QDateTime::currentDateTime().toString(Qt::ISODate) };
    currentDateStr.replace('T', ' ');
    return currentDateStr;
}


void Debug_Interceptor::myMessageOutputHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QSharedPointer<Debug_Interceptor>instance{ getInstance() };
    if (instance) instance->myMessageOutput(type, context, msg);
}

void Debug_Interceptor::myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
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
