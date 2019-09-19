#ifndef MAINWINDOW1366768_H
#define MAINWINDOW1366768_H

#include <QWidget>
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
#include "torrent1366768.h"


namespace Ui {
class mainwindow1366768;
}

class mainwindow1366768 : public QWidget
{
    Q_OBJECT

public:
    explicit mainwindow1366768(QWidget *parent = nullptr);
    ~mainwindow1366768();
    //Essa função é pública por causa do future
    void ConfiguraArquivos();

public slots:
    //Construtores de janelas
    void InstauraPrimeiraJanela();
    void BotaoWatching();
    void BotaoCompleted();
    void BotaoOnHold();
    void BotaoDropped();
    void BotaoPlanToWatch();
    void BotaoBusca();
    void on_Lista_clicked();
    void Configurar();

    //Downloads
    void baixaImagens();
    void baixaImagensGrandes();
    void baixaImagensPequenas();
    void BuscaTorrentAnimeEspecifico();

    //Mexem com as janelas
    void OrdenaVetor();
    void mudouQualidade(int);

    //Arquivos
    void mandaRefresh();
    void refreshArquivo();

    //Informações dos animes
    void carregaInfo();

    //Listas de animes
    void proximaPagina();
    void voltaPagina();

    //Botões de configuração
    void abrePasta();
    void abreAnilist();
    void AbreEpisodio();
    void on_NotaMais_3_clicked();
    void on_NotaMenos_3_clicked();
    void on_ProgressoMais_3_clicked();
    void on_ProgressoMenos_3_clicked();

    ///Carrega botões
    void Botoes();
    void LiberaBotaoCompleted();
    void LiberaBotaoOnHold();
    void LiberaBotaoDropped();
    void LiberaBotaoPlanToWatch();

    ///Botões de carregar animes
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

    void setUser();


    //Mensagens
    void mensagemFimDownload(QString);

private slots:
    void Torrent();
    void keyPressEvent(QKeyEvent * event);

private:
    Ui::mainwindow1366768 *ui;

    JanelaConfiguracao jConfig;
    torrent1366768 jtorrent;

    //Download de imagens
    QDownloader *DownImagemListas;
    QDownloader *DownImagemGrandeListas;
    QDownloader *DownImagemPequenaListas;

    //Leitores
    leitorarquivos *leitorA;

    Organizador *organiza;
    configPC *configuracoes;
    Config *runArquivo;

    //Thread de baixar os animes
    QThread cThread;
///
    int anime0, anime1, anime2, anime3, anime4;
    int idAnime;
    int tamanhoLista;
    int numEpisodios;
    int pagina;
    int downl;

    QString ordemVetorWatching, ordemVetorCompleted, ordemVetorDropped, ordemVetorPaused, ordemVetorPlantoWatch;
    QString lista;

    QVector<int> vetorAnimes;
    QVector<int> qEpiDisponivel;

    bool primeiraLeitura;
    bool baixaQualidade;

    QPixmap pix;
};

#endif // MAINWINDOW1366768_H
