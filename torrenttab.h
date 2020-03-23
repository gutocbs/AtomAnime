#ifndef TORRENTTAB_H
#define TORRENTTAB_H

#include <QWidget>
#include <QPointer>
#include <QMap>
#include <QXmlStreamReader>

#include "torrentinfo.h"
#include "leitorlistaanimes.h"
#include "janeladeconfig.h"
#include "filedownloader.h"
#include "arquivos.h"
#include "confbase.h"

namespace Ui {
class TorrentTab;
}

class TorrentTab : public QWidget
{
    Q_OBJECT

public:
    explicit TorrentTab(QWidget *parent = nullptr);
    void fpassaPonteiros(leitorlistaanimes*, janeladeconfig*, arquivos*, confBase*);
    ~TorrentTab();

public slots:
    void fautoDownload();
    void fprocuraAnimeEspecifico(QString);

signals:
    void error(QString);
    void infoAnime(QString);

private slots:
    void on_carregaLista_clicked();

    void fleXML();
    QString fchecaFiltroFansub(QString);
    QString fchecaFiltroDownloadFromList(QString);
    QString fchecaFiltroResolution(QString);
    QString fchecaFiltroLaguage(QString);
    QString fchecaFiltroHasKeyword(QString);
    void fConstroiListaTorrents();
    void fmostraTorrent();
    void fchecaEstado();
    void fhideTudo();
    void fesperaTorrent();
    void fbaixaTorrent();
    void fbaixaLista();
    void fblockSignals();
    void funblockSignals();

    int fcalculaPrioridadeNome(QString, QString, QString,QString, int);
    int fcalculaPrioridadeSub(QString, QString);
    int fcalculaPrioridadeQualidade(QString, QString);
    int fcalculaPrioridadeFiltros(QString, QString);

    void on_botaoProximaPagina_clicked();
    void on_botaoPaginaAnterior_clicked();
    void on_botaoAnime09_clicked();
    void on_botaoAnime00_clicked();
    void on_botaoAnime01_clicked();
    void on_botaoAnime02_clicked();
    void on_botaoAnime03_clicked();
    void on_botaoAnime04_clicked();
    void on_botaoAnime05_clicked();
    void on_botaoAnime06_clicked();
    void on_botaoAnime07_clicked();
    void on_botaoAnime08_clicked();
    void on_botaoAnime10_clicked();
    void on_botaoAnime11_clicked();
    void on_botaoBaixar_clicked();
    void on_botaoSelectSubForTorrent_clicked();
    void on_botaoInfoAnime_clicked();
    void on_botaoLinkTorrent_clicked();
    void on_botaoSearchThisanime_clicked();

private:
    Ui::TorrentTab *ui;
    anitomy::Anitomy anitomy;
    anitomy::Anitomy anitomyCompare;
    leitorlistaanimes *cleitor;
    janeladeconfig *cconfig;
    arquivos *carquivos;
    confBase *cconfbase;

    QVector<torrentinfo*> torrent;
    QVector<anime*> vlistaAtual;

    QMap<QString, QStringList> vbaixar;
    QHash<QString,QString> vHashDeNomeEId;
    QHash<QString,QVector<int>> vHashDeIdEPosicaoDoTorrent;
    QVector<QString> vlistaDeIDs;
    QVector<int> vlistaDownload;

    QStringList vnomeSubs;
    QStringList vqualidadeTorrents;

    int vpagina;
    int vtorrentSelecionado;
};

#endif // TORRENTTAB_H
