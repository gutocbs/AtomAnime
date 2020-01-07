#include "janeladeconfig.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("gutocbs");
    //    QCoreApplication::setOrganizationDomain("github.com");
    QCoreApplication::setApplicationName("Atom");
    MainWindow w;
    janeladeconfig jconfig;
    if(!jconfig.fretornaUsuario().isEmpty())
        w.setWindowTitle("Atom - " + jconfig.fretornaUsuario());
    else
        w.setWindowTitle("Atom");
//    w.setWindowIcon();
    w.showMaximized();
    return a.exec();
}
