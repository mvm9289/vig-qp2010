#include "myQLCDNumber.h"

myQLCDNumber::myQLCDNumber(QWidget* parent) : QLCDNumber(parent)
{
	display(0);
}

void myQLCDNumber::changeValueOn(int increment)
{
	display(intValue() + increment);
}