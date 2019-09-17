#ifndef QDOWNLOADER_H
#define QDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QStringList>
#include <QString>
#include <QFileInfo>
#include <QThread>

#include "configpc.h"
#include "leitorarquivos.h"

class QDownloader : public QObject
{
    Q_OBJECT
public:
    explicit QDownloader(QObject *parent = nullptr);
    virtual ~QDownloader();
    void setFile(QString fileURL, QString);
    void setURL(QString);
    void setTorrent(QString, QString);
    void setFileBig(QString fileURL, QString);
//    void setFile(leitorarquivos *, int);
    void setListaAnimes(leitorarquivos *leiArq);
    void downloadImagensLista(QString, QString);
private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QFile *file;
    configPC *configura;
    leitorarquivos *leiArq;

    int indexLista;
    bool fileIsOpen;
    bool terminouLista;
    QString lista;
    QString listaAtual;
    int tamanhoLista;

signals:
    void terminouDownload();
    void filexists();

public slots:
    void setNext();
private slots:
    void onDownloadProgress(qint64,qint64);
    void onFinished(QNetworkReply*);
    void onReadyRead();
    void onReplyFinished();
};

#endif // QDOWNLOADER_H
