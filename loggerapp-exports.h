#pragma once

// Definition of DLL import/export macros

#include <QtCore/qglobal.h>

#if defined(LOGGERAPP_API_LIBRARY)
#define LOGGERAPP_API Q_DECL_EXPORT
#else
#define LOGGERAPP_API Q_DECL_IMPORT
#endif
