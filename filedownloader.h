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
    ~QDownloader();
    void setURL(QString);
    void setTorrent(QString, QString);
    void downloadImagensLista(QString, QString);
    void downloadImagensGrandesLista(QString, QString);
    void downloadImagensPequenasLista(QString, QString);
private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QFile *file;
    configPC *configura;
    leitorarquivos *leiArq;

    int indexLista;
    bool fileIsOpen;
    bool terminouLista;
    int tamanhoLista;
    QString lista;
    QString listaAtual;
    QString tamanho;

signals:
    void terminouDownload();
    void filexists();
    void listaMensagem(QString);
public slots:
    void setNext();
    void setNextBig();
    void setNextSmall();
private slots:
    void onDownloadProgress(qint64,qint64);
    void onFinished(QNetworkReply*);
    void onReadyRead();
    void onReplyFinished();
};

#endif // QDOWNLOADER_H
