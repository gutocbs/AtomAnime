#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QDebug>
#include <algorithm>
#include <QDir>

#include "leitorlistaanimes.h"
#include "confbase.h"
#include "filedownloader.h"
#include "arquivos.h"
#include "confusuario.h"
#include "logger.h"

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
    void on_BotaoPasta_clicked();
    void on_botaoBusca_clicked();

    void on_boxOrdemLista_activated(const QString &arg1);

private:
    Ui::MainWindow *ui;
    leitorlistaanimes *cleitorListaAnimes;
    filedownloader *cfiledownloader;
    arquivos *carquivos;

    QVector<anime*> vlistaSelecionada;
    QPointer<confBase> cconfBase;
    QPointer<confUsuario> cconfUsuario;

    //Variáveis globais
    int vanimeSelecionado;
    int vpagina;
    QString vordem;
    QString vlistaAtual;
};
#endif // MAINWINDOW_H
