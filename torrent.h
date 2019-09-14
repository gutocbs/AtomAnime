#ifndef TORRENT_H
#define TORRENT_H

#include <QWidget>
#include <QDebug>
#include <QObject>
#include "filedownloader.h"
#include "janelaconfiguracao.h"

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
signals:
    void volta();

private slots:
    void on_pushButton_clicked();
    void leXML();

    void on_XML_clicked();

private:
    Ui::torrent *ui;
    JanelaConfiguracao *jconfig;
    QString url;
};

#endif // TORRENT_H
