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

class QDownloader : public QObject
{
    Q_OBJECT
public:
    explicit QDownloader(QObject *parent = 0);
    virtual ~QDownloader();
    void setFile(QString fileURL, QString);
    void setFileBig(QString fileURL, QString);
//    void setFile(leitorarquivos *, int);
    void IniciaThread(QThread &dThread);

private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QFile *file;
    configPC *configura;
    int i;
    leitorarquivos *A;

signals:
    void terminouDownload();
public slots:
    void run();

private slots:
    void onDownloadProgress(qint64,qint64);
    void onFinished(QNetworkReply*);
    void onReadyRead();
    void onReplyFinished();
};

#endif // QDOWNLOADER_H
