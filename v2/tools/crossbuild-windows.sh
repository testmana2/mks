#!/bin/sh

QT_WIN32_VERSION_DEFAULT="4.7.4"
QT_WIN32_VERSION="$1"

if [ -z "$QT_WIN32_VERSION" ]; then
    echo "Undefined version, defaulting to QT_WIN32_VERSION_DEFAULT"
    QT_WIN32_VERSION="$QT_WIN32_VERSION_DEFAULT"
fi

export QT_WIN32_VERSION="$QT_WIN32_VERSION"

echo "Crossbuilding for windows using Qt version $QT_WIN32_VERSION"

qmake-qt4 -r -spec "$HOME/.qt/win32-x11-g++" ./build.pro
make -j 6