#include "TestClass.h"
#include <QLoggingCategory>


Q_LOGGING_CATEGORY(testClass, "project.testClass")

TestClass::TestClass()
{

}

void TestClass::loggingMessages()
{
    qCDebug(testClass)<< QStringLiteral("This is a debug logging");
    qCInfo(testClass)<< QStringLiteral("This is an info logging");
    qCWarning(testClass)<< QStringLiteral("This is a warning logging");
    qCCritical(testClass)<< QStringLiteral("This is a Critical logging");

}
