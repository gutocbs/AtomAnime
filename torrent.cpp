#include "torrent.h"
#include "ui_torrent.h"

torrent::torrent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::torrent)
{
    ui->setupUi(this);
    anitomy = new anitomy::Anitomy;
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

void torrent::Download(){
    //sE UI->LISTATORRENTS->ITEM
//    ui->ListaTorrents->item(0,0)->checkState() == Checked; -> baixar
}

void torrent::preencheTabela(){
    ui->ListaTorrents->setRowCount(nome.length());
    ui->ListaTorrents->setSortingEnabled(true);
    for(int i = 0; i < nome.length(); i++){
        for(int w = 0; w < 5; w++){
            ui->ListaTorrents->setColumnWidth(1, 400);
            QTableWidgetItem *item = new QTableWidgetItem;
            QCheckBox *box = new QCheckBox;
            if(w == 0){
                ui->ListaTorrents->setCellWidget(i,w,box);
            }
            if(w == 1){
                item->setText(nome[i]);
                ui->ListaTorrents->setItem(i,w, item);
            }
            else if(w == 2){
                item->setText(episodio[i]);
                ui->ListaTorrents->setItem(i,w, item);
            }
            else if(w == 3){
                if(resolucao[i] == "360p")
                    resolucao[i] = "0360p";
                else if(resolucao[i] == "480p")
                    resolucao[i] = "0480p";
                else if(resolucao[i] == "720p")
                    resolucao[i] = "0720p";
                item->setText(resolucao[i]);
                ui->ListaTorrents->setItem(i,w, item);
            }
            else if(w == 4){
                item->setText(fansub[i]);
                ui->ListaTorrents->setItem(i,w, item);
            }
        }
    }
    ui->ListaTorrents->update();
}

void torrent::leXML(){
    ui->label->setText("Carregando torrents");
    QString arquivoLer = "Configurações/rss.xml";
    QFile lerXML(arquivoLer);
    //Checa se o arquivo pode ser aberto
    if (lerXML.open(QIODevice::ReadOnly))
    {
        //Cria variáveis pra ler os arquivos do taiga
        QTextStream xml(&lerXML);
        //Checa se é possível escrever no arquivo de configuração
        //Lê o arquivo usr até o fim
        while (!xml.atEnd())
        {
            //le cada linha do arquivo
            QString linha = xml.readLine();
            //Checa se a linha tem o dado que procuro //ID no caso
            if(linha.contains("Tokyo Toshokan") == false){
                if(linha.contains("<title>["))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<title>");
                    linha.remove("</title>");
                    anitomy::Anitomy anitomy;
                    anitomy.Parse(linha.toStdWString());
                    const auto& elements = anitomy.elements();
                    nome.append(QString::fromStdWString(elements.get(anitomy::kElementAnimeTitle)));
                    episodio.append(QString::fromStdWString(elements.get(anitomy::kElementEpisodeNumber)));
                    fansub.append(QString::fromStdWString(elements.get(anitomy::kElementReleaseGroup)));
                    resolucao.append(QString::fromStdWString(elements.get(anitomy::kElementVideoResolution)));
                    marcadoDownload.append(false);
                }
                else if(linha.contains("<link>"))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<link><![CDATA[");
                    linha.remove("]]></link>");
                    link.append(linha);
                }
            }
        }
        lerXML.close();
    }
    ui->label->setText("Pronto");
    preencheTabela();
}
