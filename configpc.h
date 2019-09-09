#ifndef CONFIGPC_H
#define CONFIGPC_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDirIterator>
#include <QVector>
#include <QDebug>

#include "leitorarquivos.h"
#include "anitomy/anitomy.h"

class configPC
{
public:
    configPC();

    int RetornaTamanhoVector();
    int BuscaPastasAnimesEspecificos(QString, QString, QString);

    void EscreveArquivo();//Arquivo de configuração do software
    void CriaPastasBase();
    std::string RetornaDiretorioTaiga();

    QString RetornaDiretorioAnimeEspecifico(int);


    QVector<QString> diretorioAnimes;

    QString diretorioImagensMedio;
    QString diretorioImagensGrandes;
private:
    anitomy::Anitomy *anitomy;
    QString diretorioTaiga;
    QVector<QString> diretorioAnimeEspecifico;
    QVector<QString> idAnime;
};

#endif // CONFIGPC_H
