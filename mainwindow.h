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

#include "leitorlistaanimes.h"
#include "confbase.h"
#include "filedownloader.h"
#include "arquivos.h"
#include "confusuario.h"
#include "logger.h"


#include "anilist.h"
#include "janeladeconfig.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void finfoAnimeSelecionado();
    void fcarregaImagensLista();
    void fbloqueiaSinaisBotoes();
    void fliberaSinaisBotoes();
    void fatualizaRefreshTimer();
    void fatualizaAnilist();

    bool fcarregaImagensBackground();

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
    void on_boxOrdemLista_activated(const QString &arg1);
    void on_botaoHome_clicked();
    void on_botaoConfiguracao_clicked();
    void on_botaoNotaMais_clicked();
    void on_botaoNotaMenos_clicked();
    void on_botaoProgressoMais_clicked();
    void on_botaoProgressoMenos_clicked();

private:
    Ui::MainWindow *ui;
    leitorlistaanimes *cleitorListaAnimes;
    filedownloader *cfiledownloader;
    arquivos *carquivos;
    anilist *canilist;

    QVector<anime*> vlistaSelecionada;
    QVector<anime*> vcarregaListaBackground;
    QPointer<confBase> cconfBase;
    QPointer<confUsuario> cconfUsuario;

    QFuture<void> vfuture;

    //Variáveis globais
    int vanimeSelecionado;
    int vpagina;
    int vtimerSegundos;
    QString vordem;
    QString vlistaAtual;
    bool vrefreshAcontecendo = false;

    QTimer *timer;
    QTimer *timerRefresh;
    QTimer *timerAcao;

    QVector<QStringList> vlistaAcoes;

    janeladeconfig jconfig;
};
#endif // MAINWINDOW_H
