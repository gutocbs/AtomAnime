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

#include "anitomy/anitomy.h"
#include "filedownloader.h"
#include "janelaconfiguracao.h"
#include "leitorarquivos.h"

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
    void preencheTabela();
signals:
    void volta();
private slots:
    void on_pushButton_clicked();
    void leXML();
    void Download();

    void on_XML_clicked();

private:
    Ui::torrent *ui;
    anitomy::Anitomy *anitomy;
    JanelaConfiguracao *jconfig;
    leitorarquivos *qleitor;

    QString url;

    QVector<bool> marcadoDownload;
    QVector<QString> nome;
    QVector<QString> fansub;
    QVector<QString> episodio;
    QVector<QString> resolucao;
    QVector<QString> link;
    QVector<QString> lista;
};

#endif // TORRENT_H
