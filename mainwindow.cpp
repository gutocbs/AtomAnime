#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "janelaconfiguracao.h"
#include <QObject>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//    bool operator<(const vetorAnimes& a, const vetorAnimes& b) { return a.intensity < b.intensity; }
    anime0 = -1;
    idAnime = 0;
    pagina = 1;
    downl = 0;
    primeiraLeitura = false;

    lista = "watching";
    leitorWatching = new leitorarquivos;
    leitorCompleted = new leitorarquivos;
    leitorOnHold = new leitorarquivos;
    leitorDropped = new leitorarquivos;
    leitorPlanToWatch = new leitorarquivos;
    leitorA = leitorWatching;
    runArquivo = new Config;

    runArquivo->IniciaThread(cThread);
    runArquivo->moveToThread(&cThread);
    cThread.start();

    configuracoes = new configPC;
//    configuracoes->IniciaThread(confThread);
//    configuracoes->moveToThread(&confThread);
//    confThread.start();

    connect(runArquivo, SIGNAL(terminouSetArquivo()),this,SLOT(InstauraPrimeiraJanela()));
    connect(runArquivo, SIGNAL(terminouCompleted()),this,SLOT(LiberaBotaoCompleted()));
    connect(runArquivo, SIGNAL(terminouOnHold()),this,SLOT(LiberaBotaoOnHold()));
    connect(runArquivo, SIGNAL(terminouDropped()),this,SLOT(LiberaBotaoDropped()));
    connect(runArquivo, SIGNAL(terminouPlanToWatch()),this,SLOT(LiberaBotaoPlanToWatch()));
//    connect(runArquivo, SIGNAL(terminouCompleted()),this,SLOT(BotoesCompleted()));//Quando terminar a coisa, o botão é liberado
    //Fazer com que o botão mude de cor e tenha uma barra carregando
    anime0 = -1;
    numEpisodios = 0;
    ui->setupUi(this);
    ui->label->setText("Carregando lista de animes");
    Botoes();
}

MainWindow::~MainWindow()
{
    delete leitorA;
    delete qdown;
    delete configuracoes;
    delete organiza;
    delete ui;
    delete pasta;
    delete runArquivo;
//    delete vetorAnimes;
    cThread.quit();
}

void MainWindow::keyPressEvent(QKeyEvent * event){
    if(event->key() == Qt::Key_U){
        mudaImagem();
//        qDebug() << "oi";
    }
    else if(event->key() == Qt::Key_X){
        AbreEpisodio();
    }
}

void MainWindow::Botoes(){
    connect(ui->PaginaProxima, SIGNAL(clicked()),this,SLOT(mudaImagem()));
    connect(ui->PaginaProxima, SIGNAL(clicked()),this,SLOT(RestauraJanela()));
    connect(ui->PaginaAnterior, SIGNAL(clicked()),this,SLOT(voltaPagina()));
    connect(ui->configteste , SIGNAL(clicked()),this,SLOT(Configurar()));
    connect(ui->AbreEpi, SIGNAL(clicked()),this,SLOT(AbreEpisodio()));
    connect(ui->clickAnime1, SIGNAL(clicked()),this,SLOT(carregaAnime1()));
    connect(ui->clickAnime2, SIGNAL(clicked()),this,SLOT(carregaAnime2()));
    connect(ui->clickAnime3, SIGNAL(clicked()),this,SLOT(carregaAnime3()));
    connect(ui->clickAnime4, SIGNAL(clicked()),this,SLOT(carregaAnime4()));
    connect(ui->clickAnime5, SIGNAL(clicked()),this,SLOT(carregaAnime5()));
    connect(ui->clickAnime6, SIGNAL(clicked()),this,SLOT(carregaAnime6()));
    connect(ui->clickAnime7, SIGNAL(clicked()),this,SLOT(carregaAnime7()));
    connect(ui->clickAnime8, SIGNAL(clicked()),this,SLOT(carregaAnime8()));
    connect(ui->clickAnime9, SIGNAL(clicked()),this,SLOT(carregaAnime9()));
    connect(ui->clickAnime10, SIGNAL(clicked()),this,SLOT(carregaAnime10()));
    connect(ui->clickAnime11, SIGNAL(clicked()),this,SLOT(carregaAnime11()));
    connect(ui->clickAnime12, SIGNAL(clicked()),this,SLOT(carregaAnime12()));
    connect(ui->clickAnime13, SIGNAL(clicked()),this,SLOT(carregaAnime13()));
    connect(ui->clickAnime14, SIGNAL(clicked()),this,SLOT(carregaAnime14()));
    connect(ui->clickAnime15, SIGNAL(clicked()),this,SLOT(carregaAnime15()));
    connect(ui->clickAnime16, SIGNAL(clicked()),this,SLOT(carregaAnime16()));
    connect(ui->clickAnime17, SIGNAL(clicked()),this,SLOT(carregaAnime17()));
    connect(ui->clickAnime18, SIGNAL(clicked()),this,SLOT(carregaAnime18()));
    connect(ui->clickAnime19, SIGNAL(clicked()),this,SLOT(carregaAnime19()));
    connect(ui->clickAnime20, SIGNAL(clicked()),this,SLOT(carregaAnime20()));
    connect(ui->clickAnime21, SIGNAL(clicked()),this,SLOT(carregaAnime21()));
    connect(ui->clickAnime22, SIGNAL(clicked()),this,SLOT(carregaAnime22()));
    connect(ui->clickAnime23, SIGNAL(clicked()),this,SLOT(carregaAnime23()));
    connect(ui->clickAnime24, SIGNAL(clicked()),this,SLOT(carregaAnime24()));
    connect(ui->clickAnime25, SIGNAL(clicked()),this,SLOT(carregaAnime25()));
    connect(ui->clickAnime26, SIGNAL(clicked()),this,SLOT(carregaAnime26()));
    connect(ui->clickAnime27, SIGNAL(clicked()),this,SLOT(carregaAnime27()));
    connect(ui->clickAnime28, SIGNAL(clicked()),this,SLOT(carregaAnime28()));
    connect(ui->Watching, SIGNAL(clicked()),this,SLOT(BotaoWatching()));
    connect(ui->OrdenarBotao, SIGNAL(clicked()), this,SLOT(OrdenaVetorNome()));
}


void MainWindow::LiberaBotaoCompleted(){
    ui->Completed->setStyleSheet("background: white;");
    connect(ui->Completed, SIGNAL(clicked()),this,SLOT(BotaoCompleted()));
}
void MainWindow::LiberaBotaoOnHold(){
    ui->OnHold->setStyleSheet("background: white;");
    connect(ui->OnHold, SIGNAL(clicked()),this,SLOT(BotaoOnHold()));
}
void MainWindow::LiberaBotaoDropped(){
    ui->Dropped->setStyleSheet("background: white;");
    connect(ui->Dropped, SIGNAL(clicked()),this,SLOT(BotaoDropped()));
}
void MainWindow::LiberaBotaoPlanToWatch(){
    ui->PlanToWatch->setStyleSheet("background: white;");
    connect(ui->PlanToWatch, SIGNAL(clicked()),this,SLOT(BotaoPlanToWatch()));
    if(primeiraLeitura == false){
        ui->label->setText("tudo pronto! - Se as imagens não carregarem, estão sendo baixadas. Dê um refresh que talvez elas apareçam!");
        primeiraLeitura = true;
    }
}

void MainWindow::InstauraPrimeiraJanela(){
    ui->label->setText("Carregando!");
    leitorA->leLinha("watching");
//    leitorA->OrdenaVetorNome();
//    qDebug() << "oi";
    qdown = new QDownloader[leitorA->retornaTamanhoLista()];
    downImagemGrandeWatching = new QDownloader[leitorA->retornaTamanhoLista()];
    organiza = new Organizador(configuracoes);
//    QString fileURL = "https://nyaa.si/download/1173229.torrent";
//    qdown[0].setFile("https://nyaa.si/download/1173229.torrent","1");
//    organiza->abre("E:/Programas/Programação/QT/AtomAniem/build-AtomAnime-Desktop_Qt_5_13_0_MinGW_64_bit-Debug/Configurações/Imagens/Medio/1.torrent");
    baixaImagens("watching");
    imagemBig();

//    extern void ConfiguraArquivos();
    QFuture<void> future = QtConcurrent::run(this, &MainWindow::ConfiguraArquivos);
    tamanhoLista = leitorA->retornaTamanhoLista();
    if(idAnime >= tamanhoLista){
        idAnime = tamanhoLista - 28;
    }
    anime0 = 0;
    for(int w = 0; w < tamanhoLista; w++){
        vetorAnimes.append(w);
    }
    carregaInfo();
//    RestauraJanela();
}

void MainWindow::BotaoWatching(){
    ui->label->setText("Carregando!");
    lista = "watching";
    ui->Dropped->setStyleSheet("background: white;");
    ui->Completed->setStyleSheet("background: white;");
    ui->PlanToWatch->setStyleSheet("background: white;");
    ui->OnHold->setStyleSheet("background: white;");
    leitorA = leitorWatching;
    leitorA->leLinha(lista);
//    qDebug() << leitorA->retornaTamanhoLista();
    qdownload = new QDownloader[leitorA->retornaTamanhoLista()];
    downImagemGrandeWatching = new QDownloader[leitorA->retornaTamanhoLista()];
//    qDebug() << qdownload->
    tamanhoLista = leitorA->retornaTamanhoLista();
//    qDebug() << anime0;
//    qDebug() << leitorA->retornaNome(anime0);
    if(idAnime >= tamanhoLista){
        idAnime = tamanhoLista - 16;
    }
    anime0 = 0;
    idAnime = 0;
    pagina = 1;
    vetorAnimes.clear();
    for(int w = 0; w < tamanhoLista; w++){
        vetorAnimes.append(w);
    }
    RestauraJanela();
}

void MainWindow::BotaoCompleted(){
    ui->label->setText("Carregando!");
    lista = "completed";
    ui->Dropped->setStyleSheet("background: white;");
    ui->PlanToWatch->setStyleSheet("background: white;");
    ui->OnHold->setStyleSheet("background: white;");
    ui->Watching->setStyleSheet("background: white;");
    leitorA = leitorCompleted;
    leitorA->leLinha(lista);
    qdownload = new QDownloader[leitorA->retornaTamanhoLista()];
    downImagemGrandeCompleted = new QDownloader[leitorA->retornaTamanhoLista()];
    tamanhoLista = leitorA->retornaTamanhoLista();
    imagemBig();
    if(idAnime >= tamanhoLista){
        idAnime = tamanhoLista - 16;
    }
    anime0 = 0;
    idAnime = 0;
    pagina = 1;
    vetorAnimes.clear();
    for(int w = 0; w < tamanhoLista; w++){
        vetorAnimes.append(w);
    }
    RestauraJanela();
}

void MainWindow::OrdenaVetorNome(){
    leitorA->OrdenaVetorNome();
    carregaInfo();
}

void MainWindow::BotaoOnHold(){
    ui->label->setText("Carregando!");
    lista = "onhold";
    ui->Dropped->setStyleSheet("background: white;");
    ui->Completed->setStyleSheet("background: white;");
    ui->PlanToWatch->setStyleSheet("background: white;");
    ui->Watching->setStyleSheet("background: white;");
    leitorA = leitorOnHold;
    leitorA->leLinha(lista);
    baixaOnHold = new QDownloader[leitorA->retornaTamanhoLista()];
    downImagemGrandeOnHold= new QDownloader[leitorA->retornaTamanhoLista()];
    imagemBig();
    tamanhoLista = leitorA->retornaTamanhoLista();
    if(idAnime >= tamanhoLista){
        idAnime = tamanhoLista - 16;
    }
    anime0 = 0;
    idAnime = 0;
    pagina = 1;
    vetorAnimes.clear();
    for(int w = 0; w < tamanhoLista; w++){
        vetorAnimes.append(w);
    }
    RestauraJanela();
}
void MainWindow::BotaoDropped(){
    ui->label->setText("Carregando!");
    lista = "dropped";
    ui->Completed->setStyleSheet("background: white;");
    ui->PlanToWatch->setStyleSheet("background: white;");
    ui->OnHold->setStyleSheet("background: white;");
    ui->Watching->setStyleSheet("background: white;");
    leitorA = leitorDropped;
    leitorA->leLinha(lista);
    baixaDropped = new QDownloader[leitorA->retornaTamanhoLista()];
    downImagemGrandeDropped= new QDownloader[leitorA->retornaTamanhoLista()];
    imagemBig();
    tamanhoLista = leitorA->retornaTamanhoLista();
    if(idAnime >= tamanhoLista){
        idAnime = tamanhoLista - 16;
    }
    anime0 = 0;
    idAnime = 0;
    pagina = 1;
    vetorAnimes.clear();
    for(int w = 0; w < tamanhoLista; w++){
        vetorAnimes.append(w);
    }
    RestauraJanela();
}
void MainWindow::BotaoPlanToWatch(){
    ui->label->setText("Carregando!");
    lista = "plantowatch";
    ui->Dropped->setStyleSheet("background: white;");
    ui->Completed->setStyleSheet("background: white;");
    ui->OnHold->setStyleSheet("background: white;");
    ui->Watching->setStyleSheet("background: white;");
    leitorA = leitorPlanToWatch;
    leitorA->leLinha(lista);
    baixaPlanToWatch = new QDownloader[leitorA->retornaTamanhoLista()];
    downImagemGrandePlanToWatch= new QDownloader[leitorA->retornaTamanhoLista()];
    imagemBig();
    tamanhoLista = leitorA->retornaTamanhoLista();
    if(idAnime >= tamanhoLista){
        idAnime = tamanhoLista - 16;
    }
    anime0 = 0;
    idAnime = 0;
    pagina = 1;
    vetorAnimes.clear();
    for(int w = 0; w < tamanhoLista; w++){
        vetorAnimes.append(w);
    }
    RestauraJanela();
}

void MainWindow::RestauraJanela(){
    if(lista == "watching"){
        ui->Watching->setStyleSheet("background: red;");
    }
    else if(lista == "onhold"){
        ui->OnHold->setStyleSheet("background: red;");
    }
    else if(lista == "completed"){
        ui->Completed->setStyleSheet("background: red;");
    }
    else if(lista == "dropped"){
        ui->Dropped->setStyleSheet("background: red;");
    }
    else if(lista == "plantowatch"){
        ui->PlanToWatch->setStyleSheet("background: red;");
    }
    leitorA->leLinha(lista);
    baixaImagens(lista);
    tamanhoLista = leitorA->retornaTamanhoLista();
    carregaInfo();
    ui->label->setText("tudo pronto!");
}

void MainWindow::setSinopse(){
    ui->sinopse->setText(leitorA->retornaSinopse(anime0));
}

void MainWindow::mudaImagem(){
     if(idAnime+28 < tamanhoLista){
        idAnime = idAnime+28;
        pagina++;
    }
    carregaInfo();
}
void MainWindow::voltaPagina(){
    if(idAnime-28 >= 0){
        idAnime = idAnime-28;
        pagina--;
    }
    carregaInfo();
}


void MainWindow::AbreEpisodio()
{

    if(lista == "completed"){
        ui->label->setText("Você já viu todos os episódios, não tem um próximo!");
    }
    else{
        if(organiza->AbreArquivo(leitorA->retornaProgresso(anime0), leitorA->retornaId(anime0).toInt(),leitorA->retornaNome(anime0), leitorA->retornaNomeIngles(anime0)) == 0){
            ui->label->setText("Encontrei o anime, abrindo episódio");
        }
        else{
            ui->label->setText("Seu pc de bosta não tem o anime que você quer ver");
        }
        QTimer::singleShot(240000, this, SLOT(RestauraJanela()));
    }
}

void MainWindow::baixaImagens(QString lista)
{
    if(lista == "watching"){
        for(int i = 0; i < leitorA->retornaTamanhoLista();i++){
            qdown[i].setFile(leitorA->retornaLink(i),leitorA->retornaId(i));
        }
    }
    else if(lista == "completed"){
        for(int i = 0; i < leitorA->retornaTamanhoLista();i++){
            qdownload[i].setFile(leitorA->retornaLink(i),leitorA->retornaId(i));
        }
    }
    else if(lista == "onhold"){
        for(int i = 0; i < leitorA->retornaTamanhoLista();i++){
            baixaOnHold[i].setFile(leitorA->retornaLink(i),leitorA->retornaId(i));
        }
    }
    else if(lista == "dropped"){
        for(int i = 0; i < leitorA->retornaTamanhoLista();i++){
            baixaDropped[i].setFile(leitorA->retornaLink(i),leitorA->retornaId(i));
        }
    }
    else if(lista == "plantowatch"){
        for(int i = 0; i < leitorA->retornaTamanhoLista();i++){
            baixaPlanToWatch[i].setFile(leitorA->retornaLink(i),leitorA->retornaId(i));
        }
    }
}

void MainWindow::imagemBig(){
    if(lista == "watching"){
        for(int i = 0; i < leitorA->retornaTamanhoLista();i++){
            downImagemGrandeWatching[i].setFileBig(leitorA->retornaLink(i), leitorA->retornaId(i));
        }
    }
    else if(lista == "completed"){
        for(int i = 0; i < leitorA->retornaTamanhoLista();i++){
            downImagemGrandeCompleted[i].setFileBig(leitorA->retornaLink(i), leitorA->retornaId(i));
        }
    }
    else if(lista == "onhold"){
        for(int i = 0; i < leitorA->retornaTamanhoLista();i++){
            downImagemGrandeOnHold[i].setFileBig(leitorA->retornaLink(i), leitorA->retornaId(i));
        }
    }
    else if(lista == "dropped"){
        for(int i = 0; i < leitorA->retornaTamanhoLista();i++){
            downImagemGrandeDropped[i].setFileBig(leitorA->retornaLink(i), leitorA->retornaId(i));
        }
    }
    else if(lista == "plantowatch"){
        for(int i = 0; i < leitorA->retornaTamanhoLista();i++){
            downImagemGrandePlanToWatch[i].setFileBig(leitorA->retornaLink(i), leitorA->retornaId(i));
        }
    }
}
void MainWindow::ConfiguraArquivos(){
    ui->label->setText("Buscando animes");
    leitorconf = leitorWatching;
    leitorconf->leLinha("watching");
    for(int i = 0; i < leitorconf->retornaTamanhoLista(); i++){
        configuracoes->BuscaPastasAnimesEspecificos(leitorconf->retornaNome(i), leitorconf->retornaNomeIngles(i), leitorconf->retornaId(i));
    }
    leitorconf = leitorOnHold;
    leitorconf->leLinha("onhold");
    for(int i = 0; i < leitorconf->retornaTamanhoLista(); i++){
        configuracoes->BuscaPastasAnimesEspecificos(leitorconf->retornaNome(i), leitorconf->retornaNomeIngles(i), leitorconf->retornaId(i));
    }
    leitorconf = leitorDropped;
    leitorconf->leLinha("dropped");
    for(int i = 0; i < leitorconf->retornaTamanhoLista(); i++){
        configuracoes->BuscaPastasAnimesEspecificos(leitorconf->retornaNome(i), leitorconf->retornaNomeIngles(i), leitorconf->retornaId(i));
    }
    leitorconf = leitorCompleted;
    leitorconf->leLinha("completed");
    for(int i = 0; i < leitorconf->retornaTamanhoLista(); i++){
        configuracoes->BuscaPastasAnimesEspecificos(leitorconf->retornaNome(i), leitorconf->retornaNomeIngles(i), leitorconf->retornaId(i));
    }
    leitorconf = leitorPlanToWatch;
    leitorconf->leLinha("plantowatch");
    for(int i = 0; i < leitorconf->retornaTamanhoLista(); i++){
        configuracoes->BuscaPastasAnimesEspecificos(leitorconf->retornaNome(i), leitorconf->retornaNomeIngles(i), leitorconf->retornaId(i));
    }
    configuracoes->EscreveArquivo();
    leitorconf = leitorWatching;
    leitorconf->leLinha("watching");
    ui->label->setText("Animes baixados encontrados!");
}

void MainWindow::Configurar(){
    JanelaConfiguracao jConfig;
    jConfig.setModal(true);
    jConfig.exec();
}

void MainWindow::carregaAnime1(){
    if(idAnime <= tamanhoLista){
        anime0 = idAnime;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime2(){
    if(idAnime+1 < tamanhoLista){
        anime0 = idAnime+1;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime3(){
    if(idAnime+2 < tamanhoLista){
        anime0 = idAnime+2;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime4(){
    if(idAnime+3 < tamanhoLista){
        anime0 = idAnime+3;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime5(){
    if(idAnime+4 < tamanhoLista){
        anime0 = idAnime+4;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime6(){
    if(idAnime+5 < tamanhoLista){
        anime0 = idAnime+5;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime7(){
    if(idAnime+6 < tamanhoLista){
        anime0 = idAnime+6;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime8(){
    if(idAnime+7 < tamanhoLista){
        anime0 = idAnime+7;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime9(){
    if(idAnime+8 < tamanhoLista){
        anime0 = idAnime+8;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime10(){
    if(idAnime+9 < tamanhoLista){
        anime0 = idAnime+9;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime11(){
    if(idAnime+10 < tamanhoLista){
        anime0 = idAnime+10;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime12(){
    if(idAnime+11 < tamanhoLista){
        anime0 = idAnime+11;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime13(){
    if(idAnime+12 < tamanhoLista){
        anime0 = idAnime+12;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime14(){
    if(idAnime+13 < tamanhoLista){
        anime0 = idAnime+13;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime15(){
    if(idAnime+14 < tamanhoLista){
        anime0 = idAnime+14;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime16(){
    if(idAnime+15 < tamanhoLista){
        anime0 = idAnime+15;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime17(){
    if(idAnime+16 < tamanhoLista){
        anime0 = idAnime+16;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime18(){
    if(idAnime+17 < tamanhoLista){
        anime0 = idAnime+17;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime19(){
    if(idAnime+18 < tamanhoLista){
        anime0 = idAnime+18;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime20(){
    if(idAnime+19 < tamanhoLista){
        anime0 = idAnime+19;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime21(){
    if(idAnime+20 < tamanhoLista){
        anime0 = idAnime+20;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime22(){
    if(idAnime+21 < tamanhoLista){
        anime0 = idAnime+21;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime23(){
    if(idAnime+22 < tamanhoLista){
        anime0 = idAnime+22;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime24(){
    if(idAnime+23 < tamanhoLista){
        anime0 = idAnime+23;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime25(){
    if(idAnime+24 < tamanhoLista){
        anime0 = idAnime+24;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime26(){
    if(idAnime+25 < tamanhoLista){
        anime0 = idAnime+25;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime27(){
    if(idAnime+26 < tamanhoLista){
        anime0 = idAnime+26;
        RestauraJanela();
    }
}
void MainWindow::carregaAnime28(){
    if(idAnime+27 < tamanhoLista){
        anime0 = idAnime+27;
        RestauraJanela();
    }
}

void MainWindow::carregaInfo(){
    leitorA->retornaNumEpisodiosLancados(anime0);
    ui->NumPagina->setText("Página "+ QString::number(pagina) + "/" + QString::number(((tamanhoLista-1)/28)+1));
    ui->Nome->setWordWrap(true);
    ui->Nome->setText(leitorA->retornaNome(anime0));
    ui->NomeIngles->setWordWrap(true);
    ui->NomeIngles->setText(leitorA->retornaNomeIngles(anime0));
    ui->Status->setWordWrap(true);
    ui->Status->setText(leitorA->retornaStatus(anime0));
    ui->NotaGeral->setText(leitorA->retornaNotaGeral(anime0));
    ui->Season->setWordWrap(true);
    ui->Season->setText(leitorA->retornaSeason(anime0));
    ui->sinopse->setText(leitorA->retornaSinopse(anime0));
    ui->sinopse->setWordWrap(true);
    numEpisodios = 0;
    numEpisodios = organiza->retornaNumEpiNaPasta(numEpisodios, leitorA->retornaId(anime0).toInt());
    ui->ProxEpi->setText(QString::number(leitorA->retornaProgresso(anime0)));
    if(numEpisodios > 1){
        ui->EpiDisponivel->setText(QString::number(numEpisodios) + " episódios baixados");
    }
    else if(numEpisodios - leitorA->retornaProgresso(anime0) == 1){
        ui->EpiDisponivel->setText("1 episódio baixado");
    }
    else{
        ui->EpiDisponivel->setText("Nenhum episódio baixado");
    }
    ui->Nota->setText(QString::number(leitorA->retornaScore(anime0)));
    int i = idAnime;
    if(i <= tamanhoLista && tamanhoLista > 0){
        if(pix.load(leitorA->imagem(anime0, configuracoes->diretorioImagensGrandes), "jpg")){
            ui->picBig->setScaledContents(true);
            ui->picBig->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(anime0, configuracoes->diretorioImagensGrandes), "png")){
            ui->picBig->setScaledContents(true);
            ui->picBig->setPixmap(pix);
        }
        else{
            if(pix.load(leitorA->imagem(anime0, configuracoes->diretorioImagensMedio), "jpg")){
                ui->picBig->setScaledContents(true);
                ui->picBig->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(anime0, configuracoes->diretorioImagensMedio), "png")){
                ui->picBig->setScaledContents(true);
                ui->picBig->setPixmap(pix);
            }
        }
        if(pix.load(leitorA->imagem(vetorAnimes[i], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime1->setScaledContents(true);
            ui->anime1->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i], configuracoes->diretorioImagensMedio), "png")){
            ui->anime1->setScaledContents(true);
            ui->anime1->setPixmap(pix);
        }
    }
    else{
        ui->anime1->clear();
        ui->anime1->setStyleSheet("background: transparent;");
    }
    if(i+1 < tamanhoLista){
//        if(configuracoes->RetornaDiretorioAnimeEspecifico(i+1) != "0"){
//            pix.load(":/Imagens/C:/Users/Guto/Desktop/ballG.png");
//            ui->DisponibilidadeAnime2->setPixmap(pix);
//        }
//        else {
//            pix.load(":/Imagens/C:/Users/Guto/Desktop/ballR.png");
//            ui->DisponibilidadeAnime2->setPixmap(pix);
//        }
        if(pix.load(leitorA->imagem(vetorAnimes[i+1], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime2->setScaledContents(true);
                ui->anime2->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+1], configuracoes->diretorioImagensMedio), "png")){
            ui->anime2->setScaledContents(true);
            ui->anime2->setPixmap(pix);
        }
    }
    else{
        ui->anime2->clear();
        ui->anime2->setStyleSheet("background: transparent;");
    }
    if(i+2 < tamanhoLista){
//        if(configuracoes->RetornaDiretorioAnimeEspecifico(i+2) != "0"){
//            pix.load(":/Imagens/C:/Users/Guto/Desktop/ballG.png");
//            ui->DisponibilidadeAnime3->setPixmap(pix);
//        }
//        else {
//            pix.load(":/Imagens/C:/Users/Guto/Desktop/ballR.png");
//            ui->DisponibilidadeAnime3->setPixmap(pix);
//        }
        if(pix.load(leitorA->imagem(vetorAnimes[i+2], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime3->setScaledContents(true);
            ui->anime3->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+2], configuracoes->diretorioImagensMedio), "png")){
            ui->anime3->setScaledContents(true);
            ui->anime3->setPixmap(pix);
        }
    }
    else{
        ui->anime3->clear();
        ui->anime3->setStyleSheet("background: transparent;");
    }
    if(i+3 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+3], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime4->setScaledContents(true);
            ui->anime4->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+3], configuracoes->diretorioImagensMedio), "png")){
            ui->anime4->setScaledContents(true);
            ui->anime4->setPixmap(pix);
        }
    }
    else{
        ui->anime4->clear();
        ui->anime4->setStyleSheet("background: transparent;");
    }
    if(i+4 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+4], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime5->setScaledContents(true);
            ui->anime5->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+4], configuracoes->diretorioImagensMedio), "png")){
            ui->anime5->setScaledContents(true);
            ui->anime5->setPixmap(pix);
        }
    }
    else{
        ui->anime5->clear();
        ui->anime5->setStyleSheet("background: transparent;");
    }
    if(i+5 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+5], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime6->setScaledContents(true);
            ui->anime6->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+5], configuracoes->diretorioImagensMedio), "png")){
            ui->anime6->setScaledContents(true);
            ui->anime6->setPixmap(pix);
        }
    }
    else{
        ui->anime6->clear();
        ui->anime6->setStyleSheet("background: transparent;");
    }
    if(i+6 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+6], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime7->setScaledContents(true);
            ui->anime7->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+6], configuracoes->diretorioImagensMedio), "png")){
            ui->anime7->setScaledContents(true);
            ui->anime7->setPixmap(pix);
        }
    }
    else{
        ui->anime7->clear();
        ui->anime7->setStyleSheet("background: transparent;");
    }
    if(i+7 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+7], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime8->setScaledContents(true);
            ui->anime8->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+7], configuracoes->diretorioImagensMedio), "png")){
            ui->anime8->setScaledContents(true);
            ui->anime8->setPixmap(pix);
        }
    }
    else{
        ui->anime8->clear();
        ui->anime8->setStyleSheet("background: transparent;");
    }
    if(i+8 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+8], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime9->setScaledContents(true);
            ui->anime9->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+8], configuracoes->diretorioImagensMedio), "png")){
            ui->anime9->setScaledContents(true);
            ui->anime9->setPixmap(pix);
        }
    }
    else{
        ui->anime9->clear();
        ui->anime9->setStyleSheet("background: transparent;");
    }
    if(i+9 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+9], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime10->setScaledContents(true);
            ui->anime10->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+9], configuracoes->diretorioImagensMedio), "png")){
            ui->anime10->setScaledContents(true);
            ui->anime10->setPixmap(pix);
        }
    }
    else{
        ui->anime10->clear();
        ui->anime10->setStyleSheet("background: transparent;");
    }
    if(i+10 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+10], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime11->setScaledContents(true);
            ui->anime11->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+10], configuracoes->diretorioImagensMedio), "png")){
            ui->anime11->setScaledContents(true);
            ui->anime11->setPixmap(pix);
        }
    }
    else{
        ui->anime11->clear();
        ui->anime11->setStyleSheet("background: transparent;");
    }
    if(i+11 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+11], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime12->setScaledContents(true);
            ui->anime12->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+11], configuracoes->diretorioImagensMedio), "png")){
            ui->anime12->setScaledContents(true);
            ui->anime12->setPixmap(pix);
        }
    }
    else{
        ui->anime12->clear();
        ui->anime12->setStyleSheet("background: transparent;");
    }
    if(i+12 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+12], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime13->setScaledContents(true);
            ui->anime13->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+12], configuracoes->diretorioImagensMedio), "png")){
            ui->anime13->setScaledContents(true);
            ui->anime13->setPixmap(pix);
        }
    }
    else{
        ui->anime13->clear();
        ui->anime13->setStyleSheet("background: transparent;");
    }
    if(i+13 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+13], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime14->setScaledContents(true);
            ui->anime14->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+13], configuracoes->diretorioImagensMedio), "png")){
            ui->anime14->setScaledContents(true);
            ui->anime14->setPixmap(pix);
        }
    }
    else{
        ui->anime14->clear();
        ui->anime14->setStyleSheet("background: transparent;");
    }
    if(i+14 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+14], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime15->setScaledContents(true);
            ui->anime15->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+14], configuracoes->diretorioImagensMedio), "png")){
            ui->anime15->setScaledContents(true);
            ui->anime15->setPixmap(pix);
        }
    }
    else{
        ui->anime15->clear();
        ui->anime15->setStyleSheet("background: transparent;");
    }
    if(i+15 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+15], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime16->setScaledContents(true);
            ui->anime16->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+15], configuracoes->diretorioImagensMedio), "png")){
            ui->anime16->setScaledContents(true);
            ui->anime16->setPixmap(pix);
        }
    }
    else{
        ui->anime16->clear();
        ui->anime16->setStyleSheet("background: transparent;");
    }
    if(i+16 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+16], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime17->setScaledContents(true);
            ui->anime17->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+16], configuracoes->diretorioImagensMedio), "png")){
            ui->anime17->setScaledContents(true);
            ui->anime17->setPixmap(pix);
        }
    }
    else{
        ui->anime17->clear();
        ui->anime17->setStyleSheet("background: transparent;");
    }
    if(i+17 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+17], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime18->setScaledContents(true);
            ui->anime18->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+17], configuracoes->diretorioImagensMedio), "png")){
            ui->anime18->setScaledContents(true);
            ui->anime18->setPixmap(pix);
        }
    }
    else{
        ui->anime18->clear();
        ui->anime18->setStyleSheet("background: transparent;");
    }
    if(i+18 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+18], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime19->setScaledContents(true);
            ui->anime19->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+18], configuracoes->diretorioImagensMedio), "png")){
            ui->anime19->setScaledContents(true);
            ui->anime19->setPixmap(pix);
        }
    }
    else{
        ui->anime19->clear();
        ui->anime19->setStyleSheet("background: transparent;");
    }
    if(i+19 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+19], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime20->setScaledContents(true);
            ui->anime20->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+19], configuracoes->diretorioImagensMedio), "png")){
            ui->anime20->setScaledContents(true);
            ui->anime20->setPixmap(pix);
        }
    }
    else{
        ui->anime20->clear();
        ui->anime20->setStyleSheet("background: transparent;");
    }
    if(i+20 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+20], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime21->setScaledContents(true);
            ui->anime21->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+20], configuracoes->diretorioImagensMedio), "png")){
            ui->anime21->setScaledContents(true);
            ui->anime21->setPixmap(pix);
        }
    }
    else{
        ui->anime21->clear();
        ui->anime21->setStyleSheet("background: transparent;");
    }
    if(i+21 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+21], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime22->setScaledContents(true);
            ui->anime22->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+21], configuracoes->diretorioImagensMedio), "png")){
            ui->anime22->setScaledContents(true);
            ui->anime22->setPixmap(pix);
        }
    }
    else{
        ui->anime22->clear();
        ui->anime22->setStyleSheet("background: transparent;");
    }
    if(i+22 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+22], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime23->setScaledContents(true);
            ui->anime23->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+22], configuracoes->diretorioImagensMedio), "png")){
            ui->anime23->setScaledContents(true);
            ui->anime23->setPixmap(pix);
        }
    }
    else{
        ui->anime23->clear();
        ui->anime23->setStyleSheet("background: transparent;");
    }
    if(i+23 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+23], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime24->setScaledContents(true);
            ui->anime24->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+23], configuracoes->diretorioImagensMedio), "png")){
            ui->anime24->setScaledContents(true);
            ui->anime24->setPixmap(pix);
        }
    }
    else{
        ui->anime24->clear();
        ui->anime24->setStyleSheet("background: transparent;");
    }
    if(i+24 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+24], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime25->setScaledContents(true);
            ui->anime25->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+24], configuracoes->diretorioImagensMedio), "png")){
            ui->anime25->setScaledContents(true);
            ui->anime25->setPixmap(pix);
        }
    }
    else{
        ui->anime25->clear();
        ui->anime25->setStyleSheet("background: transparent;");
    }
    if(i+25 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+25], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime26->setScaledContents(true);
            ui->anime26->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+25], configuracoes->diretorioImagensMedio), "png")){
            ui->anime26->setScaledContents(true);
            ui->anime26->setPixmap(pix);
        }
    }
    else{
        ui->anime26->clear();
        ui->anime26->setStyleSheet("background: transparent;");
    }
    if(i+26 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+26], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime27->setScaledContents(true);
            ui->anime27->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+26], configuracoes->diretorioImagensMedio), "png")){
            ui->anime27->setScaledContents(true);
            ui->anime27->setPixmap(pix);
        }
    }
    else{
        ui->anime27->clear();
        ui->anime27->setStyleSheet("background: transparent;");
    }
    if(i+27 < tamanhoLista){
        if(pix.load(leitorA->imagem(vetorAnimes[i+27], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime28->setScaledContents(true);
            ui->anime28->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+27], configuracoes->diretorioImagensMedio), "png")){
            ui->anime28->setScaledContents(true);
            ui->anime28->setPixmap(pix);
        }
    }
    else{
        ui->anime28->clear();
        ui->anime28->setStyleSheet("background: transparent;");
    }
}
