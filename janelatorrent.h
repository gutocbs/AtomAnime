#ifndef JANELATORRENT_H
#define JANELATORRENT_H

#include <QBoxLayout> //Pra poder por as caixas e tickar elas
#include <QCheckBox>
#include <QDebug>
#include <QDesktopServices>
#include <QDesktopServices> //Abre o site do torrent
#include <QDesktopWidget>   //Pra resolução
#include <QDir>
#include <QFile>
#include <QFuture>
#include <QMutexLocker> //Pra lidar com os mapas nas threads
#include <QPointer>
#include <QProcess>
#include <QScreen> //Resolução também
#include <QTableWidgetItem>
#include <QTimer>
#include <QUrl>
#include <QWidget>
#include <QXmlStreamReader>
#include <QtConcurrent>
#include <QtMath> //Também resolução

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

  void fpassaPonteiros(leitorlistaanimes *, janeladeconfig *, arquivos *);
  void fprocuraAnimeEspecifico(const QString &rnomeAnimeBuscado);
  void fautoDownload();
  void fmudaResolucao();
  QString fchecaFiltroFansub(const QString &lid);
  QString fchecaFiltroDownloadFromList(const QString &lid);
  QString fchecaFiltroResolution(const QString &lid);
  QString fchecaFiltroLaguage(const QString &lid);
  QString fchecaFiltroHasKeyword(const QString &lid);

private slots:
  void on_botaoDownload_clicked();
  void on_botaoAtualizaLista_clicked();
  void on_botaoInfoAnime_clicked();
  void on_botaoLinkTorrent_clicked();
  void on_botaoSearchThisanime_clicked();
  void on_botaoSelectSubForTorrent_clicked();

  void fpreparaLeituraArquivoTorrent();
  void fpreencheTabela();
  void fesperaTorrent();
  void fleXML();
  void fbaixaTorrent();
  void fchecaEstado(int);

  int fcalculaPrioridadeNome(QString ridAnime, const QString &repisodioAime,
                             const QString &repisodiosAssistidos,
                             const QString &rlista, int);
  int fcalculaPrioridadeSub(const QString &rfansub, const QString &rid);
  int fcalculaPrioridadeQualidade(const QString &resolucao, const QString &id);
  int fcalculaPrioridadeFiltros(const QString &description, const QString &rid);

signals:
  void error(QString);
  void infoAnime(QString);
  void fimDownloadTorrent();

private:
  Ui::janelatorrent *ui;
  anitomy::Anitomy anitomy;
  anitomy::Anitomy anitomyCompare;
  leitorlistaanimes *cleitor;
  janeladeconfig *cconfig;
  arquivos *carquivos;

  QFuture<void> vleArquivo;
  QMutex vmutex;

  QMap<int, QCheckBox *> vcontroladorCheckbox;
  QVector<torrentinfo *> torrent;
  QVector<anime *> vlistaAtual;
  QVector<int> vlistaDownload;
  QMap<QString, QStringList> vbaixar;
  QHash<QString, QString> vHashDeNomeEId;
};

#endif // JANELATORRENT_H
