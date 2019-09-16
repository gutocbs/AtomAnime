#include "mainwindow1366768.h"
#include "ui_mainwindow1366768.h"

mainwindow1366768::mainwindow1366768(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainwindow1366768)
{
    ui->setupUi(this);
}

mainwindow1366768::~mainwindow1366768()
{
    delete ui;
}
