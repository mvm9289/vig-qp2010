CONFIG+=designer plugin release
TEMPLATE=lib
TARGET=
DEPENDPATH+=.
INCLUDEPATH+=.

HEADERS+=myQPushButton.h myQPushButtonPlugin.h
SOURCES+=myQPushButton.cpp myQPushButtonPlugin.cpp

target.path=~/.designer/plugins/designer
INSTALLS+=target
