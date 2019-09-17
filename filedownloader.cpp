#include "filedownloader.h"
#include <QDesktopServices>

QDownloader::QDownloader(QObject *parent) :
    QObject(parent)
{
    configura = new configPC;
    manager = new QNetworkAccessManager;
}

QDownloader::~QDownloader()
{
//    manager->deleteLater();
    delete manager;
    delete reply;
    delete file;
    delete configura;
}

void QDownloader::IniciaThread(QThread &dThread){
    connect(&dThread, SIGNAL(started()), this, SLOT(run()));
}


void QDownloader::setFile(QString fileURL, QString w)
{
    QString filePath = fileURL;
    QString saveFilePath;
    QStringList filePathList = filePath.split('/');
        //QString fileName = filePathList.at(filePathList.count() - 1);
//    fileURL.replace("medium", "small");
    QString arquivo = configura->diretorioImagensMedio;
    arquivo.append(w);
    arquivo.append(fileURL.mid(fileURL.lastIndexOf(QChar('.'))));//lastIndexOf(QChar('/'))+1)
//    right(fileURL.lastIndexOf(QChar('.')))

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


//        connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(onDownloadProgress(qint64,qint64)));
        connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(onFinished(QNetworkReply*)));
        connect(reply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
        connect(reply,SIGNAL(finished()),this,SLOT(onReplyFinished()));
    }
    else{
        emit filexists();
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
}

void QDownloader::setFileBig(QString fileURL, QString w)
{
    QString filePath = fileURL;
    QString saveFilePath;
    QStringList filePathList = filePath.split('/');
        //QString fileName = filePathList.at(filePathList.count() - 1);
    fileURL.replace("medium", "large");
    QString arquivo = configura->diretorioImagensGrandes;
    arquivo.append(w);
    arquivo.append(fileURL.mid(fileURL.lastIndexOf(QChar('.'))));//lastIndexOf(QChar('/'))+1)
//    right(fileURL.lastIndexOf(QChar('.')))

    saveFilePath = arquivo;
    bool fileExists = QFileInfo::exists(saveFilePath) && QFileInfo(saveFilePath).isFile();

    if(!fileExists){
        QNetworkRequest request;
        request.setUrl(QUrl(fileURL));
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

void QDownloader::run(){
}
