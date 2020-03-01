#include "janelatorrent.h"
#include "ui_janelatorrent.h"

janelatorrent::janelatorrent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::janelatorrent)
{
    ui->setupUi(this);
    ui->listaTorrents->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->listaTorrents->setColumnHidden(7,true);
    ui->listaTorrents->setColumnHidden(8,true);
    ui->listaTorrents->setColumnWidth(6, 1034);
    ui->listaTorrents->setColumnWidth(0, 50);
    ui->listaTorrents->setColumnWidth(1, 400);
    ui->listaTorrents->setColumnWidth(5, 400);
    ui->listaTorrents->setSortingEnabled(true);
}

janelatorrent::~janelatorrent()
{
    qDeleteAll(torrent);
    delete ui;
}

void janelatorrent::fleXML()
{
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
    int lprioridade = 0;
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
            fpreencheTabela();
            break;
        case QXmlStreamReader::StartElement:
            if(stream.name() == "title") {
                QString name = stream.readElementText();
                if(name.contains("Tokyo Toshokan"))
                    break;
                anitomy.Parse(name.toStdWString());
                const auto& elements = anitomy.elements();
                lnomeTorrent = name;
                lnomeAnime = QString::fromStdWString(elements.get(anitomy::kElementAnimeTitle));
                lepisodioAnime = QString::fromStdWString(elements.get(anitomy::kElementEpisodeNumber));
                for(int w = 0; w < cconfig->fretornaDownloadListasAnimes().size(); w++) {
                    switch (cconfig->fretornaDownloadListasAnimes()[w]) {
                    case 'w':
                        if(cconfig->fretornaDownloadListasAnimes().contains('w')) {
                            vlistaAtual = cleitor->retornaListaWatching();
                        }
                        break;
                    case 'd':
                        if(cconfig->fretornaDownloadListasAnimes().contains('d')) {
                            vlistaAtual = cleitor->retornaListaDropped();
                        }
                        break;
                    case 'o':
                        if(cconfig->fretornaDownloadListasAnimes().contains('o')) {
                            vlistaAtual = cleitor->retornaListaOnHold();
                        }
                        break;
                    case 'p':
                        if(cconfig->fretornaDownloadListasAnimes().contains('p')) {
                            vlistaAtual = cleitor->retornaListaPlanToWatch();
                        }
                    }
                    for(int i = 0; i < vlistaAtual.size(); i++) {
                        if(lnomeAnime.contains(vlistaAtual[i]->vnome, Qt::CaseInsensitive)) {
                            lprioridade++;
                            if(lepisodioAnime.toInt() > vlistaAtual[i]->vnumEpisodiosAssistidos.toInt()) {
                                if(carquivos->fprocuraUltimoEpisodio(vlistaAtual[i],lepisodioAnime).isEmpty()) {
                                    lprioridade++;
                                }
                                else
                                    lprioridade = -4;
                            }
                            else
                                lprioridade--;
                            break;
                        }

                    }
                }
                lfansub = QString::fromStdWString(elements.get(anitomy::kElementReleaseGroup));
                if(lfansub.compare(cconfig->fretornaSubEscolhido(), Qt::CaseInsensitive) == 0) {
                    lprioridade++;
                }
                lresolucao = QString::fromStdWString(elements.get(anitomy::kElementVideoResolution));
                if(cconfig->fretornaQualidadeEscolhida().contains(lresolucao.toUtf8()))
                    lprioridade++;
            }
            if(stream.name() == "link") {
                QString link = stream.readElementText();
                if(link.contains("https://www.tokyotosho.info"))
                    break;
                link.remove("<![CDATA[");
                link.remove("]]>");
                llinkTorrent = link;
            }
            if(stream.name() == "description") {
                QString description = stream.readElementText();
                if(description.contains("Torrent Listing"))
                    break;
                QStringList comment = description.split("/>");
                if(comment.size() > 2) {
                    llinkInfoTorrent = comment.at(2);
                    llinkInfoTorrent.remove(0,1);
                    llinkInfoTorrent.remove("<a href=\"");
                    llinkInfoTorrent.remove("\">Tokyo Tosho</a><br ");
                    description = comment.at(comment.size()-1);
                    description.remove("]]>");
                    description.remove(0,10);
                    ldescricaoTorrent = description;
                }
                else {
                    comment = description.split(" | ");
                    llinkInfoTorrent = comment.at(0);
                    llinkInfoTorrent = llinkInfoTorrent.split("\"").at(1);
                    comment.removeAt(0);
                    description = comment.join(" | ");
                    description.remove("]]>");
                    description.remove("</a>");
                    ldescricaoTorrent = description;
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
                if(vbaixar.contains(lnomeAnime)) {
                    if(vbaixar.value(lnomeAnime).at(0).toInt() < 4 && lprioridade == 4) {
                        QStringList value = (QString::number(lprioridade)+":"+QString::number(torrent.size())).split(":");
                        vbaixar.insert(lnomeAnime, value);
                    }
                }
                else if(lprioridade >= 3) {
                    QStringList value = (QString::number(lprioridade)+":"+QString::number(torrent.size())).split(":");
                    vbaixar.insert(lnomeAnime, value);
                }
                torrent.append(ltorrentAux);
                lprioridade = 0;
            }
            break;
        case QXmlStreamReader::EndElement:
            break;
        }
    }
}

void janelatorrent::fpreencheTabela()
{
    //Criamos o número de linhas necessário
    ui->listaTorrents->setRowCount(torrent.size());
    for(int i = 0; i < torrent.size(); i++) {
//        ui->listaTorrents->setRowHeight(i, 100);
        for(int w = 0; w < ui->listaTorrents->columnCount(); w++) {
            QTableWidgetItem *litem = new QTableWidgetItem;
            switch (w) {
            case 0: {
                foreach (QString key, vbaixar.keys()) {
                    if(vbaixar[key].at(1).toInt() == i) {
                        torrent[i]->vbox.setCheckState(Qt::Checked);
                        vbaixar.remove(key);
                    }
                }
                QModelIndex index = ui->listaTorrents->model()->index(i, w);
                QWidget *centerdCheckBoxWidget = new QWidget();
                QCheckBox *checkBox = new QCheckBox();
                QHBoxLayout *checkBoxLayout = new QHBoxLayout(centerdCheckBoxWidget);
                if(torrent[i]->vbox.isChecked())
                    checkBox->setCheckState(Qt::Checked);
                checkBoxLayout->addWidget(checkBox);
                checkBoxLayout->setAlignment(Qt::AlignCenter);
                checkBoxLayout->setContentsMargins(0, 0, 0, 0);
                centerdCheckBoxWidget->setLayout(checkBoxLayout);
                ui->listaTorrents->setIndexWidget(index, centerdCheckBoxWidget);
                break;
            }
            case 1:
                litem->setText(torrent[i]->vnomeAnime);
                ui->listaTorrents->setItem(i,w,litem);
                break;
            case 2:
                litem->setText(torrent[i]->vfansub);
                ui->listaTorrents->setItem(i,w, litem);
                break;
            case 3:
                litem->setText(torrent[i]->vresolucao);
                ui->listaTorrents->setItem(i,w, litem);
                break;
            case 4:
                litem->setText(torrent[i]->vepisodioAnime);
                ui->listaTorrents->setItem(i,w, litem);
                break;
            case 5:
                litem->setText(torrent[i]->vnomeTorrent);
                ui->listaTorrents->setItem(i,w, litem);
                break;
            case 6:
                litem->setText(torrent[i]->vdescricaoTorrent);
                ui->listaTorrents->setItem(i,w, litem);
                break;
            case 7:
                litem->setText(torrent[i]->vtorrentInfoLink);
                ui->listaTorrents->setItem(i,w, litem);
                break;
            case 8:
                if(torrent[i]->vbox.isChecked())
                    litem->setText("2");
                else
                    litem->setText("1");
                ui->listaTorrents->setItem(i,w, litem);
                break;
            }
        }
    }
    ui->listaTorrents->sortByColumn(6);
    ui->botaoAtualizaLista->blockSignals(false);
}

void janelatorrent::fpassaPonteiros(leitorlistaanimes *lleitor,  janeladeconfig *lconf, arquivos *lcarq)
{
    cleitor = lleitor;
    cconfig = lconf;
    carquivos = lcarq;
}

void janelatorrent::on_botaoDownload_clicked()
{   ////CHECAR SE É WINDOWS OU LINUX NESSA PARTE
    if(cconfig->fretornaTorrentEscolhido() == "uTorrent")
        QDesktopServices::openUrl(QUrl("file:///"+ QDir::homePath() + "/AppData/Roaming/uTorrent/uTorrent.exe",QUrl::TolerantMode));
    else if(cconfig->fretornaTorrentEscolhido() == "qBittorrent") {
        QDesktopServices::openUrl(QUrl("file:///C:/Program Files/qBittorrent/qbittorrent.exe",QUrl::TolerantMode));
    }
    for(int i = 0; i < torrent.size(); i++) {
        if(torrent[i]->vbox.isChecked() == true) {
            vlistaDownload.append(i);
            QPointer<filedownloader> qdown(new filedownloader);
            qdown->fdownloadTorrent(torrent[i]->vlinkTorrent, torrent[i]->vnomeTorrent);
            torrent[i]->vbox.setCheckState(Qt::Unchecked);
            connect(qdown, &filedownloader::storrent, this, &janelatorrent::fesperaTorrent);
        }
    }
    fpreencheTabela();
}

void janelatorrent::fesperaTorrent() {
    QTimer lespera5Segundos;
    lespera5Segundos.singleShot(5000, this, &janelatorrent::fbaixaTorrent);
}

void janelatorrent::fbaixaTorrent()
{
    if(cconfig->fretornaPastaSalvarAnimes().isEmpty()) {
        qWarning() << "Error: Trying to download without setting a download folder";
        emit error("No download folder");
        return;
    }
    QProcess lprocesso;
    if(cconfig->fretornaTorrentEscolhido() == "uTorrent") {
        lprocesso.execute(QDir::homePath() + "/AppData/Roaming/uTorrent/uTorrent.exe",
                          QStringList() << "/DIRECTORY" << cconfig->fretornaPastaSalvarAnimes() + "/" +
                          torrent[vlistaDownload[0]]->vnomeAnime << QDir::currentPath() + "/Configurações/Temp/Torrents/"
                          + torrent[vlistaDownload[0]]->vnomeTorrent + ".torrent");
    }
    else if(cconfig->fretornaTorrentEscolhido() == "qBittorrent") {
        lprocesso.execute("\"C:/Program Files/qBittorrent/qbittorrent.exe\" --add-paused=false --skip-dialog=true --save-path=" +
                          cconfig->fretornaPastaSalvarAnimes() + "/" + torrent[vlistaDownload[0]]->vnomeAnime,
                          QStringList() <<  QDir::currentPath() + "/Configurações/Temp/Torrents/" + torrent[vlistaDownload[0]]->vnomeTorrent
                          + ".torrent");
    }
    torrent[vlistaDownload[0]]->vbox.setCheckState(Qt::Unchecked);
    vlistaDownload.remove(0);
    lprocesso.kill();
}

void janelatorrent::on_botaoAtualizaLista_clicked()
{
    ui->botaoAtualizaLista->blockSignals(true);
    if(!torrent.isEmpty())
        torrent.clear();
//    ui->listaTorrents->clear();
    QPointer<filedownloader> lbaixaXML(new filedownloader);
    lbaixaXML->fdownloadXMLTorrentList(cconfig->fretornaFeedAnime());
    connect(lbaixaXML, &filedownloader::sxml, this, &janelatorrent::fleXML);
}

void janelatorrent::fprocuraAnimeEspecifico(QString rnomeAnimeBuscado) {
    ui->botaoAtualizaLista->blockSignals(true);
    if(!torrent.isEmpty())
        torrent.clear();
//    ui->listaTorrents->clear();
    QPointer<filedownloader> lbaixaXML(new filedownloader);
    //https://nyaa.si/?page=rss&c=1_2&f=0&q=Ishuzoku Reviewers
    QString lfeedEspecifico = cconfig->fretornaFeedAnimeEspecifico();
    lfeedEspecifico.replace("%title%", rnomeAnimeBuscado);
    lbaixaXML->fdownloadXMLTorrentList(lfeedEspecifico);
    connect(lbaixaXML, &filedownloader::sxml, this, &janelatorrent::fleXML);
}

void janelatorrent::on_botaoInfoAnime_clicked()
{
    QModelIndexList select = ui->listaTorrents->selectionModel()->selectedRows();
    emit infoAnime(ui->listaTorrents->item(select.at(0).row(),1)->text());
}

void janelatorrent::on_botaoLinkTorrent_clicked()
{
    QModelIndexList select = ui->listaTorrents->selectionModel()->selectedRows();
    QString llink = ui->listaTorrents->item(select.at(0).row(),7)->text();
    QDesktopServices::openUrl(QUrl(llink,QUrl::TolerantMode));
}
