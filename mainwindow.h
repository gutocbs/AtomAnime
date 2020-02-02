#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QDebug>
#include <algorithm>
#include <QDir>
#include <QFuture> //Importante para baixar as imagens no background
#include <QtConcurrent> //E rodar a função em uma thread separada
#include <QTimer> //Para atualizar a lista automaticamente
#include <QDesktopServices> //Abre sites
#include <QDesktopWidget> //Pra resolução
#include <QScreen> //Resolução também
#include <QtMath> //Também resolução

#include "leitorlistaanimes.h"
#include "confbase.h"
#include "filedownloader.h"
#include "arquivos.h"
#include "confusuario.h"
#include "logger.h"
#include "anilist.h"

#include "janeladeconfig.h"
#include "janelatorrent.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool fcarregaImagensBackground();
    void fcarregaImagensLista();
    void finfoAnimeSelecionado();
    void fatualizaRefreshTimer();
    void fatualizaAnilist();

    void fbloqueiaSinaisBotoes();
    void fliberaSinaisBotoes();
    void frefreshListas(bool);
    void fmandaDiretoriosArquivos();
    void favisoErro(QString);
    void fmudaResolucao();
    void finiciaPrograma();
    void fcarregouListaSucesso();
    void fcarregouListaFalha();

private slots:
    void on_botaoAnime00_clicked();
    void on_botaoAnime01_clicked();
    void on_botaoAnime02_clicked();
    void on_botaoAnime03_clicked();
    void on_botaoAnime04_clicked();
    void on_botaoAnime05_clicked();
    void on_botaoAnime06_clicked();
    void on_botaoAnime07_clicked();
    void on_botaoAnime08_clicked();
    void on_botaoAnime09_clicked();
    void on_botaoAnime10_clicked();
    void on_botaoAnime11_clicked();

    void on_botaoProximaPagina_clicked();
    void on_botaoPaginaAnterior_clicked();

    void on_Watching_clicked();
    void on_Completed_clicked();
    void on_OnHold_clicked();
    void on_Dropped_clicked();
    void on_PlanToWatch_clicked();

    void on_botaoProximoEpisodio_clicked();
    void on_botaoAbrePasta_clicked();
    void on_botaoBusca_clicked();
    void on_botaoRefresh_clicked();
    void on_botaoHome_clicked();
    void on_botaoConfiguracao_clicked();
    void on_botaoNotaMais_clicked();
    void on_botaoNotaMenos_clicked();
    void on_botaoProgressoMais_clicked();
    void on_botaoProgressoMenos_clicked();
    void on_botaoMudarPraLista_clicked();
    void on_botaoAnilist_clicked();
    void on_botaoCrunchyroll_clicked();
    void on_botaoTorrent_clicked();
    void on_botaoRemoverdaLista_clicked();
    void on_botaoOrdemAlfabetica_clicked();
    void on_botaoOrdemProgresso_clicked();
    void on_botaoOrdemScore_clicked();
    void on_botaoOrdemRelease_clicked();
    void on_botaoOrdemType_clicked();

    void fretryAnilist();
    void fcarregouListaTeste();

private:
    Ui::MainWindow *ui;

    leitorlistaanimes *cleitorListaAnimes;
    filedownloader *cfiledownloader;
    arquivos *carquivos;
    anilist *canilist;

    //Esses dois vetores não estão criando uma lista, mas recebendo uma lista criada na classe leitorListaAnimes.
    //A classe já está deletando todos os ponteiros. Tentar deletar isso vai ser tentar deletar uma lista vaiza e possivelemnte irá
    //Crashar o programa
    QVector<anime*> vlistaSelecionada;
    QVector<anime*> vcarregaListaBackground;

    QPointer<confBase> cconfBase;
    QPointer<confUsuario> cconfUsuario;

    //Thread que vai carregar as imagens do anime. Como essa thread fica dentro da própria classe, temos que usar QFuture
    QFuture<void> vfuture;
    QFuture<void> vcarregaImagens;

    //Variáveis globais
    int vanimeSelecionado;
    int vpagina;
    int vtimerSegundos;
    QString vordem;
    QString vlistaAtual;
    QString dirGrande;
    QString dirMedio;
    QString dirPequeno;
    bool vrefreshAcontecendo = false;
    bool vlistaLidaSucesso = false;
    bool vprimeiraVezThread = true;
    bool vlistaBaixada = false;

    QTimer *timer;
    QTimer *tryTimer;
    QTimer *timerRefresh;
    QTimer *timerAcao;

    QMap<QStringList, QString> vlistaAcoes;

    //Thread que vai baixar a lista de anime e busca as pastas. Como essa thread fica fora da classe, temos que usar QThread
    QThread cThread;
    QThread dThread;

    //Janelas
    janeladeconfig jconfig;
    janelatorrent jtorrent;
};
#endif // MAINWINDOW_H
