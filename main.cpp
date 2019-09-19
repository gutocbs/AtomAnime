#include "mainwindow.h"
#include "telainicial.h"
#include <QApplication>
#include <QtDebug>
#include <QScreen>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
//    MainWindow w;
    telainicial w;
    w.showMaximized();

//    JanelaConfiguracao as;
//    as.showMaximized();
    //Basicamente, criar uma janela pra cada resolução é a melhor escolha
    return a.exec();
}
