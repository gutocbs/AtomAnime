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
    ~configPC();

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
    void setFeedBusca(QString);
    void setDetection(int);
    void setDownloadListas(int);
    void setTempoDownload(int);
    void setDownloadAutomatico(int);
    void setDownloadFolder(QString);
    void setFansub(QString);
    void setQualidade(QString);
    void setTorrentPadrao(QString);

private:
    anitomy::Anitomy *anitomy;
    leitorarquivos *arquivo;
    JanelaConfiguracao *jconfig;
    QString diretorioTaiga;
    QVector<QString> diretorioAnimeEspecifico;
    QVector<QString> idAnime;
    QString usernameAnilist;
    QString ordemLista;
    QString feedTorrentGeral;
    QString feedTorrentEspecifico;
    QString downloadFolder;
    QString preFfansub;
    QString prefQualidade;
    QString prefTorrent;
    int detection;
    int downloadListas;
    int tempoDownload;
    int downloadAutomatico;
};

#endif // CONFIGPC_H
