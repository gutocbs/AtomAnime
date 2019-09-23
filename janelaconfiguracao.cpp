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

QString JanelaConfiguracao::returnDownloadListas(){
    QString marcado = "";
    if(ui->DownloadWatching->checkState() == 2)
        marcado.append("w");
    if(ui->DownloadOnHold->checkState() == 2)
        marcado.append("o");
    if(ui->DownloadDropped->checkState() == 2)
        marcado.append("d");
    if(ui->DownloadPlanToWatch->checkState() == 2)
        marcado.append("p");
    emit dListas(marcado);
    return marcado;
}

void JanelaConfiguracao::setDownloadListas(QString jdl){
    if(jdl.contains("w") == true)
        ui->DownloadWatching->setCheckState(Qt::Checked);
    else
        ui->DownloadWatching->setCheckState(Qt::Unchecked);
    if(jdl.contains("d") == true)
        ui->DownloadDropped->setCheckState(Qt::Checked);
    else
        ui->DownloadDropped->setCheckState(Qt::Unchecked);
    if(jdl.contains("o") == true)
        ui->DownloadOnHold->setCheckState(Qt::Checked);
    else
        ui->DownloadOnHold->setCheckState(Qt::Unchecked);
    if(jdl.contains("p") == true)
        ui->DownloadPlanToWatch->setCheckState(Qt::Checked);
    else
        ui->DownloadOnHold->setCheckState(Qt::Unchecked);
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
                downloadListas = linha.at(1);
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
