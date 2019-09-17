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

signals:
    void volta();
private slots:
    void baixaTorrent();
    void on_pushButton_clicked();
    void leXML();
    void Download();

    void on_XML_clicked();
    void on_Download_clicked();
    void esperaTerminarSalvar();

private:
    int globalDownload;
    bool *primeiroDownload;

    Ui::torrent *ui;
    anitomy::Anitomy *anitomy;
    JanelaConfiguracao *jconfig;
    leitorarquivos *qleitor;
    Organizador *organiza;
//    QVector<QCheckBox> box;
    QDownloader *baixaXML;
    QVector<QCheckBox*> box;
    QVector<QWidget*> pWidget;
    QVector<QHBoxLayout*> pLayout;

    QString url;
    QString diretorioDownloads;

    QVector<bool> marcadoDownload;
    QVector<QString> nomeTorrent;
    QVector<QString> nome;
    QVector<QString> fansub;
    QVector<QString> episodio;
    QVector<QString> resolucao;
    QVector<QString> link;
    QVector<QString> lista;
    QVector<QString> tier;
    bool listaWatching;

    QString prefQualidade;
    QString prefSub;
    //Nome, nome alt, id, tempqualidade, prefqualidade, tempsub, prefsub, tempepisodiotorrent, tempepisodiolistawatching, temptorrent
    bool filtros(QString, QString, QString, QString, QString, QString, QString, QString, QString, int);
};

#endif // TORRENT_H
