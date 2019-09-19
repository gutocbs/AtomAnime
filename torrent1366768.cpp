#include "torrent1366768.h"
#include "ui_torrent1366768.h"

torrent1366768::torrent1366768(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::torrent1366768)
{
    ui->setupUi(this);
    anitomy = new anitomy::Anitomy;
    listaWatching = false;
    ui->ListaTorrents->setColumnHidden(7, true);
    ui->ListaTorrents->setColumnWidth(1, 300);
    ui->ListaTorrents->setColumnWidth(6, 500);
    downloadAutomatico = false;
}

torrent1366768::~torrent1366768()
{
    qDeleteAll(box.begin(),box.end());
    qDeleteAll(pWidget.begin(), pWidget.end());
    qDeleteAll(pLayout.begin(), pLayout.end());

    delete anitomy;
    delete jconfig;
    delete qleitor;
    delete organiza;
    delete baixaXML;
    delete ui;
}

//Recebe dados da janela de configuração
void torrent1366768::getJConfig(JanelaConfiguracao *JanelaConfig){
    jconfig = JanelaConfig;
    prefTorrent = jconfig->returnTorrentPadrao();
    diretorioDownloads = jconfig->returnDownloadFolder();
    prefSub = jconfig->returnFansub();
    prefQualidade = jconfig->returnQualidade();
    //O tempo de download automático é definido em minutos, porém o timer é definido em milisegundos.
    //Aqui é feita a conversão do tempo
    tempoDownloadAutomatico = jconfig->returnTempoDownload() * 60000;
    //Checa se o download automático de torrents está ligado ou não
    if(jconfig->returnDownloadAutomatico() == 1){
        downloadAutomatico = true;
        //Caso esteja, automatiza o sistema pra download após o tempo certo
        QTimer::singleShot(tempoDownloadAutomatico, this, SLOT(on_XML_clicked()));
    }
}

void torrent1366768::getLeitorArquivos(leitorarquivos *leitorArquiv){
    qleitor = leitorArquiv;
}

void torrent1366768::getOrganizador(Organizador *organi){
    organiza = organi;
}

void torrent1366768::mudaTorrentPadrao(QString torrentPadrao){
    prefTorrent = torrentPadrao;
}


//Baixa a lista de torrents disponíveis
void torrent1366768::getRss(){
    QStringList feed = jconfig->returnFeedBusca().split("-");
    baixaXML = new QDownloader;
//    baixaXML->setURL(jconfig->returnRSS());

    baixaXML->setURL(feed.at(0));
    //Espera alguns segundo para o arquivo terminar de ser salvo e fechado pelo objeto de download
    QObject::connect(baixaXML, SIGNAL(terminouDownload()), this, SLOT(leXML()));
}


void torrent1366768::on_pushButton_clicked()
{
    emit volta();
}

void torrent1366768::on_XML_clicked()
{
    nomeTorrent.clear();
    nome.clear();
    fansub.clear();
    episodio.clear();
    resolucao.clear();
    link.clear();
    lista.clear();
    tier.clear();
    dublado.clear();
    comentario.clear();

    ui->label->setText("Carregando lista.");
    getRss();
    getRss();
}

void torrent1366768::Download(){
    for(int i = 0; i < nome.length(); i++){
        if(box[i]->isChecked() == true){
            globalDownload.append(i);
//            qDebug() << nomeTorrent[i];
            QDownloader *qdown = new QDownloader;
            qdown->setTorrent(link[i], nomeTorrent[i]);
            QObject::connect(qdown, SIGNAL(terminouDownload()), this, SLOT(baixaTorrent()));
        }
    }
}


//Abre o programa de torrent antes de abrir o arquivo
void torrent1366768::baixaTorrent(){
    ui->label->setText("Carregando os torrents.");
    if(prefTorrent == "uTorrent")
        QDesktopServices::openUrl(QUrl("file:///"+ QDir::homePath() + "/AppData/Roaming/uTorrent/uTorrent.exe",QUrl::TolerantMode));
    else if(prefTorrent == "qBittorrent"){
        QDesktopServices::openUrl(QUrl("file:///C:/Program Files/qBittorrent/qbittorrent.exe",QUrl::TolerantMode));
    }
    //Espera o programa abrir pra colocar o torrent lá
    QTimer::singleShot(5000, this, SLOT(esperaTerminarSalvar()));
}

//O programa abre muito rápido. O programa começa o download muito rápido em comandos separados.
//O programa demora demais pra abrir e começar o download em um único comando
void torrent1366768::esperaTerminarSalvar(){
    QProcess process;
    //Assim que baixar o arquivo e abrir o programa, começa a baixar o torrent
    if(prefTorrent == "uTorrent"){
        process.execute(QDir::homePath() + "/AppData/Roaming/uTorrent/uTorrent.exe",
                        QStringList() << "/DIRECTORY" << diretorioDownloads + "/" + nome[globalDownload[0]] <<
                        QDir::currentPath() + "/Configurações/Temp/Torrents/" + nomeTorrent[globalDownload[0]] + ".torrent");
    }
    else if(prefTorrent == "qBittorrent"){
        process.execute("\"C:/Program Files/qBittorrent/qbittorrent.exe\" --add-paused=false --skip-dialog=true --save-path=" + diretorioDownloads + "/" + nome[globalDownload[0]],
                         QStringList() <<  QDir::currentPath() + "/Configurações/Temp/Torrents/" + nomeTorrent[globalDownload[0]] + ".torrent");
    }
    box[globalDownload[0]]->setCheckState(Qt::Unchecked);
    process.kill();
    globalDownload.remove(0);
    if(downloadAutomatico == true)
        QTimer::singleShot(600000, this, SLOT(on_XML_clicked()));
}
void torrent1366768::preencheTabela(){
    box.clear();
    pWidget.clear();
    pLayout.clear();

    ui->ListaTorrents->setRowCount(0);
    ui->ListaTorrents->setRowCount(nome.length());
    ui->ListaTorrents->setSortingEnabled(true);
    ui->ListaTorrents->sortByColumn(7, Qt::AscendingOrder);

    for(int i = 0; i < nome.length(); i++){
        box.append(new QCheckBox);
        pWidget.append(new QWidget());
        pLayout.append(new QHBoxLayout(pWidget[i]));
        pLayout[i]->addWidget(box[i]);
        pLayout[i]->setAlignment(Qt::AlignCenter);
        pLayout[i]->setContentsMargins(0,0,0,0);
        pWidget[i]->setLayout(pLayout[i]);
        for(int w = 0; w < 8; w++){
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
//            item->setBackground(QColor("transparent"));
//            item->setForeground(QColor::fromRgb(220,220,220));
            if(w == 0){
                if(tier[i] == "1"){
                    box[i]->setCheckState(Qt::Checked);
                }
                else{
                    box[i]->setCheckState(Qt::Unchecked);
                }
                ui->ListaTorrents->setCellWidget(i,0, pWidget[i]);
            }
            if(w == 1){
                item->setText(nome[i]);
                ui->ListaTorrents->setItem(i,w, item);
//                ui->ListaTorrents->itemAt(i,w)->setForeground(QColor::fromRgb(220,220,220));//isso é prata, da cor da fonte dos labels
            }
            else if(w == 2){
//                item->setForeground(QColor::fromRgb(220,220,220));
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
//                item->setForeground(QColor::fromRgb(220,220,220));
                item->setText(fansub[i]);
                ui->ListaTorrents->setItem(i,w, item);
            }
            else if(w == 5){
                item->setText(fansub[i]);
                item->setText(dublado[i]);
                ui->ListaTorrents->setItem(i,w, item);
            }
            else if(w == 6){
                item->setText(comentario[i]);
//                qDebug() << comentario[i];
                ui->ListaTorrents->setItem(i,w,item);
            }
            else if(w == 7){
//                item->setForeground(QColor::fromRgb(220,220,220));
                item->setText(tier[i]);
                ui->ListaTorrents->setItem(i,w,item);
            }
        }
    }

    ui->ListaTorrents->update();
    if(downloadAutomatico == true)
        Download();
}

void torrent1366768::leXML(){
    ui->label->setText("Carregando torrents");
    QString arquivoLer = "Configurações/rss.xml";
    QFile lerXML(arquivoLer);
    QString tempTorrentNome;
    QString tempTier;
    QString tempQualidade;
    QString tempEpisodioTorrent;
    QString tempId;
    QString tempNome, tempNomeAlternativo;
    QString tempSub;
    QVector<int> tempEpiD;
    bool tempDown = false;
        int tempEpisodioListaWatching = 0;
    if(lerXML.size() != 0){
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
                        tempTorrentNome = QString::fromStdWString(elements.get(anitomy::kElementAnimeTitle));
                        episodio.append(QString::fromStdWString(elements.get(anitomy::kElementEpisodeNumber)));
                        tempEpisodioTorrent = QString::fromStdWString(elements.get(anitomy::kElementEpisodeNumber));
                        fansub.append(QString::fromStdWString(elements.get(anitomy::kElementReleaseGroup)));
                        tempSub = QString::fromStdWString(elements.get(anitomy::kElementReleaseGroup));
                        resolucao.append(QString::fromStdWString(elements.get(anitomy::kElementVideoResolution)));
                        tempQualidade = QString::fromStdWString(elements.get(anitomy::kElementVideoResolution));
                        qleitor->leLinha("watching");

                        tempTorrentNome.remove(".");
                        tempTorrentNome.remove("?");
                        tempTorrentNome.remove("S1");
                        tempTorrentNome.remove("S2");
                        tempTorrentNome.remove("s1");
                        tempTorrentNome.remove("s2");
                        tempTorrentNome.remove("s01");
                        tempTorrentNome.remove("s02");
                        tempTorrentNome= tempTorrentNome.simplified();

                        for(int i = 0; i < qleitor->retornaTamanhoLista(); i++){
                            if(qleitor->retornaNome(i).contains(tempTorrentNome) || qleitor->retornaNomeIngles(i).contains(tempTorrentNome)){
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
//                            qDebug() << tempEpiD;
                            if(tempEpiD.isEmpty() == false && tempEpisodioTorrent.toInt() == tempEpiD.last()+1){
                                tempDown = true;
                            }
                        }
                        if(tempTier == "1" && tempQualidade == prefQualidade && tempEpisodioTorrent.toInt() > tempEpisodioListaWatching
                            && tempDown == true && tempSub == prefSub)
                            tier.append("1");
                        else
                            tier.append("5");
                        if(linha.contains("Dub") || linha.contains("dub") || linha.contains("DUB")){
                            dublado.append("Sim");
                        }
                        else{
                            dublado.append("Nao");
                        }
                    }
                    else if(linha.contains("<link>"))
                    {
                        //Remove dados desnecessários da linha e salva no vetor
                        linha.remove("<link><![CDATA[");
                        linha.remove("]]></link>");
                        link.append(linha);
                    }
                    else if(linha.contains("Comment")){
                        if(linha.contains("|]]></description>"))
                            linha.remove("|]]></description>");
                        else
                            linha.remove("]]></description>");
//                        qDebug() << linha;
                        comentario.append(linha);
                    }
                    else if(linha.contains("</description>") == true && linha.contains("Comment") == false){
                            comentario.append("");
                    }
                }
                tempDown = false;
                tempTier = "2";
            }
            lerXML.close();
        }
    }
    else
        leXML();
    ui->label->setText("Torrents encontrados");
    tempEpiD.clear();
    preencheTabela();
}

void torrent1366768::getRSSEspecifico(QString nomeAnime){
    ui->label->setText("Buscando torrents.");
    nomeTorrent.clear();
    nome.clear();
    fansub.clear();
    episodio.clear();
    resolucao.clear();
    link.clear();
    lista.clear();
    tier.clear();
    dublado.clear();
    comentario.clear();

    QStringList feed = jconfig->returnFeedBusca().split("-");
    QString xml = feed.at(1);
    xml.replace("%title%", "");
    xml.append(nomeAnime);
    baixaXML = new QDownloader;
    baixaXML->setURL(xml);
    baixaXML->setURL(xml);
//    //Espera alguns segundo para o arquivo terminar de ser salvo e fechado pelo objeto de download
    QObject::connect(baixaXML, SIGNAL(terminouDownload()), this, SLOT(leXMLEspecifico()));
}

void torrent1366768::leXMLEspecifico(){
    ui->label->setText("Carregando torrents");
    QString arquivoLer = "Configurações/rss.xml";
    QFile lerXML(arquivoLer);
    if(lerXML.size() != 0){
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
                if(linha.contains("<title>Nyaa") == false){
                    if(linha.contains("<title>["))
                    {
                        if(linha.contains("Dub") || linha.contains("dub") || linha.contains("DUB"))
                            dublado.append("Sim");
                        else
                            dublado.append("Não");
                        comentario.append("");
                        //Remove dados desnecessários da linha e salva no vetor
                        linha.remove("<title>");
                        linha.remove("</title>");
                        linha = linha.simplified();
                        nomeTorrent.append(linha);
                        anitomy::Anitomy anitomy;
                        anitomy.Parse(linha.toStdWString());
                        const auto& elements = anitomy.elements();
                        nome.append(QString::fromStdWString(elements.get(anitomy::kElementAnimeTitle)));
                        episodio.append(QString::fromStdWString(elements.get(anitomy::kElementEpisodeNumber)));
                        fansub.append(QString::fromStdWString(elements.get(anitomy::kElementReleaseGroup)));
                        resolucao.append(QString::fromStdWString(elements.get(anitomy::kElementVideoResolution)));
                        tier.append("5");

                    }
                }
            }
            lerXML.close();
        }
    }
    else
        leXMLEspecifico();
    ui->label->setText("Torrents encontrados");
    preencheTabela();
}


void torrent1366768::on_Download_clicked()
{
    Download();
}
