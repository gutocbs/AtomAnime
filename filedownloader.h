#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QPointer>
#include <QtNetwork>

#include "confbase.h"
#include "leitorlistaanimes.h"

class filedownloader : public QObject {
  Q_OBJECT
public:
  explicit filedownloader(QObject *parent = nullptr);
  ~filedownloader();
  void fsetLeitorListaAnimes(leitorlistaanimes *);
  void fsetConfBase(confBase *);
  void fdownloadAvatarUsuario(QString);
  void fdownloadTorrent(QString, QString);
  void fdownloadXMLTorrentList(QString);

signals:
  void slistaMensagem(QString);
  void savatar();
  void sxml();
  void storrent();
  void sid(QString);
  void sidGrande(QString);
  void sterminouLista(QString);

public slots:
  void fdownloadMedio();
  void fdownloadGrande();
  void fdownloadPequeno();
  void fsetNext();
  void fsetNextBig();
  void fsetNextSmall();
  void fresetCounters();

private slots:
  void onFinished(QNetworkReply *);
  void onReadyRead();

private:
  QNetworkAccessManager *vmanager;
  QNetworkReply *vreply;
  QFile *vfile;
  QPointer<confBase> cconfBase;
  QPointer<leitorlistaanimes> cleitorlistaanimes;
  QVector<anime *> vlistaSelecionada;

  QString vlistaAtual; // Lista que está sendo lida
  QString vsaveFilePath;
  QString vlink;

  bool vfileIsOpen;
  bool vterminouLista;
  int vlista;
  int vindexLista;
};

#endif // FILEDOWNLOADER_H
