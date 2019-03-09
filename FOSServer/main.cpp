#include "fosserverwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FOSServerWidget w;
    w.show();

    return a.exec();
}
