#include "janelaconfiguracao.h"
#include "ui_janelaconfiguracao.h"

JanelaConfiguracao::JanelaConfiguracao(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JanelaConfiguracao)
{
    ui->setupUi(this);
}

JanelaConfiguracao::~JanelaConfiguracao()
{
    delete ui;
}
