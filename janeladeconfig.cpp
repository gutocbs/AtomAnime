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
        }
        larquivo.close();
    }
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
        larquivo.close();
    }
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
