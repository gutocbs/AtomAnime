#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPointer>

#include "leitorlistaanimes.h"
#include "confbase.h"

class filedownloader : public QObject
{
    Q_OBJECT
public:
    explicit filedownloader(QObject *parent = nullptr);
    ~filedownloader();
    void fdownloadImagensLista(QString, QString);
    void fdownloadImagensGrandesLista(QString, QString);
    void fdownloadImagensPequenasLista(QString, QString);
    void fsetLeitorListaAnimes(leitorlistaanimes*);
    void fsetConfBase(confBase*);
    void fdownloadAvatarUsuario(QString);
    void fdownloadTorrent(QString, QString);
    void fdownloadXMLTorrentList(QString);

signals:
    void slistaMensagem(QString);
    void savatar();
    void sxml();
    void storrent();

public slots:
    void fsetNext();
    void fsetNextBig();
    void fsetNextSmall();

private slots:
    void onFinished(QNetworkReply*);
    void onReadyRead();

private:
    QNetworkAccessManager *vmanager;
    QNetworkReply *vreply;
    QFile *vfile;

    QPointer<confBase> cconfBase;
    QPointer<leitorlistaanimes> cleitorlistaanimes;

    QVector<anime*> vlistaSelecionada;

    QString vlistaAtual;//Lista que está sendo lida
    bool vfileIsOpen;
    bool vterminouLista;
    int vlista;
    int vindexLista;
};

#endif // FILEDOWNLOADER_H
