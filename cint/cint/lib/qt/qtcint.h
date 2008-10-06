/*************************************************************************
 * Copyright(c) 1995~2005  Masaharu Goto (cint@pcroot.cern.ch)
 *
 * For the licensing terms see the file COPYING
 *
 ************************************************************************/
// qtcint.h

#define  __attribute__(x)
// Lie to CINT to make it go!
#ifdef __CINT__

#ifdef QT3_SUPPORT
# undef QT3_SUPPORT
#endif
#ifndef QT_NO_TEXTCODEC
#  define QT_NO_TEXTCODEC
#endif

typedef long long __int64;
typedef __int64 Q_UINT64;
typedef __int64 quint64;
typedef __int64 __w64
typedef unsigned int uint;
static const int white = 0xff;
static const int black = 0x0;
typedef void* QTSFUNC;
class QPoint;
class QCursor {public:  const QPoint& pos(); };
typedef int Tag;
#define UINT_MAX qtcint_U_max
#define ULONG_MAX qtcint_UL_max
// Qt4 
class QTextStreamFunction;
typedef  char basic_string<wchar_t>

static unsigned int UINT_MAX;
static unsigned long ULONG_MAX;

#define QLIST_H
template <typename T> class QList;
template <typename T> class QListIterator;
template <typename T> class QMutableListIterator;

#define QVECTOR_H
template <typename T> class QVector;

#define QHASH_H
// originally it's not templated but hey who cares
template <typename T> uint qHash(T);

#define QSTYLEOPTION_H
class QStyleOption;
class QStyleOptionViewItem;
class QStyleOptionComboBox;

namespace QVariant {
   enum Type;
}

// const bool FALSE=false;
// const bool TRUE=true;

//#ifndef sizeof
//#  define sizeof(A)  1000
// #endif
class Data { int d; };

#ifndef typename
#  define  typename class
#endif

#ifdef  Q_TYPENAME 
#undef  Q_TYPENAME 
#endif
#define  Q_TYPENAME 

#ifdef Q_EXPORT
#undef Q_EXPORT
#endif
#define Q_EXPORT

#ifdef Q_INLINE_TEMPLATES
# undef Q_INLINE_TEMPLATES
#endif
#define Q_INLINE_TEMPLATES

#define __declspec(fake) 
#ifdef dllimport
# undef dllimport
#endif
#define dllimport
#endif

#include "qplatformdefs.h"

#ifdef QT3_SUPPORT
# undef QT3_SUPPORT
#endif
#ifndef QT_NO_TEXTCODEC
#  define QT_NO_TEXTCODEC
#endif
// add-tions 
#ifndef QT_NO_USERDATA
# define QT_NO_USERDATA
#endif

#ifndef QT_NO_PROPERTIES
#  define QT_NO_PROPERTIES
#endif

#ifndef  QT_NO_STL
#  define  QT_NO_STL
#endif

#ifndef QT_NO_REGEXP
#  define  QT_NO_REGEXP
#endif

#ifndef QT_NO_MEMBER_TEMPLATES
#  define QT_NO_MEMBER_TEMPLATES
#endif

#ifndef QT_NO_QOBJECT_CHECK
#  define  QT_NO_QOBJECT_CHECK
#endif

#ifndef QT_NO_DEBUG_STREAM 
#  define  QT_NO_DEBUG_STREAM
#endif

// ---- may be removed above 
#include <QtCore/QtCore>
#include <QtGui/QtGui>

#include "qtclasses.h"
#include "qtglobals.h"
#include "qtfunctions.h"
