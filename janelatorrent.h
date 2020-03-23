#ifndef JANELATORRENT_H
#define JANELATORRENT_H

#include <QWidget>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QXmlStreamReader>
#include <QPointer>
#include <QBoxLayout> //Pra poder por as caixas e tickar elas
#include <QCheckBox>
#include <QWidget>
#include <QDesktopServices>
#include <QProcess>
#include <QTimer>
#include <QTableWidgetItem>
#include <QPointer>
#include <QDesktopServices> //Abre o site do torrent
#include <QUrl>

#include <anitomy/anitomy.h>
#include "torrentinfo.h"
#include "leitorlistaanimes.h"
#include "janeladeconfig.h"
#include "filedownloader.h"
#include "arquivos.h"


namespace Ui {
class janelatorrent;
}

class janelatorrent : public QWidget
{
    Q_OBJECT

public:
    explicit janelatorrent(QWidget *parent = nullptr);
    ~janelatorrent();

    void fpreencheTabela();
    void fpassaPonteiros(leitorlistaanimes*, janeladeconfig*, arquivos*);
    void fprocuraAnimeEspecifico(QString);
    void fautoDownload();
    QString fchecaFiltroFansub(QString);
    QString fchecaFiltroDownloadFromList(QString);
    QString fchecaFiltroResolution(QString);
    QString fchecaFiltroLaguage(QString);
    QString fchecaFiltroHasKeyword(QString);

private slots:
    void fesperaTorrent();
    void fleXML();
    void fbaixaTorrent();
    void on_botaoDownload_clicked();
    void on_botaoAtualizaLista_clicked();
    void on_botaoInfoAnime_clicked();
    void on_botaoLinkTorrent_clicked();
    void on_botaoSearchThisanime_clicked();
    void on_botaoSelectSubForTorrent_clicked();
    void fchecaEstado(int);

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
    filedownloader *qdown;

    QMap<int,QCheckBox*> vcontroladorCheckbox;
    QVector<torrentinfo*> torrent;
    QVector<anime*> vlistaAtual;
    QVector<int> vlistaDownload;
    QMap<QString, QStringList> vbaixar;
    QHash<QString,QString> vHashDeNomeEId;
};

#endif // JANELATORRENT_H
