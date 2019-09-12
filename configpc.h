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
#include "janelaconfiguracao.h"

class configPC : public QObject
{
    Q_OBJECT
public:
    configPC();

    int RetornaTamanhoVector();
    int BuscaPastasAnimesEspecificos(QString, QString, QString);

    void EscreveArquivo();//Arquivo de configuração do software
    void EscreveConfig();
    void CriaPastasBase();
    void recebeJConfig(JanelaConfiguracao *JanelaConfg);
    void setOrdem(QString);

    std::string RetornaDiretorioTaiga();

    QString RetornaDiretorioAnimeEspecifico(int);
    QString retornaUser();
    QString getOrdem();

    QVector<QString> diretorioAnimes;

    QString diretorioImagensMedio;
    QString diretorioImagensGrandes;

public slots:
    void setUser();
    void addDir(QString);
    void rmvDir(QString);

private:
    anitomy::Anitomy *anitomy;
    leitorarquivos *arquivo;
    JanelaConfiguracao *jconfig;
    QString diretorioTaiga;
    QVector<QString> diretorioAnimeEspecifico;
    QVector<QString> idAnime;
    QString usernameAnilist;
    QString ordemLista;
};

#endif // CONFIGPC_H
