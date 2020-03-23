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
    ui->listaTorrents->setColumnHidden(9,true);
    ui->listaTorrents->setColumnWidth(6, 1034);
    ui->listaTorrents->setColumnWidth(0, 50);
    ui->listaTorrents->setColumnWidth(1, 400);
    ui->listaTorrents->setColumnWidth(5, 400);
    ui->listaTorrents->setSortingEnabled(true);
    qdown = new filedownloader;
}

janelatorrent::~janelatorrent()
{
    qDeleteAll(torrent);
    qdown->deleteLater();
    delete ui;
}


//SEMPRE vou pegar animes não baixados, desde que estejam na lista e/ou cumpram os filtros.
//SE já tiver visto/baixado, ou tenha um filtro NOT, não entra na lista.
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

                lepisodioAnime = QString::fromStdWString(elements.get(anitomy::kElementEpisodeNumber));
                lnomeAnime = QString::fromStdWString(elements.get(anitomy::kElementAnimeTitle));

                if(vHashDeNomeEId.contains(lnomeAnime))
                    lid = vHashDeNomeEId[lnomeAnime];
                else
                    lid = cleitor->fprocuraIdNasListasRapido(lnomeAnime);
                if(!vHashDeNomeEId.contains(lnomeAnime))
                    vHashDeNomeEId.insert(lnomeAnime,lid);

                lepisodiosAssistidos = cleitor->fbuscaAnimePorIDERetornaEpisodio(lid);
                lista = cleitor->fbuscaAnimePorIDERetornaLista(lid);
                lposicaoAnimeNaLista = cleitor->fbuscaAnimePorIDERetornaPosicao(lid);

                filtros = fchecaFiltroDownloadFromList(lid).split(";");
                filtros.removeAll(QString(""));
                //Caso o filtro seja de não baixar o anime de ID x, não baixamos.
                if(!filtros.isEmpty()){
                    if(filtros.at(0).contains("not", Qt::CaseInsensitive))
                        lprioridade -= 10;
                    else if(lista.compare(filtros.at(1), Qt::CaseInsensitive) == 0){
                        if(lista.compare("Watching", Qt::CaseInsensitive) == 0){
                            vlistaAtual = cleitor->retornaListaWatching();
                        }
                        else if(lista.compare("Plan to Watch", Qt::CaseInsensitive) == 0){
                            vlistaAtual = cleitor->retornaListaPlanToWatch();
                        }
                        else if(lista.compare("On Hold", Qt::CaseInsensitive) == 0){
                            vlistaAtual = cleitor->retornaListaOnHold();
                        }
                        else if(lista.compare("Completed", Qt::CaseInsensitive) == 0){
                            vlistaAtual = cleitor->retornaListaCompleted();
                        }
                        else if(lista.compare("Dropped", Qt::CaseInsensitive) == 0){
                            vlistaAtual = cleitor->retornaListaDropped();
                        }
                        else
                            vlistaAtual.clear();
                        if(lepisodioAnime.toInt() > lepisodiosAssistidos.toInt()){
                            //Checa se é o próximo episódio que deve ser assistido. Se estiver vazio, o episódio não foi baixado
                            //Se não estiver, o episódio já existe no computador.
                            if(lposicaoAnimeNaLista != -1 || vlistaAtual.isEmpty()){
                                QString lultimoEpisodioBaixado = carquivos->fprocuraEpisodioEspecifico
                                        (vlistaAtual[lposicaoAnimeNaLista],lepisodioAnime.toInt());
                                //Caso não exista o episódio na pasta, a string retorna vazia.
                                lultimoEpisodioBaixado = lultimoEpisodioBaixado.mid(lultimoEpisodioBaixado.lastIndexOf("/")+1);
                                if(lultimoEpisodioBaixado.isEmpty())
                                    lprioridade += 10;
                                else
                                    lprioridade -= 10;
                            }
                        }
                        else
                            lprioridade -= 10;
                    }
                }
                else{
                    if(lista.compare("Watching", Qt::CaseInsensitive) == 0 &&
                            cconfig->fretornaDownloadListasAnimes().contains("w")){
                        vlistaAtual = cleitor->retornaListaWatching();
                    }
                    else if(lista.compare("Plan to Watch", Qt::CaseInsensitive) == 0 &&
                            cconfig->fretornaDownloadListasAnimes().contains("p")){
                        vlistaAtual = cleitor->retornaListaPlanToWatch();
                    }
                    else if(lista.compare("On Hold", Qt::CaseInsensitive) == 0 &&
                            cconfig->fretornaDownloadListasAnimes().contains("o")){
                        vlistaAtual = cleitor->retornaListaOnHold();
                    }
                    else if(lista.compare("Dropped", Qt::CaseInsensitive) == 0 &&
                            cconfig->fretornaDownloadListasAnimes().contains("d")){
                        vlistaAtual = cleitor->retornaListaDropped();
                    }
                    else
                        vlistaAtual.clear();
                    if(lepisodioAnime.toInt() > lepisodiosAssistidos.toInt()){
                        //Checa se é o próximo episódio que deve ser assistido. Se estiver vazio, o episódio não foi baixado
                        //Se não estiver, o episódio já existe no computador.
                        if(lposicaoAnimeNaLista != -1){
                            QString lultimoEpisodioBaixado = "NotEmpty";
                            if(!vlistaAtual.isEmpty())
                                lultimoEpisodioBaixado = carquivos->fprocuraEpisodioEspecifico
                                    (vlistaAtual[lposicaoAnimeNaLista],lepisodioAnime.toInt());
                            //Caso não exista o episódio na pasta, a string retorna vazia.
                            lultimoEpisodioBaixado = lultimoEpisodioBaixado.mid(lultimoEpisodioBaixado.lastIndexOf("/")+1);
                            if(lultimoEpisodioBaixado.isEmpty()){
//                                qDebug() << vlistaAtual[lposicaoAnimeNaLista]->vid << vlistaAtual[lposicaoAnimeNaLista]->vnome;
                                lprioridade += 10;
                            }
                            //Se já tiver baixado, sai da lista
                            else
                                lprioridade -= 10;
                        }
                    }
                    //Caso já tenha sido assistido, sai da lista.
                    else
                        lprioridade -= 10;
                }
                //Se for do sub certo +2
                lfansub = QString::fromStdWString(elements.get(anitomy::kElementReleaseGroup));
                filtros = fchecaFiltroFansub(lid).split(";");
                filtros.removeAll(QString(""));
                if(!filtros.isEmpty()){
                    if(filtros.at(0).contains("not", Qt::CaseInsensitive)){
                        if(filtros.at(1).compare(lfansub, Qt::CaseInsensitive) == 0){
                            lprioridade = -10;
                        }
                    }
                    else if(lfansub.compare(filtros.at(1), Qt::CaseInsensitive) == 0)
                        lprioridade+=2;
                }
                else{
                    if(lfansub.compare(cconfig->fretornaSubEscolhido(), Qt::CaseInsensitive) == 0)
                        lprioridade+=2;
                }
                //Se for a resolução certa +2
                lresolucao = QString::fromStdWString(elements.get(anitomy::kElementVideoResolution));
                filtros = fchecaFiltroResolution(lid).split(";");
                filtros.removeAll(QString(""));
                if(!filtros.isEmpty()){
                    if(filtros.at(0).contains("not", Qt::CaseInsensitive)){
                        if(filtros.at(1).contains(lresolucao, Qt::CaseInsensitive)){
                            lprioridade -= 10;
                        }
                    }
                    else if(filtros.at(1).contains(lresolucao, Qt::CaseInsensitive))
                        lprioridade+=2;
                }
                else{
                    if(cconfig->fretornaQualidadeEscolhida().compare(lresolucao.toUtf8(), Qt::CaseInsensitive) == 0)
                        lprioridade+=2;
                }
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

                    filtros = fchecaFiltroLaguage(lid).split(";");
                    filtros.removeAll(QString(""));
                    if(!filtros.isEmpty()){
                        if(filtros.at(0).contains("not", Qt::CaseInsensitive)){
                            if(description.contains(filtros.at(1), Qt::CaseInsensitive)){
                                lprioridade = -10;
                            }
                        }
                        else if(description.contains(filtros.at(1), Qt::CaseInsensitive))
                            lprioridade++;
                    }
                    filtros = fchecaFiltroHasKeyword(lid).split(";");
                    filtros.removeAll(QString(""));
                    if(!filtros.isEmpty()){
                        if(filtros.at(0).contains("not", Qt::CaseInsensitive)){
                            if(description.contains(filtros.at(1), Qt::CaseInsensitive)){
                                lprioridade = -10;
                            }
                        }
                        else if(description.contains(filtros.at(1), Qt::CaseInsensitive))
                            lprioridade++;
                    }
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
                else if(lprioridade >= 14){
                    QStringList value = (QString::number(lprioridade)+":"+QString::number(torrent.size())).split(":");
                    vbaixar.insert(lnomeAnime+lepisodioAnime, value);
                }
                if(!ltorrentAux->vnomeTorrent.contains("Torrent File RSS"))
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
    for(int i = 0; i < torrent.size(); i++){
//        ui->listaTorrents->setRowHeight(i, 100);
        for(int w = 0; w < ui->listaTorrents->columnCount(); w++){
            QTableWidgetItem *litem = new QTableWidgetItem;
            switch (w) {
            case 0: {
                foreach (QString key, vbaixar.keys()) {
                    if(vbaixar[key].at(1).toInt() == i){
                        torrent[i]->vbox.setCheckState(Qt::Checked);
                        vbaixar.remove(key);
                    }
                }
                QModelIndex index = ui->listaTorrents->model()->index(i, w);
                QWidget *centerdCheckBoxWidget = new QWidget();
                QCheckBox *checkBox = new QCheckBox();
                vcontroladorCheckbox.insert(i,checkBox);
                connect(checkBox, &QCheckBox::stateChanged, this, &janelatorrent::fchecaEstado);
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
                if(torrent[i]->vbox.isChecked())
                    litem->setTextColor("royalblue");
                ui->listaTorrents->setItem(i,w,litem);
            break;
            case 2:
                litem->setText(torrent[i]->vfansub);
                if(torrent[i]->vbox.isChecked())
                    litem->setTextColor("royalblue");
                ui->listaTorrents->setItem(i,w, litem);
            break;
            case 3:
                litem->setText(torrent[i]->vresolucao);
                if(torrent[i]->vbox.isChecked())
                    litem->setTextColor("royalblue");
                ui->listaTorrents->setItem(i,w, litem);
            break;
            case 4:
                litem->setText(torrent[i]->vepisodioAnime);
                if(torrent[i]->vbox.isChecked())
                    litem->setTextColor("royalblue");
                ui->listaTorrents->setItem(i,w, litem);
            break;
            case 5:
                litem->setText(torrent[i]->vnomeTorrent);
                if(torrent[i]->vbox.isChecked())
                    litem->setTextColor("royalblue");
                ui->listaTorrents->setItem(i,w, litem);
            break;
            case 6:
                litem->setText(torrent[i]->vdescricaoTorrent);
                if(torrent[i]->vbox.isChecked())
                    litem->setTextColor("royalblue");
                ui->listaTorrents->setItem(i,w, litem);
            break;
            case 7:
                litem->setText(torrent[i]->vtorrentInfoLink);
                if(torrent[i]->vbox.isChecked())
                    litem->setTextColor("royalblue");
                ui->listaTorrents->setItem(i,w, litem);
            break;
            case 8:{
                int prioridade = 0;
                if(torrent[i]->vlista.compare("Watching") == 0 && cconfig->fretornaDownloadListasAnimes().contains("w"))
                    prioridade++;
                else if(torrent[i]->vlista.compare("On Hold") == 0 && cconfig->fretornaDownloadListasAnimes().contains("o"))
                    prioridade++;
                else if(torrent[i]->vlista.compare("Dropped") == 0 && cconfig->fretornaDownloadListasAnimes().contains("d"))
                    prioridade++;
                else if(torrent[i]->vlista.compare("Plan to Watch") == 0 && cconfig->fretornaDownloadListasAnimes().contains("p"))
                    prioridade++;
                if(torrent[i]->vbox.isChecked())
                    prioridade++;
                litem->setText(QString::number(prioridade));
                ui->listaTorrents->setItem(i,w, litem);
            }
            break;
            case 9:
                litem->setText(QString::number(i));
                ui->listaTorrents->setItem(i,w, litem);
            break;
            }
        }
    }
    ui->listaTorrents->sortByColumn(8);
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
    QProcess lprocesso;
    if(cconfig->fretornaTorrentEscolhido() == "uTorrent")
        lprocesso.setProgram(QDir::homePath() + "/AppData/Roaming/uTorrent/uTorrent.exe");
    else if(cconfig->fretornaTorrentEscolhido() == "qBittorrent")
        lprocesso.setProgram(QDir::rootPath() + "/Program Files/qBittorrent/qbittorrent.exe");

    lprocesso.startDetached();

    for(int i = 0; i < torrent.size(); i++){
        if(torrent[i]->vbox.isChecked() == true){
            qDebug() << torrent[i]->vnomeAnime << torrent[i]->vnomeTorrent;
            vlistaDownload.append(i);
            qdown->fdownloadTorrent(torrent[i]->vlinkTorrent, torrent[i]->vnomeTorrent);
            torrent[i]->vbox.setCheckState(Qt::Unchecked);
            connect(qdown, &filedownloader::storrent, this, &janelatorrent::fesperaTorrent);
        }
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

    torrent[vlistaDownload[0]]->vbox.setCheckState(Qt::Unchecked);
    vlistaDownload.remove(0);
}

void janelatorrent::on_botaoAtualizaLista_clicked()
{
    ui->botaoAtualizaLista->blockSignals(true);
    if(!torrent.isEmpty())
        torrent.clear();
    QPointer<filedownloader> lbaixaXML(new filedownloader);
    lbaixaXML->fdownloadXMLTorrentList(cconfig->fretornaFeedAnime());
    connect(lbaixaXML, &filedownloader::sxml, this, &janelatorrent::fleXML);
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
    QModelIndexList select = ui->listaTorrents->selectionModel()->selectedRows();
    emit infoAnime(ui->listaTorrents->item(select.at(0).row(),1)->text());
}

void janelatorrent::on_botaoLinkTorrent_clicked()
{
    QModelIndexList select = ui->listaTorrents->selectionModel()->selectedRows();
    QString llink = ui->listaTorrents->item(select.at(0).row(),7)->text();
    QDesktopServices::openUrl(QUrl(llink,QUrl::TolerantMode));
}

void janelatorrent::on_botaoSearchThisanime_clicked()
{
    QModelIndexList select = ui->listaTorrents->selectionModel()->selectedRows();
    QString lnome = ui->listaTorrents->item(select.at(0).row(),1)->text();
    fprocuraAnimeEspecifico(lnome);
}

void janelatorrent::on_botaoSelectSubForTorrent_clicked()
{
    QModelIndexList select = ui->listaTorrents->selectionModel()->selectedRows();
    QString lnome = ui->listaTorrents->item(select.at(0).row(),1)->text();
    QString lsub = ui->listaTorrents->item(select.at(0).row(),2)->text();
    QString id = cleitor->fprocuraAnimeNasListas(lnome);
    cconfig->fselectSubFromTorrent(id,lsub);
}

void janelatorrent::fchecaEstado(int estado)
{
    for(int i = 0; i < vcontroladorCheckbox.size(); i++){
        if(vcontroladorCheckbox[i]->isChecked()){
            qDebug() << ui->listaTorrents->item(i,1)->text();
            if(torrent.size() > i)
                torrent[i]->vbox.setCheckState(Qt::Checked);
        }
    }
}
