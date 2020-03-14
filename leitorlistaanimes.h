#ifndef LEITORLISTAANIMES_H
#define LEITORLISTAANIMES_H

#include <QObject>
#include <QDebug>
#include <QVector> //Cria a lista de animes
#include <QFile> //Abre arquivo do anilist/taiga para leitura
#include <QTextStream> //Lê linhas do arquivo
#include <QPointer> // Crio o ponteiro pro anime
#include <QDateTime> //Data que lança o episódio
#include <QHash> //Pra procurar o anime com rapidez

#include "anime.h"

class leitorlistaanimes : public QObject
{
    Q_OBJECT
public:
    explicit leitorlistaanimes(QObject *parent = nullptr);
    ~leitorlistaanimes();
    bool fleJson();
    bool fmudaLista(QString, QString, QString);
    bool fdeletedaLista(QString, QString);
    void fdeletaListaAnimes();
    QString fbuscaAnimePorIDERetornaEpisodio(QString);
    QString fbuscaAnimePorIDERetornaTitulo(QString);
    QString fbuscaAnimePorIDERetornaLista(QString);
    QString fprocuraAnimeNasListas(QString);
    QVector<anime*> retornaListaWatching();
    QVector<anime*> retornaListaCompleted();
    QVector<anime*> retornaListaOnHold();
    QVector<anime*> retornaListaDropped();
    QVector<anime*> retornaListaPlanToWatch();
    QVector<anime*> retornaListaMangaReading();
    QVector<anime*> retornaListaMangaCompleted();
    QVector<anime*> retornaListaMangaOnHold();
    QVector<anime*> retornaListaMangaDropped();
    QVector<anime*> retornaListaMangaPlanToRead();
    QVector<anime*> retornaListaNovelReading();
    QVector<anime*> retornaListaNovelCompleted();
    QVector<anime*> retornaListaNovelOnHold();
    QVector<anime*> retornaListaNovelDropped();
    QVector<anime*> retornaListaNovelPlanToRead();
    QVector<anime*> sortLista(QString, QString);
    QVector<anime*> fbuscaLista(QString, QString);

signals:
    void sAnimeAdicionadoNaLista(QString);
    void sMangaAdicionadoNaLista(QString);
    void sNovelAdicionadoNaLista(QString);

private:
    QVector<anime*> vlistaWatching;
    QVector<anime*> vlistaCompleted;
    QVector<anime*> vlistaOnHold;
    QVector<anime*> vlistaDropped;
    QVector<anime*> vlistaPlanToWatch;
    QVector<anime*> vlistaBusca;
    QVector<anime*> llistaTemp;
    QVector<anime*> vlistaMangaReading;
    QVector<anime*> vlistaMangaCompleted;
    QVector<anime*> vlistaMangaOnHold;
    QVector<anime*> vlistaMangaDropped;
    QVector<anime*> vlistaMangaPlanToRead;
    QVector<anime*> vlistaNovelReading;
    QVector<anime*> vlistaNovelCompleted;
    QVector<anime*> vlistaNovelOnHold;
    QVector<anime*> vlistaNovelDropped;
    QVector<anime*> vlistaNovelPlanToRead;
    QHash<QString,QString> vHashListaAnimesPorId;
};

#endif // LEITORLISTAANIMES_H
