#include "janeladeconfig.h"
#include "ui_janeladeconfig.h"

janeladeconfig::janeladeconfig(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::janeladeconfig)
{
    ui->setupUi(this);
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
                ui->textoUser->setPlainText(lstreamTexto);
                vuser = lstreamTexto;
            }
            else if(lstreamTexto.contains("Directory:")){
                lstreamTexto.remove("Directory:");
                vdirectory = lstreamTexto.split(";");
            }
            else if(lstreamTexto.contains("Authorization:")){
                lstreamTexto.remove("Authorization:");
                vauthorization = lstreamTexto;
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
                vtorrentSoftware = lstreamTexto;
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
                vsaveFolder = lstreamTexto;
                ui->textoSalvaTorrents->setPlainText(lstreamTexto);
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
                vsub = lstreamTexto;
                ui->textoSubPreferido->setPlainText(lstreamTexto);
            }
            else if(lstreamTexto.contains("Quality:")){
                lstreamTexto.remove("Quality:");
                vquality = lstreamTexto;
                ui->boxQualidadeEscolhida->setCurrentText(lstreamTexto);
            }
            else if(lstreamTexto.contains("TorrentFeed:")){
                lstreamTexto.remove("TorrentFeed:");
                vtorrentFeed = lstreamTexto;
                ui->textoTorrentFeed->setPlainText(lstreamTexto);
            }
            else if(lstreamTexto.contains("AnimeTorrentFeed:")){
                lstreamTexto.remove("AnimeTorrentFeed:");
                vanimeTorrentFeed = lstreamTexto;
                ui->textoTorrentFeedEspecifico->setPlainText(lstreamTexto);
            }
        }
        larquivo.close();
    }
}

QByteArray janeladeconfig::fretornaUsuario(){
    return ui->textoUser->toPlainText().toLocal8Bit();
}

QByteArray janeladeconfig::fretornaDiretorios()
{
//    QTableView
//    QByteArray ldiretorios;
//    for(int i = 0; i < ui->tabelaDiretorios.co; i++){

//    }
    return "oi";
}

QByteArray janeladeconfig::fretornaCodigoAutorizacao()
{
    if(vauthorization.isEmpty())
        return ui->textoCodigoAutorizacao->toPlainText().toLocal8Bit();
    else if(vauthorization != ui->textoCodigoAutorizacao->toPlainText())
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
    return ui->textoSubPreferido->toPlainText().toLocal8Bit();
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
        lstreamTexto << "Directory:" << fretornaDiretorios() << endl;
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
        lstreamTexto << "AnimeTorrentFeed:" << fretornaFeedAnimeEspecifico() << endl;
        larquivo.close();
    }
}
