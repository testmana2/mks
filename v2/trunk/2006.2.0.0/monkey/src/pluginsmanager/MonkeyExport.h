#ifndef MONKEY_EXPORT_H
#define MONKEY_EXPORT_H
//
#include <qglobal.h>
//
#if defined(MONKEY_CORE_BUILD)
#define Q_MONKEY_EXPORT Q_DECL_EXPORT
#else
#define Q_MONKEY_EXPORT Q_DECL_IMPORT
#endif
//
#endif // MONKEY_EXPORT_H
