#ifndef TORRENT_H
#define TORRENT_H

#include <QWidget>
#include <QDebug>
#include <QObject>
#include <QFile>
#include <QVector>
#include <QTableWidgetItem>
#include <QListWidgetItem>
#include <QCheckBox>
#include <QDir>
#include <QTimer>
#include <QBoxLayout>
#include <QtConcurrent>

#include "anitomy/anitomy.h"
#include "filedownloader.h"
#include "janelaconfiguracao.h"
#include "leitorarquivos.h"
#include "organizador.h"

namespace Ui {
class torrent;
}

class torrent : public QWidget
{
    Q_OBJECT

public:
    explicit torrent(QWidget *parent = nullptr);
    ~torrent();
    void getRss();
    void getJConfig(JanelaConfiguracao *JanelaConfig);
    void getLeitorArquivos(leitorarquivos *leitorArquiv);
    void getOrganizador(Organizador *organi);
    void preencheTabela();

    void getRSSEspecifico(QString);

signals:
    void volta();

public slots:
    void mudaTorrentPadrao(QString);
private slots:
    void baixaTorrent();
    void on_pushButton_clicked();
    void leXML();
    void leXMLEspecifico();
    void Download();

    void on_XML_clicked();
    void on_Download_clicked();
    void esperaTerminarSalvar();
private:
    int tempoDownloadAutomatico;

    Ui::torrent *ui;
    anitomy::Anitomy *anitomy;
    JanelaConfiguracao *jconfig;
    leitorarquivos *qleitor;
    Organizador *organiza;
    QDownloader *baixaXML;
    QVector<QCheckBox*> box;
    QVector<QWidget*> pWidget;
    QVector<QHBoxLayout*> pLayout;

    QString url;
    QString diretorioDownloads;
    QString listasAnime;

    QVector<QString> nomeTorrent;
    QVector<QString> nome;
    QVector<QString> fansub;
    QVector<QString> episodio;
    QVector<QString> resolucao;
    QVector<QString> link;
    QVector<QString> lista;
    QVector<QString> tier;
    QVector<QString> comentario;
    QVector<QString> dublado;
    QVector<int> globalDownload;

    bool listaWatching;
    bool downloadAutomatico;

    QString prefTorrent;
    QString prefQualidade;
    QString prefSub;
};

#endif // TORRENT_H
