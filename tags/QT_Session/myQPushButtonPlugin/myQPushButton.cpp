#include "myQPushButton.h"

myQPushButton::myQPushButton(QWidget* parent) : QPushButton(parent)
{
	setInvisible();
}

void myQPushButton::changeState()
{
	switch(state)
	{
		case VISIBLE:
			setInvisible();
			break;
		case INVISIBLE:
			setVisible();
			break;
		default:
			break;
	}
	emit stateChanged(state);
}

inline void myQPushButton::setInvisible()
{
	state = INVISIBLE;
	setIconSize(QSize(0, 0));
}

inline void myQPushButton::setVisible()
{
	state = VISIBLE;
	setIconSize(QSize(ICON_SIZE, ICON_SIZE));
}
