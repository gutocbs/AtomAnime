#include "torrent.h"
#include "ui_torrent.h"

torrent::torrent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::torrent)
{
    ui->setupUi(this);
    anitomy = new anitomy::Anitomy;
    listaWatching = false;
    prefQualidade = "1080p";
    prefSub = "HorribleSubs";
    diretorioDownloads = QDir::homePath() + "/Downloads/Animes/";
    ui->ListaTorrents->setColumnHidden(5, true);
    ui->ListaTorrents->setColumnWidth(0,2);
    ui->ListaTorrents->setColumnWidth(1, 50);
}

torrent::~torrent()
{
    delete ui;
}

void torrent::getJConfig(JanelaConfiguracao *JanelaConfig){
    jconfig = JanelaConfig;
}

void torrent::getLeitorArquivos(leitorarquivos *leitorArquiv){
    qleitor = leitorArquiv;
}

void torrent::getOrganizador(Organizador *organi){
    organiza = organi;
//    qDebug() << "organi: " << organi->retornaEpisodiosDisponiveis();
}

void torrent::getRss(){
    QDownloader *baixaXML = new QDownloader;
    baixaXML->setURL(jconfig->returnRSS());
    QObject::connect(baixaXML, SIGNAL(terminouDownload()), this, SLOT(leXML()));
}

void torrent::on_pushButton_clicked()
{
    emit volta();
}

void torrent::on_XML_clicked()
{
    getRss();
    getRss();
}

void torrent::Download(){
    for(int i = 0; i < nome.length(); i++){
        if(box[i].isChecked() == true){
            globalDownload = i;
            QDownloader *qdown = new QDownloader;
            qdown->setTorrent(link[i+1], nomeTorrent[i]);
            QObject::connect(qdown, SIGNAL(terminouDownload()), this, SLOT(baixaTorrent()));
        }
    }
}

void torrent::baixaTorrent(){//Por um botão pra escolher qual torrent quer usar nas configurações
    QProcess process;
    process.execute(QDir::homePath() + "/AppData/Roaming/uTorrent/uTorrent.exe",
                    QStringList() << "/DIRECTORY" << diretorioDownloads + nome[globalDownload] <<
                    QDir::currentPath() + "/Configurações/Temp/Torrents/" + nomeTorrent[globalDownload] + ".torrent");
    process.kill();
    QProcess process2;
    process2.execute("\"C:/Program Files/qBittorrent/qbittorrent.exe\" --add-paused=false --skip-dialog=true --save-path=" + diretorioDownloads + nome[globalDownload],
                     QStringList() <<  QDir::currentPath() + "/Configurações/Temp/Torrents/" + nomeTorrent[globalDownload] + ".torrent");
    process2.kill();
    box[globalDownload].setCheckState(Qt::Unchecked);
}

void torrent::preencheTabela(){
//    qDebug() << nome.length() << " - " << tier.length();
    ui->ListaTorrents->setRowCount(nome.length());
    ui->ListaTorrents->setSortingEnabled(true);
    ui->ListaTorrents->sortByColumn(5, Qt::AscendingOrder);
    box = new QCheckBox[nome.length()];
    for(int i = 0; i < nome.length(); i++){
//        box.append(QCheckBox());
        for(int w = 0; w < 6; w++){
            ui->ListaTorrents->setColumnWidth(1, 400);
            QTableWidgetItem *item = new QTableWidgetItem;
            if(w == 0){
                if(tier[i] == "1"){
                    box[i].setCheckState(Qt::Checked);
                }
                ui->ListaTorrents->setCellWidget(i,w, &box[i]);
            }
            if(w == 1){
                item->setText(nome[i]);
                ui->ListaTorrents->setItem(i,w, item);
//                ui->ListaTorrents->itemAt(i,w)->setForeground(QColor::fromRgb(220,220,220));
            }
            else if(w == 2){
                item->setText(episodio[i]);
                ui->ListaTorrents->setItem(i,w, item);
            }
            else if(w == 3){
                if(resolucao[i] == "360p")
                    resolucao[i] = "0360p";
                else if(resolucao[i] == "480p")
                    resolucao[i] = "0480p";
                else if(resolucao[i] == "720p")
                    resolucao[i] = "0720p";
                item->setText(resolucao[i]);
                ui->ListaTorrents->setItem(i,w, item);
            }
            else if(w == 4){
                item->setText(fansub[i]);
                ui->ListaTorrents->setItem(i,w, item);
            }
            else if(w == 5){
//                qDebug() << tier[i] << nome[i];
                item->setText(tier[i]);
                ui->ListaTorrents->setItem(i,w,item);
            }
        }
    }
    ui->ListaTorrents->update();
}

void torrent::leXML(){
    ui->label->setText("Carregando torrents");
    QString arquivoLer = "Configurações/rss.xml";
    QFile lerXML(arquivoLer);
    QString tempTier;
    QString tempQualidade;
    QString tempEpisodioTorrent;
    QString tempId;
    QString tempNome, tempNomeAlternativo;
    QString tempSub;
    QVector<int> tempEpiD;
    bool tempDown = false;
    int tempEpisodioListaWatching = 0;
    //Checa se o arquivo pode ser aberto
    if (lerXML.open(QIODevice::ReadOnly))
    {
        //Cria variáveis pra ler os arquivos do taiga
        QTextStream xml(&lerXML);
        //Checa se é possível escrever no arquivo de configuração
        //Lê o arquivo usr até o fim
        while (!xml.atEnd())
        {
            //le cada linha do arquivo
            QString linha = xml.readLine();
            //Checa se a linha tem o dado que procuro //ID no caso
            if(linha.contains("</title><link>") == false){
                if(linha.contains("<title>["))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<title>");
                    linha.remove("</title>");
                    nomeTorrent.append(linha);
                    anitomy::Anitomy anitomy;
                    anitomy.Parse(linha.toStdWString());
                    const auto& elements = anitomy.elements();
                    nome.append(QString::fromStdWString(elements.get(anitomy::kElementAnimeTitle)));
                    episodio.append(QString::fromStdWString(elements.get(anitomy::kElementEpisodeNumber)));
                    tempEpisodioTorrent = QString::fromStdWString(elements.get(anitomy::kElementEpisodeNumber));
                    fansub.append(QString::fromStdWString(elements.get(anitomy::kElementReleaseGroup)));
                    tempSub = QString::fromStdWString(elements.get(anitomy::kElementReleaseGroup));
                    resolucao.append(QString::fromStdWString(elements.get(anitomy::kElementVideoResolution)));
                    tempQualidade = QString::fromStdWString(elements.get(anitomy::kElementVideoResolution));
                    marcadoDownload.append(false);
                    for(int i = 0; i < qleitor->retornaTamanhoLista(); i++){
                        if(linha.contains(qleitor->retornaNome(i))){
                            tempTier = "1";
                            tempId = qleitor->retornaId(i);
                            tempNome = qleitor->retornaNome(i);
                            tempNomeAlternativo = qleitor->retornaNomeIngles(i);
                            organiza->retornaNumEpiNaPasta(0, tempId.toInt(), qleitor->retornaNumEpi(i).toInt());
                            tempEpisodioListaWatching = qleitor->retornaProgresso(i);
                            break;
                        }
                    }
                    if(tempTier == "1"){
                        tempEpiD = organiza->retornaEpisodiosDisponiveis();
                        if(tempEpiD.isEmpty() == false && tempEpisodioTorrent.toInt() == tempEpiD.last()+1){
                            tempDown = true;
                        }
                    }
                    if(tempTier == "1" && tempQualidade == prefQualidade && tempEpisodioTorrent.toInt() > tempEpisodioListaWatching
                        && tempDown == true && tempSub == prefSub)
                        tier.append("1");
                    else
                        tier.append("5");
                }
                else if(linha.contains("<link>"))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<link><![CDATA[");
                    linha.remove("]]></link>");
                    link.append(linha);
                }
            }
            tempDown = false;
            tempTier = "2";
        }
        lerXML.close();
    }
    ui->label->setText("Pronto");
    preencheTabela();
}

void torrent::on_Download_clicked()
{
    Download();
}
