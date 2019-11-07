#include "mainwindow.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("gutocbs");
    QCoreApplication::setApplicationName("Atom");
    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
    return a.exec();
}
