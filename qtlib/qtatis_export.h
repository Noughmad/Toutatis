#ifndef QTATIS_EXPORT_H
#define QTATIS_EXPORT_H

#ifndef QTATIS_EXPORT
#  if defined BUILD_QTATIS_LIB
#    define QTATIS_EXPORT Q_DECL_EXPORT
#  else
#    define QTATIS_EXPORT Q_DECL_IMPORT
#  endif
#endif

#endif // QTATIS_EXPORT_H