#include "janelaconfiguracao.h"
#include "ui_janelaconfiguracao.h"

JanelaConfiguracao::JanelaConfiguracao(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JanelaConfiguracao)
{
    ui->setupUi(this);
//    setDiretoriosAnimes();
    leArquivoConf();
}

JanelaConfiguracao::~JanelaConfiguracao()
{
    delete ui;
}

void JanelaConfiguracao::on_BotaoCancelar_clicked()
{
    emit cancelado();
}


void JanelaConfiguracao::setDiretoriosAnimes(){
    diretorioAnime.append("E:/Animes");
    diretorioAnime.append(QDir::homePath() + "/Downloads/Animes");
}

QVector<QString> JanelaConfiguracao::retornaDiretorioAnime(){
    return diretorioAnime;
}

QString JanelaConfiguracao::returnUserAnilist(){
    return userAnilist;
}

QString JanelaConfiguracao::returnOrdemLista(){
    return ordem;
}

QString JanelaConfiguracao::returnRSS(){
//    https://www.tokyotosho.info/rss.php?filter=1&terms=%5B1080%5D
//    https://www.tokyotosho.info/rss.php?filter=1,11&zwnj=0
    return "https://www.tokyotosho.info/rss.php?filter=1,11&zwnj=0";
}

void JanelaConfiguracao::on_DiretorioAnimesAdiciona_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(dir != ""){
        diretorioAnime.append(dir);
        ui->DiretorioAnimesLista->addItem(dir);
        emit dirAdd(dir);
    }
}

void JanelaConfiguracao::on_DiretorioAnimesRemove_clicked()
{
    QString CI = ui->DiretorioAnimesLista->currentItem()->text();
    qDeleteAll(ui->DiretorioAnimesLista->selectedItems());
    emit dirRem(CI);
}

void JanelaConfiguracao::on_BotaoAutorizar_clicked()
{
    userAnilist = ui->anilistUsername->toPlainText();
    emit user();
}

void JanelaConfiguracao::mensagem(QString mens){
    if(mens.compare("EUsr") == 0){
        ui->displayMensagem->setText("Erro: Usuário não encontrado.");
        userAnilist = "";
        emit user();
    }
    else if(mens.compare("AUsr") == 0){
        ui->displayMensagem->setText("Usuário encontrado.");
    }
}

void JanelaConfiguracao::leArquivoConf(){
    QFile inputFile("Configurações/conf.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       QStringList linha;
       while (!in.atEnd())
       {
           linha = in.readLine().split('>');
           if(linha.at(0).compare("D01") == 0){
               diretorioAnime.append(linha.at(2));
               ui->DiretorioAnimesLista->addItem(linha.at(2));
           }
       }
       inputFile.close();
    }
    QFile inputFileUser("Configurações/user.txt");
    if(inputFileUser.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFileUser);
       QStringList linha;
       while (!in.atEnd())
       {
           linha = in.readLine().split('>');
           if(linha.at(0).compare("user") == 0){
               userAnilist = linha.at(1);
               ui->anilistUsername->setPlainText(userAnilist);
           }
           else if(linha.at(0).compare("ordem") == 0){
               ordem = linha.at(1);
           }
       }
       inputFile.close();
    }
    ui->DiretorioAnimesLista->update();
}
