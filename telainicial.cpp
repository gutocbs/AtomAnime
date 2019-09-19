#include "telainicial.h"
#include "ui_telainicial.h"

telainicial::telainicial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::telainicial)
{
    ui->setupUi(this);
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect  screenGeometry = screen->geometry();
        int height = screenGeometry.height();
//        int width = screenGeometry.width();
        if(height == 1080){
            ui->tela19201080->addWidget(&main19201080);
            ui->tela19201080->setCurrentIndex(1);
            ui->tela1366768->hide();
        }
        else if(height == 768){
            ui->tela1366768->addWidget(&main1336768);
            ui->tela1366768->setCurrentIndex(1);
        }
}

telainicial::~telainicial()
{
    delete ui;
}
