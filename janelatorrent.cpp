#include "janelatorrent.h"
#include "ui_janelatorrent.h"

janelatorrent::janelatorrent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::janelatorrent)
{
    ui->setupUi(this);
    ui->listaTorrents->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->listaTorrents->setColumnHidden(7,true);
    ui->listaTorrents->setColumnHidden(0,true);
    ui->listaTorrents->setColumnHidden(8,true);
    ui->listaTorrents->setColumnHidden(9,true);
    ui->listaTorrents->setColumnWidth(7, 670);
    ui->listaTorrents->setColumnWidth(0, 50);
    ui->listaTorrents->setColumnWidth(1, 50);
    ui->listaTorrents->setColumnWidth(2, 470);
    ui->listaTorrents->setColumnWidth(6, 400);
//    int hhh = 1920 - ui->listaTorrents->columnWidth(1) - ui->listaTorrents->columnWidth(2) - ui->listaTorrents->columnWidth(3)
//            - ui->listaTorrents->columnWidth(4) - ui->listaTorrents->columnWidth(5) - ui->listaTorrents->columnWidth(6);
//    qDebug() << hhh;
    ui->listaTorrents->setSortingEnabled(true);
    connect(this, &janelatorrent::fimDownloadTorrent, this, &janelatorrent::fpreencheTabela, Qt::QueuedConnection);
}

janelatorrent::~janelatorrent()
{
    qDeleteAll(torrent);
    delete ui;
}


//SEMPRE vou pegar animes não baixados, desde que estejam na lista e/ou cumpram os filtros.
//SE já tiver visto/baixado, ou tenha um filtro NOT, não entra na lista.
void janelatorrent::fleXML()
{
//    QMutexLocker llocker(&vmutex);
    QFile file(QDir::currentPath() + "/Configurações/Temp/torrents.xml");
    qDebug() << "Tentando pesquisar torrents disponíveis";
    if(!file.open(QIODevice::ReadOnly)) {
        qCritical() << "Não foi possível encontrar os torrents!";
        qCritical() << file.errorString();
        return;
    }

    QByteArray ldata = file.readAll();
    file.close();

    QString lnomeTorrent;
    QString lnomeAnime;
    QString lfansub;
    QString lresolucao;
    QString lepisodioAnime;
    QString llinkTorrent;
    QString ldescricaoTorrent;
    QString llinkInfoTorrent;
    QString lfiltrocondicao;
    QString lfiltroespecifico;
    int lprioridade = 0;

    QString lid;
    QString lepisodiosAssistidos;
    QString lista;
    int lposicaoAnimeNaLista;
    QStringList filtros;

    QXmlStreamReader stream(ldata);
    while (!stream.atEnd()) {
        QXmlStreamReader::TokenType token = stream.readNext();
        switch (token) {
        case QXmlStreamReader::Comment:
            break;
        case QXmlStreamReader::DTD:
            break;
        case QXmlStreamReader::Characters:
            break;
        case QXmlStreamReader::ProcessingInstruction:
            break;
        case QXmlStreamReader::EntityReference:
            break;
        case QXmlStreamReader::NoToken:
            break;
        case QXmlStreamReader::Invalid:
            break;
        case QXmlStreamReader::StartDocument:
            break;
        case QXmlStreamReader::EndDocument:
            emit fimDownloadTorrent();
            break;
        case QXmlStreamReader::StartElement:
            if(stream.name() == "title") {
                QString name = stream.readElementText();
                if(name.contains("Tokyo Toshokan"))
                    break;
                anitomy.Parse(name.toStdWString());
                const auto& elements = anitomy.elements();
                lnomeTorrent = name;

                lepisodioAnime = QString::fromStdWString(elements.get(anitomy::kElementEpisodeNumber));
                lnomeAnime = QString::fromStdWString(elements.get(anitomy::kElementAnimeTitle));
                lfansub = QString::fromStdWString(elements.get(anitomy::kElementReleaseGroup));
                lresolucao = QString::fromStdWString(elements.get(anitomy::kElementVideoResolution));


                //Talvez role achar os ids se for em thread?
                if(vHashDeNomeEId.contains(lnomeAnime))
                    lid = vHashDeNomeEId[lnomeAnime];
                else
//                    lid = cleitor->fprocuraAnimeNasListas(lnomeAnime);
                    lid = cleitor->fprocuraIdNasListasRapido(lnomeAnime);
                if(!vHashDeNomeEId.contains(lnomeAnime))
                    vHashDeNomeEId.insert(lnomeAnime,lid);

                lepisodiosAssistidos = cleitor->fbuscaAnimePorIDERetornaEpisodio(lid);
                lista = cleitor->fbuscaAnimePorIDERetornaLista(lid);
                lposicaoAnimeNaLista = cleitor->fbuscaAnimePorIDERetornaPosicao(lid);


                lprioridade += fcalculaPrioridadeNome(lid,lepisodioAnime,lepisodiosAssistidos,lista,lposicaoAnimeNaLista);
                lprioridade += fcalculaPrioridadeSub(lfansub, lid);
                lprioridade += fcalculaPrioridadeQualidade(lresolucao, lid);


            }
            if(stream.name() == "link") {
                QString link = stream.readElementText();
                if(link.contains("https://www.tokyotosho.info"))
                    break;
                link.remove("<![CDATA[");
                link.remove("]]>");
                llinkTorrent = link;
            }
            if(stream.name() == "description"){
                QString description = stream.readElementText();
                if(description.contains("Torrent Listing"))
                    break;
                QStringList comment = description.split("/>");
                if(comment.size() > 2){
                    llinkInfoTorrent = comment.at(2);
                    llinkInfoTorrent.remove(0,1);
                    llinkInfoTorrent.remove("<a href=\"");
                    llinkInfoTorrent.remove("\">Tokyo Tosho</a><br ");
                    description = comment.at(comment.size()-1);
                    description.remove("]]>");
                    description.remove(0,10);
                    ldescricaoTorrent = description;
                }
                else{
                    comment = description.split(" | ");
                    llinkInfoTorrent = comment.at(0);
                    llinkInfoTorrent = llinkInfoTorrent.split("\"").at(1);
                    comment.removeAt(0);
                    description = comment.join(" | ");
                    description.remove("]]>");
                    description.remove("</a>");
                    ldescricaoTorrent = description;

                    lprioridade += fcalculaPrioridadeFiltros(ldescricaoTorrent, lid);
                }

                QPointer<torrentinfo> ltorrentAux(new torrentinfo);
                ltorrentAux->vnomeTorrent = lnomeTorrent;
                ltorrentAux->vdescricaoTorrent = ldescricaoTorrent;
                ltorrentAux->vlinkTorrent = llinkTorrent;
                ltorrentAux->vnomeAnime = lnomeAnime;
                ltorrentAux->vfansub = lfansub;
                ltorrentAux->vresolucao = lresolucao;
                ltorrentAux->vepisodioAnime = lepisodioAnime;
                ltorrentAux->vtorrentInfoLink = llinkInfoTorrent;
                ltorrentAux->vlista = lista;

                //Checamos se já existe um anime na lista de downloads com esse nome e episódio
                if(vbaixar.contains(lnomeAnime+lepisodioAnime)){
                    //Caso o torrent existente tenha menos prioridade do que o torrent atual, ele é substituido
                    //A prioridade mínima é 14. Com 14, posso ter um conjunto de dois:
                    //sub, resolução e (lingua+keyword)
                    //E sempre tem que não ter sido baixado
                    if(vbaixar.value(lnomeAnime+lepisodioAnime).at(0).toInt() < 16 && lprioridade == 16){
                        QStringList value = (QString::number(lprioridade)+":"+QString::number(torrent.size())).split(":");
                        vbaixar.insert(lnomeAnime+lepisodioAnime, value);
                    }
                    else if(vbaixar.value(lnomeAnime+lepisodioAnime).at(0).toInt() < 15 && lprioridade == 15){
                        QStringList value = (QString::number(lprioridade)+":"+QString::number(torrent.size())).split(":");
                        vbaixar.insert(lnomeAnime+lepisodioAnime, value);
                    }
                    else if(vbaixar.value(lnomeAnime+lepisodioAnime).at(0).toInt() < 14 && lprioridade == 14){
                        QStringList value = (QString::number(lprioridade)+":"+QString::number(torrent.size())).split(":");
                        vbaixar.insert(lnomeAnime+lepisodioAnime, value);
                    }
                }
                //Caso não exista nenhum torrent na lista de downloads com o anime certo, é colocado um, mesmo que
                //Não esteja nas condições ideiais.
                //Filtros com NOT não deixam chegar aqui.
                else if(lprioridade >= 12){
                    QStringList value = (QString::number(lprioridade)+":"+QString::number(torrent.size())).split(":");
                    vbaixar.insert(lnomeAnime+lepisodioAnime, value);
                }
                if(!ltorrentAux->vnomeTorrent.contains("Torrent File RSS")){
                    torrent.append(ltorrentAux);
                }
                lprioridade = 0;
            }
            break;
        case QXmlStreamReader::EndElement:
            break;
        }
    }
    for(int i = 0; i < torrent.size(); i++){
        foreach (QString key, vbaixar.keys()) {
            if(vbaixar[key].at(1).toInt() == i){
                torrent[i]->vbaixar = true;
            }
        }
    }
}

void janelatorrent::fpreencheTabela()
{
    vcontroladorCheckbox.clear();
    //Criamos o número de linhas necessário
    ui->listaTorrents->setRowCount(torrent.size());
    for(int i = 0; i < torrent.size(); i++){
        torrent[i]->vlista.compare("Watching");
//        ui->listaTorrents->verticalHeader()->setStyleSheet("color: red");
//        ui->listaTorrents->verticalHeaderItem(i)->setTextColor("lightpink");
        ui->listaTorrents->verticalHeader()->hide();
        for(int w = 0; w < ui->listaTorrents->columnCount(); w++){
            QTableWidgetItem *litem = new QTableWidgetItem;
            switch (w) {
            case 0:
                if(torrent[i]->vlista.compare("Watching") == 0){
                    litem->setForeground(QBrush(QColor("forestgreen")));
                    litem->setText("W");
                }
                else if(torrent[i]->vlista.compare("On Hold") == 0){
                    litem->setForeground(QBrush(QColor("darkorange")));
                    litem->setText("OH");
                }
                else if(torrent[i]->vlista.compare("Dropped") == 0){
                    litem->setForeground(QBrush(QColor("navy")));
                    litem->setText("D");
                }
                else if(torrent[i]->vlista.compare("Plan to Watch") == 0){
                    litem->setForeground(QBrush(QColor("darkorchid")));
                    litem->setText("PTW");
                }
                else if(torrent[i]->vlista.compare("Completed") == 0){
                    litem->setForeground(QBrush(QColor("indianred")));
                    litem->setText("C");
                }
                litem->setTextAlignment(Qt::AlignCenter);
                litem->setFont(QFont("Times", 10, QFont::Bold));
                ui->listaTorrents->setItem(i,w, litem);
            break;
            case 1: {
                foreach (QString key, vbaixar.keys()) {
                    if(vbaixar[key].at(1).toInt() == i){
                        torrent[i]->vbaixar = true;
                        vbaixar.remove(key);
                    }
                }
                QModelIndex index = ui->listaTorrents->model()->index(i, w);
                QWidget *centerdCheckBoxWidget = new QWidget();
                QCheckBox *checkBox = new QCheckBox();
                vcontroladorCheckbox.insert(i,checkBox);
                QHBoxLayout *checkBoxLayout = new QHBoxLayout(centerdCheckBoxWidget);
                if(torrent[i]->vbaixar)
                    checkBox->setCheckState(Qt::Checked);
                checkBoxLayout->addWidget(checkBox);
                checkBoxLayout->setAlignment(Qt::AlignCenter);
                checkBoxLayout->setContentsMargins(0, 0, 0, 0);
                centerdCheckBoxWidget->setLayout(checkBoxLayout);
                connect(checkBox, &QCheckBox::stateChanged, this, &janelatorrent::fchecaEstado);
                ui->listaTorrents->setIndexWidget(index, centerdCheckBoxWidget);
                break;
            }
            case 2:
                litem->setText(torrent[i]->vnomeAnime);
                if(torrent[i]->vbaixar)
                    litem->setTextColor("royalblue");
                ui->listaTorrents->setItem(i,w,litem);
            break;
            case 3:
                litem->setText(torrent[i]->vfansub);
                if(torrent[i]->vbaixar)
                    litem->setTextColor("royalblue");
                ui->listaTorrents->setItem(i,w, litem);
            break;
            case 4:
                litem->setText(torrent[i]->vresolucao);
                if(torrent[i]->vbaixar)
                    litem->setTextColor("royalblue");
                ui->listaTorrents->setItem(i,w, litem);
            break;
            case 5:
                litem->setText(torrent[i]->vepisodioAnime);
                if(torrent[i]->vbaixar)
                    litem->setTextColor("royalblue");
                ui->listaTorrents->setItem(i,w, litem);
            break;
            case 6:
                litem->setText(torrent[i]->vnomeTorrent);
                if(torrent[i]->vbaixar)
                    litem->setTextColor("royalblue");
                ui->listaTorrents->setItem(i,w, litem);
            break;
            case 7:
                litem->setText(torrent[i]->vdescricaoTorrent);
                if(torrent[i]->vbaixar)
                    litem->setTextColor("royalblue");
                ui->listaTorrents->setItem(i,w, litem);
            break;
            case 8:
                litem->setText(torrent[i]->vtorrentInfoLink);
                if(torrent[i]->vbaixar)
                    litem->setTextColor("royalblue");
                ui->listaTorrents->setItem(i,w, litem);
            break;
            case 9:{
                int prioridade = 0;
                if(torrent[i]->vlista.compare("Watching") == 0 && cconfig->fretornaDownloadListasAnimes().contains("w"))
                    prioridade+=8;
                else if(torrent[i]->vlista.compare("Plan to Watch") == 0 && cconfig->fretornaDownloadListasAnimes().contains("p"))
                    prioridade+=7;
                else if(torrent[i]->vlista.compare("On Hold") == 0 && cconfig->fretornaDownloadListasAnimes().contains("o"))
                    prioridade+=6;
                else if(torrent[i]->vlista.compare("Dropped") == 0 && cconfig->fretornaDownloadListasAnimes().contains("d"))
                    prioridade+=5;
                else if(torrent[i]->vlista.compare("Watching") == 0)
                    prioridade+=4;
                else if(torrent[i]->vlista.compare("Plan to Watch") == 0)
                    prioridade+=3;
                else if(torrent[i]->vlista.compare("On Hold") == 0)
                    prioridade+=2;
                else if(torrent[i]->vlista.compare("Dropped") == 0)
                    prioridade++;
                else if(torrent[i]->vlista.compare("Completed") == 0)
                    prioridade++;
                if(torrent[i]->vbaixar)
                    prioridade++;
                litem->setText(QString::number(prioridade));
                ui->listaTorrents->setItem(i,w, litem);
            }
            break;
            }
        }
    }
    ui->listaTorrents->sortByColumn(9);
    ui->botaoAtualizaLista->blockSignals(false);
}

void janelatorrent::fpassaPonteiros(leitorlistaanimes *lleitor,  janeladeconfig *lconf, arquivos *lcarq)
{
    cleitor = lleitor;
    cconfig = lconf;
    carquivos = lcarq;
}

void janelatorrent::on_botaoDownload_clicked()
{////CHECAR SE É WINDOWS OU LINUX NESSA PARTE
    bool achouPraDownload = false;

    for(int i = 0; i < torrent.size(); i++){
        if(torrent[i]->vbaixar){
            vlistaDownload.append(i);
            QPointer<filedownloader> qdown(new filedownloader);
            qdown->fdownloadTorrent(torrent[i]->vlinkTorrent, torrent[i]->vnomeTorrent);
            torrent[i]->vbaixar = false;
            achouPraDownload = true;
            connect(qdown, &filedownloader::storrent, this, &janelatorrent::fesperaTorrent);
        }
    }
    if(achouPraDownload){
        QProcess lprocesso;
        if(cconfig->fretornaTorrentEscolhido() == "uTorrent")
            lprocesso.setProgram(QDir::homePath() + "/AppData/Roaming/uTorrent/uTorrent.exe");
        else if(cconfig->fretornaTorrentEscolhido() == "qBittorrent")
            lprocesso.setProgram(QDir::rootPath() + "/Program Files/qBittorrent/qbittorrent.exe");

        lprocesso.startDetached();
    }
    fpreencheTabela();
}

void janelatorrent::fesperaTorrent(){
    QTimer lespera5Segundos;
    lespera5Segundos.singleShot(5000, this, &janelatorrent::fbaixaTorrent);
}

void janelatorrent::fbaixaTorrent()
{
    if(cconfig->fretornaPastaSalvarAnimes().isEmpty()){
        qWarning() << "Error: Trying to download without setting a download folder";
        emit error("No download folder");
        return;
    }
    QProcess lprocesso;
    QStringList argumentos;
    if(cconfig->fretornaTorrentEscolhido() == "uTorrent"){
        lprocesso.setProgram(QDir::homePath() + "/AppData/Roaming/uTorrent/uTorrent.exe");
        argumentos.append("/DIRECTORY");
        argumentos.append(cconfig->fretornaPastaSalvarAnimes() + "/" + torrent[vlistaDownload[0]]->vnomeAnime);
        argumentos.append(QDir::currentPath() + "/Configurações/Temp/Torrents/" + torrent[vlistaDownload[0]]->vnomeTorrent +
                ".torrent");
    }
    else if(cconfig->fretornaTorrentEscolhido() == "qBittorrent"){
        lprocesso.setProgram(QDir::rootPath() + "/Program Files/qBittorrent/qbittorrent.exe");
        argumentos.append("--add-paused=false");
        argumentos.append("--skip-dialog=true");
        argumentos.append("--save-path=" + cconfig->fretornaPastaSalvarAnimes() + "/" + torrent[vlistaDownload[0]]->vnomeAnime);
        argumentos.append(QDir::currentPath() + "/Configurações/Temp/Torrents/" + torrent[vlistaDownload[0]]->vnomeTorrent
                + ".torrent");
    }
    lprocesso.setArguments(argumentos);
    if(lprocesso.startDetached())
        qDebug() << "Download started successfully!";

    torrent[vlistaDownload[0]]->vbaixar = false;
    vlistaDownload.remove(0);
}

void janelatorrent::on_botaoAtualizaLista_clicked()
{
    ui->botaoAtualizaLista->blockSignals(true);
    if(!torrent.isEmpty())
        torrent.clear();
    QPointer<filedownloader> lbaixaXML(new filedownloader);
    lbaixaXML->fdownloadXMLTorrentList(cconfig->fretornaFeedAnime());
    connect(lbaixaXML, &filedownloader::sxml, this, &janelatorrent::fpreparaLeituraArquivoTorrent);
}

void janelatorrent::fprocuraAnimeEspecifico(QString rnomeAnimeBuscado){
    ui->botaoAtualizaLista->blockSignals(true);
    if(!torrent.isEmpty())
        torrent.clear();
    QPointer<filedownloader> lbaixaXML(new filedownloader);
    QString lfeedEspecifico = cconfig->fretornaFeedAnimeEspecifico();
    lfeedEspecifico.replace("%title%", rnomeAnimeBuscado);
    lbaixaXML->fdownloadXMLTorrentList(lfeedEspecifico);
    connect(lbaixaXML, &filedownloader::sxml, this, &janelatorrent::fleXML);
}

void janelatorrent::fautoDownload()
{
    void on_botaoAtualizaLista_clicked();
    void on_botaoDownload_clicked();
}

QString janelatorrent::fchecaFiltroFansub(QString lid)
{
    for(int i = 0; i < cconfig->vfiltrosAnimes.size(); i++){
        for(int w = 0; w < cconfig->vfiltrosAnimes[i]->idAnimesAfetados.size(); w++){
            if(cconfig->vfiltrosAnimes[i]->idAnimesAfetados[w].compare(lid) == 0){
                if(cconfig->vfiltrosAnimes[i]->filtroDeCondicao.contains("fansub", Qt::CaseInsensitive))
                    return cconfig->vfiltrosAnimes[i]->filtroDeCondicao+";"+cconfig->vfiltrosAnimes[i]->filtroEspecifico;
            }
        }
    }
    return nullptr;
}

QString janelatorrent::fchecaFiltroDownloadFromList(QString lid)
{
    for(int i = 0; i < cconfig->vfiltrosAnimes.size(); i++){
        for(int w = 0; w < cconfig->vfiltrosAnimes[i]->idAnimesAfetados.size(); w++){
            if(cconfig->vfiltrosAnimes[i]->idAnimesAfetados[w].compare(lid) == 0){
                if(cconfig->vfiltrosAnimes[i]->filtroDeCondicao.contains("list", Qt::CaseInsensitive))
                    return cconfig->vfiltrosAnimes[i]->filtroDeCondicao+";"+cconfig->vfiltrosAnimes[i]->filtroEspecifico;
            }
        }
    }
    return nullptr;
}

QString janelatorrent::fchecaFiltroResolution(QString lid)
{
    for(int i = 0; i < cconfig->vfiltrosAnimes.size(); i++){
        for(int w = 0; w < cconfig->vfiltrosAnimes[i]->idAnimesAfetados.size(); w++){
            if(cconfig->vfiltrosAnimes[i]->idAnimesAfetados[w].compare(lid) == 0){
                if(cconfig->vfiltrosAnimes[i]->filtroDeCondicao.contains("resolution", Qt::CaseInsensitive))
                    return cconfig->vfiltrosAnimes[i]->filtroDeCondicao+";"+cconfig->vfiltrosAnimes[i]->filtroEspecifico;
            }
        }
    }
    return nullptr;
}

QString janelatorrent::fchecaFiltroLaguage(QString lid)
{
    for(int i = 0; i < cconfig->vfiltrosAnimes.size(); i++){
        for(int w = 0; w < cconfig->vfiltrosAnimes[i]->idAnimesAfetados.size(); w++){
            if(cconfig->vfiltrosAnimes[i]->idAnimesAfetados[w].compare(lid) == 0){
                if(cconfig->vfiltrosAnimes[i]->filtroDeCondicao.contains("language", Qt::CaseInsensitive))
                    return cconfig->vfiltrosAnimes[i]->filtroDeCondicao+";"+cconfig->vfiltrosAnimes[i]->filtroEspecifico;
            }
        }
    }
    return nullptr;
}

QString janelatorrent::fchecaFiltroHasKeyword(QString lid)
{
    for(int i = 0; i < cconfig->vfiltrosAnimes.size(); i++){
        for(int w = 0; w < cconfig->vfiltrosAnimes[i]->idAnimesAfetados.size(); w++){
            if(cconfig->vfiltrosAnimes[i]->idAnimesAfetados[w].compare(lid) == 0){
                if(cconfig->vfiltrosAnimes[i]->filtroDeCondicao.contains("keyword", Qt::CaseInsensitive))
                    return cconfig->vfiltrosAnimes[i]->filtroDeCondicao+";"+cconfig->vfiltrosAnimes[i]->filtroEspecifico;
            }
        }
    }
    return nullptr;

}

void janelatorrent::on_botaoInfoAnime_clicked()
{
    if(!ui->listaTorrents->selectionModel()->selectedRows().isEmpty()){
        QModelIndexList select = ui->listaTorrents->selectionModel()->selectedRows();
        emit infoAnime(ui->listaTorrents->item(select.at(0).row(),2)->text());
    }
}

void janelatorrent::on_botaoLinkTorrent_clicked()
{
    if(!ui->listaTorrents->selectionModel()->selectedRows().isEmpty()){
        QModelIndexList select = ui->listaTorrents->selectionModel()->selectedRows();
        QString llink = ui->listaTorrents->item(select.at(0).row(),8)->text();
        QDesktopServices::openUrl(QUrl(llink,QUrl::TolerantMode));
    }
}

void janelatorrent::on_botaoSearchThisanime_clicked()
{
    if(!ui->listaTorrents->selectionModel()->selectedRows().isEmpty()){
        QModelIndexList select = ui->listaTorrents->selectionModel()->selectedRows();
        QString lnome = ui->listaTorrents->item(select.at(0).row(),2)->text();
        fprocuraAnimeEspecifico(lnome);
    }
}

void janelatorrent::on_botaoSelectSubForTorrent_clicked()
{
    if(!ui->listaTorrents->selectionModel()->selectedRows().isEmpty()){
        QModelIndexList select = ui->listaTorrents->selectionModel()->selectedRows();
        QString lnome = ui->listaTorrents->item(select.at(0).row(),1)->text();
        QString lsub = ui->listaTorrents->item(select.at(0).row(),3)->text();
        QString id = cleitor->fprocuraAnimeNasListas(lnome);
        cconfig->fselectSubFromTorrent(id,lsub);
    }
}

void janelatorrent::fpreparaLeituraArquivoTorrent()
{
    ui->labelMensagem->setText("Buscando torrents...");
    if(!vleArquivo.isRunning())
        vleArquivo = QtConcurrent::run(this, &janelatorrent::fleXML);
}

void janelatorrent::fchecaEstado(int estado)
{
    Q_UNUSED(estado)
    for(int i = 0; i < vcontroladorCheckbox.size(); i++){
        if(vcontroladorCheckbox[i]->isChecked()){
            if(torrent.size() > i){
                torrent[i]->vbaixar = true;
            }
        }
    }
}

int janelatorrent::fcalculaPrioridadeNome(QString ridAnime, QString repisodioAnime,
                                          QString repisodiosAssistidos, QString rlista, int rposicaoNaLista)
{
    QStringList filtros = fchecaFiltroDownloadFromList(ridAnime).split(";");
    filtros.removeAll(QString(""));
    //Caso o filtro seja de não baixar o anime de ID x, não baixamos.
    if(!filtros.isEmpty())
    {
        if(filtros.at(0).contains("not", Qt::CaseInsensitive))
            return -10;
        else if(rlista.compare(filtros.at(1), Qt::CaseInsensitive) == 0){
            if(rlista.compare("Watching", Qt::CaseInsensitive) == 0){
                vlistaAtual = cleitor->retornaListaWatching();
            }
            else if(rlista.compare("Plan to Watch", Qt::CaseInsensitive) == 0){
                vlistaAtual = cleitor->retornaListaPlanToWatch();
            }
            else if(rlista.compare("On Hold", Qt::CaseInsensitive) == 0){
                vlistaAtual = cleitor->retornaListaOnHold();
            }
            else if(rlista.compare("Completed", Qt::CaseInsensitive) == 0){
                vlistaAtual = cleitor->retornaListaCompleted();
            }
            else if(rlista.compare("Dropped", Qt::CaseInsensitive) == 0){
                vlistaAtual = cleitor->retornaListaDropped();
            }
            else
                vlistaAtual.clear();
            if(repisodioAnime.toInt() > repisodiosAssistidos.toInt()){
                //Checa se é o próximo episódio que deve ser assistido. Se estiver vazio, o episódio não foi baixado
                //Se não estiver, o episódio já existe no computador.
                if(rposicaoNaLista != -1 || vlistaAtual.isEmpty()){
                    QString lultimoEpisodioBaixado = carquivos->fprocuraEpisodioEspecifico
                            (vlistaAtual[rposicaoNaLista],repisodioAnime.toInt());
                    //Caso não exista o episódio na pasta, a string retorna vazia.
                    lultimoEpisodioBaixado = lultimoEpisodioBaixado.mid(lultimoEpisodioBaixado.lastIndexOf("/")+1);
                    if(lultimoEpisodioBaixado.isEmpty())
                        return 10;
                    else
                        return -10;
                }
            }
            else
                return -10;
        }
    }
    else
    {
        if(rlista.compare("Watching", Qt::CaseInsensitive) == 0 &&
                cconfig->fretornaDownloadListasAnimes().contains("w")){
            vlistaAtual = cleitor->retornaListaWatching();
        }
        else if(rlista.compare("Plan to Watch", Qt::CaseInsensitive) == 0 &&
                cconfig->fretornaDownloadListasAnimes().contains("p")){
            vlistaAtual = cleitor->retornaListaPlanToWatch();
        }
        else if(rlista.compare("On Hold", Qt::CaseInsensitive) == 0 &&
                cconfig->fretornaDownloadListasAnimes().contains("o")){
            vlistaAtual = cleitor->retornaListaOnHold();
        }
        else if(rlista.compare("Dropped", Qt::CaseInsensitive) == 0 &&
                cconfig->fretornaDownloadListasAnimes().contains("d")){
            vlistaAtual = cleitor->retornaListaDropped();
        }
        else
            vlistaAtual.clear();
        if(repisodioAnime.toInt() > repisodiosAssistidos.toInt()){
            //Checa se é o próximo episódio que deve ser assistido. Se estiver vazio, o episódio não foi baixado
            //Se não estiver, o episódio já existe no computador.
            if(rposicaoNaLista != -1){
                QString lultimoEpisodioBaixado = "NotEmpty";
                if(!vlistaAtual.isEmpty())
                    lultimoEpisodioBaixado = carquivos->fprocuraEpisodioEspecifico
                            (vlistaAtual[rposicaoNaLista],repisodioAnime.toInt());
                //Caso não exista o episódio na pasta, a string retorna vazia.
                lultimoEpisodioBaixado = lultimoEpisodioBaixado.mid(lultimoEpisodioBaixado.lastIndexOf("/")+1);
                if(lultimoEpisodioBaixado.isEmpty()){
                    //                                qDebug() << vlistaAtual[lposicaoAnimeNaLista]->vid << vlistaAtual[lposicaoAnimeNaLista]->vnome;
                    return 10;
                }
                //Se já tiver baixado, sai da lista
                else
                    return -10;
            }
        }
        //Caso já tenha sido assistido, sai da lista.
        else
            return -10;
    }
    return 0;
}

int janelatorrent::fcalculaPrioridadeSub(QString rfansub, QString rid)
{
    QStringList filtros = fchecaFiltroFansub(rid).split(";");
    filtros.removeAll(QString(""));
    if(!filtros.isEmpty()){
        if(filtros.at(0).contains("not", Qt::CaseInsensitive)){
            if(filtros.at(1).compare(rfansub, Qt::CaseInsensitive) == 0){
                return -10;
            }
        }
        else if(rfansub.compare(filtros.at(1), Qt::CaseInsensitive) == 0)
            return 2;
    }
    else{
        if(rfansub.compare(cconfig->fretornaSubEscolhido(), Qt::CaseInsensitive) == 0)
            return 2;
    }
    return 0;
}

int janelatorrent::fcalculaPrioridadeQualidade(QString resolucao, QString id)
{
    QStringList filtros = fchecaFiltroResolution(id).split(";");
    filtros.removeAll(QString(""));
    if(!filtros.isEmpty()){
        if(filtros.at(0).contains("not", Qt::CaseInsensitive)){
            if(filtros.at(1).contains(resolucao, Qt::CaseInsensitive)){
                return -10;
            }
        }
        else if(filtros.at(1).contains(resolucao, Qt::CaseInsensitive))
            return 2;
    }
    else{
        if(cconfig->fretornaQualidadeEscolhida().compare(resolucao.toUtf8(), Qt::CaseInsensitive) == 0)
            return 2;
    }
    return 0;
}

int janelatorrent::fcalculaPrioridadeFiltros(QString description, QString rid)
{
    QStringList filtros = fchecaFiltroLaguage(rid).split(";");
    filtros.removeAll(QString(""));
    if(!filtros.isEmpty()){
        if(filtros.at(0).contains("not", Qt::CaseInsensitive)){
            if(description.contains(filtros.at(1), Qt::CaseInsensitive)){
                return -10;
            }
        }
        else if(description.contains(filtros.at(1), Qt::CaseInsensitive))
            return 1;
    }
    filtros = fchecaFiltroHasKeyword(rid).split(";");
    filtros.removeAll(QString(""));
    if(!filtros.isEmpty()){
        if(filtros.at(0).contains("not", Qt::CaseInsensitive)){
            if(description.contains(filtros.at(1), Qt::CaseInsensitive)){
                return -10;
            }
        }
        else if(description.contains(filtros.at(1), Qt::CaseInsensitive))
            return 1;
    }
    return 0;
}
