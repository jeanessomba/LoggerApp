#pragma once

#include <QObject>
#include <QtDebug>
#include <QFile>
#include <stdlib.h>
#include <stdio.h>
#include <QMutex>
#include <QDir>


class LoggerClass
{
private:
    LoggerClass() {}

    static bool _isInit;
    static QSharedPointer<QFile> _logFile;
    static bool _logToFile;
    static uint _logFileSize;
    static QMutex _mutex;
    [[nodiscard]]static bool logFilesManager(const QString &type, const QMessageLogContext &context, const QString &msg);
    [[nodiscard]]static bool writeLogFiles(const QString &name, const QString &type, const QMessageLogContext &context, const QString &msg);
    static void clearLogFiles(const QString &name);

    /// \brief Configure and redirect log output to stderr or in text file
    static void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

public:

    ///
    /// \brief Installs the message handler and setup logToFile initial values.
    /// \param logToFile set this to true enable the logging into log file.
    /// \param logFileSize is the maximal size of the log file
    ///
    static void init(bool logToFile = false, uint logFileSize = 1024 * 2);

};
