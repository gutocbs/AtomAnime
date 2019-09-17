#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDebug>
#include <QPixmap>
#include <QKeyEvent>
#include <QTimer>
#include <QVector>
#include <QtConcurrent>
#include <QProcess>
#include <QScreen>

#include "infoanime.h"
#include "config.h"
#include "filedownloader.h"
#include "leitorarquivos.h"
#include "organizador.h"
#include "configpc.h"


#include "janelaconfiguracao.h"
#include "torrent.h"
#include "mainwindow1366768.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void ConfiguraArquivos();

private:
    Ui::MainWindow *ui;
    JanelaConfiguracao jConfig;
    torrent jtorrent;
    mainwindow1366768 main768;

    //Download de imagens
    QDownloader *qdown;
    QDownloader *qdownload;
    QDownloader *baixaOnHold;
    QDownloader *baixaDropped;
    QDownloader *baixaPlanToWatch;
    QDownloader *baixaBusca;
    QDownloader *downImagemGrandeWatching;
    QDownloader *downImagemGrandeCompleted;
    QDownloader *downImagemGrandeOnHold;
    QDownloader *downImagemGrandeDropped;
    QDownloader *downImagemGrandePlanToWatch;
    QDownloader *downImagemGrandeBusca;

    //Leitores
    leitorarquivos *leitorA;
    leitorarquivos *leitorconf;
    leitorarquivos *leitorCompleted;
    leitorarquivos *leitorWatching;
    leitorarquivos *leitorOnHold;
    leitorarquivos *leitorDropped;
    leitorarquivos *leitorPlanToWatch;

    infoanime *pasta;
    Organizador *organiza;
    configPC *configuracoes;
    Config *runArquivo;

    QThread cThread; //Thread de baixar os animes
    QThread confThread;
///
    int anime0, anime1, anime2, anime3, anime4;
    int idAnime;
    QVector<int> vetorAnimes;
    int tamanhoLista;
    int numEpisodios;
    int pagina;
    int downl;

    int tamanhoListaWatching;
    int tamanhoListaCompleted;
    int tamanhoListaDropped;
    int tamanhoListaOnHold;
    int tamanhoListaPlanToWatch;


    QString ordemVetorWatching, ordemVetorCompleted, ordemVetorDropped, ordemVetorPaused, ordemVetorPlantoWatch;

    QVector<int> qEpiDisponivel;

    QString lista;

    bool primeiraLeitura;

    QPixmap pix;

public slots:
    void InstauraPrimeiraJanela();
    void RestauraJanela();
    void mandaRefresh();
    void refreshArquivo();
    void mudaImagem();
    void voltaPagina();
    void setSinopse();
    void carregaInfo();
    void baixaImagens(QString);
    void imagemBig();
    void abrePasta();
    void abreAnilist();

    ///Carrega botões
    void Botoes();
    void LiberaBotaoCompleted();
    void LiberaBotaoOnHold();
    void LiberaBotaoDropped();
    void LiberaBotaoPlanToWatch();

    ///Botões
    void carregaAnime1();
    void carregaAnime2();
    void carregaAnime3();
    void carregaAnime4();
    void carregaAnime5();
    void carregaAnime6();
    void carregaAnime7();
    void carregaAnime8();
    void carregaAnime9();
    void carregaAnime10();
    void carregaAnime11();
    void carregaAnime12();
    void carregaAnime13();
    void carregaAnime14();
    void carregaAnime15();
    void carregaAnime16();
    void carregaAnime17();
    void carregaAnime18();
    void carregaAnime19();
    void carregaAnime20();
    void carregaAnime21();
    void carregaAnime22();
    void carregaAnime23();
    void carregaAnime24();
    void carregaAnime25();
    void carregaAnime26();
    void carregaAnime27();
    void carregaAnime28();

    void BotaoWatching();
    void BotaoCompleted();
    void BotaoOnHold();
    void BotaoDropped();
    void BotaoPlanToWatch();
    void BotaoBusca();
    void OrdenaVetor();
    void setUser();

private slots:
    void Configurar();
    void ConfigCancelada();
    void Torrent();
    void voltaTorrent();
    void AbreEpisodio();
    void keyPressEvent(QKeyEvent * event);
    void on_NotaMais_clicked();
    void on_NotaMenos_clicked();
    void on_ProgressoMais_clicked();
    void on_ProgressoMenos_clicked();
    void on_Lista_clicked();
};

#endif // MAINWINDOW_H
