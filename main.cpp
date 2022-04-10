#include "menu.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion");
    menu w;
    w.setWindowTitle("Money Manager");
    w.show();
    return a.exec();
}
