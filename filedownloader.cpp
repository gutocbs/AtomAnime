#include "filedownloader.h"

filedownloader::filedownloader(QObject *parent) : QObject(parent)
{
    vmanager = new QNetworkAccessManager;
    vfileIsOpen = false;
    vterminouLista = false;
    vlista = 0;
}

filedownloader::~filedownloader(){
    vmanager->deleteLater();
    cconfBase->deleteLater();
}

void filedownloader::fsetLeitorListaAnimes(leitorlistaanimes *lleitorlistaanimes){
    cleitorlistaanimes = lleitorlistaanimes;
}

void filedownloader::fsetConfBase(confBase *rconfbase){
    cconfBase = rconfbase;
}

void filedownloader::fdownloadImagensLista(QString fileURL, QString id)
{
    QString lsaveFilePath = cconfBase->vdiretorioImagensMedio;
    lsaveFilePath.append(id);
    lsaveFilePath.append(fileURL.mid(fileURL.lastIndexOf(QChar('.'))));

    if(QFile(lsaveFilePath).size() == 0)
        QFile(lsaveFilePath).remove();

    bool fileExists = QFileInfo::exists(lsaveFilePath) && QFileInfo(lsaveFilePath).isFile();

    if(!fileExists){
        QNetworkRequest lrequest;
        lrequest.setUrl(QUrl(fileURL));
        vreply = vmanager->get(lrequest);

        vfile = new QFile;
        vfile->setFileName(lsaveFilePath);
        vfile->open(QIODevice::WriteOnly);

        connect(vmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
        connect(vreply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
        connect(vreply,SIGNAL(finished()),this,SLOT(fsetNext()));
        vfileIsOpen = true;
    }
    else{
        vfileIsOpen = false;
        fsetNext();
    }
}

void filedownloader::fdownloadImagensGrandesLista(QString fileURL, QString id)
{
    fileURL.replace("medium", "large");
    QString lsaveFilePath = cconfBase->vdiretorioImagensGrandes;
    lsaveFilePath.append(id);
    lsaveFilePath.append(fileURL.mid(fileURL.lastIndexOf(QChar('.'))));

    if(QFile(lsaveFilePath).size() == 0)
        QFile(lsaveFilePath).remove();

    bool fileExists = QFileInfo::exists(lsaveFilePath) && QFileInfo(lsaveFilePath).isFile();

    if(!fileExists){
        QNetworkRequest lrequest;
        lrequest.setUrl(QUrl(fileURL));
        vreply = vmanager->get(lrequest);

        vfile = new QFile;
        vfile->setFileName(lsaveFilePath);
        vfile->open(QIODevice::WriteOnly);

        connect(vmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
        connect(vreply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
        connect(vreply,SIGNAL(finished()),this,SLOT(fsetNextBig()));
        vfileIsOpen = true;
    }
    else{
        vfileIsOpen = false;
        fsetNextBig();
    }
}

void filedownloader::fdownloadImagensPequenasLista(QString fileURL, QString id)
{
    fileURL.replace("medium", "small");
    QString lsaveFilePath = cconfBase->vdiretorioImagensMedio;
    QStringList lfilePathList = fileURL.split('/');
    lsaveFilePath.append(id);
    lsaveFilePath.append(fileURL.mid(fileURL.lastIndexOf(QChar('.'))));

    if(QFile(lsaveFilePath).size() == 0)
        QFile(lsaveFilePath).remove();

    bool fileExists = QFileInfo::exists(lsaveFilePath) && QFileInfo(lsaveFilePath).isFile();

    if(!fileExists){
        QNetworkRequest lrequest;
        lrequest.setUrl(QUrl(fileURL));
        vreply = vmanager->get(lrequest);

        vfile = new QFile;
        vfile->setFileName(lsaveFilePath);
        vfile->open(QIODevice::WriteOnly);

        connect(vmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
        connect(vreply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
        connect(vreply,SIGNAL(finished()),this,SLOT(fsetNextSmall()));
        vfileIsOpen = true;
    }
    else{
        vfileIsOpen = false;
        fsetNextSmall();
    }
}

void filedownloader::fsetNext()
{
    if(vfileIsOpen == true){
        if(vfile->isOpen())
        {
            vfile->close();
            vfile->deleteLater();
        }
    }
    if(vlista == 0){
        qDebug() << "Checking images";
        vlistaSelecionada = cleitorlistaanimes->retornaListaWatching();
        vlistaAtual = "watching";
        vindexLista = -1;
    }
    else if(vlista == 1){
        vlistaSelecionada = cleitorlistaanimes->retornaListaCompleted();
        vlistaAtual = "completed";
        vindexLista = -1;
    }
    else if(vlista == 2){
        vlistaSelecionada = cleitorlistaanimes->retornaListaOnHold();
        vlistaAtual = "onhold";
        vindexLista = -1;
    }
    else if(vlista == 3){
        vlistaSelecionada = cleitorlistaanimes->retornaListaDropped();
        vlistaAtual = "dropped";
        vindexLista = -1;
    }
    else if(vlista == 4){
        vlistaSelecionada = cleitorlistaanimes->retornaListaPlanToWatch();
        vlistaAtual = "plantowatch";
        vindexLista = -1;
    }
    vlista = 9;
    vindexLista++;
    if(vterminouLista == false){
        if(vindexLista < vlistaSelecionada.size()){
            fdownloadImagensLista(vlistaSelecionada[vindexLista]->vLinkImagemMedia, vlistaSelecionada[vindexLista]->vid);
        }
        else{
            if(vlistaAtual == "watching"){
                vlista = 1;
                emit slistaMensagem("Watching");
            }
            else if(vlistaAtual == "completed"){
                vlista = 2;
                emit slistaMensagem("Completed");
            }
            else if(vlistaAtual == "onhold"){
                vlista = 3;
                emit slistaMensagem("On Hold");
            }
            else if(vlistaAtual == "dropped"){
                vlista = 4;
                emit slistaMensagem("Dropped");
            }
            else if(vlistaAtual == "plantowatch"){
                vterminouLista = true;
                emit slistaMensagem("Plan to Watch");
            }
            fsetNext();
        }
    }
    else{
        emit slistaMensagem("all");
        vfileIsOpen = false;
        vterminouLista = false;
        vlista = 0;
        fsetNextBig();
    }
}

void filedownloader::fsetNextBig()
{
    if(vfileIsOpen == true){
        if(vfile->isOpen())
        {
            vfile->close();
            vfile->deleteLater();
        }
    }
    if(vlista == 0){
        vlistaSelecionada = cleitorlistaanimes->retornaListaWatching();
        vlistaAtual = "watching";
        vindexLista = -1;
    }
    else if(vlista == 1){
        vlistaSelecionada = cleitorlistaanimes->retornaListaCompleted();
        vlistaAtual = "completed";
        vindexLista = -1;
    }
    else if(vlista == 2){
        vlistaSelecionada = cleitorlistaanimes->retornaListaOnHold();
        vlistaAtual = "onhold";
        vindexLista = -1;
    }
    else if(vlista == 3){
        vlistaSelecionada = cleitorlistaanimes->retornaListaDropped();
        vlistaAtual = "dropped";
        vindexLista = -1;
    }
    else if(vlista == 4){
        vlistaSelecionada = cleitorlistaanimes->retornaListaPlanToWatch();
        vlistaAtual = "plantowatch";
        vindexLista = -1;
    }
    vlista = 9;
    vindexLista++;
    if(vterminouLista == false){
        if(vindexLista < vlistaSelecionada.size()){
            fdownloadImagensGrandesLista(vlistaSelecionada[vindexLista]->vLinkImagemMedia, vlistaSelecionada[vindexLista]->vid);
        }
        else{
            if(vlistaAtual == "watching"){
                vlista = 1;
                emit slistaMensagem("Watching");
            }
            else if(vlistaAtual == "completed"){
                vlista = 2;
                emit slistaMensagem("Completed");
            }
            else if(vlistaAtual == "onhold"){
                vlista = 3;
                emit slistaMensagem("On Hold");
            }
            else if(vlistaAtual == "dropped"){
                vlista = 4;
                emit slistaMensagem("Dropped");
            }
            else if(vlistaAtual == "plantowatch"){
                vterminouLista = true;
                emit slistaMensagem("Plan to Watch");
                fsetNextBig();
            }
        }
    }
    else{
        emit slistaMensagem("big");
        vfileIsOpen = false;
        vterminouLista = false;
        vlista = 0;
        qDebug() << "Finished downloading all images avaliable";
    }
}

void filedownloader::fsetNextSmall()
{
    if(vfileIsOpen == true){
        if(vfile->isOpen())
        {
            vfile->close();
            vfile->deleteLater();
        }
    }
    if(vlista == 0){
        vlistaSelecionada = cleitorlistaanimes->retornaListaWatching();
        vlistaAtual = "watching";
        vindexLista = -1;
    }
    else if(vlista == 1){
        vlistaSelecionada = cleitorlistaanimes->retornaListaCompleted();
        vlistaAtual = "completed";
        vindexLista = -1;
    }
    else if(vlista == 2){
        vlistaSelecionada = cleitorlistaanimes->retornaListaOnHold();
        vlistaAtual = "onhold";
        vindexLista = -1;
    }
    else if(vlista == 3){
        vlistaSelecionada = cleitorlistaanimes->retornaListaDropped();
        vlistaAtual = "dropped";
        vindexLista = -1;
    }
    else if(vlista == 4){
        vlistaSelecionada = cleitorlistaanimes->retornaListaPlanToWatch();
        vlistaAtual = "plantowatch";
        vindexLista = -1;
    }
    vlista = 9;
    vindexLista++;
    if(vterminouLista == false){
        if(vindexLista < vlistaSelecionada.size()){
            fdownloadImagensPequenasLista(vlistaSelecionada[vindexLista]->vLinkImagemMedia, vlistaSelecionada[vindexLista]->vid);
        }
        else{
            if(vlistaAtual == "watching"){
                vlista = 1;
                emit slistaMensagem("Watching");
            }
            else if(vlistaAtual == "completed"){
                vlista = 2;
                emit slistaMensagem("Completed");
            }
            else if(vlistaAtual == "onhold"){
                vlista = 3;
                emit slistaMensagem("On Hold");
            }
            else if(vlistaAtual == "dropped"){
                vlista = 4;
                emit slistaMensagem("Dropped");
            }
            else if(vlistaAtual == "plantowatch"){
                vterminouLista = true;
                emit slistaMensagem("Plan to Watch");
            }
            fsetNextSmall();
        }
    }
    else{
        emit slistaMensagem("small");
        vfileIsOpen = false;
        vterminouLista = false;
        vlista = 0;
    }
}


void filedownloader::onFinished(QNetworkReply * reply)
{
    switch(reply->error())
    {
        case QNetworkReply::NoError:
        {
//            qDebug("file is downloaded successfully.");
        }break;
        default:{
//            qDebug(reply->errorString().toLatin1());
        }
    }

    if(vfile->isOpen())
    {
        vfile->close();
        vfile->deleteLater();
    }
}

void filedownloader::onReadyRead()
{
    vfile->write(vreply->readAll());
}
