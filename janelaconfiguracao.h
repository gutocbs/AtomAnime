#ifndef JANELACONFIGURACAO_H
#define JANELACONFIGURACAO_H

#include <QDialog>
#include <QDir>
#include <QVector>
#include <QDebug>
#include <QFileDialog>

namespace Ui {
class JanelaConfiguracao;
}

class JanelaConfiguracao : public QDialog
{
    Q_OBJECT

public:
    explicit JanelaConfiguracao(QWidget *parent = nullptr);
    ~JanelaConfiguracao();
    void setDiretoriosAnimes();
    void leArquivoConf();
    int returnDetection();
    int returnTempoDownload();
    int returnDownloadAutomatico();
    int returnImagemBaixaQualidade();
    QString returnFeedBusca();
    QString returnFansub();
    QString returnQualidade();
    QString returnTorrentPadrao();
    QString returnDownloadFolder();
    QString returnDownloadListas();

    QVector<QString> retornaDiretorioAnime();
    QString returnUserAnilist();
    QString returnOrdemLista();
    QString returnRSS();

    void setDetection(int);
    void setDownloadListas(QString);
    void setDownloadAutomatico(int);

private:
    Ui::JanelaConfiguracao *ui;
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
    QString downloadListas;

    int detection;
    int tempoDownload;
    int downloadAutomatico;
signals:
    void save();
    void cancelado();
    void user();
    void dirAdd(QString);
    void dirRem(QString);
    void detec(int);
    void dListas(QString);
    void tDownload(int);
    void dAutomatico(int);
    void dFolder(QString);
    void fBusca(QString);
    void fSub(QString);
    void quality(QString);
    void tPadrao(QString);
    void bDownload(int);
    void salvaArquivo();

public slots:
    void mensagem(QString);
private slots:
    void on_DiretorioAnimesAdiciona_clicked();
    void on_BotaoAutorizar_clicked();
    void on_DiretorioAnimesRemove_clicked();
    void on_BotaoSelecionarPasta_clicked();
    void on_BotaoSalvar_clicked();
};

#endif // JANELACONFIGURACAO_H
