#include "janeladeconfig.h"
#include "ui_janeladeconfig.h"

janeladeconfig::janeladeconfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::janeladeconfig)
{
    ui->setupUi(this);
}

janeladeconfig::~janeladeconfig()
{
    delete ui;
}
