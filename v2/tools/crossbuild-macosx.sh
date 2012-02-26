#!/bin/sh

QT_MAC_VERSION_DEFAULT="4.7.4"
QT_MAC_VERSION="$1"

if [ -z "$QT_MAC_VERSION" ]; then
    echo "Undefined version, defaulting to QT_MAC_VERSION_DEFAULT"
    QT_MAC_VERSION="$QT_MAC_VERSION_DEFAULT"
fi

export QT_MAC_VERSION="$QT_MAC_VERSION"

echo "Crossbuilding for mac os x using Qt version $QT_MAC_VERSION"

qmake-qt4 -r -spec "$HOME/.qt/macx-x11-g++42" ./build.pro
make -j 6