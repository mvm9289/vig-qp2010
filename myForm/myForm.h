#ifndef __MYFORM__
#define __MYFORM__

#include "ui_myForm.h"

class myForm : public QMainWindow
{
	Q_OBJECT

	private:
		Ui::P0_Application ui;

	public:
		myForm(QWidget* parent=0);
};

#endif
