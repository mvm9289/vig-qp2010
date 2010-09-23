TEMPLATE = app
TARGET =
DEPENDPATH += . ../myQPushButtonPlugin ../myQLCDNumberPlugin
INCLUDEPATH += . ../myQPushButtonPlugin ../myQLCDNumberPlugin

HEADERS += myForm.h ../myQPushButtonPlugin/myQPushButton.h ../myQLCDNumberPlugin/myQLCDNumber.h
FORMS += myForm.ui
SOURCES += main.cpp myForm.cpp ../myQPushButtonPlugin/myQPushButton.cpp ../myQLCDNumberPlugin/myQLCDNumber.cpp
