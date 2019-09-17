#ifndef ORGANIZADOR_H
#define ORGANIZADOR_H

#include <QString>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QDesktopServices>
#include <QUrl>
#include <QVector>
#include <QThread>

#include "anitomy/anitomy.h"
#include "configpc.h"

class Organizador
{
public:
    Organizador(configPC*);
    ~Organizador();
    void criaPastas(QString);
    void mudaArquivo(QString, QString);
    void abreAnilist(QString);
    int AbreArquivo(int, int, QString, QString);
    int abrePasta(QString);
    int buscaArquivo(int, int, QString, QString);
    //Chamar essa função para mover os arquivos. As outras não devem ser chamadas
    //Chamar a função com
    //MoveArquivoDeDownloadPraPasta(diretorio, nome do anime);
    void MoveArquivoDeDownloadPraPasta(QString, QString);

    QString retornaEpisodeNumber(QString);
    QString retornaNomeAnime(QString nome);

    int retornaNumEpiNaPasta(int, int, int);
    QVector<int> retornaEpisodiosDisponiveis();



private:
    anitomy::Anitomy *anitomy;
    configPC *conf;
    QVector<int> epiDis;
};

#endif // ORGANIZADOR_H
