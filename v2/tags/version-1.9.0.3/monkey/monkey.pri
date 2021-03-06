# monkey includes path to include in plugins/core project

# include path
INCLUDEPATH *= $${PWD}/src
#INCLUDEPATH    *= $$getFolders( $${PWD}/src, resources )

# dependency
PRE_TARGETDEPS  *= $${PWD}

# library integration
greaterThan(QT_MAJOR_VERSION, 4):QT *= widgets printsupport
LIBS    *= -L$${PACKAGE_BUILD_PATH}/$${Q_TARGET_ARCH}/$$buildMode()
win32:LIBS  *= -l$${PACKAGE_TARGET}
