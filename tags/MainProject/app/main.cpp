#include <qapplication.h>
#include "Principal.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    Principal w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();

}
