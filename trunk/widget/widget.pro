CONFIG     += designer plugin debug_and_release qt warn_on release
TARGET      = pluginGLWidget
QT         += opengl
OBJECTS_DIR = .obj
MOC_DIR     = .moc
TEMPLATE    = lib app
target.path = ~/.designer/plugins/designer
INSTALLS   += target
HEADERS     =  glwidgetPlugin.h glwidget.h
SOURCES     =  glwidgetPlugin.cpp glwidget.cpp
LANGUAGE	= C++

HEADERS	+= 	object.h \
	referencia.h \
   tram.h \
	scene.h \
	face.h \
	vertex.h \
	color.h \
	box.h \
	XML.h \
	material.h \
	material_lib.h \
	point.h \
	tinystr.h \
	tinyxml.h \
	vehicle.h \
	llum.h \
	finestraLlums.h \
	sliderText.h \
	boto.h \
	textField.h

SOURCES	+= object.cpp \
	referencia.cpp \
   tram.cpp \
	scene.cpp \
	face.cpp \
	vertex.cpp \
	color.cpp \
	box.cpp \
	XML.cpp \
	material.cpp \
	material_lib.cpp \
	point.cpp \
	tinystr.cpp \
	tinyxml.cpp \
	tinyxmlerror.cpp \
	tinyxmlparser.cpp \
	vehicle.cpp\
	finestraLlums.cpp \
	sliderText.cpp \
	boto.cpp \
	textField.cpp

FORMS	= EditorLlums.ui
