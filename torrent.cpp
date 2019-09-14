#include "torrent.h"
#include "ui_torrent.h"

torrent::torrent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::torrent)
{
    ui->setupUi(this);
}

torrent::~torrent()
{
    delete ui;
}

void torrent::getJConfig(JanelaConfiguracao *JanelaConfig){
    jconfig = JanelaConfig;
}

void torrent::getRss(){
    QDownloader *baixaXML = new QDownloader;
    qDebug() << jconfig->returnRSS();
    baixaXML->setURL(jconfig->returnRSS());
    QObject::connect(baixaXML, SIGNAL(terminouDownload()), this, SLOT(leXML()));
}

void torrent::on_pushButton_clicked()
{
    emit volta();
}

void torrent::on_XML_clicked()
{
    getRss();
}

void torrent::leXML(){
    qDebug() << "oi";
}
