#ifndef LEITORLISTAANIMES_H
#define LEITORLISTAANIMES_H

#include <QObject>
#include <QDebug>
#include <QVector> //Cria a lista de animes
#include <QFile> //Abre arquivo do anilist/taiga para leitura
#include <QTextStream> //Lê linhas do arquivo
#include <QDir> ///Abre os arquivos do taiga - Remover depois
#include <QPointer> // Crio o ponteiro pro anime

#include "anime.h"

class leitorlistaanimes : public QObject
{
    Q_OBJECT
public:
    explicit leitorlistaanimes(QObject *parent = nullptr);
    ~leitorlistaanimes();
    bool fadicionaAnimes();
    void fdeletaListaAnimes();

    QVector<anime*> retornaListaWatching();
    QVector<anime*> retornaListaCompleted();
    QVector<anime*> retornaListaOnHold();
    QVector<anime*> retornaListaDropped();
    QVector<anime*> retornaListaPlanToWatch();
    QVector<anime*> sortLista(QString, QString);
    QVector<anime*> fbuscaLista(QString);

signals:

private:
    QVector<anime*> vlistaWatching;
    QVector<anime*> vlistaCompleted;
    QVector<anime*> vlistaOnHold;
    QVector<anime*> vlistaDropped;
    QVector<anime*> vlistaPlanToWatch;
    QVector<anime*> vlistaBusca;
};

#endif // LEITORLISTAANIMES_H
