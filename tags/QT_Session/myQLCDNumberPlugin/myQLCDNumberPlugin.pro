CONFIG+=designer plugin release
TEMPLATE=lib
TARGET=
DEPENDPATH+=.
INCLUDEPATH+=.

HEADERS+=myQLCDNumber.h myQLCDNumberPlugin.h
SOURCES+=myQLCDNumber.cpp myQLCDNumberPlugin.cpp

target.path=~/.designer/plugins/designer
INSTALLS+=target
