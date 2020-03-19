#include "janeladeconfig.h"
#include "ui_janeladeconfig.h"

janeladeconfig::janeladeconfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::janeladeconfig)
{
    ui->setupUi(this);
    ui->textoSalvaTorrents->setMaximumBlockCount(1);
    ui->textoTempoDownloadAutomatico->setMaximumBlockCount(1);
    ui->textoUser->setMaximumBlockCount(1);
    ui->textoCodigoAutorizacao->setMaximumBlockCount(1);
    ui->textoTorrentFeedEspecifico->setMaximumBlockCount(1);
    ui->textoTorrentFeed->setMaximumBlockCount(1);
    ui->textoSubPreferido->setMaximumBlockCount(1);
    ui->textoCodigoAutorizacao->hide();
    ui->labelCodigoAutenticar->hide();
    ui->botaoSalvarCodigo->hide();
    ui->boxReconhecimento01->setCheckState(Qt::Checked);
    ui->boxReconhecimento02->setCheckState(Qt::Checked);
    ui->boxReconhecimento03->setCheckState(Qt::Checked);
    ui->boxReconhecimento04->setCheckState(Qt::Checked);
    ui->boxReconhecimento05->setCheckState(Qt::Checked);
    ui->boxReconhecimento06->setCheckState(Qt::Checked);
    ui->boxReconhecimento07->setCheckState(Qt::Checked);
    ui->boxReconhecimento08->setCheckState(Qt::Checked);
    ui->boxReconhecimento09->setCheckState(Qt::Checked);
    ui->boxReconhecimento10->setCheckState(Qt::Checked);
    ui->boxReconhecimento11->setCheckState(Qt::Checked);
    ui->boxReconhecimento12->setCheckState(Qt::Checked);
    ui->boxReconhecimento13->setCheckState(Qt::Checked);
    ui->boxReconhecimento14->setCheckState(Qt::Checked);
    ui->boxReconhecimento15->setCheckState(Qt::Checked);
    ui->boxReconhecimento16->setCheckState(Qt::Checked);
    ui->boxReconhecimento17->setCheckState(Qt::Checked);
    ui->boxReconhecimento18->setCheckState(Qt::Checked);
    ui->radioNoQualidade->setChecked(true);
    ui->radioNoDownload->setChecked(true);
    ui->checkStream->setCheckState(Qt::Checked);
    ui->checkMediaPlayers->setCheckState(Qt::Checked);
    ui->tabelaAnimes->setSortingEnabled(true);
    ui->tabelaAnimes->setColumnHidden(1,true);
    ui->tabelaAnimes->setColumnHidden(2,true);
    ui->tabelaAnimes->setColumnHidden(3,true);
    ui->tabelaAnimes->setColumnHidden(4,true);
    ui->tabelaAnimes->setColumnHidden(5,true);
    ui->tabelaAnimes->setColumnHidden(6,true);
    ui->tabelaAnimes->setColumnHidden(7,true);
    ui->tabelaAnimesSelecionados->setColumnHidden(1,true);
    ui->tabelaAnimesSelecionados->setColumnHidden(2,true);
    ui->tabelaAnimesSelecionados->setColumnHidden(3,true);
    ui->tabelaAnimesSelecionados->setColumnHidden(4,true);
    ui->tabelaAnimesSelecionados->setColumnHidden(5,true);
    ui->tabelaAnimesSelecionados->setColumnHidden(6,true);
    ui->tabelaAnimesSelecionados->setColumnHidden(7,true);
    ui->tabelaFiltros->setColumnHidden(0,true);
    ui->tabelaAnimes->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tabelaAnimesSelecionados->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tabelaFiltros->setSelectionBehavior(QAbstractItemView::SelectRows);
    fleArquivoConfig();
}

janeladeconfig::~janeladeconfig()
{
    delete ui;
}

void janeladeconfig::fleArquivoConfig(){
    QFile larquivo("Configurações/userConfig.txt");
    if(larquivo.size() == 0)
        return;
    if(larquivo.open(QIODevice::ReadOnly)){
        while(!larquivo.atEnd()){
            QString lstreamTexto = larquivo.readLine();
            if(lstreamTexto.contains("User:")){
                lstreamTexto.remove("User:");
                vuser = lstreamTexto.trimmed();
                ui->textoUser->setPlainText(vuser);
            }
            else if(lstreamTexto.contains("Directory;")){
                lstreamTexto.remove("Directory;");
                vdirectory = lstreamTexto.trimmed().split(";");
                for(int i = 0; i < vdirectory.size(); i++){
                    ui->tabelaDiretorios->addItem(vdirectory.at(i));
                }
            }
            else if(lstreamTexto.contains("Authorization:")){
                lstreamTexto.remove("Authorization:");
                vauthorization = lstreamTexto.trimmed();
                ui->textoCodigoAutorizacao->setPlainText(vauthorization);
            }
            else if(lstreamTexto.contains("Detection:")){
                lstreamTexto.remove("Detection:");
                vdetection = lstreamTexto;
                if(lstreamTexto.contains("m"))
                    ui->checkMediaPlayers->setCheckState(Qt::Checked);
                if(lstreamTexto.contains("s"))
                    ui->checkStream->setCheckState(Qt::Checked);
            }
            else if(lstreamTexto.contains("LowQuality:")){
                lstreamTexto.remove("LowQuality:");
                if(lstreamTexto.contains("yes")){
                    vlowQuality = true;
                    ui->radioYesQualidade->setChecked(true);
                }
                else
                    vlowQuality = false;
            }
            else if(lstreamTexto.contains("TorrentSoftware:")){
                lstreamTexto.remove("TorrentSoftware:");
                vtorrentSoftware = lstreamTexto.trimmed();
                ui->boxTorrentEscolhido->setCurrentText(lstreamTexto);
            }
            else if(lstreamTexto.contains("AutoDownload:")){
                lstreamTexto.remove("AutoDownload:");
                if(lstreamTexto.contains("yes")){
                    ui->radioYesDownload->setChecked(true);
                    vautoDownload = true;
                }
                else
                    vautoDownload = false;
            }
            else if(lstreamTexto.contains("SaveFolder:")){
                lstreamTexto.remove("SaveFolder:");
                vsaveFolder = lstreamTexto.trimmed();
                ui->textoSalvaTorrents->setPlainText(vsaveFolder);
            }
            else if(lstreamTexto.contains("DownloadListas:")){
                lstreamTexto.remove("DownloadListas:");
                vdownloadListas = lstreamTexto;
                if(lstreamTexto.contains("w"))
                    ui->checkWatching->setCheckState(Qt::Checked);
                if(lstreamTexto.contains("p"))
                    ui->checkPlanToWatch->setCheckState(Qt::Checked);
                if(lstreamTexto.contains("d"))
                    ui->checkDropped->setCheckState(Qt::Checked);
                if(lstreamTexto.contains("o"))
                    ui->checkOnHold->setCheckState(Qt::Checked);
            }
            else if(lstreamTexto.contains("Sub:")){
                lstreamTexto.remove("Sub:");
                vsub = lstreamTexto.trimmed();
                ui->textoSubPreferido->setPlainText(vsub);
            }
            else if(lstreamTexto.contains("Quality:")){
                lstreamTexto.remove("Quality:");
                vquality = lstreamTexto.trimmed();
                ui->boxQualidadeEscolhida->setCurrentText(lstreamTexto);
            }
            else if(lstreamTexto.contains("TorrentFeed")){
                lstreamTexto.remove("TorrentFeed:");
                vtorrentFeed = lstreamTexto.trimmed();
                ui->textoTorrentFeed->setPlainText(vtorrentFeed);
            }
            else if(lstreamTexto.contains("AnimeFeed")){
                lstreamTexto.remove("AnimeFeed:");
                vanimeTorrentFeed = lstreamTexto.trimmed();
                ui->textoTorrentFeedEspecifico->setPlainText(vanimeTorrentFeed);
            }
            else if(lstreamTexto.contains("Players")){
                lstreamTexto.remove("Players:");
                vPlayers = lstreamTexto.split(";");
                if(vPlayers.contains("Crunchyroll"))
                    ui->boxReconhecimento01->setCheckState(Qt::Checked);
                if(vPlayers.contains("AnimeLab"))
                    ui->boxReconhecimento02->setCheckState(Qt::Checked);
                if(vPlayers.contains("Funimation"))
                    ui->boxReconhecimento03->setCheckState(Qt::Checked);
                if(vPlayers.contains("Hulu"))
                    ui->boxReconhecimento04->setCheckState(Qt::Checked);
                if(vPlayers.contains("VRV"))
                    ui->boxReconhecimento05->setCheckState(Qt::Checked);
                if(vPlayers.contains("Netflix"))
                    ui->boxReconhecimento06->setCheckState(Qt::Checked);
                if(vPlayers.contains("Baka MPlayer"))
                    ui->boxReconhecimento07->setCheckState(Qt::Checked);
                if(vPlayers.contains("BS.Player"))
                    ui->boxReconhecimento08->setCheckState(Qt::Checked);
                if(vPlayers.contains("DivX Player"))
                    ui->boxReconhecimento09->setCheckState(Qt::Checked);
                if(vPlayers.contains("Media Player Classic"))
                    ui->boxReconhecimento10->setCheckState(Qt::Checked);
                if(vPlayers.contains("mpv"))
                    ui->boxReconhecimento11->setCheckState(Qt::Checked);
                if(vPlayers.contains("VLC media player"))
                    ui->boxReconhecimento12->setCheckState(Qt::Checked);
                if(vPlayers.contains("WebTorrent Desktop"))
                    ui->boxReconhecimento13->setCheckState(Qt::Checked);
                if(vPlayers.contains("Winamp"))
                    ui->boxReconhecimento14->setCheckState(Qt::Checked);
                if(vPlayers.contains("Windows Media Player"))
                    ui->boxReconhecimento15->setCheckState(Qt::Checked);
                if(vPlayers.contains("x"))
                    ui->boxReconhecimento16->setCheckState(Qt::Checked);
                if(vPlayers.contains("z"))
                    ui->boxReconhecimento17->setCheckState(Qt::Checked);
                if(vPlayers.contains("w"))
                    ui->boxReconhecimento18->setCheckState(Qt::Checked);
            }
        }
        larquivo.close();
    }
    fupdateFiltros();
    ui->textoCodigoAutorizacao->setPlainText(vauthorization);
}

void janeladeconfig::frecebeListasAnimes(leitorlistaanimes *lleitor)
{
    cleitor = lleitor;
}

void janeladeconfig::fgetAnimeLists()
{
    vlistaAtual = cleitor->retornaListaWatching();
    vlistaAtual.append(cleitor->retornaListaCompleted());
    vlistaAtual.append(cleitor->retornaListaOnHold());
    vlistaAtual.append(cleitor->retornaListaDropped());
    vlistaAtual.append(cleitor->retornaListaPlanToWatch());
}

void janeladeconfig::flimpaFiltros()
{
    int rowcount = ui->tabelaAnimesSelecionados->rowCount();
    if(rowcount != 0)
        ui->tabelaAnimesSelecionados->selectRow(0);
    for(int i = 0; i < rowcount; i++){
        on_botaoRemoverAnime_clicked();
    }
    ui->boxFiltroEspecifico->clear();
    ui->boxNomeFiltro->clear();
    ui->boxListaFiltros->setCurrentIndex(0);
}

QByteArray janeladeconfig::fretornaUsuario(){
    return ui->textoUser->toPlainText().toLocal8Bit();
}

QStringList janeladeconfig::fretornaDiretorios()
{
    return vdirectory;
}

QByteArray janeladeconfig::fretornaCodigoAutorizacao()
{
    if(vauthorization.isEmpty() || vauthorization != ui->textoCodigoAutorizacao->toPlainText())
        return ui->textoCodigoAutorizacao->toPlainText().toLocal8Bit();
    return vauthorization.toLocal8Bit();
}

QByteArray janeladeconfig::fretornaDeteccao()
{
    QByteArray ldetec;
    if(ui->checkMediaPlayers->isChecked())
        ldetec.append("m");
    if(ui->checkStream->isChecked())
        ldetec.append("s");
    return ldetec;
}

QByteArray janeladeconfig::fretornaBaixaQualidade()
{
    if(ui->radioYesQualidade->isChecked())
        return "yes";
    return "no";
}

QByteArray janeladeconfig::fretornaTorrentEscolhido()
{
    return ui->boxTorrentEscolhido->currentText().toLocal8Bit();
}

QByteArray janeladeconfig::fretornaDownloadAutomatico()
{
    if(ui->radioYesDownload->isChecked())
        return "yes";
    return "no";
}

QByteArray janeladeconfig::fretornaTempoDownload()
{
    return ui->textoTempoDownloadAutomatico->toPlainText().toLocal8Bit();
}

QByteArray janeladeconfig::fretornaPastaSalvarAnimes()
{
    return ui->textoSalvaTorrents->toPlainText().toLocal8Bit();
}

QByteArray janeladeconfig::fretornaDownloadListasAnimes()
{
    QByteArray llista;
    if(ui->checkWatching->isChecked())
        llista.append("w");
    if(ui->checkPlanToWatch->isChecked())
        llista.append("p");
    if(ui->checkOnHold->isChecked())
        llista.append("o");
    if(ui->checkDropped->isChecked())
        llista.append("d");
    return llista;
}

QByteArray janeladeconfig::fretornaSubEscolhido()
{
    return ui->textoSubPreferido->toPlainText().toLocal8Bit().trimmed();
}

QByteArray janeladeconfig::fretornaQualidadeEscolhida()
{
    return ui->boxQualidadeEscolhida->currentText().toLocal8Bit();
}

QByteArray janeladeconfig::fretornaFeedAnime()
{
    return ui->textoTorrentFeed->toPlainText().toLocal8Bit();
}

QByteArray janeladeconfig::fretornaFeedAnimeEspecifico()
{
    return ui->textoTorrentFeedEspecifico->toPlainText().toLocal8Bit();
}

QByteArray janeladeconfig::fretornaFiltrosDownload()
{
    return "oi";
}

QStringList janeladeconfig::fretornaPlayers()
{
    vPlayers.clear();
    if(ui->boxReconhecimento01->isChecked())
        vPlayers.append("Crunchyroll");
    if(ui->boxReconhecimento02->isChecked())
        vPlayers.append("AnimeLab");
    if(ui->boxReconhecimento03->isChecked())
        vPlayers.append("Funimation");
    if(ui->boxReconhecimento04->isChecked())
        vPlayers.append("Hulu");
    if(ui->boxReconhecimento05->isChecked())
        vPlayers.append("VRV");
    if(ui->boxReconhecimento06->isChecked())
        vPlayers.append("Netflix");
    if(ui->boxReconhecimento07->isChecked())
        vPlayers.append("Baka MPlayer");
    if(ui->boxReconhecimento08->isChecked())
        vPlayers.append("BS.Player");
    if(ui->boxReconhecimento09->isChecked())
        vPlayers.append("DivX Player");
    if(ui->boxReconhecimento10->isChecked())
        vPlayers.append("Media Player Classic");
    if(ui->boxReconhecimento11->isChecked())
        vPlayers.append("mpv");
    if(ui->boxReconhecimento12->isChecked())
        vPlayers.append("VLC media player");
    if(ui->boxReconhecimento13->isChecked())
        vPlayers.append("WebTorrent Desktop");
    if(ui->boxReconhecimento14->isChecked())
        vPlayers.append("Winamp");
    if(ui->boxReconhecimento15->isChecked())
        vPlayers.append("Windows Media Player");
    if(ui->boxReconhecimento16->isChecked())
        vPlayers.append("x");
    if(ui->boxReconhecimento17->isChecked())
        vPlayers.append("z");
    if(ui->boxReconhecimento18->isChecked())
        vPlayers.append("w");
    return vPlayers;
}

void janeladeconfig::on_botaoSalvar_clicked()
{
//        qEncode(fretornaUsuario());///Encode de alguma maneira
    QFile larquivo("Configurações/userConfig.txt");
    if(larquivo.open(QIODevice::WriteOnly)){
        QTextStream lstreamTexto(&larquivo);
        lstreamTexto << "User:" << fretornaUsuario() << endl;
        lstreamTexto << "Directory";
        for(int i = 0; i < vdirectory.size(); i++){
             lstreamTexto << ";" << fretornaDiretorios().at(i);
        }
        lstreamTexto << endl;
        lstreamTexto << "Authorization:" << fretornaCodigoAutorizacao() << endl;
        lstreamTexto << "Detection:" << fretornaDeteccao() << endl;
        lstreamTexto << "LowQuality:" << fretornaBaixaQualidade() << endl;
        lstreamTexto << "TorrentSoftware:" << fretornaTorrentEscolhido() << endl;
        lstreamTexto << "AutoDownload:" << fretornaDownloadAutomatico() << endl;
        lstreamTexto << "SaveFolder:" << fretornaPastaSalvarAnimes() << endl;
        lstreamTexto << "DownloadListas:" << fretornaDownloadListasAnimes() << endl;
        lstreamTexto << "Sub:" << fretornaSubEscolhido() << endl;
        lstreamTexto << "Quality:" << fretornaQualidadeEscolhida() << endl;
        lstreamTexto << "TorrentFeed:" << fretornaFeedAnime() << endl;
        lstreamTexto << "AnimeFeed:" << fretornaFeedAnimeEspecifico() << endl;
        lstreamTexto << "Players";
        for(int i = 0; i < vPlayers.size(); i++){
            if(!vPlayers.at(i).isEmpty())
                lstreamTexto << ";" << vPlayers.at(i);
        }
        larquivo.close();
    }
    QFile larquivoFiltros("Configurações/animeFilters.txt");
    if(larquivo.open(QIODevice::WriteOnly)){
        QTextStream lstreamTexto(&larquivo);
        for(int i = 0; i < vfiltrosAnimes.size(); i++){
            lstreamTexto << vfiltrosAnimes[i]->nome + ";";
            lstreamTexto << vfiltrosAnimes[i]->filtroDeCondicao + ";";
            lstreamTexto << vfiltrosAnimes[i]->filtroEspecifico + ";";
            if(vfiltrosAnimes[i]->Ativo)
                lstreamTexto << "true;";
            else
                lstreamTexto << "false;";
            for(int w = 0; w < vfiltrosAnimes[i]->idAnimesAfetados.size(); w++){
                lstreamTexto << vfiltrosAnimes[i]->idAnimesAfetados[w] + ";";
            }
            lstreamTexto << endl;
        }
        larquivo.close();
    }
    emit ssavebutton();
}

void janeladeconfig::on_botaoAdicionar_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(dir != ""){
        vdirectory.append(dir);
        ui->tabelaDiretorios->addItem(dir);
    }
}

void janeladeconfig::on_botaoRemover_clicked()
{
    if(!ui->tabelaDiretorios->selectedItems().isEmpty())
        qDeleteAll(ui->tabelaDiretorios->selectedItems());
}

void janeladeconfig::on_botaoSelecionarPastaDownload_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(dir != ""){
        ui->textoSalvaTorrents->setPlainText(dir);
        vsaveFolder = dir;
    }
}

void janeladeconfig::on_botaoAutorizarUser_clicked()
{
    QDesktopServices::openUrl(QUrl("https://anilist.co/api/v2/oauth/authorize?client_id=2620&response_type=token",QUrl::TolerantMode));
    vuser = ui->textoUser->toPlainText();
    ui->textoCodigoAutorizacao->show();
    ui->labelCodigoAutenticar->show();
    ui->botaoSalvarCodigo->show();
}

void janeladeconfig::on_botaoSalvarCodigo_clicked()
{
    vauthorization = ui->textoCodigoAutorizacao->toPlainText();
    ui->textoCodigoAutorizacao->hide();
    ui->labelCodigoAutenticar->hide();
    ui->botaoSalvarCodigo->hide();
    emit sauthcodesave();
}

void janeladeconfig::on_botaoWatching_clicked()
{
    ui->tabelaAnimes->sortByColumn(2);

}

void janeladeconfig::on_botaoCompleted_clicked()
{
    ui->tabelaAnimes->sortByColumn(3);
}

void janeladeconfig::on_botaoOnHold_clicked()
{
    ui->tabelaAnimes->sortByColumn(4);
}

void janeladeconfig::on_botaoDropped_clicked()
{
    ui->tabelaAnimes->sortByColumn(5);
}

void janeladeconfig::on_botaoPlanToWatch_clicked()
{
    ui->tabelaAnimes->sortByColumn(6);
}

void janeladeconfig::on_botaoFiltroAdd_clicked()
{
    QTableWidgetItem *testList = nullptr;
    fgetAnimeLists();
    flimpaFiltros();
    if(vlistaAtual.size() - ui->tabelaAnimes->rowCount() > 0){
        ui->tabelaAnimes->setRowCount(vlistaAtual.size());
        for(int i = 0; i < vlistaAtual.size(); i++){
            testList = ui->tabelaAnimes->item(i,1);
            if(testList != nullptr){
                for(int w = 0; w < 7; w++){
                     switch (w) {
                     case 0:
                         ui->tabelaAnimes->item(i,w)->setText(vlistaAtual[i]->vnome);
                     break;
                     case 1:
                         ui->tabelaAnimes->item(i,w)->setText(vlistaAtual[i]->vid);
                     break;
                     case 2:
                        if(vlistaAtual[i]->vlista.compare("Watching") == 0)
                            ui->tabelaAnimes->item(i,w)->setText("0");
                        else
                            ui->tabelaAnimes->item(i,w)->setText("1");
                     break;
                     case 3:
                         if(vlistaAtual[i]->vlista.compare("Completed") == 0)
                             ui->tabelaAnimes->item(i,w)->setText("0");
                         else
                             ui->tabelaAnimes->item(i,w)->setText("1");
                     break;
                     case 4:
                         if(vlistaAtual[i]->vlista.compare("On Hold") == 0)
                             ui->tabelaAnimes->item(i,w)->setText("0");
                         else
                             ui->tabelaAnimes->item(i,w)->setText("1");
                     break;
                     case 5:
                         if(vlistaAtual[i]->vlista.compare("Dropped") == 0)
                             ui->tabelaAnimes->item(i,w)->setText("0");
                         else
                             ui->tabelaAnimes->item(i,w)->setText("1");
                     break;
                     case 6:
                         if(vlistaAtual[i]->vlista.compare("Plan to Watch") == 0)
                             ui->tabelaAnimes->item(i,w)->setText("0");
                         else
                             ui->tabelaAnimes->item(i,w)->setText("1");
                     break;
                     }
                }
            }
            else{
                for(int w = 0; w < 7; w++){
                    QTableWidgetItem *litem = new QTableWidgetItem;
                    switch (w) {
                    case 0:
                        litem->setText(vlistaAtual[i]->vnome);
                        ui->tabelaAnimes->setItem(i,w,litem);
                    break;
                    case 1:
                        litem->setText(vlistaAtual[i]->vid);
                        ui->tabelaAnimes->setItem(i,w, litem);
                    break;
                    case 2:
                        if(vlistaAtual[i]->vlista.compare("Watching") == 0)
                            litem->setText("0");
                        else
                            litem->setText("1");
                        ui->tabelaAnimes->setItem(i,w, litem);
                    break;
                    case 3:
                        if(vlistaAtual[i]->vlista.compare("Completed") == 0)
                            litem->setText("0");
                        else
                            litem->setText("1");
                        ui->tabelaAnimes->setItem(i,w, litem);
                    break;
                    case 4:
                        if(vlistaAtual[i]->vlista.compare("On Hold") == 0)
                            litem->setText("0");
                        else
                            litem->setText("1");
                        ui->tabelaAnimes->setItem(i,w, litem);
                    break;
                    case 5:
                        if(vlistaAtual[i]->vlista.compare("Dropped") == 0)
                            litem->setText("0");
                        else
                            litem->setText("1");
                        ui->tabelaAnimes->setItem(i,w, litem);
                    break;
                    case 6:
                        if(vlistaAtual[i]->vlista.compare("Plan to Watch") == 0)
                            litem->setText("0");
                        else
                            litem->setText("1");
                        ui->tabelaAnimes->setItem(i,w, litem);
                    break;
                    }
                }
            }
        }
    }
    ui->tabelaAnimes->sortItems(0);
    delete testList;
}

void janeladeconfig::on_botaoFiltroRemove_clicked()
{
    QModelIndexList select = ui->tabelaFiltros->selectionModel()->selectedRows();
    ui->tabelaFiltros->removeRow(select.at(0).row());
    vfiltrosAnimes.remove(select.at(0).row());
}


void janeladeconfig::fupdateFiltros()
{
    QFile larquivo("Configurações/animeFilters.txt");
    if(larquivo.size() == 0)
        return;
    if(larquivo.open(QIODevice::ReadOnly)){
        while(!larquivo.atEnd()){
            QString lstreamTexto = larquivo.readLine();
            QStringList filtro = lstreamTexto.split(";");
            QPointer<Filtros> f(new Filtros);
            f->nome = filtro.at(0);
            f->filtroDeCondicao = filtro.at(1);
            f->filtroEspecifico = filtro.at(2);
            if(filtro.at(3) == "true")
                f->Ativo = true;
            else
                f->Ativo = false;
            if(filtro.size() >= 5){
                for(int i = 4; i < filtro.size(); i++){
                    f->idAnimesAfetados.append(filtro[i]);
                }
            }
            vfiltrosAnimes.append(f);
        }
        larquivo.close();
    }
    fupdateTabelaFiltros();
}

void janeladeconfig::fupdateTabelaFiltros()
{
    ui->tabelaFiltros->setRowCount(vfiltrosAnimes.size());
    for(int i = 0; i < vfiltrosAnimes.size(); i++){
        for(int w = 0; w < ui->tabelaFiltros->columnCount(); w++){
            QTableWidgetItem *litem = new QTableWidgetItem;
            switch (w) {
            case 0:
                litem->setText(QString::number(i));
                ui->tabelaFiltros->setItem(i,w,litem);
            break;
            case 1:
                if(!vfiltrosAnimes[i]->Ativo)
                    litem->setTextColor("red");
                else
                    litem->setTextColor("black");
                litem->setText(vfiltrosAnimes[i]->nome);
                ui->tabelaFiltros->setItem(i,w,litem);
            break;
            }
        }
    }
    ui->tabelaFiltros->sortItems(0);
}

//Limpa tabela e salva tudo num ponteiro, adicionando na tabela de filtros
void janeladeconfig::on_botaoOk_clicked()
{
    QPointer<Filtros> f(new Filtros);
    qDebug() << ui->tabelaAnimesSelecionados->rowCount();
    QModelIndexList select = ui->tabelaAnimesSelecionados->selectionModel()->selectedRows();
    for(int i = 0; i < ui->tabelaAnimesSelecionados->rowCount(); i++){
        f->idAnimesAfetados.append(ui->tabelaAnimesSelecionados->item(i,1)->text());
    }
    f->nome = ui->boxNomeFiltro->toPlainText();
    f->filtroDeCondicao = ui->boxListaFiltros->currentText();
    f->filtroEspecifico = ui->boxFiltroEspecifico->toPlainText();
    f->Ativo = true;
    vfiltrosAnimes.append(f);
    flimpaFiltros();
    fupdateTabelaFiltros();
}

void janeladeconfig::on_botaoCheck_clicked()
{
    flimpaFiltros();
    QModelIndexList select = ui->tabelaFiltros->selectionModel()->selectedRows();
    int index = ui->tabelaFiltros->item(select.at(0).row(),0)->text().toInt();
    for(int i = 0; i < ui->boxListaFiltros->count(); i++){
       if(ui->boxListaFiltros->itemText(i).compare(vfiltrosAnimes[index]->filtroDeCondicao) == 0){
           ui->boxListaFiltros->setCurrentIndex(i);
           break;
       }
    }
    ui->boxNomeFiltro->setPlainText(vfiltrosAnimes[index]->nome);
    ui->boxFiltroEspecifico->setPlainText(vfiltrosAnimes[index]->filtroEspecifico);
    QModelIndexList selectAnimes = ui->tabelaAnimes->selectionModel()->selectedRows();
    ui->tabelaAnimes->selectRow(0);
    for(int w = 0; w < vfiltrosAnimes[index]->idAnimesAfetados.size(); w++){
        for(int i = 0; i < ui->tabelaAnimes->rowCount(); i++){
           qDebug() << ui->tabelaAnimes->item(i,1)->text();
           if(ui->tabelaAnimes->item(i,1)->text().compare(vfiltrosAnimes[index]->idAnimesAfetados[w]) == 0){
               ui->tabelaAnimes->selectRow(i);
               on_botaoInserirAnime_clicked();
               break;
           }
        }
    }
}

void janeladeconfig::on_botaoInserirAnime_clicked()
{
    if(ui->tabelaAnimes->selectionModel()->selectedRows().isEmpty())
        return;
    QModelIndexList select = ui->tabelaAnimes->selectionModel()->selectedRows();
    ui->tabelaAnimesSelecionados->insertRow(ui->tabelaAnimesSelecionados->rowCount());
    int lnewRow = ui->tabelaAnimesSelecionados->rowCount()-1;
    int lselectedRow = select.at(0).row();
    for(int w = 0; w < 7; w++){
        QTableWidgetItem *litem = new QTableWidgetItem;
        litem->setText(ui->tabelaAnimes->item(lselectedRow,w)->text());
        ui->tabelaAnimesSelecionados->setItem(lnewRow,w,litem);
    }
    ui->tabelaAnimes->hideRow(select.at(0).row());
    ui->tabelaAnimes->selectRow(select.at(0).row()+1);
}

void janeladeconfig::on_botaoRemoverAnime_clicked()
{
    if(ui->tabelaAnimesSelecionados->selectionModel()->selectedRows().isEmpty())
        return;
    QModelIndexList select = ui->tabelaAnimesSelecionados->selectionModel()->selectedRows();
    QString id = ui->tabelaAnimesSelecionados->item(select.at(0).row(),1)->text();
    for(int i = 0; i < ui->tabelaAnimes->rowCount(); i++){
        if(ui->tabelaAnimes->item(i,1)->text().compare(id) == 0){
            ui->tabelaAnimes->showRow(i);
            ui->tabelaAnimesSelecionados->removeRow(select.at(0).row());
            break;
        }
    }
    if(ui->tabelaAnimesSelecionados->rowCount() > select.at(0).row())
        ui->tabelaAnimesSelecionados->selectRow(select.at(0).row());
    else if(ui->tabelaAnimesSelecionados->rowCount() <= select.at(0).row() && ui->tabelaAnimesSelecionados->rowCount() > 0)
        ui->tabelaAnimesSelecionados->selectRow(select.at(0).row()-1);
}

void janeladeconfig::on_botaoEnableDisable_clicked()
{
    QModelIndexList select = ui->tabelaFiltros->selectionModel()->selectedRows();
    int index = ui->tabelaFiltros->item(select.at(0).row(),0)->text().toInt();
    if(vfiltrosAnimes[index]->Ativo){
        vfiltrosAnimes[index]->Ativo = false;
        ui->tabelaFiltros->item(index,1)->setTextColor("red");
    }
    else{
        vfiltrosAnimes[index]->Ativo = true;
        ui->tabelaFiltros->item(index,1)->setTextColor("black");
    }
}
