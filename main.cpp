#include "mainwindow.h"
#include <QApplication>
#include <QtDebug>
#include <QScreen>
#include "config.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.showMaximized();
//    JanelaConfiguracao as;
//    as.showMaximized();
    //Basicamente, criar uma janela pra cada resolução é a melhor escolha
    return a.exec();
}
