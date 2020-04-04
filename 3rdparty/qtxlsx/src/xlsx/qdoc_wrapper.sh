#!/bin/sh
QT_VERSION=0.3.0
export QT_VERSION
QT_VER=0.3
export QT_VER
QT_VERSION_TAG=030
export QT_VERSION_TAG
QT_INSTALL_DOCS=/home/itt/anaconda3/doc
export QT_INSTALL_DOCS
BUILDDIR=/home/itt/Coding/QtCoding/QtXlsxWriter/src/xlsx
export BUILDDIR
exec /home/itt/anaconda3/bin/qdoc "$@"
