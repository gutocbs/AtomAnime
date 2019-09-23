#include "mainwindow.h"
#include "mainwindow1366768.h"
#include <QApplication>
#include <QtDebug>
#include <QScreen>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
//        int width = screenGeometry.width();
    MainWindow w;
    mainwindow1366768 w1;
    if(height == 1080){
        w.showMaximized();
        w1.close();
    }
    else{
        w1.showMaximized();
        w.close();
    }

    return a.exec();
}
