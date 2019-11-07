#ifndef JANELADECONFIG_H
#define JANELADECONFIG_H

#include <QWidget>
#include <QTextStream> //Para escrever o arquivo de configuração

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
    QByteArray fretornaDiretorios();
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
private slots:
    void on_botaoSalvar_clicked();

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
