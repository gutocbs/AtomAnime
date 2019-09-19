#include "janelaconfiguracao.h"
#include "ui_janelaconfiguracao.h"

JanelaConfiguracao::JanelaConfiguracao(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JanelaConfiguracao)
{
    ui->setupUi(this);
//    setDiretoriosAnimes();
    leArquivoConf();
    ui->FeedTorrentGeral->setPlainText("https://www.tokyotosho.info/rss.php?filter=1,11&zwnj=0");
    ui->FeedTorrentEspecfico->setPlainText("https://nyaa.si/?page=rss&c=1_2&f=0&q=%title%");
    ui->torrentTempo->setPlainText("10");

    ui->anilistUsername->setMaximumBlockCount(1);
    ui->PastaDownloads->setMaximumBlockCount(1);
    ui->torrentTempo->setMaximumBlockCount(1);
}

JanelaConfiguracao::~JanelaConfiguracao()
{
    delete ui;
}

void JanelaConfiguracao::setDiretoriosAnimes(){
    diretorioAnime.append("E:/Animes");
    diretorioAnime.append(QDir::homePath() + "/Downloads/Animes");
}

//Retorna os dois como uma string.
//Devem ser separados com uma string list para ser lidos
QString JanelaConfiguracao::returnFeedBusca(){
    emit fBusca(ui->FeedTorrentGeral->toPlainText() + "-" + ui->FeedTorrentEspecfico->toPlainText());
    return ui->FeedTorrentGeral->toPlainText() + "-" + ui->FeedTorrentEspecfico->toPlainText();
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

int JanelaConfiguracao::returnDetection(){
    //Caso os dois estejam marcados
    if(ui->DetectionPlayer->checkState() == 2 && ui->DetectionStream->checkState() == 2){
        emit detec(3);
        return 3;
    }
    //Caso apenas o Player esteja marcado
    else if(ui->DetectionPlayer->checkState() == 2 && ui->DetectionStream->checkState() == 0){
        emit detec(2);
        return 2;
    }
    //Caso apenas a stream esteja marcado
    else if(ui->DetectionPlayer->checkState() == 0 && ui->DetectionStream->checkState() == 2){
        emit detec(1);
        return 1;
    }
    //Caso nenhum esteja marcado
    else{
        emit detec(0);
        return 0;
    }
}

void JanelaConfiguracao::setDetection(int dtc){
    if(dtc == 3){
        ui->DetectionPlayer->setCheckState(Qt::Checked);
        ui->DetectionStream->setCheckState(Qt::Checked);
    }
    //Caso apenas o Player esteja marcado
    else if(dtc == 2){
        ui->DetectionPlayer->setCheckState(Qt::Checked);
        ui->DetectionStream->setCheckState(Qt::Unchecked);
    }
    //Caso apenas a stream esteja marcado
    else if(dtc == 1){
        ui->DetectionPlayer->setCheckState(Qt::Unchecked);
        ui->DetectionStream->setCheckState(Qt::Checked);
    }
    //Caso nenhum esteja marcado
    else{
        ui->DetectionPlayer->setCheckState(Qt::Unchecked);
        ui->DetectionStream->setCheckState(Qt::Unchecked);
    }
}

int JanelaConfiguracao::returnDownloadListas(){
    if(ui->DownloadWatching->checkState() == 2 && ui->DownloadDropped->checkState() == 2 &&
            ui->DownloadOnHold->checkState() == 2 && ui->DownloadPlanToWatch->checkState() == 2){
        emit dListas(15);
        return 15;
    }
    //Caso apenas 3 estejam marcadas
    else if(ui->DownloadWatching->checkState() == 2 && ui->DownloadDropped->checkState() == 2 &&
            ui->DownloadOnHold->checkState() == 2 && ui->DownloadPlanToWatch->checkState() == 0){
        emit dListas(14);
        return 14;
    }
    else if(ui->DownloadWatching->checkState() == 2 && ui->DownloadDropped->checkState() == 2 &&
            ui->DownloadOnHold->checkState() == 0 && ui->DownloadPlanToWatch->checkState() == 2){
        emit dListas(13);
        return 13;
    }
    else if(ui->DownloadWatching->checkState() == 2 && ui->DownloadDropped->checkState() == 0 &&
            ui->DownloadOnHold->checkState() == 2 && ui->DownloadPlanToWatch->checkState() == 2){
        emit dListas(12);
        return 12;
    }
    else if(ui->DownloadWatching->checkState() == 0 && ui->DownloadDropped->checkState() == 2 &&
            ui->DownloadOnHold->checkState() == 2 && ui->DownloadPlanToWatch->checkState() == 2){
        emit dListas(11);
        return 11;
    }
    //Caso apenas 2 estejam marcados
    else if(ui->DownloadWatching->checkState() == 2 && ui->DownloadDropped->checkState() == 2 &&
            ui->DownloadOnHold->checkState() == 0 && ui->DownloadPlanToWatch->checkState() == 0){
        emit dListas(10);
        return 10;
    }
    else if(ui->DownloadWatching->checkState() == 2 && ui->DownloadDropped->checkState() == 0 &&
            ui->DownloadOnHold->checkState() == 2 && ui->DownloadPlanToWatch->checkState() == 0){
        emit dListas(9);
        return 9;
    }
    else if(ui->DownloadWatching->checkState() == 0 && ui->DownloadDropped->checkState() == 2 &&
            ui->DownloadOnHold->checkState() == 2 && ui->DownloadPlanToWatch->checkState() == 0){
        emit dListas(8);
        return 8;
    }
    else if(ui->DownloadWatching->checkState() == 2 && ui->DownloadDropped->checkState() == 0 &&
            ui->DownloadOnHold->checkState() == 0 && ui->DownloadPlanToWatch->checkState() == 2){
        emit dListas(7);
        return 7;
    }
    else if(ui->DownloadWatching->checkState() == 0 && ui->DownloadDropped->checkState() == 2 &&
            ui->DownloadOnHold->checkState() == 0 && ui->DownloadPlanToWatch->checkState() == 2){
        emit dListas(6);
        return 6;
    }
    else if(ui->DownloadWatching->checkState() == 0 && ui->DownloadDropped->checkState() == 0 &&
            ui->DownloadOnHold->checkState() == 2 && ui->DownloadPlanToWatch->checkState() == 2){
        emit dListas(5);
        return 5;
    }
    //Caso apenas um esteja marcado
    else if(ui->DownloadWatching->checkState() == 0 && ui->DownloadDropped->checkState() == 0 &&
            ui->DownloadOnHold->checkState() == 0 && ui->DownloadPlanToWatch->checkState() == 2){
        emit dListas(4);
        return 4;
    }
    else if(ui->DownloadWatching->checkState() == 0 && ui->DownloadDropped->checkState() == 0 &&
            ui->DownloadOnHold->checkState() == 2 && ui->DownloadPlanToWatch->checkState() == 0){
        emit dListas(3);
        return 3;
    }
    else if(ui->DownloadWatching->checkState() == 0 && ui->DownloadDropped->checkState() == 2 &&
            ui->DownloadOnHold->checkState() == 0 && ui->DownloadPlanToWatch->checkState() == 0){
        emit dListas(2);
        return 2;
    }
    else if(ui->DownloadWatching->checkState() == 2 && ui->DownloadDropped->checkState() == 0 &&
            ui->DownloadOnHold->checkState() == 0 && ui->DownloadPlanToWatch->checkState() == 0){
        emit dListas(1);
        return 1;
    }
    //Caso nenhum esteja marcado
    else{
        emit dListas(0);
        return 0;
    }
}

void JanelaConfiguracao::setDownloadListas(int jdl){
    if(jdl == 15){
        ui->DownloadWatching->setCheckState(Qt::Checked);
        ui->DownloadDropped->setCheckState(Qt::Checked);
        ui->DownloadOnHold->setCheckState(Qt::Checked);
        ui->DownloadPlanToWatch->setCheckState(Qt::Checked);
    }
    //Caso apenas 3 estejam marcadas
    else if(jdl == 14){
        ui->DownloadWatching->setCheckState(Qt::Checked);
        ui->DownloadDropped->setCheckState(Qt::Checked);
        ui->DownloadOnHold->setCheckState(Qt::Checked);
        ui->DownloadPlanToWatch->setCheckState(Qt::Unchecked);
    }
    else if(jdl == 13){
        ui->DownloadWatching->setCheckState(Qt::Checked);
        ui->DownloadDropped->setCheckState(Qt::Checked);
        ui->DownloadOnHold->setCheckState(Qt::Unchecked);
        ui->DownloadPlanToWatch->setCheckState(Qt::Checked);
    }
    else if(jdl == 12){
        ui->DownloadWatching->setCheckState(Qt::Checked);
        ui->DownloadDropped->setCheckState(Qt::Unchecked);
        ui->DownloadOnHold->setCheckState(Qt::Checked);
        ui->DownloadPlanToWatch->setCheckState(Qt::Checked);
    }
    else if(jdl == 11){
        ui->DownloadWatching->setCheckState(Qt::Unchecked);
        ui->DownloadDropped->setCheckState(Qt::Checked);
        ui->DownloadOnHold->setCheckState(Qt::Checked);
        ui->DownloadPlanToWatch->setCheckState(Qt::Checked);
    }
    //Caso apenas 2 estejam marcados
    else if(jdl == 10){
        ui->DownloadWatching->setCheckState(Qt::Checked);
        ui->DownloadDropped->setCheckState(Qt::Checked);
        ui->DownloadOnHold->setCheckState(Qt::Unchecked);
        ui->DownloadPlanToWatch->setCheckState(Qt::Unchecked);
    }
    else if(jdl == 9){
        ui->DownloadWatching->setCheckState(Qt::Checked);
        ui->DownloadDropped->setCheckState(Qt::Unchecked);
        ui->DownloadOnHold->setCheckState(Qt::Checked);
        ui->DownloadPlanToWatch->setCheckState(Qt::Unchecked);
    }
    else if(jdl == 8){
        ui->DownloadWatching->setCheckState(Qt::Unchecked);
        ui->DownloadDropped->setCheckState(Qt::Checked);
        ui->DownloadOnHold->setCheckState(Qt::Checked);
        ui->DownloadPlanToWatch->setCheckState(Qt::Unchecked);
    }
    else if(jdl == 7){
        ui->DownloadWatching->setCheckState(Qt::Checked);
        ui->DownloadDropped->setCheckState(Qt::Unchecked);
        ui->DownloadOnHold->setCheckState(Qt::Unchecked);
        ui->DownloadPlanToWatch->setCheckState(Qt::Checked);
    }
    else if(jdl == 6){
        ui->DownloadWatching->setCheckState(Qt::Unchecked);
        ui->DownloadDropped->setCheckState(Qt::Checked);
        ui->DownloadOnHold->setCheckState(Qt::Unchecked);
        ui->DownloadPlanToWatch->setCheckState(Qt::Checked);
    }
    else if(jdl == 5){
        ui->DownloadWatching->setCheckState(Qt::Unchecked);
        ui->DownloadDropped->setCheckState(Qt::Unchecked);
        ui->DownloadOnHold->setCheckState(Qt::Checked);
        ui->DownloadPlanToWatch->setCheckState(Qt::Checked);
    }
    //Caso apenas um esteja marcado
    else if(jdl == 4){
        ui->DownloadWatching->setCheckState(Qt::Unchecked);
        ui->DownloadDropped->setCheckState(Qt::Unchecked);
        ui->DownloadOnHold->setCheckState(Qt::Unchecked);
        ui->DownloadPlanToWatch->setCheckState(Qt::Checked);
    }
    else if(jdl == 3){
        ui->DownloadWatching->setCheckState(Qt::Unchecked);
        ui->DownloadDropped->setCheckState(Qt::Unchecked);
        ui->DownloadOnHold->setCheckState(Qt::Checked);
        ui->DownloadPlanToWatch->setCheckState(Qt::Unchecked);
    }
    else if(jdl == 2){
        ui->DownloadWatching->setCheckState(Qt::Unchecked);
        ui->DownloadDropped->setCheckState(Qt::Checked);
        ui->DownloadOnHold->setCheckState(Qt::Unchecked);
        ui->DownloadPlanToWatch->setCheckState(Qt::Unchecked);
    }
    else if(jdl == 1){
        ui->DownloadWatching->setCheckState(Qt::Checked);
        ui->DownloadDropped->setCheckState(Qt::Unchecked);
        ui->DownloadOnHold->setCheckState(Qt::Unchecked);
        ui->DownloadPlanToWatch->setCheckState(Qt::Unchecked);
    }
    //Caso nenhum esteja marcado
    else{
        ui->DownloadWatching->setCheckState(Qt::Unchecked);
        ui->DownloadDropped->setCheckState(Qt::Unchecked);
        ui->DownloadOnHold->setCheckState(Qt::Unchecked);
        ui->DownloadPlanToWatch->setCheckState(Qt::Unchecked);
    }
}


QString JanelaConfiguracao::returnFansub(){
    emit fSub(ui->StringFansub->toPlainText());
    return ui->StringFansub->toPlainText();
}

QString JanelaConfiguracao::returnQualidade(){
    emit quality(ui->QualidadeTorrent->currentText());
    return ui->QualidadeTorrent->currentText();
}

int JanelaConfiguracao::returnTempoDownload(){
    emit tDownload(ui->torrentTempo->toPlainText().toInt());
    return ui->torrentTempo->toPlainText().toInt();
}

int JanelaConfiguracao::returnImagemBaixaQualidade(){
    if(ui->qbImagemSim->isChecked() == true){
        emit bDownload(0);
        return 0;//Mudar isso pra um bool
    }
    else if(ui->qbImagemNao->isChecked() == true){
        emit bDownload(1);
        return 1;
    }
    return 1;
}


int JanelaConfiguracao::returnDownloadAutomatico(){
    if(ui->TorrentAutomaticoSim->isChecked() == true){
        emit dAutomatico(0);
        return 0;
    }
    else{
        emit dAutomatico(1);
        return 1;
    }
}


void JanelaConfiguracao::setDownloadAutomatico(int jda){
    if(jda == 1)
        ui->TorrentAutomaticoSim->setChecked(true);
    else
        ui->TorrentAutomaticoNao->setChecked(true);
}

QString JanelaConfiguracao::returnTorrentPadrao(){
    emit tPadrao(ui->TorrentPadrao->currentText());
    return ui->TorrentPadrao->currentText();
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
           else if(linha.at(0).compare("feedgeral") == 0){
               if(linha.at(1) == "")
                   feedGeral = "https://www.tokyotosho.info/rss.php?filter=1,11&zwnj=0";
               else
                   feedGeral = linha.at(1);
               ui->FeedTorrentGeral->setPlainText(feedGeral);
           }
           else if(linha.at(0).compare("feedespecifico") == 0){
               if(linha.at(1) == "")
                   feedEspecifico = "https://nyaa.si/?page=rss&c=1_2&f=0&q=%title%";
               else
                   feedEspecifico = linha.at(1);
               ui->FeedTorrentEspecfico->setPlainText(feedEspecifico);
           }
           else if(linha.at(0).compare("detection") == 0){
               detection = linha.at(1).toInt();
               setDetection(detection);
           }
           else if(linha.at(0).compare("downloadlistas") == 0){
                downloadListas = linha.at(1).toInt();
                setDownloadListas(downloadListas);
           }
           else if(linha.at(0).compare("tempodownload") == 0){
               tempoDownload = linha.at(1).toInt();
               ui->torrentTempo->setPlainText(QString::number(tempoDownload));
           }
           else if(linha.at(0).compare("downloadautomatico") == 0){
               downloadAutomatico = linha.at(1).toInt();
               setDownloadAutomatico(downloadAutomatico);
           }
           else if(linha.at(0).compare("downloadfolder") == 0){
               downloadFolder = linha.at(1);
               ui->PastaDownloads->setPlainText(downloadFolder);
           }
           else if(linha.at(0).compare("fansub") == 0){
               fansub = linha.at(1);
               ui->StringFansub->setPlainText(fansub);
           }
           else if(linha.at(0).compare("qualidade") == 0){
               qualidade = linha.at(1);
               for(int i = 0; i < ui->QualidadeTorrent->count(); i++){
                   if(ui->QualidadeTorrent->currentText() == qualidade){
                       ui->QualidadeTorrent->setCurrentIndex(i);
                       break;
                   }
               }
           }
           else if(linha.at(0).compare("torrent") == 0){
               torrent = linha.at(1);
               for(int i = 0; i < ui->TorrentPadrao->count(); i++){
                   if(ui->TorrentPadrao->currentText() == torrent){
                       ui->TorrentPadrao->setCurrentIndex(i);

                       break;
                   }
               }
           }
           else if(linha.at(0).compare("imagemBaixaQualidade") == 0){
               if(linha.at(1).toInt() == 0)
                   ui->qbImagemSim->setChecked(true);
               else
                   ui->qbImagemNao->setChecked(true);
           }
       }
       inputFile.close();
    }
    ui->DiretorioAnimesLista->update();
}

void JanelaConfiguracao::on_BotaoSelecionarPasta_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(dir != ""){
        ui->PastaDownloads->setPlainText(dir);
        emit dFolder(dir);
    }

}

QString JanelaConfiguracao::returnDownloadFolder(){
    return ui->PastaDownloads->toPlainText();
}

void JanelaConfiguracao::on_BotaoSalvar_clicked()
{
    returnUserAnilist();
    returnDownloadListas();
    returnDetection();
    returnTempoDownload();
    returnDownloadAutomatico();
    returnFeedBusca();
    returnFansub();
    returnQualidade();
    returnTorrentPadrao();
    returnImagemBaixaQualidade();
    emit salvaArquivo();
}
