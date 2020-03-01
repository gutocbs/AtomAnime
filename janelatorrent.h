#ifndef JANELATORRENT_H
#define JANELATORRENT_H

#include <QBoxLayout> //Pra poder por as caixas e tickar elas
#include <QCheckBox>
#include <QDebug>
#include <QDesktopServices>
#include <QDesktopServices> //Abre o site do torrent
#include <QDir>
#include <QFile>
#include <QPointer>
#include <QProcess>
#include <QTableWidgetItem>
#include <QTimer>
#include <QUrl>
#include <QWidget>
#include <QXmlStreamReader>

#include "arquivos.h"
#include "filedownloader.h"
#include "janeladeconfig.h"
#include "leitorlistaanimes.h"
#include "torrentinfo.h"
#include <anitomy/anitomy.h>

namespace Ui {
class janelatorrent;
}

class janelatorrent : public QWidget {
  Q_OBJECT

public:
  explicit janelatorrent(QWidget *parent = nullptr);
  ~janelatorrent();

  void fpreencheTabela();
  void fpassaPonteiros(leitorlistaanimes *, janeladeconfig *, arquivos *);
  void fprocuraAnimeEspecifico(QString);

private slots:
  void fesperaTorrent();
  void fleXML();
  void fbaixaTorrent();
  void on_botaoDownload_clicked();
  void on_botaoAtualizaLista_clicked();

  void on_botaoInfoAnime_clicked();

  void on_botaoLinkTorrent_clicked();

signals:
  void error(QString);
  void infoAnime(QString);

private:
  Ui::janelatorrent *ui;
  anitomy::Anitomy anitomy;
  anitomy::Anitomy anitomyCompare;
  leitorlistaanimes *cleitor;
  janeladeconfig *cconfig;
  arquivos *carquivos;
  QVector<torrentinfo *> torrent;
  QVector<anime *> vlistaAtual;
  QVector<int> vlistaDownload;
  QMap<QString, QStringList> vbaixar;
};

#endif // JANELATORRENT_H
