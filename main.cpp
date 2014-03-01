#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Sylvain Mariel");
    QCoreApplication::setOrganizationDomain("sylvain-mariel.fr");
    QCoreApplication::setApplicationName("Plock");


    MainWindow w;
    w.show();

    return a.exec();
}
