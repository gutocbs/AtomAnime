#ifndef TELAINICIAL_H
#define TELAINICIAL_H

#include <QWidget>
#include <QScreen>
#include <QDebug>
#include "mainwindow.h"
#include "mainwindow1366768.h"

namespace Ui {
class telainicial;
}

class telainicial : public QWidget
{
    Q_OBJECT

public:
    explicit telainicial(QWidget *parent = nullptr);
    ~telainicial();
    MainWindow main19201080;
    mainwindow1366768 main1336768;

private:
    Ui::telainicial *ui;

};

#endif // TELAINICIAL_H
