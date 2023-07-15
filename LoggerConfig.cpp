#include "LoggerConfig.h"
#include <QTime>
#include <QDate>
#include <QCoreApplication>
#include <QLoggingCategory>

using namespace Logging;

bool LoggerConfig::_isInit = false;
QSharedPointer<QFile> LoggerConfig::_logFile = Q_NULLPTR;
bool LoggerConfig::_logToFile = false;
QMutex LoggerConfig::_mutex;
uint LoggerConfig::_logFileSize = 1024 * 2;

bool LoggerConfig::logFilesManager(const QString &type, const QMessageLogContext &context, const QString &msg)
{
    bool result{false};
    QString appCurrentPath{QDir::currentPath()};
    QString logFolderName("LogFiles");

    QDir logDir(appCurrentPath);
    if(!logDir.mkpath(logFolderName))// Will return false if the directory already exists
    {
        qWarning()<< QStringLiteral("Failed to creates the directory path!");
        return result;
    }
    QString absoluteFilePath = appCurrentPath + "/" + logFolderName;
    clearLogFiles(absoluteFilePath);

    return writeLogFiles(absoluteFilePath, type, context, msg);
}

bool LoggerConfig::writeLogFiles(const QString &name, const QString &type, const QMessageLogContext &context, const QString &msg)
{
    bool result{false};
    QString time = QTime::currentTime().toString("hh:mm:ss");
    QString logFileName = "/logFile.txt" ;

    if(_logFile.isNull())
        _logFile.reset(new QFile(name + logFileName));

    result = _logFile->open(QIODevice::WriteOnly | QIODevice::Append);
    if(result)
    {
        QTextStream textStream(_logFile.data());
        textStream << QString("%1 %2: %3 (%4)").arg(time, type, msg,  context.file) << Qt::endl;
    }
    _logFile->close();

    return result;
}

void LoggerConfig::clearLogFiles(const QString &name)
{
    QString logFileName = "/logFile.txt" ;

    if(_logFile.isNull() | !_logFile.isNull())
        _logFile.reset(new QFile(name + logFileName));

    int fileSize = _logFile->size();
    if(fileSize > _logFileSize)
    {
        _logFile->open(QIODevice::WriteOnly | QIODevice::Truncate);
    }

    _logFile->close();
}

void LoggerConfig::init(bool logToFile, uint logFileSize)
{
    if(_isInit)
        return;

    _logToFile = logToFile;
    _logFileSize = logFileSize;
    if(_logToFile)
        qInfo() << QStringLiteral("For logfile, see: %1").arg(QCoreApplication::applicationDirPath()+"/LogFiles...");

    qInstallMessageHandler(LoggerConfig::myMessageOutput);
    _isInit = true;
}

void LoggerConfig::setFilterRules(const QString &filter)
{
#ifdef QT_DEBUG
     // For no logging, set the type to false. e.g "project.testClass.debug=false\n"
    QLoggingCategory::setFilterRules(filter);
#else
  QLoggingCategory::setFilterRules("*.debug=false\n"
                                   "");
#endif
}

void LoggerConfig::myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QMutexLocker locker(&_mutex);

    QHash<QtMsgType, QString> msgTypeHash({{QtDebugMsg, "Debug"}, {QtInfoMsg, "Info"}, {QtWarningMsg, "Warning"}, {QtCriticalMsg, "Critical"}, {QtFatalMsg, "Fatal"}});
    QTime time = QTime::currentTime();
    QString formattedTime = time.toString("hh:mm:ss");
    QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();
    const char *color = "";
    const char *colorReset="\x1b[m";

    if(_logToFile)
    {
        if(!logFilesManager(msgTypeHash.value(type), context, msg))
            qDebug()<<"Failed to make log files folders";
    }
    else
    {
        QString appCurrentPath{QDir::currentPath()};
        QString absoluteFilePath = appCurrentPath + "/" + "LogFiles";
        clearLogFiles(absoluteFilePath);
        switch (type) {
        case QtDebugMsg:
            color = "\x1b[m";
            fprintf(stdout, "%s: %sDebug:%s %s (%s:%u, %s)\n", formattedTimeMsg.constData(),color, colorReset, msg.toLocal8Bit().constData(), context.file, context.line, context.function);
            fflush(stdout);
            break;
        case QtInfoMsg:
            color = "\x1b[96m";
            fprintf(stderr, "%s: %sInfo:%s %s (%s:%u, %s)\n", formattedTimeMsg.constData(), color, colorReset, msg.toLocal8Bit().constData(), context.file, context.line, context.function);
            break;
        case QtWarningMsg:
            color="\x1b[93m";
            fprintf(stderr, "%s: %sWarning: %s%s (%s:%u, %s)\n", formattedTimeMsg.constData(), color, msg.toLocal8Bit().constData(), colorReset, context.file, context.line, context.function);
            break;
        case QtCriticalMsg:
            color="\x1b[91m";
            fprintf(stderr, "%s: %sCritical: %s%s (%s:%u, %s)\n", formattedTimeMsg.constData(), color, msg.toLocal8Bit().constData(), colorReset, context.file, context.line, context.function);
            break;
        case QtFatalMsg:
            color="\x1b[7;91m";
            fprintf(stderr, "%s: %sFatal: %s%s (%s:%u, %s)\n", formattedTimeMsg.constData(), color, msg.toLocal8Bit().constData(), colorReset, context.file, context.line, context.function);
            break;
        }
        fflush(stderr);
    }

    if(type == QtMsgType::QtFatalMsg)
        abort();
}
