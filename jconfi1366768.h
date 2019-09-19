#ifndef JCONFI1366768_H
#define JCONFI1366768_H

#include <QWidget>
#include <QDir>
#include <QVector>
#include <QDebug>
#include <QFileDialog>
#include <QDialog>

namespace Ui {
class jconfi1366768;
}

class jconfi1366768 : public QWidget
{
    Q_OBJECT

public:
    explicit jconfi1366768(QWidget *parent = nullptr);
    ~jconfi1366768();
    void setDiretoriosAnimes();
    void leArquivoConf();
    int returnDownloadListas();
    int returnDetection();
    int returnTempoDownload();
    int returnDownloadAutomatico();
    int returnImagemBaixaQualidade();
    QString returnFeedBusca();
    QString returnFansub();
    QString returnQualidade();
    QString returnTorrentPadrao();
    QString returnDownloadFolder();

    QVector<QString> retornaDiretorioAnime();
    QString returnUserAnilist();
    QString returnOrdemLista();
    QString returnRSS();

    void setDetection(int);
    void setDownloadListas(int);
    void setDownloadAutomatico(int);

private:
    Ui::jconfi1366768 *ui;
    QVector<QString> diretorioAnime;
    QString userAnilist;
    QString ordem;
    QString rss;
    QString feedGeral;
    QString feedEspecifico;
    QString downloadFolder;
    QString fansub;
    QString qualidade;
    QString torrent;

    int detection;
    int downloadListas;
    int tempoDownload;
    int downloadAutomatico;
signals:
    void save();
    void cancelado();
    void user();
    void dirAdd(QString);
    void dirRem(QString);
    void detec(int);
    void dListas(int);
    void tDownload(int);
    void dAutomatico(int);
    void dFolder(QString);
    void fBusca(QString);
    void fSub(QString);
    void quality(QString);
    void tPadrao(QString);
    void bDownload(int);

public slots:
    void mensagem(QString);
private slots:
    void on_DiretorioAnimesAdiciona_clicked();
    void on_BotaoAutorizar_clicked();
    void on_DiretorioAnimesRemove_clicked();
    void on_BotaoSelecionarPasta_clicked();
    void on_BotaoSalvar_clicked();
};

#endif // JCONFI1366768_H
