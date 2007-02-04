#configuration
CONFIG +=  release stl qt_no_framework
QT_ARCH = i386
QT_EDITION = OpenSource
QT_CONFIG +=  qt3support accessibility minimal-config small-config medium-config large-config full-config ipv6 getaddrinfo ipv6ifname getifaddrs png no-gif system-freetype system-zlib nis iconv glib x11sm xshape xcursor xrandr xrender fontconfig tablet xkb release

#versioning
QT_VERSION = 4.2.2
QT_MAJOR_VERSION = 4
QT_MINOR_VERSION = 2
QT_PATCH_VERSION = 2

QMAKE_RPATHDIR += "/usr/local/Trolltech/Qt-4.2.2/lib"
