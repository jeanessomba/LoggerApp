// Qt includes
#include <QCoreApplication>
#include <QLoggingCategory>

// Project includes
#include "LoggerClass.h"
#include "TestClass.h"



int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);

     LoggerClass::init(true);

#ifdef QT_DEBUG
     // For no logging, set the type to false. e.g "project.testClass.debug=false\n"
  QLoggingCategory::setFilterRules(
              "*.debug=true\n"
      "project.testClass.debug=true\n"
              "project.testClass.warning=true\n"
              "project.testClass.info=true\n"
              "project.testClass.critical=true\n"
      "");

#else
  QLoggingCategory::setFilterRules("*.debug=false\n"
                                   "");
#endif

  TestClass testClass;
  testClass.loggingMessages();

    return a.exec();
}
