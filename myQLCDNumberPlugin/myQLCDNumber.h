#ifndef __MYQLCDNUMBER__
#define __MYQLCDNUMBER__

#include <QtDesigner/QDesignerExportWidget>
#include <QLCDNumber>

class QDESIGNER_WIDGET_EXPORT myQLCDNumber : public QLCDNumber
{
	Q_OBJECT
	
	public:
		myQLCDNumber(QWidget *parent = 0);
	
	public slots:
		void changeValueOn(int);
};

#endif
