#include "myQLCDNumber.h"
#include "myQLCDNumberPlugin.h"
#include <QtPlugin>

myQLCDNumberPlugin :: myQLCDNumberPlugin(QObject *parent): QObject(parent)
{
	initialized = false;
}

void myQLCDNumberPlugin ::initialize(QDesignerFormEditorInterface * core)
{
	if (initialized) return;
	initialized = true;
}

bool myQLCDNumberPlugin ::isInitialized() const
{
	return initialized;
}

QWidget * myQLCDNumberPlugin ::createWidget(QWidget *parent)
{
	return new myQLCDNumber(parent); // Build the widget
}

QString myQLCDNumberPlugin ::name() const
{
	return "myQLCDNumber"; // Name of widget class
}

QString myQLCDNumberPlugin ::group() const
{
	return "My Widgets";
}

QIcon myQLCDNumberPlugin ::icon() const
{
	return QIcon();
}

QString myQLCDNumberPlugin ::toolTip() const
{
	return "";
}

QString myQLCDNumberPlugin ::whatsThis() const
{
	return "";
}

bool myQLCDNumberPlugin ::isContainer() const
{
	return false;
}

QString myQLCDNumberPlugin ::domXml() const
{
	return "<widget class=\"myQLCDNumber\" name=\"myQLCDNumberObject\">\n"
	" <property name=\"geometry\">\n"
	" <rect>\n"
	" <x>0</x>\n"
	" <y>0</y>\n"
	" <width>100</width>\n"
	" <height>100</height>\n"
	" </rect>\n"
	" </property>\n"
	"</widget>\n";
}

QString myQLCDNumberPlugin ::includeFile() const
{
	return "myQLCDNumber.h";
}

Q_EXPORT_PLUGIN2(myQLCDNumber, myQLCDNumberPlugin)
