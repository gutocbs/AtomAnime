#include "torrenttab.h"
#include "ui_torrenttab.h"

TorrentTab::TorrentTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TorrentTab)
{
    ui->setupUi(this);
    vpagina = 1;
    vtorrentSelecionado = 0;
    fhideTudo();
    connect(ui->checkTorrent00, &QCheckBox::stateChanged, this, &TorrentTab::fchecaEstado);
    connect(ui->checkTorrent01, &QCheckBox::stateChanged, this, &TorrentTab::fchecaEstado);
    connect(ui->checkTorrent02, &QCheckBox::stateChanged, this, &TorrentTab::fchecaEstado);
    connect(ui->checkTorrent03, &QCheckBox::stateChanged, this, &TorrentTab::fchecaEstado);
    connect(ui->checkTorrent04, &QCheckBox::stateChanged, this, &TorrentTab::fchecaEstado);
    connect(ui->checkTorrent05, &QCheckBox::stateChanged, this, &TorrentTab::fchecaEstado);
    connect(ui->checkTorrent06, &QCheckBox::stateChanged, this, &TorrentTab::fchecaEstado);
    connect(ui->checkTorrent07, &QCheckBox::stateChanged, this, &TorrentTab::fchecaEstado);
    connect(ui->checkTorrent08, &QCheckBox::stateChanged, this, &TorrentTab::fchecaEstado);
    connect(ui->checkTorrent09, &QCheckBox::stateChanged, this, &TorrentTab::fchecaEstado);
}

TorrentTab::~TorrentTab()
{
    qDeleteAll(torrent);
    delete ui;
}

void TorrentTab::fautoDownload()
{
    on_carregaLista_clicked();
    on_botaoBaixar_clicked();
}

void TorrentTab::fleXML()
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
            fConstroiListaTorrents();
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
                else if(lprioridade >= 14){
                    QStringList value = (QString::number(lprioridade)+":"+QString::number(torrent.size())).split(":");
                    vbaixar.insert(lnomeAnime+lepisodioAnime, value);
                }
                if(!ltorrentAux->vnomeTorrent.contains("Torrent File RSS")){
                    torrent.append(ltorrentAux);
                    if(!vHashDeIdEPosicaoDoTorrent.contains(lid)){
                        QVector<int> posicaoTorrent;
                        posicaoTorrent.append(torrent.size()-1);
                        vHashDeIdEPosicaoDoTorrent.insert(lid, posicaoTorrent);
                        if(lprioridade >= 14)
                            vlistaDeIDs.prepend(lid);
                        else if(lprioridade >= 12)
                            vlistaDeIDs.insert(vlistaDeIDs.size()/2,lid);
                        else
                            vlistaDeIDs.append(lid);
                    }
                    else
                        vHashDeIdEPosicaoDoTorrent[lid].append(torrent.size()-1);
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
                vbaixar.remove(key);
            }
        }
    }
    funblockSignals();
}

void TorrentTab::fpassaPonteiros(leitorlistaanimes *lleitor,  janeladeconfig *lconf, arquivos *lcarq, confBase *lcconfbase)
{
    cleitor = lleitor;
    cconfig = lconf;
    carquivos = lcarq;
    cconfbase = lcconfbase;
}


QString TorrentTab::fchecaFiltroFansub(QString lid)
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

QString TorrentTab::fchecaFiltroDownloadFromList(QString lid)
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

QString TorrentTab::fchecaFiltroResolution(QString lid)
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

QString TorrentTab::fchecaFiltroLaguage(QString lid)
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

QString TorrentTab::fchecaFiltroHasKeyword(QString lid)
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

void TorrentTab::fConstroiListaTorrents()
{
    QPixmap pix;
    if(vlistaDeIDs.size() > 0+(12*(vpagina-1))){
        ui->imagemAnime00->setScaledContents(true);
        ui->labelFundoAnime00Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime00Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime00Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[0+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 47)
            ui->labelAnime00Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[0+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 58)
            ui->labelAnime00Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime00Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelFundoAnime00Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime00Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime00Titulo->setStyleSheet("background: rgb(185,201,250);");
        ui->labelAnime00Titulo->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[0+(12*(vpagina-1))]].at(0)]->vnomeAnime);
        ui->labelAnime00Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime00Titulo->setWordWrap(true);
        ui->labelAnime00Progresso->setAlignment(Qt::AlignCenter);
        ui->labelAnime00Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime00Nota->setAlignment(Qt::AlignCenter);
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[0+(12*(vpagina-1))]].at(0)]->vbaixar){
            ui->labelAnime00Progresso->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[0+(12*(vpagina-1))]].at(0)]->vfansub);
            ui->labelAnime00Nota->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[0+(12*(vpagina-1))]].at(0)]->vresolucao);
        }
        else{
            ui->labelAnime00Progresso->setText("Not selected for download.");
            ui->labelAnime00Nota->clear();
        }
        if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[0+(12*(vpagina-1))]+".jpg", "jpg")){
            ui->imagemAnime00->setPixmap(pix);
        }
        else if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[0+(12*(vpagina-1))]+".png", "png")){
            ui->imagemAnime00->setPixmap(pix);
        }
        else{
            if(pix.load(cconfbase->vimagemStaple, "jpg"))
                ui->imagemAnime00->setPixmap(pix);
        }
    }
    if(vlistaDeIDs.size() > 1+(12*(vpagina-1))){
        ui->imagemAnime01->setScaledContents(true);
        ui->labelFundoAnime01Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime01Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime01Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[1+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 47)
            ui->labelAnime01Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[1+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 58)
            ui->labelAnime01Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime01Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime01Titulo->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[1+(12*(vpagina-1))]].at(0)]->vnomeAnime);
        ui->labelAnime01Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime01Titulo->setWordWrap(true);
        ui->labelAnime01Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[1+(12*(vpagina-1))]].at(0)]->vbaixar){
            ui->labelAnime01Progresso->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[1+(12*(vpagina-1))]].at(0)]->vfansub);
            ui->labelAnime01Nota->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[1+(12*(vpagina-1))]].at(0)]->vresolucao);
        }
        else{
            ui->labelAnime01Progresso->setText("Not selected for download.");
            ui->labelAnime01Nota->clear();
        }
        if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[1+(12*(vpagina-1))]+".jpg", "jpg")){
            ui->imagemAnime01->setPixmap(pix);
        }
        else if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[1+(12*(vpagina-1))]+".png", "png")){
            ui->imagemAnime01->setPixmap(pix);
        }
        else{
            if(pix.load(cconfbase->vimagemStaple, "jpg"))
                ui->imagemAnime01->setPixmap(pix);
        }
    }
    else{
        ui->imagemAnime01->clear();
        ui->imagemAnime01->setStyleSheet("background: transparent;");
        ui->labelAnime01Titulo->clear();
        ui->labelAnime01Progresso->clear();
        ui->labelAnime01Nota->clear();
        ui->labelFundoAnime01Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime01Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime01Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime01Lista->clear();
        ui->labelFundoAnime01Lista->setStyleSheet("background: transparent");
    }
    if(vlistaDeIDs.size() > 2+(12*(vpagina-1))){
        ui->imagemAnime02->setScaledContents(true);
        ui->labelFundoAnime02Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime02Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime02Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[2+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 47)
            ui->labelAnime02Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[2+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 58)
            ui->labelAnime02Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime02Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime02Titulo->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[2+(12*(vpagina-1))]].at(0)]->vnomeAnime);
        ui->labelAnime02Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime02Titulo->setWordWrap(true);
        ui->labelAnime02Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[2+(12*(vpagina-1))]].at(0)]->vbaixar){
            ui->labelAnime02Progresso->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[2+(12*(vpagina-1))]].at(0)]->vfansub);
            ui->labelAnime02Nota->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[2+(12*(vpagina-1))]].at(0)]->vresolucao);
        }
        else{
            ui->labelAnime02Progresso->setText("Not selected for download.");
            ui->labelAnime02Nota->clear();
        }
        if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[2+(12*(vpagina-1))]+".jpg", "jpg")){
            ui->imagemAnime02->setPixmap(pix);
        }
        else if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[2+(12*(vpagina-1))]+".png", "png")){
            ui->imagemAnime02->setPixmap(pix);
        }
        else{
            if(pix.load(cconfbase->vimagemStaple, "jpg"))
                ui->imagemAnime02->setPixmap(pix);
        }
    }
    else{
        ui->imagemAnime02->clear();
        ui->imagemAnime02->setStyleSheet("background: transparent;");
        ui->labelAnime02Titulo->clear();
        ui->labelAnime02Progresso->clear();
        ui->labelAnime02Nota->clear();
        ui->labelFundoAnime02Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime02Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime02Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime02Lista->clear();
        ui->labelFundoAnime02Lista->setStyleSheet("background: transparent");
    }
    if(vlistaDeIDs.size() > 3+(12*(vpagina-1))){
        ui->imagemAnime03->setScaledContents(true);
        ui->labelFundoAnime03Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime03Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime03Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[3+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 47)
            ui->labelAnime03Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[3+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 58)
            ui->labelAnime03Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime03Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime03Titulo->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[3+(12*(vpagina-1))]].at(0)]->vnomeAnime);
        ui->labelAnime03Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime03Titulo->setWordWrap(true);
        ui->labelAnime03Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[3+(12*(vpagina-1))]].at(0)]->vbaixar){
            ui->labelAnime03Progresso->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[3+(12*(vpagina-1))]].at(0)]->vfansub);
            ui->labelAnime03Nota->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[3+(12*(vpagina-1))]].at(0)]->vresolucao);
        }
        else{
            ui->labelAnime03Progresso->setText("Not selected for download.");
            ui->labelAnime03Nota->clear();
        }
        if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[3+(12*(vpagina-1))]+".jpg", "jpg")){
            ui->imagemAnime03->setPixmap(pix);
        }
        else if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[3+(12*(vpagina-1))]+".png", "png")){
            ui->imagemAnime03->setPixmap(pix);
        }
        else{
            if(pix.load(cconfbase->vimagemStaple, "jpg"))
                ui->imagemAnime03->setPixmap(pix);
        }
    }
    else{
        ui->imagemAnime03->clear();
        ui->imagemAnime03->setStyleSheet("background: transparent;");
        ui->labelAnime03Titulo->clear();
        ui->labelAnime03Progresso->clear();
        ui->labelAnime03Nota->clear();
        ui->labelFundoAnime03Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime03Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime03Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime03Lista->clear();
        ui->labelFundoAnime03Lista->setStyleSheet("background: transparent");
    }
    if(vlistaDeIDs.size() > 4+(12*(vpagina-1))){
        ui->imagemAnime04->setScaledContents(true);
        ui->labelFundoAnime04Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime04Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime04Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[4+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 47)
            ui->labelAnime04Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[4+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 58)
            ui->labelAnime04Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime04Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime04Titulo->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[4+(12*(vpagina-1))]].at(0)]->vnomeAnime);
        ui->labelAnime04Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime04Titulo->setWordWrap(true);
        ui->labelAnime04Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[4+(12*(vpagina-1))]].at(0)]->vbaixar){
            ui->labelAnime04Progresso->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[4+(12*(vpagina-1))]].at(0)]->vfansub);
            ui->labelAnime04Nota->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[4+(12*(vpagina-1))]].at(0)]->vresolucao);
        }
        else{
            ui->labelAnime04Progresso->setText("Not selected for download.");
            ui->labelAnime04Nota->clear();
        }
        if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[4+(12*(vpagina-1))]+".jpg", "jpg")){
            ui->imagemAnime04->setPixmap(pix);
        }
        else if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[4+(12*(vpagina-1))]+".png", "png")){
            ui->imagemAnime04->setPixmap(pix);
        }
        else{
            if(pix.load(cconfbase->vimagemStaple, "jpg"))
                ui->imagemAnime04->setPixmap(pix);
        }
    }
    else{
        ui->imagemAnime04->clear();
        ui->imagemAnime04->setStyleSheet("background: transparent;");
        ui->labelAnime04Titulo->clear();
        ui->labelAnime04Progresso->clear();
        ui->labelAnime04Nota->clear();
        ui->labelFundoAnime04Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime04Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime04Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime04Lista->clear();
        ui->labelFundoAnime04Lista->setStyleSheet("background: transparent");
    }
    if(vlistaDeIDs.size() > 5+(12*(vpagina-1))){
        ui->imagemAnime05->setScaledContents(true);
        ui->labelFundoAnime05Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime05Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime05Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[5+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 47)
            ui->labelAnime05Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[5+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 58)
            ui->labelAnime05Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime05Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime05Titulo->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[5+(12*(vpagina-1))]].at(0)]->vnomeAnime);
        ui->labelAnime05Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime05Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[5+(12*(vpagina-1))]].at(0)]->vbaixar){
            ui->labelAnime05Progresso->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[5+(12*(vpagina-1))]].at(0)]->vfansub);
            ui->labelAnime05Nota->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[5+(12*(vpagina-1))]].at(0)]->vresolucao);
        }
        else{
            ui->labelAnime05Progresso->setText("Not selected for download.");
            ui->labelAnime05Nota->clear();
        }
        if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[5+(12*(vpagina-1))]+".jpg", "jpg")){
            ui->imagemAnime05->setPixmap(pix);
        }
        else if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[5+(12*(vpagina-1))]+".png", "png")){
            ui->imagemAnime05->setPixmap(pix);
        }
        else{
            if(pix.load(cconfbase->vimagemStaple, "jpg"))
                ui->imagemAnime05->setPixmap(pix);
        }
    }
    else{
        ui->imagemAnime05->clear();
        ui->imagemAnime05->setStyleSheet("background: transparent;");
        ui->labelAnime05Titulo->clear();
        ui->labelAnime05Progresso->clear();
        ui->labelAnime05Nota->clear();
        ui->labelFundoAnime05Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime05Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime05Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime05Lista->clear();
        ui->labelFundoAnime05Lista->setStyleSheet("background: transparent");
    }
    if(vlistaDeIDs.size() > 6+(12*(vpagina-1))){
        ui->imagemAnime06->setScaledContents(true);
        ui->labelFundoAnime06Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime06Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime06Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[6+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 47)
            ui->labelAnime06Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[6+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 58)
            ui->labelAnime06Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime06Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime06Titulo->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[6+(12*(vpagina-1))]].at(0)]->vnomeAnime);
        ui->labelAnime06Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime06Titulo->setWordWrap(true);
        ui->labelAnime06Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[6+(12*(vpagina-1))]].at(0)]->vbaixar){
            ui->labelAnime06Progresso->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[6+(12*(vpagina-1))]].at(0)]->vfansub);
            ui->labelAnime06Nota->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[6+(12*(vpagina-1))]].at(0)]->vresolucao);
        }
        else{
            ui->labelAnime06Progresso->setText("Not selected for download.");
            ui->labelAnime06Nota->clear();
        }
        if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[6+(12*(vpagina-1))]+".jpg", "jpg")){
            ui->imagemAnime06->setPixmap(pix);
        }
        else if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[6+(12*(vpagina-1))]+".png", "png")){
            ui->imagemAnime06->setPixmap(pix);
        }
        else{
            if(pix.load(cconfbase->vimagemStaple, "jpg"))
                ui->imagemAnime06->setPixmap(pix);
        }
    }
    else{
        ui->imagemAnime06->clear();
        ui->imagemAnime06->setStyleSheet("background: transparent;");
        ui->labelAnime06Titulo->clear();
        ui->labelAnime06Progresso->clear();
        ui->labelAnime06Nota->clear();
        ui->labelFundoAnime06Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime06Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime06Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime06Lista->clear();
        ui->labelFundoAnime06Lista->setStyleSheet("background: transparent");
    }
    if(vlistaDeIDs.size() > 7+(12*(vpagina-1))){
        ui->imagemAnime07->setScaledContents(true);
        ui->labelFundoAnime07Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime07Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime07Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[7+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 47)
            ui->labelAnime07Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[7+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 58)
            ui->labelAnime07Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime07Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime07Titulo->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[7+(12*(vpagina-1))]].at(0)]->vnomeAnime);
        ui->labelAnime07Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime07Titulo->setWordWrap(true);
        ui->labelAnime07Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[7+(12*(vpagina-1))]].at(0)]->vbaixar){
            ui->labelAnime07Progresso->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[7+(12*(vpagina-1))]].at(0)]->vfansub);
            ui->labelAnime07Nota->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[7+(12*(vpagina-1))]].at(0)]->vresolucao);
        }
        else{
            ui->labelAnime07Progresso->setText("Not selected for download.");
            ui->labelAnime07Nota->clear();
        }
        if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[7+(12*(vpagina-1))]+".jpg", "jpg")){
            ui->imagemAnime07->setPixmap(pix);
        }
        else if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[7+(12*(vpagina-1))]+".png", "png")){
            ui->imagemAnime07->setPixmap(pix);
        }
        else{
            if(pix.load(cconfbase->vimagemStaple, "jpg"))
                ui->imagemAnime07->setPixmap(pix);
        }
    }
    else{
        ui->imagemAnime07->clear();
        ui->imagemAnime07->setStyleSheet("background: transparent;");
        ui->labelAnime07Titulo->clear();
        ui->labelAnime07Progresso->clear();
        ui->labelAnime07Nota->clear();
        ui->labelFundoAnime07Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime07Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime07Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime07Lista->clear();
        ui->labelFundoAnime07Lista->setStyleSheet("background: transparent");
    }
    if(vlistaDeIDs.size() > 8+(12*(vpagina-1))){
        ui->imagemAnime08->setScaledContents(true);
        ui->labelFundoAnime08Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime08Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime08Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[8+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 47)
            ui->labelAnime08Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[8+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 58)
            ui->labelAnime08Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime08Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime08Titulo->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[8+(12*(vpagina-1))]].at(0)]->vnomeAnime);
        ui->labelAnime08Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime08Titulo->setWordWrap(true);
        ui->labelAnime08Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[8+(12*(vpagina-1))]].at(0)]->vbaixar){
            ui->labelAnime08Progresso->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[8+(12*(vpagina-1))]].at(0)]->vfansub);
            ui->labelAnime08Nota->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[8+(12*(vpagina-1))]].at(0)]->vresolucao);
        }
        else{
            ui->labelAnime08Progresso->setText("Not selected for download.");
            ui->labelAnime08Nota->clear();
        }
        if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[8+(12*(vpagina-1))]+".jpg", "jpg")){
            ui->imagemAnime08->setPixmap(pix);
        }
        else if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[8+(12*(vpagina-1))]+".png", "png")){
            ui->imagemAnime08->setPixmap(pix);
        }
        else{
            if(pix.load(cconfbase->vimagemStaple, "jpg"))
                ui->imagemAnime08->setPixmap(pix);
        }
    }
    else{
        ui->imagemAnime08->clear();
        ui->imagemAnime08->setStyleSheet("background: transparent;");
        ui->labelAnime08Titulo->clear();
        ui->labelAnime08Progresso->clear();
        ui->labelAnime08Nota->clear();
        ui->labelFundoAnime08Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime08Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime08Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime08Lista->clear();
        ui->labelFundoAnime08Lista->setStyleSheet("background: transparent");
    }
    if(vlistaDeIDs.size() > 9+(12*(vpagina-1))){
        ui->imagemAnime09->setScaledContents(true);
        ui->labelFundoAnime09Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime09Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime09Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[9+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 47)
            ui->labelAnime09Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[9+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 58)
            ui->labelAnime09Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime09Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime09Titulo->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[9+(12*(vpagina-1))]].at(0)]->vnomeAnime);
        ui->labelAnime09Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime09Titulo->setWordWrap(true);
        ui->labelAnime09Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[9+(12*(vpagina-1))]].at(0)]->vbaixar){
            ui->labelAnime09Progresso->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[9+(12*(vpagina-1))]].at(0)]->vfansub);
            ui->labelAnime09Nota->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[9+(12*(vpagina-1))]].at(0)]->vresolucao);
        }
        else{
            ui->labelAnime09Progresso->setText("Not selected for download.");
            ui->labelAnime09Nota->clear();
        }
        if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[9+(12*(vpagina-1))]+".jpg", "jpg")){
            ui->imagemAnime09->setPixmap(pix);
        }
        else if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[9+(12*(vpagina-1))]+".png", "png")){
            ui->imagemAnime09->setPixmap(pix);
        }
        else{
            if(pix.load(cconfbase->vimagemStaple, "jpg"))
                ui->imagemAnime09->setPixmap(pix);
        }
    }
    else{
        ui->imagemAnime09->clear();
        ui->imagemAnime09->setStyleSheet("background: transparent;");
        ui->labelAnime09Titulo->clear();
        ui->labelAnime09Progresso->clear();
        ui->labelAnime09Nota->clear();
        ui->labelFundoAnime09Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime09Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime09Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime09Lista->clear();
        ui->labelFundoAnime09Lista->setStyleSheet("background: transparent");
    }
    if(vlistaDeIDs.size() > 10+(12*(vpagina-1))){
        ui->imagemAnime10->setScaledContents(true);
        ui->labelFundoAnime10Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime10Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime10Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[10+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 47)
            ui->labelAnime10Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[10+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 58)
            ui->labelAnime10Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime10Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime10Titulo->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[10+(12*(vpagina-1))]].at(0)]->vnomeAnime);
        ui->labelAnime10Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime10Titulo->setWordWrap(true);
        ui->labelAnime10Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[10+(12*(vpagina-1))]].at(0)]->vbaixar){
            ui->labelAnime10Progresso->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[10+(12*(vpagina-1))]].at(0)]->vfansub);
            ui->labelAnime10Nota->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[10+(12*(vpagina-1))]].at(0)]->vresolucao);
        }
        else{
            ui->labelAnime10Progresso->setText("Not selected for download.");
            ui->labelAnime10Nota->clear();
        }
        if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[10+(12*(vpagina-1))]+".jpg", "jpg")){
            ui->imagemAnime10->setPixmap(pix);
        }
        else if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[10+(12*(vpagina-1))]+".png", "png")){
            ui->imagemAnime10->setPixmap(pix);
        }
        else{
            if(pix.load(cconfbase->vimagemStaple, "jpg"))
                ui->imagemAnime10->setPixmap(pix);
        }
    }
    else{
        ui->imagemAnime10->clear();
        ui->imagemAnime10->setStyleSheet("background: transparent;");
        ui->labelAnime10Titulo->clear();
        ui->labelAnime10Progresso->clear();
        ui->labelAnime10Nota->clear();
        ui->labelFundoAnime10Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime10Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime10Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime10Lista->clear();
        ui->labelFundoAnime10Lista->setStyleSheet("background: transparent");
    }
    if(vlistaDeIDs.size() > 11+(12*(vpagina-1))){
        ui->imagemAnime11->setScaledContents(true);
        ui->labelFundoAnime11Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime11Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime11Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[11+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 47)
            ui->labelAnime11Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[11+(12*(vpagina-1))]].at(0)]->vnomeAnime.size() < 58)
            ui->labelAnime11Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime11Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime11Titulo->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[11+(12*(vpagina-1))]].at(0)]->vnomeAnime);
        ui->labelAnime11Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime11Titulo->setWordWrap(true);
        ui->labelAnime11Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        if(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[11+(12*(vpagina-1))]].at(0)]->vbaixar){
            ui->labelAnime11Progresso->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[11+(12*(vpagina-1))]].at(0)]->vfansub);
            ui->labelAnime11Nota->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[11+(12*(vpagina-1))]].at(0)]->vresolucao);
        }
        else{
            ui->labelAnime11Progresso->setText("Not selected for download.");
            ui->labelAnime11Nota->clear();
        }
        if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[11+(12*(vpagina-1))]+".jpg", "jpg")){
            ui->imagemAnime11->setPixmap(pix);
        }
        else if(pix.load(cconfbase->vdiretorioImagensMedio+vlistaDeIDs[11+(12*(vpagina-1))]+".png", "png")){
            ui->imagemAnime11->setPixmap(pix);
        }
        else{
            if(pix.load(cconfbase->vimagemStaple, "jpg"))
                ui->imagemAnime11->setPixmap(pix);
        }
    }
    else{
        ui->imagemAnime11->clear();
        ui->imagemAnime11->setStyleSheet("background: transparent;");
        ui->labelAnime11Titulo->clear();
        ui->labelAnime11Progresso->clear();
        ui->labelAnime11Nota->clear();
        ui->labelFundoAnime11Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime11Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime11Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime11Lista->clear();
        ui->labelFundoAnime11Lista->setStyleSheet("background: transparent");
    }
}

void TorrentTab::fmostraTorrent()
{
    ui->listaSubs->clear();
    vnomeSubs.clear();
    ui->labelInfoNomeAnime->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(0)]->vnomeAnime);
    for(int i = 0; i < vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].size(); i++){
        if(!vnomeSubs.contains(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(i)]->vfansub)
                && !torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(i)]->vfansub.isEmpty()){
            vnomeSubs.append(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(i)]->vfansub);
            ui->listaSubs->addItem(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(i)]->vfansub);
        }
    }

    if(vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].size() > 0){
        ui->checkTorrent00->show();
        if(!torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(0)]->vbaixar)
            ui->checkTorrent00->setCheckState(Qt::Unchecked);
        else
            ui->checkTorrent00->setCheckState(Qt::Checked);
        ui->qualidadeTorrent00->show();
        ui->episodioTorrent00->show();
        ui->nomeTorrent00->show();
        ui->descricaoTorrent00->show();
        ui->checkTorrent00->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(0)]->vfansub);
        ui->qualidadeTorrent00->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(0)]->vresolucao);
        ui->episodioTorrent00->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(0)]->vepisodioAnime);
        ui->nomeTorrent00->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(0)]->vnomeTorrent);
        ui->descricaoTorrent00->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(0)]->
                vdescricaoTorrent);
    }
    else{
        ui->checkTorrent00->hide();
        ui->qualidadeTorrent00->hide();
        ui->episodioTorrent00->hide();
        ui->nomeTorrent00->hide();
        ui->descricaoTorrent00->hide();
    }
    if(vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].size() > 1){
        ui->checkTorrent01->show();
        if(!torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(0)]->vbaixar)
            ui->checkTorrent00->setCheckState(Qt::Unchecked);
        else
            ui->checkTorrent00->setCheckState(Qt::Checked);
        ui->qualidadeTorrent01->show();
        ui->episodioTorrent01->show();
        ui->nomeTorrent01->show();
        ui->descricaoTorrent01->show();
        ui->checkTorrent01->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(1)]->vfansub);
        ui->qualidadeTorrent01->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(1)]->vresolucao);
        ui->episodioTorrent01->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(1)]->vepisodioAnime);
        ui->nomeTorrent01->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(1)]->vnomeTorrent);
        ui->descricaoTorrent01->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(1)]->
                vdescricaoTorrent);
    }
    else{
        ui->checkTorrent01->hide();
        ui->qualidadeTorrent01->hide();
        ui->episodioTorrent01->hide();
        ui->nomeTorrent01->hide();
        ui->descricaoTorrent01->hide();
    }
    if(vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].size() > 2){
        ui->checkTorrent02->show();
        if(!torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(0)]->vbaixar)
            ui->checkTorrent00->setCheckState(Qt::Unchecked);
        else
            ui->checkTorrent00->setCheckState(Qt::Checked);
        ui->qualidadeTorrent02->show();
        ui->episodioTorrent02->show();
        ui->nomeTorrent02->show();
        ui->descricaoTorrent02->show();
        ui->checkTorrent02->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(2)]->vfansub);
        ui->qualidadeTorrent02->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(2)]->vresolucao);
        ui->episodioTorrent02->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(2)]->vepisodioAnime);
        ui->nomeTorrent02->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(2)]->vnomeTorrent);
        ui->descricaoTorrent02->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(2)]->
                vdescricaoTorrent);
    }
    else{
        ui->checkTorrent02->hide();
        ui->qualidadeTorrent02->hide();
        ui->episodioTorrent02->hide();
        ui->nomeTorrent02->hide();
        ui->descricaoTorrent02->hide();
    }
    if(vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].size() > 3){
        ui->checkTorrent03->show();
        if(!torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(0)]->vbaixar)
            ui->checkTorrent00->setCheckState(Qt::Unchecked);
        else
            ui->checkTorrent00->setCheckState(Qt::Checked);
        ui->qualidadeTorrent03->show();
        ui->episodioTorrent03->show();
        ui->nomeTorrent03->show();
        ui->descricaoTorrent03->show();
        ui->checkTorrent03->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(3)]->vfansub);
        ui->qualidadeTorrent03->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(3)]->vresolucao);
        ui->episodioTorrent03->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(3)]->vepisodioAnime);
        ui->nomeTorrent03->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(3)]->vnomeTorrent);
        ui->descricaoTorrent03->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(3)]->
                vdescricaoTorrent);
    }
    else{
        ui->checkTorrent03->hide();
        ui->qualidadeTorrent03->hide();
        ui->episodioTorrent03->hide();
        ui->nomeTorrent03->hide();
        ui->descricaoTorrent03->hide();
    }
    if(vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].size() > 4){
        ui->checkTorrent04->show();
        if(!torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(0)]->vbaixar)
            ui->checkTorrent00->setCheckState(Qt::Unchecked);
        else
            ui->checkTorrent00->setCheckState(Qt::Checked);
        ui->qualidadeTorrent04->show();
        ui->episodioTorrent04->show();
        ui->nomeTorrent04->show();
        ui->descricaoTorrent04->show();
        ui->checkTorrent04->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(4)]->vfansub);
        ui->qualidadeTorrent04->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(4)]->vresolucao);
        ui->episodioTorrent04->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(4)]->vepisodioAnime);
        ui->nomeTorrent04->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(4)]->vnomeTorrent);
        ui->descricaoTorrent04->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(4)]->
                vdescricaoTorrent);
    }
    else{
        ui->checkTorrent04->hide();
        ui->qualidadeTorrent04->hide();
        ui->episodioTorrent04->hide();
        ui->nomeTorrent04->hide();
        ui->descricaoTorrent04->hide();
    }
    if(vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].size() > 5){
        ui->checkTorrent05->show();
        if(!torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(0)]->vbaixar)
            ui->checkTorrent00->setCheckState(Qt::Unchecked);
        else
            ui->checkTorrent00->setCheckState(Qt::Checked);
        ui->qualidadeTorrent05->show();
        ui->episodioTorrent05->show();
        ui->nomeTorrent05->show();
        ui->descricaoTorrent05->show();
        ui->checkTorrent05->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(5)]->vfansub);
        ui->qualidadeTorrent05->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(5)]->vresolucao);
        ui->episodioTorrent05->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(5)]->vepisodioAnime);
        ui->nomeTorrent05->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(5)]->vnomeTorrent);
        ui->descricaoTorrent05->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(5)]->
                vdescricaoTorrent);
    }
    else{
        ui->checkTorrent05->hide();
        ui->qualidadeTorrent05->hide();
        ui->episodioTorrent05->hide();
        ui->nomeTorrent05->hide();
        ui->descricaoTorrent05->hide();
    }
    if(vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].size() > 6){
        ui->checkTorrent06->show();
        if(!torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(0)]->vbaixar)
            ui->checkTorrent00->setCheckState(Qt::Unchecked);
        else
            ui->checkTorrent00->setCheckState(Qt::Checked);
        ui->qualidadeTorrent06->show();
        ui->episodioTorrent06->show();
        ui->nomeTorrent06->show();
        ui->descricaoTorrent06->show();
        ui->checkTorrent06->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(6)]->vfansub);
        ui->qualidadeTorrent06->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(6)]->vresolucao);
        ui->episodioTorrent06->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(6)]->vepisodioAnime);
        ui->nomeTorrent06->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(6)]->vnomeTorrent);
        ui->descricaoTorrent06->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(6)]->
                vdescricaoTorrent);
    }
    else{
        ui->checkTorrent06->hide();
        ui->qualidadeTorrent06->hide();
        ui->episodioTorrent06->hide();
        ui->nomeTorrent06->hide();
        ui->descricaoTorrent06->hide();
    }
    if(vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].size() > 7){
        ui->checkTorrent07->show();
        if(!torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(0)]->vbaixar)
            ui->checkTorrent00->setCheckState(Qt::Unchecked);
        else
            ui->checkTorrent00->setCheckState(Qt::Checked);
        ui->qualidadeTorrent07->show();
        ui->episodioTorrent07->show();
        ui->nomeTorrent07->show();
        ui->descricaoTorrent07->show();
        ui->checkTorrent07->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(7)]->vfansub);
        ui->qualidadeTorrent07->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(7)]->vresolucao);
        ui->episodioTorrent07->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(7)]->vepisodioAnime);
        ui->nomeTorrent07->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(7)]->vnomeTorrent);
        ui->descricaoTorrent07->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(7)]->
                vdescricaoTorrent);
    }
    else{
        ui->checkTorrent07->hide();
        ui->qualidadeTorrent07->hide();
        ui->episodioTorrent07->hide();
        ui->nomeTorrent07->hide();
        ui->descricaoTorrent07->hide();
    }
    if(vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].size() > 8){
        ui->checkTorrent08->show();
        if(!torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(0)]->vbaixar)
            ui->checkTorrent00->setCheckState(Qt::Unchecked);
        else
            ui->checkTorrent00->setCheckState(Qt::Checked);
        ui->qualidadeTorrent08->show();
        ui->episodioTorrent08->show();
        ui->nomeTorrent08->show();
        ui->descricaoTorrent08->show();
        ui->checkTorrent08->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(8)]->vfansub);
        ui->qualidadeTorrent08->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(8)]->vresolucao);
        ui->episodioTorrent08->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(8)]->vepisodioAnime);
        ui->nomeTorrent08->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(8)]->vnomeTorrent);
        ui->descricaoTorrent08->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(8)]->
                vdescricaoTorrent);
    }
    else{
        ui->checkTorrent08->hide();
        ui->qualidadeTorrent08->hide();
        ui->episodioTorrent08->hide();
        ui->nomeTorrent08->hide();
        ui->descricaoTorrent08->hide();
    }
    if(vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].size() > 9){
        ui->checkTorrent09->show();
        if(!torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(0)]->vbaixar)
            ui->checkTorrent00->setCheckState(Qt::Unchecked);
        else
            ui->checkTorrent00->setCheckState(Qt::Checked);
        ui->qualidadeTorrent09->show();
        ui->episodioTorrent09->show();
        ui->nomeTorrent09->show();
        ui->descricaoTorrent09->show();
        ui->checkTorrent09->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(9)]->vfansub);
        ui->qualidadeTorrent09->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(9)]->vresolucao);
        ui->episodioTorrent09->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(9)]->vepisodioAnime);
        ui->nomeTorrent09->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(9)]->vnomeTorrent);
        ui->descricaoTorrent09->setText(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(9)]->
                vdescricaoTorrent);
    }
    else{
        ui->checkTorrent09->hide();
        ui->qualidadeTorrent09->hide();
        ui->episodioTorrent09->hide();
        ui->nomeTorrent09->hide();
        ui->descricaoTorrent09->hide();
    }
}

void TorrentTab::fchecaEstado()
{
    if(vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].size() > 0){
        if(ui->checkTorrent00->isChecked())
            torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(0)]->vbaixar = true;
        else
            torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(0)]->vbaixar = false;
    }
    if(vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].size() > 1){
        if(ui->checkTorrent01->isChecked())
            torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(1)]->vbaixar = true;
        else
            torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(1)]->vbaixar = false;
    }
    if(vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].size() > 2){
        if(ui->checkTorrent02->isChecked())
            torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(2)]->vbaixar = true;
        else
            torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(2)]->vbaixar = false;
    }
    if(vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].size() > 3){
        if(ui->checkTorrent03->isChecked())
            torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(3)]->vbaixar = true;
        else
            torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(3)]->vbaixar = false;
    }
    if(vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].size() > 4){
        if(ui->checkTorrent04->isChecked())
            torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(4)]->vbaixar = true;
        else
            torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(4)]->vbaixar = false;
    }
    if(vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].size() > 5){
        if(ui->checkTorrent05->isChecked())
            torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(5)]->vbaixar = true;
        else
            torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(5)]->vbaixar = false;
    }
    if(vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].size() > 6){
        if(ui->checkTorrent06->isChecked())
            torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(6)]->vbaixar = true;
        else
            torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(6)]->vbaixar = false;
    }
    if(vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].size() > 7){
        if(ui->checkTorrent07->isChecked())
            torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(7)]->vbaixar = true;
        else
            torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(7)]->vbaixar = false;
    }
    if(vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].size() > 8){
        if(ui->checkTorrent08->isChecked())
            torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(8)]->vbaixar = true;
        else
            torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(8)]->vbaixar = false;
    }
    if(vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].size() > 9){
        if(ui->checkTorrent09->isChecked())
            torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(9)]->vbaixar = true;
        else
            torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(9)]->vbaixar = false;
    }
    fConstroiListaTorrents();
}

void TorrentTab::fhideTudo()
{
    ui->checkTorrent00->hide();
    ui->qualidadeTorrent00->hide();
    ui->episodioTorrent00->hide();
    ui->nomeTorrent00->hide();
    ui->descricaoTorrent00->hide();
    ui->checkTorrent01->hide();
    ui->qualidadeTorrent01->hide();
    ui->episodioTorrent01->hide();
    ui->nomeTorrent01->hide();
    ui->descricaoTorrent01->hide();
    ui->checkTorrent02->hide();
    ui->qualidadeTorrent02->hide();
    ui->episodioTorrent02->hide();
    ui->nomeTorrent02->hide();
    ui->descricaoTorrent02->hide();
    ui->checkTorrent03->hide();
    ui->qualidadeTorrent03->hide();
    ui->episodioTorrent03->hide();
    ui->nomeTorrent03->hide();
    ui->descricaoTorrent03->hide();
    ui->checkTorrent04->hide();
    ui->qualidadeTorrent04->hide();
    ui->episodioTorrent04->hide();
    ui->nomeTorrent04->hide();
    ui->descricaoTorrent04->hide();
    ui->checkTorrent05->hide();
    ui->qualidadeTorrent05->hide();
    ui->episodioTorrent05->hide();
    ui->nomeTorrent05->hide();
    ui->descricaoTorrent05->hide();
    ui->checkTorrent06->hide();
    ui->qualidadeTorrent06->hide();
    ui->episodioTorrent06->hide();
    ui->nomeTorrent06->hide();
    ui->descricaoTorrent06->hide();
    ui->checkTorrent07->hide();
    ui->qualidadeTorrent07->hide();
    ui->episodioTorrent07->hide();
    ui->nomeTorrent07->hide();
    ui->descricaoTorrent07->hide();
    ui->checkTorrent08->hide();
    ui->qualidadeTorrent08->hide();
    ui->episodioTorrent08->hide();
    ui->nomeTorrent08->hide();
    ui->descricaoTorrent08->hide();
    ui->checkTorrent09->hide();
    ui->qualidadeTorrent09->hide();
    ui->episodioTorrent09->hide();
    ui->nomeTorrent09->hide();
    ui->descricaoTorrent09->hide();
}

int TorrentTab::fcalculaPrioridadeNome(QString ridAnime, QString repisodioAnime,
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

int TorrentTab::fcalculaPrioridadeSub(QString rfansub, QString rid)
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

int TorrentTab::fcalculaPrioridadeQualidade(QString resolucao, QString id)
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

int TorrentTab::fcalculaPrioridadeFiltros(QString description, QString rid)
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

void TorrentTab::on_carregaLista_clicked()
{
    fbaixaLista();
}

void TorrentTab::on_botaoProximaPagina_clicked()
{
    qDebug() << "Trying to go to page " << vpagina+1 << " from " << vpagina;
    if(vlistaDeIDs.size() > 12+(12*(vpagina-1))){
        vpagina++;
        ui->NumPagina->setText("Torrents - " + QString::number(vlistaDeIDs.size()) +
                               " animes for download - Page "+QString::number(vpagina)+"/"+
                               QString::number(((vlistaDeIDs.size()-1)/12)+1));
        fConstroiListaTorrents();
    }
}

void TorrentTab::on_botaoPaginaAnterior_clicked()
{
    qDebug() << "Trying to go to page " << vpagina-1 << " from " << vpagina;
    if(vpagina > 1){
        vpagina--;
        ui->NumPagina->setText("Torrents - " + QString::number(vlistaDeIDs.size()) +
                               " animes for download - Page "+QString::number(vpagina)+"/"+
                               QString::number(((vlistaDeIDs.size()-1)/12)+1));
        fConstroiListaTorrents();
    }
}

void TorrentTab::on_botaoAnime09_clicked()
{
    vtorrentSelecionado = 9+(12*(vpagina-1));
    fmostraTorrent();
}

void TorrentTab::on_botaoAnime00_clicked()
{
    vtorrentSelecionado = 0+(12*(vpagina-1));
    fmostraTorrent();
}

void TorrentTab::on_botaoAnime01_clicked()
{
    vtorrentSelecionado = 1+(12*(vpagina-1));
    fmostraTorrent();
}

void TorrentTab::on_botaoAnime02_clicked()
{
    vtorrentSelecionado = 2+(12*(vpagina-1));
    fmostraTorrent();
}

void TorrentTab::on_botaoAnime03_clicked()
{
    vtorrentSelecionado = 3+(12*(vpagina-1));
    fmostraTorrent();
}

void TorrentTab::on_botaoAnime04_clicked()
{
    vtorrentSelecionado = 4+(12*(vpagina-1));
    fmostraTorrent();
}

void TorrentTab::on_botaoAnime05_clicked()
{
    vtorrentSelecionado = 5+(12*(vpagina-1));
    fmostraTorrent();
}

void TorrentTab::on_botaoAnime06_clicked()
{
    vtorrentSelecionado = 6+(12*(vpagina-1));
    fmostraTorrent();
}

void TorrentTab::on_botaoAnime07_clicked()
{
    vtorrentSelecionado = 7+(12*(vpagina-1));
    fmostraTorrent();
}

void TorrentTab::on_botaoAnime08_clicked()
{
    vtorrentSelecionado = 8+(12*(vpagina-1));
    fmostraTorrent();
}

void TorrentTab::on_botaoAnime10_clicked()
{
    vtorrentSelecionado = 10+(12*(vpagina-1));
    fmostraTorrent();
}

void TorrentTab::on_botaoAnime11_clicked()
{
    vtorrentSelecionado = 11+(12*(vpagina-1));
    fmostraTorrent();
}

void TorrentTab::on_botaoBaixar_clicked()
{
    QProcess lprocesso;
    if(cconfig->fretornaTorrentEscolhido() == "uTorrent")
        lprocesso.setProgram(QDir::homePath() + "/AppData/Roaming/uTorrent/uTorrent.exe");
    else if(cconfig->fretornaTorrentEscolhido() == "qBittorrent")
        lprocesso.setProgram(QDir::rootPath() + "/Program Files/qBittorrent/qbittorrent.exe");

    lprocesso.startDetached();

    for(int i = 0; i < torrent.size(); i++){
        if(torrent[i]->vbaixar == true){
            vlistaDownload.append(i);
            QPointer<filedownloader> qdown(new filedownloader);
            qdown->fdownloadTorrent(torrent[i]->vlinkTorrent, torrent[i]->vnomeTorrent);
            torrent[i]->vbox.setCheckState(Qt::Unchecked);
            connect(qdown, &filedownloader::storrent, this, &TorrentTab::fesperaTorrent);
        }
    }
}

void TorrentTab::fesperaTorrent()
{
    QTimer lespera5Segundos;
    lespera5Segundos.singleShot(5000, this, &TorrentTab::fbaixaTorrent);
}

void TorrentTab::fbaixaTorrent()
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

void TorrentTab::on_botaoSelectSubForTorrent_clicked()
{
    QString lsub = ui->listaSubs->currentText();
    QString id = vlistaDeIDs[vtorrentSelecionado];
    cconfig->fselectSubFromTorrent(id,lsub);
}

void TorrentTab::on_botaoInfoAnime_clicked()
{
    emit infoAnime(torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(0)]->vnomeAnime);
}

void TorrentTab::on_botaoLinkTorrent_clicked()
{
    QString llink = torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(0)]->vlinkTorrent;
    QDesktopServices::openUrl(QUrl(llink,QUrl::TolerantMode));
}

void TorrentTab::on_botaoSearchThisanime_clicked()
{
    QString lnome = torrent[vHashDeIdEPosicaoDoTorrent[vlistaDeIDs[vtorrentSelecionado]].at(0)]->vnomeAnime;
    fprocuraAnimeEspecifico(lnome);
}

void TorrentTab::fprocuraAnimeEspecifico(QString rnomeAnimeBuscado)
{
    fblockSignals();
    vlistaDeIDs.clear();
    vbaixar.clear();
    vlistaDownload.clear();
    if(!torrent.isEmpty())
        torrent.clear();
    QPointer<filedownloader> lbaixaXML(new filedownloader);
    QString lfeedEspecifico = cconfig->fretornaFeedAnimeEspecifico();
    lfeedEspecifico.replace("%title%", rnomeAnimeBuscado);
    lbaixaXML->fdownloadXMLTorrentList(lfeedEspecifico);
    connect(lbaixaXML, &filedownloader::sxml, this, &TorrentTab::fleXML);
}

void TorrentTab::fbaixaLista()
{
    fblockSignals();
    vlistaDeIDs.clear();
    vbaixar.clear();
    vlistaDownload.clear();
    if(!torrent.isEmpty())
        torrent.clear();
    QPointer<filedownloader> lbaixaXML(new filedownloader);
    lbaixaXML->fdownloadXMLTorrentList(cconfig->fretornaFeedAnime());
    connect(lbaixaXML, &filedownloader::sxml, this, &TorrentTab::fleXML);
}

void TorrentTab::fblockSignals()
{
    QList<QPushButton *> listaBotoes = findChildren<QPushButton *>();
    foreach(QPushButton *psh, listaBotoes){
        psh->blockSignals(true);
    }
}

void TorrentTab::funblockSignals()
{
    QList<QPushButton *> listaBotoes = findChildren<QPushButton *>();
    foreach(QPushButton *psh, listaBotoes){
        psh->blockSignals(false);
    }
}

