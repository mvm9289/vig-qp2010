#ifndef __MYQPUSHBUTTON__
#define __MYQPUSHBUTTON__

#include <QtDesigner/QDesignerExportWidget>
#include <QPushButton>

// Button states
#define VISIBLE 1
#define INVISIBLE -1

// Icon Size
#define ICON_SIZE 80

class QDESIGNER_WIDGET_EXPORT myQPushButton : public QPushButton
{
	Q_OBJECT
	
	private:
		int state;
	
		void setInvisible();
		void setVisible();
	
	public:
		myQPushButton(QWidget *parent = 0);
	
	public slots:
		void changeState();
	
	signals:
		void stateChanged(int);
};

#endif
