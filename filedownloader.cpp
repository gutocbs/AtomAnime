#include "filedownloader.h"
#include <iostream>
filedownloader::filedownloader(QObject *parent) : QObject(parent)
{
    vmanager = new QNetworkAccessManager(this);
    vfileIsOpen = false;
    vterminouLista = false;
    vlista = 0;
    vindexLista = 0;
//    vindexListaPequeno = 0;
}

filedownloader::~filedownloader(){
    vmanager->deleteLater();
    cconfBase->deleteLater();
    vfile->deleteLater();
    vreply->deleteLater();
}

void filedownloader::fsetLeitorListaAnimes(leitorlistaanimes *lleitorlistaanimes){
    cleitorlistaanimes = lleitorlistaanimes;
}

void filedownloader::fsetConfBase(confBase *rconfbase){
    cconfBase = rconfbase;
}

void filedownloader::onFinished(QNetworkReply * reply)
{
    switch(reply->error())
    {
        case QNetworkReply::NoError:
        {
//            qDebug("file is downloaded successfully.");
            if(vfile->isOpen())
            {
                vfile->close();
            }
//            if(vfile->size() < 1000){
////                qDebug() << vfile->size();
//                vfile->remove();
//            }
        }break;
        default:{
        //Teste de remover a imagem se ocorrrer um erro no download
            if(vfile->isOpen())
            {
                vfile->close();
            }
        }
    }
}

void filedownloader::onReadyRead()
{
    if(vfile->isWritable()){
        QByteArray data = vreply->readAll();
        vfile->write(data);
        vfile->waitForBytesWritten(30000);
    }
    else{
        //qWarning() << vfile->errorString();// << "- Id:" << vlistaSelecionada[vindexLista]->vid << "- Medium image download failed";
        vfile->remove();
    }
}


void filedownloader::fdownloadAvatarUsuario(QString fileURL){
    QString lsaveFilePath = "Configurações/Temp/Imagens/avatar";
    lsaveFilePath.append(fileURL.mid(fileURL.lastIndexOf(QChar('.'))));

    if(QFile(lsaveFilePath).size() == 0)
        QFile(lsaveFilePath).remove();

    QNetworkRequest lrequest;
    lrequest.setUrl(QUrl(fileURL));
    vreply = vmanager->get(lrequest);

    vfile = new QFile(this);
    vfile->setFileName(lsaveFilePath);
    vfile->open(QIODevice::WriteOnly);

    connect(vmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
    connect(vreply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    connect(vreply,SIGNAL(finished()),this,SIGNAL(savatar()));
}

void filedownloader::fdownloadTorrent(QString fileURL, QString torrentName)
{
    QString lsaveFilePath = "Configurações/Temp/Torrents/";
    lsaveFilePath.append(torrentName);
    lsaveFilePath.append(".torrent");

    if(QFile(lsaveFilePath).size() == 0)
        QFile(lsaveFilePath).remove();

    if(QFile(lsaveFilePath).exists())
        emit storrent();

    QNetworkRequest lrequest;
    lrequest.setUrl(QUrl(fileURL));
    vreply = vmanager->get(lrequest);

    vfile = new QFile(this);
    vfile->setFileName(lsaveFilePath);
    vfile->open(QIODevice::WriteOnly);

    connect(vmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
    connect(vreply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    connect(vreply,SIGNAL(finished()),this,SIGNAL(storrent()));
}

void filedownloader::fdownloadXMLTorrentList(QString fileURL)
{
    QString lsaveFilePath = "Configurações/Temp/torrents.xml";

//    if(QFile(lsaveFilePath).size() == 0)
        QFile(lsaveFilePath).remove();

    QNetworkRequest lrequest;
    lrequest.setUrl(QUrl(fileURL));
    vreply = vmanager->get(lrequest);

    vfile = new QFile(this);
    vfile->setFileName(lsaveFilePath);
    vfile->open(QIODevice::WriteOnly);

    connect(vmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
    connect(vreply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    connect(vreply,SIGNAL(finished()),this,SIGNAL(sxml()));
}

void filedownloader::fsetNext()
{
    emit sid(vlistaSelecionada[vindexLista]->vid);
    if(vfileIsOpen){
        if(vfile->isOpen())
            vfile->close();
    }
    vfileIsOpen = false;
    vreply->close();
    vindexLista++;
    if(vindexLista >= vlistaSelecionada.size()){
        vindexLista = 0;
        vlista++;
    }
    fdownloadMedio();
}

void filedownloader::fsetNextBig()
{
    if(vfileIsOpen){
        if(vfile->isOpen())
            vfile->close();
    }
    vfileIsOpen = false;
//    emit sidGrande(vlistaSelecionada[vindexLista]->vid.toInt());
    vindexLista++;
    if(vindexLista >= vlistaSelecionada.size()){
        vindexLista = 0;
        vlista++;
    }
    fdownloadGrande();
}

void filedownloader::fsetNextSmall()
{
    if(vfileIsOpen){
        if(vfile->isOpen())
            vfile->close();
    }
    emit sid(vlistaSelecionada[vindexLista]->vid);
    vindexLista++;
    if(vindexLista >= vlistaSelecionada.size()){
        vindexLista = 0;
        vlista++;
    }
    fdownloadPequeno();
}

void filedownloader::fresetCounters()
{
    vindexLista = 0;
    vlista = 0;
//    vindexListaPequeno = 0;
}

void filedownloader::fdownloadMedio(){
    if(vlista == 0)
        vlistaSelecionada = cleitorlistaanimes->retornaListaWatching();
    else if(vlista == 1)
        vlistaSelecionada = cleitorlistaanimes->retornaListaCompleted();
    else if(vlista == 2)
        vlistaSelecionada = cleitorlistaanimes->retornaListaOnHold();
    else if(vlista == 3)
        vlistaSelecionada = cleitorlistaanimes->retornaListaDropped();
    else if(vlista == 4)
        vlistaSelecionada = cleitorlistaanimes->retornaListaPlanToWatch();
    else if(vlista == 5)
        vlistaSelecionada = cleitorlistaanimes->retornaListaMangaReading();
    else if(vlista == 6)
        vlistaSelecionada = cleitorlistaanimes->retornaListaMangaCompleted();
    else if(vlista == 7)
        vlistaSelecionada = cleitorlistaanimes->retornaListaMangaOnHold();
    else if(vlista == 8)
        vlistaSelecionada = cleitorlistaanimes->retornaListaMangaDropped();
    else if(vlista == 9)
        vlistaSelecionada = cleitorlistaanimes->retornaListaMangaPlanToRead();
    else if(vlista == 10)
        vlistaSelecionada = cleitorlistaanimes->retornaListaNovelReading();
    else if(vlista == 11)
        vlistaSelecionada = cleitorlistaanimes->retornaListaNovelCompleted();
    else if(vlista == 12)
        vlistaSelecionada = cleitorlistaanimes->retornaListaNovelOnHold();
    else if(vlista == 13)
        vlistaSelecionada = cleitorlistaanimes->retornaListaNovelDropped();
    else if(vlista == 14)
        vlistaSelecionada = cleitorlistaanimes->retornaListaNovelPlanToRead();
    else{
        emit sterminouLista("medium");
        vindexLista = 0;
        vlista = 0;
        return;
    }
    if(!vlistaSelecionada.isEmpty()){
        vlink = vlistaSelecionada[vindexLista]->vLinkImagemMedia;
        if(vlink.contains("large"))
            vlink.replace("large", "medium");
        else if(vlink.contains("small"))
            vlink.replace("small", "medium");
        vsaveFilePath = cconfBase->vdiretorioImagensMedio;
        vsaveFilePath.append(vlistaSelecionada[vindexLista]->vid);
        vsaveFilePath.append(vlink.mid(vlink.lastIndexOf(QChar('.'))));
        if(QFile(vsaveFilePath).exists() && QFile(vsaveFilePath).size() == 0)
            QFile(vsaveFilePath).remove();

        if(!(QFileInfo::exists(vsaveFilePath) && QFileInfo(vsaveFilePath).isFile())){
            QNetworkRequest lrequest;
            lrequest.setUrl(QUrl(vlink));
            vreply = vmanager->get(lrequest);

            vfile = new QFile;
            vfile->setFileName(vsaveFilePath);
            vfile->open(QIODevice::WriteOnly);
            vfileIsOpen = true;
            connect(vmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
            connect(vreply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
            connect(vreply,SIGNAL(finished()),this,SLOT(fsetNext()));
        }
        else{
            vfileIsOpen = false;
            fsetNext();
        }
    }
    else{
        vlista++;
        fdownloadMedio();
    }
}

void filedownloader::fdownloadGrande(){
    if(vlista == 0)
        vlistaSelecionada = cleitorlistaanimes->retornaListaWatching();
    else if(vlista == 1)
        vlistaSelecionada = cleitorlistaanimes->retornaListaCompleted();
    else if(vlista == 2)
        vlistaSelecionada = cleitorlistaanimes->retornaListaOnHold();
    else if(vlista == 3)
        vlistaSelecionada = cleitorlistaanimes->retornaListaDropped();
    else if(vlista == 4)
        vlistaSelecionada = cleitorlistaanimes->retornaListaPlanToWatch();
    else if(vlista == 5)
        vlistaSelecionada = cleitorlistaanimes->retornaListaMangaReading();
    else if(vlista == 6)
        vlistaSelecionada = cleitorlistaanimes->retornaListaMangaCompleted();
    else if(vlista == 7)
        vlistaSelecionada = cleitorlistaanimes->retornaListaMangaOnHold();
    else if(vlista == 8)
        vlistaSelecionada = cleitorlistaanimes->retornaListaMangaDropped();
    else if(vlista == 9)
        vlistaSelecionada = cleitorlistaanimes->retornaListaMangaPlanToRead();
    else if(vlista == 10)
        vlistaSelecionada = cleitorlistaanimes->retornaListaNovelReading();
    else if(vlista == 11)
        vlistaSelecionada = cleitorlistaanimes->retornaListaNovelCompleted();
    else if(vlista == 12)
        vlistaSelecionada = cleitorlistaanimes->retornaListaNovelOnHold();
    else if(vlista == 13)
        vlistaSelecionada = cleitorlistaanimes->retornaListaNovelDropped();
    else if(vlista == 14)
        vlistaSelecionada = cleitorlistaanimes->retornaListaNovelPlanToRead();
    else{
        emit sterminouLista("big");
        vindexLista = 0;
        vlista = 0;
        return;
    }
    if(!vlistaSelecionada.isEmpty()){
        vlink = vlistaSelecionada[vindexLista]->vLinkImagemMedia;
        if(vlink.contains("medium"))
            vlink.replace("medium", "large");
        else if(vlink.contains("small"))
            vlink.replace("small", "large");
        vsaveFilePath = cconfBase->vdiretorioImagensGrandes;
        vsaveFilePath.append(vlistaSelecionada[vindexLista]->vid);
        vsaveFilePath.append(vlink.mid(vlink.lastIndexOf(QChar('.'))));

        if(QFile(vsaveFilePath).exists() && QFile(vsaveFilePath).size() == 0)
            QFile(vsaveFilePath).remove();
    //    qDebug() << vsaveFilePath << vlistaSelecionada[vindexLista]->vid << vlistaSelecionada[vindexLista]->vLinkImagemMedia.mid(
    //                    vlistaSelecionada[vindexLista]->vLinkImagemMedia.lastIndexOf(QChar('.')));
        if(!(QFileInfo::exists(vsaveFilePath) && QFileInfo(vsaveFilePath).isFile())){
    //        qDebug() << vlistaSelecionada[vindexLista]->vid;
            QNetworkRequest lrequest;
            lrequest.setUrl(QUrl(vlink));
            vreply = vmanager->get(lrequest);

            vfile = new QFile;
            vfile->setFileName(vsaveFilePath);
            vfile->open(QIODevice::WriteOnly);
            vfileIsOpen = true;
            connect(vmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
            connect(vreply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
            connect(vreply,SIGNAL(finished()),this,SLOT(fsetNextBig()));
        }
        else{
            vfileIsOpen = false;
            fsetNextBig();
        }
    }
    else{
        vlista++;
        fdownloadGrande();
    }
}

void filedownloader::fdownloadPequeno(){
    if(vlista == 0){
        vlistaSelecionada = cleitorlistaanimes->retornaListaWatching();
    }
    else if(vlista == 1){
        emit slistaMensagem("SWatching");
        vlistaSelecionada = cleitorlistaanimes->retornaListaCompleted();
    }
    else if(vlista == 2){
        emit slistaMensagem("SCompleted");
        vlistaSelecionada = cleitorlistaanimes->retornaListaOnHold();
    }
    else if(vlista == 3){
        emit slistaMensagem("SOn Hold");
        vlistaSelecionada = cleitorlistaanimes->retornaListaDropped();
    }
    else if(vlista == 4){
        emit slistaMensagem("SDropped");
        vlistaSelecionada = cleitorlistaanimes->retornaListaPlanToWatch();
    }
    else{
        emit slistaMensagem("SPlan to Watch");
        vindexLista = 0;
        vlista = 0;
        return;
    }
    vsaveFilePath = cconfBase->vdiretorioImagensMedio;
    vsaveFilePath.append(vlistaSelecionada[vindexLista]->vid);
    vsaveFilePath.append(vlistaSelecionada[vindexLista]->vLinkImagemMedia.mid(
                             vlistaSelecionada[vindexLista]->vLinkImagemMedia.lastIndexOf(QChar('.'))));
    vsaveFilePath.replace("medium", "small");
    if(QFile(vsaveFilePath).exists() && QFile(vsaveFilePath).size() == 0)
        QFile(vsaveFilePath).remove();

    bool fileExists = QFileInfo::exists(vsaveFilePath) && QFileInfo(vsaveFilePath).isFile();

    if(!fileExists){
        QNetworkRequest lrequest;
        lrequest.setUrl(QUrl(vlistaSelecionada[vindexLista]->vLinkImagemMedia));
        vreply = vmanager->get(lrequest);

        vfile = new QFile;
        vfile->setFileName(vsaveFilePath);
        vfile->open(QIODevice::WriteOnly);
        vfileIsOpen = true;
        connect(vmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
        connect(vreply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
        connect(vreply,SIGNAL(finished()),this,SLOT(fsetNext()));
    }
    else
        fsetNextSmall();
}
