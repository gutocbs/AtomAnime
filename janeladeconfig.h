#ifndef JANELADECONFIG_H
#define JANELADECONFIG_H

#include <QWidget>
#include <QTextStream> //Para escrever o arquivo de configuração
#include <QFileDialog> //Pra abrir as janelas de busca de pastas
#include <QDebug>
#include <QSettings>

namespace Ui {
class janeladeconfig;
}

class janeladeconfig : public QWidget
{
    Q_OBJECT

public:
    explicit janeladeconfig(QWidget *parent = nullptr);
    ~janeladeconfig();
    QByteArray fretornaUsuario();
    QStringList fretornaDiretorios();
    QByteArray fretornaCodigoAutorizacao();
    QByteArray fretornaDeteccao();
    QByteArray fretornaBaixaQualidade();
    QByteArray fretornaTorrentEscolhido();
    QByteArray fretornaDownloadAutomatico();
    QByteArray fretornaTempoDownload();
    QByteArray fretornaPastaSalvarAnimes();
    QByteArray fretornaDownloadListasAnimes();
    QByteArray fretornaSubEscolhido();
    QByteArray fretornaQualidadeEscolhida();
    QByteArray fretornaFeedAnime();
    QByteArray fretornaFeedAnimeEspecifico();
    QByteArray fretornaFiltrosDownload();

    void fleArquivoConfig();
signals:
    void sauthcodesave();
private slots:
    void on_botaoSalvar_clicked();

    void on_botaoAdicionar_clicked();
    void on_botaoRemover_clicked();
    void on_botaoSelecionarPastaDownload_clicked();
    void on_botaoAutorizarUser_clicked();
    void on_botaoSalvarCodigo_clicked();
private:
    Ui::janeladeconfig *ui;

    QString vuser;
    QStringList vdirectory;
    QString vauthorization;
    QString vdetection;
    QString vtorrentSoftware;
    QString vsaveFolder;
    QString vdownloadListas;
    QString vsub;
    QString vquality;
    QString vtorrentFeed;
    QString vanimeTorrentFeed;
    bool vautoDownload;
    bool vlowQuality;
};

#endif // JANELADECONFIG_H
