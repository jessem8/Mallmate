#include <QApplication>
#include "parking.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ParkingWindow w;
    w.show();
    return a.exec();
}
