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
    QString diretorioImagensPequenas;



public slots:
    void setUser();
    void addDir(QString);
    void rmvDir(QString);
    void setFeedBusca(QString);
    void setDetection(int);
    void setDownloadListas(QString);
    void setTempoDownload(int);
    void setDownloadAutomatico(int);
    void setDownloadFolder(QString);
    void setFansub(QString);
    void setQualidade(QString);
    void setTorrentPadrao(QString);
    void setQualidadeImagem(int);
    void EscreveConfig();

private:
    anitomy::Anitomy *anitomy;
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
    QString downloadListas;
    int detection;
    int tempoDownload;
    int downloadAutomatico;
    int qualidadeImagem;
};

#endif // CONFIGPC_H
