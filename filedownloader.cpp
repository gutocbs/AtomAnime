#include "filedownloader.h"
#include <QDesktopServices>

QDownloader::QDownloader(QObject *parent) :
    QObject(parent)
{
    configura = new configPC;
    manager = new QNetworkAccessManager;
    lista = "0";
    fileIsOpen = false;
    terminouLista = false;
}

QDownloader::~QDownloader()
{
    manager->deleteLater();
//    delete manager;
    delete reply;
    delete file;
    delete configura;
    delete leiArq;
}

void QDownloader::downloadImagensLista(QString fileURL, QString id){
    QString filePath = fileURL;
    QString saveFilePath;
    QStringList filePathList = filePath.split('/');
    QString arquivo = configura->diretorioImagensMedio;
    arquivo.append(id);
    arquivo.append(fileURL.mid(fileURL.lastIndexOf(QChar('.'))));

    saveFilePath = arquivo;

    if(QFile(saveFilePath).size() == 0)
        QFile(saveFilePath).remove();

    bool fileExists = QFileInfo::exists(saveFilePath) && QFileInfo(saveFilePath).isFile();

    if(!fileExists){
        QNetworkRequest request;
        request.setUrl(QUrl(fileURL));
        reply = manager->get(request);

        file = new QFile;
        file->setFileName(saveFilePath);
        file->open(QIODevice::WriteOnly);

        connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
        connect(reply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
        connect(reply,SIGNAL(finished()),this,SLOT(setNext()));
        fileIsOpen = true;
    }
    else{
        fileIsOpen = false;
        setNext();
    }
}
void QDownloader::downloadImagensGrandesLista(QString fileURL, QString id){
    QString filePath = fileURL;
    QString saveFilePath;
    QStringList filePathList = filePath.split('/');
    fileURL.replace("medium", "large");
    QString arquivo = configura->diretorioImagensGrandes;
    arquivo.append(id);
    arquivo.append(fileURL.mid(fileURL.lastIndexOf(QChar('.'))));

    saveFilePath = arquivo;

    if(QFile(saveFilePath).size() == 0)
        QFile(saveFilePath).remove();

    bool fileExists = QFileInfo::exists(saveFilePath) && QFileInfo(saveFilePath).isFile();

    if(!fileExists){
        QNetworkRequest request;
        request.setUrl(QUrl(fileURL));
        reply = manager->get(request);

        file = new QFile;
        file->setFileName(saveFilePath);
        file->open(QIODevice::WriteOnly);

        connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
        connect(reply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
        connect(reply,SIGNAL(finished()),this,SLOT(setNextBig()));
        fileIsOpen = true;
    }
    else{
        fileIsOpen = false;
        setNextBig();
    }
}

void QDownloader::downloadImagensPequenasLista(QString fileURL, QString id){
    QString filePath = fileURL;
    QString saveFilePath;
    QStringList filePathList = filePath.split('/');
    fileURL.replace("medium", "small");
    QString arquivo = configura->diretorioImagensPequenas;
    arquivo.append(id);
    arquivo.append(fileURL.mid(fileURL.lastIndexOf(QChar('.'))));

    saveFilePath = arquivo;

    if(QFile(saveFilePath).size() == 0)
        QFile(saveFilePath).remove();

    bool fileExists = QFileInfo::exists(saveFilePath) && QFileInfo(saveFilePath).isFile();

    if(!fileExists){
        QNetworkRequest request;
        request.setUrl(QUrl(fileURL));
        reply = manager->get(request);

        file = new QFile;
        file->setFileName(saveFilePath);
        file->open(QIODevice::WriteOnly);

        connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
        connect(reply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
        connect(reply,SIGNAL(finished()),this,SLOT(setNextSmall()));
        fileIsOpen = true;
    }
    else{
        fileIsOpen = false;
        setNextSmall();
    }
}
void QDownloader::setURL(QString url){
    QString saveFilePath = "Configurações/rss.xml";
    bool fileExists = QFileInfo::exists(saveFilePath) && QFileInfo(saveFilePath).isFile();
    QFile::remove(saveFilePath);
    if(!fileExists){
        QNetworkRequest request;
        request.setUrl(QUrl(url));
        reply = manager->get(request);

        file = new QFile;
        file->setFileName(saveFilePath);
        file->open(QIODevice::WriteOnly);


//        connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(onDownloadProgress(qint64,qint64)));
        connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
        connect(reply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
        connect(reply,SIGNAL(finished()),this,SLOT(onReplyFinished()));
    }
}

void QDownloader::setTorrent(QString url, QString nome){
    QString saveFilePath = "Configurações/Temp/Torrents/" + nome + ".torrent";
    QFile::remove(saveFilePath);
    bool fileExists = QFileInfo::exists(saveFilePath) && QFileInfo(saveFilePath).isFile();
    if(!fileExists){
        QNetworkRequest request;
        request.setUrl(QUrl(url));
        reply = manager->get(request);

        file = new QFile;
        file->setFileName(saveFilePath);
        file->open(QIODevice::WriteOnly);


//        connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(onDownloadProgress(qint64,qint64)));
        connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
        connect(reply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
        connect(reply,SIGNAL(finished()),this,SLOT(onReplyFinished()));
    }
    else{
        emit terminouDownload();
    }
}

void QDownloader::onDownloadProgress(qint64 bytesRead,qint64 bytesTotal)
{
    qDebug(QString::number(bytesRead).toLatin1() +" - "+ QString::number(bytesTotal).toLatin1());
}

void QDownloader::onFinished(QNetworkReply * reply)
{
    switch(reply->error())
    {
        case QNetworkReply::NoError:
        {
//            qDebug("file is downloaded successfully.");
        }break;
        default:{
            qDebug(reply->errorString().toLatin1());
        }
    }

    if(file->isOpen())
    {
        file->close();
        file->deleteLater();
    }
}

void QDownloader::onReadyRead()
{
    file->write(reply->readAll());
}

void QDownloader::onReplyFinished()
{
//    qDebug() << "Terminou";
    if(file->isOpen())
    {
        file->close();
        file->deleteLater();
    }
//    qDebug() << "oi";
    emit terminouDownload();
//    setFile(A, i+1);
}

void QDownloader::setNext()
{
    if(fileIsOpen == true){
        if(file->isOpen())
        {
            file->close();
            file->deleteLater();
        }
    }
    if(lista == "0"){
        leiArq = new leitorarquivos;
        leiArq->leLinha("watching");
        tamanhoLista = leiArq->retornaTamanhoLista();
        listaAtual = "watching";
        indexLista = -1;
    }
    else if(lista == "1"){
        leiArq = new leitorarquivos;
        leiArq->leLinha("completed");
        listaAtual = "completed";
        tamanhoLista = leiArq->retornaTamanhoLista();
        indexLista = -1;
    }
    else if(lista == "2"){
        leiArq = new leitorarquivos;
        leiArq->leLinha("onhold");
        listaAtual = "onhold";
        tamanhoLista = leiArq->retornaTamanhoLista();
        indexLista = -1;
    }
    else if(lista == "3"){
        leiArq = new leitorarquivos;
        leiArq->leLinha("dropped");
        listaAtual = "dropped";
        tamanhoLista = leiArq->retornaTamanhoLista();
        indexLista = -1;
    }
    else if(lista == "4"){
        leiArq = new leitorarquivos;
        leiArq->leLinha("plantowatch");
        listaAtual = "plantowatch";
        tamanhoLista = leiArq->retornaTamanhoLista();
//        tamanhoLista = 709;
        indexLista = -1;
    }
    lista = "9";
    indexLista++;
    if(terminouLista == false){
        if(indexLista < tamanhoLista){
            downloadImagensLista(leiArq->retornaLink(indexLista), leiArq->retornaId(indexLista));
        }
        else{
            if(listaAtual == "watching"){
                delete leiArq;
                lista = "1";
                emit listaMensagem("Watching");
            }
            else if(listaAtual == "completed"){
                delete leiArq;
                lista = "2";
                emit listaMensagem("Completed");
            }
            else if(listaAtual == "onhold"){
                delete leiArq;
                lista = "3";
                emit listaMensagem("On Hold");
            }
            else if(listaAtual == "dropped"){
                delete leiArq;
                lista = "4";
                emit listaMensagem("Dropped");
            }
            else if(listaAtual == "plantowatch"){
                delete leiArq;
                terminouLista = true;
                emit listaMensagem("Plan to Watch");
            }
            setNext();
        }
    }
    else{
        emit listaMensagem("all");
    }
}

void QDownloader::setNextBig()
{
    if(fileIsOpen == true){
        if(file->isOpen())
        {
            file->close();
            file->deleteLater();
        }
    }
    if(lista == "0"){
        leiArq = new leitorarquivos;
        leiArq->leLinha("watching");
        tamanhoLista = leiArq->retornaTamanhoLista();
        listaAtual = "watching";
        indexLista = -1;
    }
    else if(lista == "1"){
        leiArq = new leitorarquivos;
        leiArq->leLinha("completed");
        listaAtual = "completed";
        tamanhoLista = leiArq->retornaTamanhoLista();
        indexLista = -1;
    }
    else if(lista == "2"){
        leiArq = new leitorarquivos;
        leiArq->leLinha("onhold");
        listaAtual = "onhold";
        tamanhoLista = leiArq->retornaTamanhoLista();
        indexLista = -1;
    }
    else if(lista == "3"){
        leiArq = new leitorarquivos;
        leiArq->leLinha("dropped");
        listaAtual = "dropped";
        tamanhoLista = leiArq->retornaTamanhoLista();
        indexLista = -1;
    }
    else if(lista == "4"){
        leiArq = new leitorarquivos;
        leiArq->leLinha("plantowatch");
        listaAtual = "plantowatch";
        tamanhoLista = leiArq->retornaTamanhoLista();
//        tamanhoLista = 709;
        indexLista = -1;
    }
    lista = "9";
    indexLista++;
    if(terminouLista == false){
        if(indexLista < tamanhoLista){
            downloadImagensGrandesLista(leiArq->retornaLink(indexLista), leiArq->retornaId(indexLista));
        }
        else{
            if(listaAtual == "watching"){
                delete leiArq;
                lista = "1";
//                emit listaMensagem("Watching");
            }
            else if(listaAtual == "completed"){
                delete leiArq;
                lista = "2";
//                emit listaMensagem("Completed");
            }
            else if(listaAtual == "onhold"){
                delete leiArq;
                lista = "3";
//                emit listaMensagem("On Hold");
            }
            else if(listaAtual == "dropped"){
                delete leiArq;
                lista = "4";
//                emit listaMensagem("Dropped");
            }
            else if(listaAtual == "plantowatch"){
                delete leiArq;
                terminouLista = true;
//                emit listaMensagem("Plan to Watch");
            }
            setNextBig();
        }
    }
    else{
        emit listaMensagem("big");
    }
}

void QDownloader::setNextSmall()
{
    if(fileIsOpen == true){
        if(file->isOpen())
        {
            file->close();
            file->deleteLater();
        }
    }
    if(lista == "0"){
        leiArq = new leitorarquivos;
        leiArq->leLinha("watching");
        tamanhoLista = leiArq->retornaTamanhoLista();
        listaAtual = "watching";
        indexLista = -1;
    }
    else if(lista == "1"){
        leiArq = new leitorarquivos;
        leiArq->leLinha("completed");
        listaAtual = "completed";
        tamanhoLista = leiArq->retornaTamanhoLista();
        indexLista = -1;
    }
    else if(lista == "2"){
        leiArq = new leitorarquivos;
        leiArq->leLinha("onhold");
        listaAtual = "onhold";
        tamanhoLista = leiArq->retornaTamanhoLista();
        indexLista = -1;
    }
    else if(lista == "3"){
        leiArq = new leitorarquivos;
        leiArq->leLinha("dropped");
        listaAtual = "dropped";
        tamanhoLista = leiArq->retornaTamanhoLista();
        indexLista = -1;
    }
    else if(lista == "4"){
        leiArq = new leitorarquivos;
        leiArq->leLinha("plantowatch");
        listaAtual = "plantowatch";
        tamanhoLista = leiArq->retornaTamanhoLista();
//        tamanhoLista = 709;
        indexLista = -1;
    }
    lista = "9";
    indexLista++;
    if(terminouLista == false){
        if(indexLista < tamanhoLista){
            downloadImagensPequenasLista(leiArq->retornaLink(indexLista), leiArq->retornaId(indexLista));
        }
        else{
            if(listaAtual == "watching"){
                delete leiArq;
                lista = "1";
                emit listaMensagem("Watching");
            }
            else if(listaAtual == "completed"){
                delete leiArq;
                lista = "2";
                emit listaMensagem("Completed");
            }
            else if(listaAtual == "onhold"){
                delete leiArq;
                lista = "3";
                emit listaMensagem("On Hold");
            }
            else if(listaAtual == "dropped"){
                delete leiArq;
                lista = "4";
                emit listaMensagem("Dropped");
            }
            else if(listaAtual == "plantowatch"){
                delete leiArq;
                terminouLista = true;
                emit listaMensagem("Plan to Watch");
            }
            setNextSmall();
        }
    }
    else{
        emit listaMensagem("all");
    }
}
