#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QObject>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();
    qDebug() << height << " - " << width;

    tamanhoListaOnHold = 0;
    tamanhoListaDropped = 0;
    tamanhoListaCompleted = 0;
    tamanhoListaPlanToWatch = 0;
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
//    leitorA = leitorWatching;
    leitorA = new leitorarquivos;
    configuracoes = new configPC();
    configuracoes->recebeJConfig(&jConfig);
    configuracoes->CriaPastasBase();
    runArquivo = new Config();
    runArquivo->setConfigs(configuracoes);
    runArquivo->IniciaThread(cThread);
    runArquivo->moveToThread(&cThread);
    cThread.start();

    connect(runArquivo, SIGNAL(mensagemConfig(QString)), &jConfig,SLOT(mensagem(QString)));
    connect(runArquivo, SIGNAL(refresh()),this,SLOT(refreshArquivo()));
    connect(runArquivo, SIGNAL(terminouSetArquivo()),this,SLOT(InstauraPrimeiraJanela()));
    connect(runArquivo, SIGNAL(terminouCompleted()),this,SLOT(LiberaBotaoCompleted()));
    connect(runArquivo, SIGNAL(terminouOnHold()),this,SLOT(LiberaBotaoOnHold()));
    connect(runArquivo, SIGNAL(terminouDropped()),this,SLOT(LiberaBotaoDropped()));
    connect(runArquivo, SIGNAL(terminouPlanToWatch()),this,SLOT(LiberaBotaoPlanToWatch()));

    anime0 = -1;
    numEpisodios = 0;

    ui->setupUi(this);
    ui->janela->addWidget(&jConfig);
    ui->janela->addWidget(&jtorrent);
//    ui->janela768->addWidget(&main768);
//    if(height == 1080){
//        ui->janela768->hide();
//    }
//    else if(height == 768){
//        ui->janela768->setCurrentIndex(1);
//    }

    jtorrent.getLeitorArquivos(leitorA);
    ui->StringBusca->setMaximumBlockCount(1);

    ui->label->setText("Carregando lista de animes");
    ui->nomeUsuario->setText(configuracoes->retornaUser());
    Botoes();
}

MainWindow::~MainWindow()
{
    cThread.quit();

    configuracoes->EscreveConfig();
    delete leitorWatching;
    delete leitorCompleted;
    delete leitorOnHold;
    delete leitorDropped;
    delete leitorPlanToWatch;
    delete leitorA;
    delete configuracoes;
    delete organiza;
    delete ui;
    delete pasta;
    delete runArquivo;

    delete qdown;
    delete qdownload;
    delete baixaOnHold;
    delete baixaDropped;
    delete baixaPlanToWatch;
    delete baixaBusca;

    delete downImagemGrandeWatching;
    delete downImagemGrandeCompleted;
    delete downImagemGrandeOnHold;
    delete downImagemGrandeDropped;
    delete downImagemGrandePlanToWatch;
    delete downImagemGrandeBusca;

    vetorAnimes.clear();
}

void MainWindow::keyPressEvent(QKeyEvent * event){
    if(event->key() == Qt::Key_U){
        mudaImagem();
    }
    else if(event->key() == Qt::Key_X){
        AbreEpisodio();
    }
}

void MainWindow::Botoes(){
    connect(ui->PaginaProxima, SIGNAL(clicked()),this,SLOT(mudaImagem()));
    connect(ui->PaginaProxima, SIGNAL(clicked()),this,SLOT(RestauraJanela()));
    connect(ui->PaginaAnterior, SIGNAL(clicked()),this,SLOT(voltaPagina()));
    connect(ui->BotaoConfig, SIGNAL(clicked()),this,SLOT(Configurar()));
    connect(ui->BotaoTorrent, SIGNAL(clicked()),this,SLOT(Torrent()));
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
    connect(ui->OrdemAnime, SIGNAL(currentIndexChanged(int)), this,SLOT(OrdenaVetor()));
    connect(ui->Busca, SIGNAL(clicked()), this,SLOT(BotaoBusca()));
    connect(ui->Refresh, SIGNAL(clicked()), this,SLOT(mandaRefresh()));
    connect(ui->BotaoPasta, SIGNAL(clicked()), this,SLOT(abrePasta()));
    connect(ui->BotaoAnilist, SIGNAL(clicked()), this,SLOT(abreAnilist()));

    connect(&jConfig, SIGNAL(user()), this, SLOT(setUser()));
    connect(&jConfig, SIGNAL(cancelado()), this, SLOT(ConfigCancelada()));
    connect(&jtorrent, SIGNAL(volta()), this, SLOT(voltaTorrent()));
}

void MainWindow::setUser(){
    ui->nomeUsuario->setText(configuracoes->retornaUser());
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
    ui->Watching->setStyleSheet("background: red;");
    leitorA->leLinha("watching");
    leitorA->OrdenaVetor(configuracoes->getOrdem());
    ordemVetorWatching = configuracoes->getOrdem();

    qdown = new QDownloader[leitorA->retornaTamanhoLista()];
    downImagemGrandeWatching = new QDownloader[leitorA->retornaTamanhoLista()];

    organiza = new Organizador(configuracoes);
    baixaImagens("watching");
    imagemBig();

    QFuture<void> future = QtConcurrent::run(this, &MainWindow::ConfiguraArquivos);
    tamanhoLista = leitorA->retornaTamanhoLista();
    tamanhoListaWatching = tamanhoLista;
    if(idAnime >= tamanhoLista){
        idAnime = tamanhoLista - 28;
    }
    anime0 = 0;
    for(int w = 0; w < tamanhoLista; w++){
        vetorAnimes.append(w);
    }
    carregaInfo();
    ui->Watching->setStyleSheet("background: red;");
    jtorrent.getOrganizador(organiza);
    connect(ui->Watching, SIGNAL(clicked()),this,SLOT(BotaoWatching()));
}

void MainWindow::BotaoWatching(){
    delete leitorA;

    ui->OrdemAnime->setCurrentIndex(0);

    ui->label->setText("Carregando!");
    lista = "watching";

    ui->Watching->setStyleSheet("background: red;");
    ui->Dropped->setStyleSheet("background: white;");
    ui->Completed->setStyleSheet("background: white;");
    ui->PlanToWatch->setStyleSheet("background: white;");
    ui->OnHold->setStyleSheet("background: white;");

    leitorA = new leitorarquivos;
    leitorA->leLinha("watching");
    leitorA->OrdenaVetor(ordemVetorWatching);
    tamanhoLista = leitorA->retornaTamanhoLista();

    if(tamanhoListaWatching != tamanhoLista){
        downImagemGrandeWatching = new QDownloader[leitorA->retornaTamanhoLista()];
        qdown = new QDownloader[leitorA->retornaTamanhoLista()];
        tamanhoLista = tamanhoLista;
    }


    if(idAnime >= tamanhoLista){
        idAnime = tamanhoLista - 28;
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
    delete leitorA;

    ui->OrdemAnime->setCurrentIndex(0);
    ui->label->setText("Carregando!");
    lista = "completed";

    ui->Completed->setStyleSheet("background: red;");
    ui->Dropped->setStyleSheet("background: white;");
    ui->PlanToWatch->setStyleSheet("background: white;");
    ui->OnHold->setStyleSheet("background: white;");
    ui->Watching->setStyleSheet("background: white;");

    leitorA = new leitorarquivos;
    leitorA->leLinha(lista);
    tamanhoLista = leitorA->retornaTamanhoLista();

    if(tamanhoListaCompleted != tamanhoLista){
        qdownload = new QDownloader[leitorA->retornaTamanhoLista()];
        downImagemGrandeCompleted = new QDownloader[leitorA->retornaTamanhoLista()];
        tamanhoListaCompleted = tamanhoLista;
    }


    if(idAnime >= tamanhoLista){
        idAnime = tamanhoLista - 28;
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

void MainWindow::BotaoOnHold(){
    delete leitorA;

    ui->OrdemAnime->setCurrentIndex(0);
    ui->label->setText("Carregando!");

    lista = "onhold";

    ui->OnHold->setStyleSheet("background: red;");
    ui->Dropped->setStyleSheet("background: white;");
    ui->Completed->setStyleSheet("background: white;");
    ui->PlanToWatch->setStyleSheet("background: white;");
    ui->Watching->setStyleSheet("background: white;");

    leitorA = new leitorarquivos;
    leitorA->leLinha(lista);
    tamanhoLista = leitorA->retornaTamanhoLista();

    if(tamanhoListaOnHold != tamanhoLista){
        baixaOnHold = new QDownloader[leitorA->retornaTamanhoLista()];
        downImagemGrandeOnHold= new QDownloader[leitorA->retornaTamanhoLista()];
        tamanhoListaOnHold = tamanhoLista;
    }

    if(idAnime >= tamanhoLista){
        idAnime = tamanhoLista - 28;
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
    delete leitorA;

    ui->OrdemAnime->setCurrentIndex(0);
    ui->label->setText("Carregando!");
    lista = "dropped";

    ui->Dropped->setStyleSheet("background: red;");
    ui->Completed->setStyleSheet("background: white;");
    ui->PlanToWatch->setStyleSheet("background: white;");
    ui->OnHold->setStyleSheet("background: white;");
    ui->Watching->setStyleSheet("background: white;");

    leitorA = new leitorarquivos;
    leitorA->leLinha(lista);
    tamanhoLista = leitorA->retornaTamanhoLista();

    if(tamanhoListaDropped != tamanhoLista){
        baixaDropped = new QDownloader[leitorA->retornaTamanhoLista()];
        downImagemGrandeDropped= new QDownloader[leitorA->retornaTamanhoLista()];
        tamanhoListaDropped = tamanhoLista;
    }


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
    delete leitorA;

    ui->OrdemAnime->setCurrentIndex(0);
    ui->label->setText("Carregando!");

    lista = "plantowatch";

    ui->PlanToWatch->setStyleSheet("background: red;");
    ui->Dropped->setStyleSheet("background: white;");
    ui->Completed->setStyleSheet("background: white;");
    ui->OnHold->setStyleSheet("background: white;");
    ui->Watching->setStyleSheet("background: white;");

    leitorA = new leitorarquivos;
    leitorA->leLinha(lista);
    tamanhoLista = leitorA->retornaTamanhoLista();

    if(tamanhoListaPlanToWatch != tamanhoLista){
        baixaPlanToWatch = new QDownloader[leitorA->retornaTamanhoLista()];
        downImagemGrandePlanToWatch= new QDownloader[leitorA->retornaTamanhoLista()];
        tamanhoListaPlanToWatch = tamanhoLista;
    }

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

void MainWindow::BotaoBusca(){
    delete leitorA;

    bool achou = false;
    int numEncontros;
    if(ui->StringBusca->toPlainText() != ""){
        ui->OrdemAnime->setCurrentIndex(0);
        ui->label->setText("Buscando " + ui->StringBusca->toPlainText());
        lista = "busca";

        ui->PlanToWatch->setStyleSheet("background: white;");
        ui->Dropped->setStyleSheet("background: white;");
        ui->Completed->setStyleSheet("background: white;");
        ui->OnHold->setStyleSheet("background: white;");
        ui->Watching->setStyleSheet("background: white;");

        leitorA = new leitorarquivos;
        numEncontros = leitorA->busca(ui->StringBusca->toPlainText());

        if(numEncontros != 0){
            tamanhoLista = leitorA->retornaTamanhoLista();

            baixaBusca = new QDownloader[leitorA->retornaTamanhoLista()];
            downImagemGrandeBusca = new QDownloader[leitorA->retornaTamanhoLista()];
            imagemBig();
            if(idAnime >= tamanhoLista){
                idAnime = tamanhoLista - 28;
            }
            anime0 = 0;
            idAnime = 0;
            pagina = 1;
            vetorAnimes.clear();
            for(int w = 0; w < tamanhoLista; w++){
                vetorAnimes.append(w);
            }
            carregaInfo();
            ui->label->setText("Todos os animes foram encontrados");
            ui->StringBusca->clear();
            achou = true;
        }
    }
    else{
        ui->label->setText("Nada foi procurado");
    }
    if(achou == false)
        ui->label->setText("Nenhum anime com essa palavra chave foi encontrado");
}

void MainWindow::OrdenaVetor(){
    if(ui->OrdemAnime->currentIndex() != 0){
        if(lista == "watching"){
            configuracoes->setOrdem(ui->OrdemAnime->currentText());
            ordemVetorWatching = ui->OrdemAnime->currentText();
        }
        else if(lista == "completed")
            ordemVetorCompleted = ui->OrdemAnime->currentText();
        else if(lista == "dropped")
            ordemVetorDropped = ui->OrdemAnime->currentText();
        else if(lista == "onhold")
            ordemVetorPaused = ui->OrdemAnime->currentText();
        else if(lista == "plantowatch")
            ordemVetorPlantoWatch = ui->OrdemAnime->currentText();
        leitorA->OrdenaVetor(ui->OrdemAnime->currentText());
        carregaInfo();
    }
}

void MainWindow::mandaRefresh(){
    runArquivo->botaoRefresh();
}

void MainWindow::refreshArquivo(){
    //O download de imagens só acontece caso a lista tenha novos animes
    //e isso vai garantir que a função vai rodar toda vez que der refresh
    tamanhoListaWatching = 0;
    tamanhoListaOnHold = 0;
    tamanhoListaDropped = 0;
    tamanhoListaCompleted = 0;
    tamanhoListaPlanToWatch = 0;
    QString currentList = lista;
    BotaoWatching();
    BotaoCompleted();
    BotaoOnHold();
    BotaoDropped();
    BotaoPlanToWatch();
    if(currentList == "watching"){
        BotaoWatching();
    }
    else if(currentList == "completed"){
        BotaoCompleted();
    }
    else if(currentList == "onhold"){
        BotaoOnHold();
    }
    else if(currentList == "dropped"){
        BotaoDropped();
    }
    else if(currentList == "plantowatch"){
        BotaoPlanToWatch();
    }
}

void MainWindow::RestauraJanela(){
    imagemBig();
    baixaImagens(lista);
    carregaInfo();
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

void MainWindow::abrePasta(){
    ui->label->clear();
    if(organiza->abrePasta(configuracoes->RetornaDiretorioAnimeEspecifico(leitorA->retornaId(anime0).toInt())) == 1)
        ui->label->setText("Pasta não encontrada");
}

void MainWindow::abreAnilist(){
    QDesktopServices::openUrl(QUrl("https://anilist.co/anime/"+ leitorA->retornaId(anime0),QUrl::TolerantMode));
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
    else if(lista == "busca"){
        for(int i = 0; i < leitorA->retornaTamanhoLista();i++){
            downImagemGrandeBusca[i].setFileBig(leitorA->retornaLink(i), leitorA->retornaId(i));
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
    delete leitorconf;
}

void MainWindow::Configurar(){
    ui->Watching->blockSignals(true);
    ui->Completed->blockSignals(true);
    ui->Dropped->blockSignals(true);
    ui->OnHold->blockSignals(true);
    ui->PlanToWatch->blockSignals(true);
    ui->Refresh->blockSignals(true);
    ui->MudarListaBotao->blockSignals(true);
    ui->MudarLista->blockSignals(true);
    ui->OrdemAnime->blockSignals(true);
    ui->Busca->blockSignals(true);
    ui->Watching->setStyleSheet("background: rgb(121, 121, 121);");
    ui->Completed->setStyleSheet("background: rgb(121, 121, 121);");
    ui->Dropped->setStyleSheet("background: rgb(121, 121, 121);");
    ui->OnHold->setStyleSheet("background: rgb(121, 121, 121);");
    ui->PlanToWatch->setStyleSheet("background: rgb(121, 121, 121);");
    ui->Refresh->setStyleSheet("background: rgb(121, 121, 121);");
    ui->MudarListaBotao->setStyleSheet("background: rgb(121, 121, 121);");
    ui->Busca->setStyleSheet("background: rgb(121, 121, 121);");
    ui->janela->setCurrentIndex(1);
}
void MainWindow::Torrent(){
    ui->Watching->blockSignals(true);
    ui->Completed->blockSignals(true);
    ui->Dropped->blockSignals(true);
    ui->OnHold->blockSignals(true);
    ui->PlanToWatch->blockSignals(true);
    ui->Refresh->blockSignals(true);
    ui->MudarListaBotao->blockSignals(true);
    ui->MudarLista->blockSignals(true);
    ui->OrdemAnime->blockSignals(true);
    ui->Busca->blockSignals(true);
    ui->Watching->setStyleSheet("background: rgb(121, 121, 121);");
    ui->Completed->setStyleSheet("background: rgb(121, 121, 121);");
    ui->Dropped->setStyleSheet("background: rgb(121, 121, 121);");
    ui->OnHold->setStyleSheet("background: rgb(121, 121, 121);");
    ui->PlanToWatch->setStyleSheet("background: rgb(121, 121, 121);");
    ui->Refresh->setStyleSheet("background: rgb(121, 121, 121);");
    ui->MudarListaBotao->setStyleSheet("background: rgb(121, 121, 121);");
    ui->Busca->setStyleSheet("background: rgb(121, 121, 121);");
    ui->janela->setCurrentIndex(2);
}

void MainWindow::ConfigCancelada(){
    ui->janela->setCurrentIndex(0);
}
void MainWindow::voltaTorrent(){
    ui->janela->setCurrentIndex(0);
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
    numEpisodios = organiza->retornaNumEpiNaPasta(numEpisodios, leitorA->retornaId(anime0).toInt(), leitorA->retornaNumEpi(anime0).toInt());
    qEpiDisponivel = organiza->retornaEpisodiosDisponiveis();
    //Caso tenham episódios disponíveis na pasta
    if(qEpiDisponivel.empty() == false){
        //Caso todos os episódios estejam disponíveis, organiza o texto e mostra no label
        //Isso poupa memória, já que não tem que passar pelo loop
        if(qEpiDisponivel.length() == leitorA->retornaNumEpi(anime0).toInt()){
            //Mostra como EPI1 to ULTIMO EPI
            ui->qEpiDisponivel->setText("1-" + leitorA->retornaNumEpi(anime0));
        }
        //Caso não sejam todos que estejam disponíveis, mas tiver mais de um episódio
        else if(qEpiDisponivel.length() >= 1){
            //Passa primeiro episódio encontrado pra string
            QString epidis = QString::number(qEpiDisponivel[0]);
            //Começa a ver quais episódios estão disponíveis
            for(int i = 1; i < qEpiDisponivel.length(); i++){
                //Caso ache algum erro no vetor de episódios, exclui
                //Erros são sempre mostrados como 0
                if(qEpiDisponivel[i] == 0){
                    qEpiDisponivel.remove(i);
                }
                //Compara os episódios. Caso sejam seguidos, ele ignora
                //Caso não sejam seguidos, é adicionado na string " TO XXX EPIDODE", onde o próximo episódio está faltando
                if(qEpiDisponivel[i-1]+1 != qEpiDisponivel[i]){
                    epidis.append("-" + QString::number(qEpiDisponivel[i-1]) + ", " + QString::number(qEpiDisponivel[i]));
                }
                //Caso chegue no fim do vetor, ele adiciona o último episódio disponível à string
                else if(i == qEpiDisponivel.length()-1){
                    epidis.append("-" + QString::number(qEpiDisponivel[i]));
                }
            }
            ui->qEpiDisponivel->setText(epidis);
        }
    }
    //Caso não tenham episódios na pasta, o vetor está vazio
    else{
        ui->qEpiDisponivel->clear();//setText("Nenhum episódio disponível");
    }
    ui->ProxEpi->setText(QString::number(leitorA->retornaProgresso(anime0)) + "/" + leitorA->retornaNumEpi(anime0));
    ui->Progresso_2->setText(QString::number(leitorA->retornaProgresso(anime0)) + "/" + leitorA->retornaNumEpi(anime0));
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
    ui->Nota_2->setText(QString::number(leitorA->retornaScore(anime0)));
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
        ui->anime1->setScaledContents(true);
        ui->anime1_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime1_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime1->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i], configuracoes->diretorioImagensMedio), "png")){
            ui->anime1->setPixmap(pix);
        }
    }
    else{
        ui->anime1->clear();
        ui->anime1->setStyleSheet("background: transparent;");
        ui->anime1_2->setStyleSheet("background: transparent;");
        ui->anime1_2->clear();
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
        ui->anime2->setScaledContents(true);
        ui->anime2_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime2_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+1], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime2->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+1], configuracoes->diretorioImagensMedio), "png")){
            ui->anime2->setPixmap(pix);
        }
    }
    else{
        ui->anime2->clear();
        ui->anime2->setStyleSheet("background: transparent;");
        ui->anime2_2->setStyleSheet("background: transparent;");
        ui->anime2_2->clear();
    }
    if(i+2 < tamanhoLista){
        ui->anime3->setScaledContents(true);
        ui->anime3_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime3_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+2], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime3->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+2], configuracoes->diretorioImagensMedio), "png")){
            ui->anime3->setPixmap(pix);
        }
    }
    else{
        ui->anime3->clear();
        ui->anime3->setStyleSheet("background: transparent;");
        ui->anime3_2->setStyleSheet("background: transparent;");
        ui->anime3_2->clear();
    }
    if(i+3 < tamanhoLista){
        ui->anime4->setScaledContents(true);
        ui->anime4_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime4_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+3], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime4->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+3], configuracoes->diretorioImagensMedio), "png")){
            ui->anime4->setPixmap(pix);
        }
    }
    else{
        ui->anime4->clear();
        ui->anime4->setStyleSheet("background: transparent;");
        ui->anime4_2->setStyleSheet("background: transparent;");
        ui->anime4_2->clear();
    }
    if(i+4 < tamanhoLista){
        ui->anime5->setScaledContents(true);
        ui->anime5_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime5_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+4], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime5->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+4], configuracoes->diretorioImagensMedio), "png")){
            ui->anime5->setPixmap(pix);
        }
    }
    else{
        ui->anime5->clear();
        ui->anime5->setStyleSheet("background: transparent;");
        ui->anime5_2->setStyleSheet("background: transparent;");
        ui->anime5_2->clear();
    }
    if(i+5 < tamanhoLista){
        ui->anime6->setScaledContents(true);
        ui->anime6_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime6_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+5], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime6->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+5], configuracoes->diretorioImagensMedio), "png")){
            ui->anime6->setPixmap(pix);
        }
    }
    else{
        ui->anime6->clear();
        ui->anime6->setStyleSheet("background: transparent;");
        ui->anime6_2->setStyleSheet("background: transparent;");
        ui->anime6_2->clear();
    }
    if(i+6 < tamanhoLista){
        ui->anime7->setScaledContents(true);
        ui->anime7_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime7_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+6], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime7->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+6], configuracoes->diretorioImagensMedio), "png")){
            ui->anime7->setPixmap(pix);
        }
    }
    else{
        ui->anime7->clear();
        ui->anime7->setStyleSheet("background: transparent;");
        ui->anime7_2->setStyleSheet("background: transparent;");
        ui->anime7_2->clear();
    }
    if(i+7 < tamanhoLista){
        ui->anime8->setScaledContents(true);
        ui->anime8_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime8_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+7], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime8->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+7], configuracoes->diretorioImagensMedio), "png")){
            ui->anime8->setPixmap(pix);
        }
    }
    else{
        ui->anime8->clear();
        ui->anime8->setStyleSheet("background: transparent;");
        ui->anime8_2->setStyleSheet("background: transparent;");
        ui->anime8_2->clear();
    }
    if(i+8 < tamanhoLista){
        ui->anime9->setScaledContents(true);
        ui->anime9_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime9_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+8], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime9->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+8], configuracoes->diretorioImagensMedio), "png")){
            ui->anime9->setPixmap(pix);
        }
    }
    else{
        ui->anime9->clear();
        ui->anime9->setStyleSheet("background: transparent;");
        ui->anime9_2->setStyleSheet("background: transparent;");
        ui->anime9_2->clear();
    }
    if(i+9 < tamanhoLista){
        ui->anime10->setScaledContents(true);
        ui->anime10_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime10_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+9], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime10->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+9], configuracoes->diretorioImagensMedio), "png")){
            ui->anime10->setPixmap(pix);
        }
    }
    else{
        ui->anime10->clear();
        ui->anime10->setStyleSheet("background: transparent;");
        ui->anime10_2->setStyleSheet("background: transparent;");
        ui->anime10_2->clear();
    }
    if(i+10 < tamanhoLista){
        ui->anime11->setScaledContents(true);
        ui->anime11_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime11_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+10], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime11->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+10], configuracoes->diretorioImagensMedio), "png")){
            ui->anime11->setPixmap(pix);
        }
    }
    else{
        ui->anime11->clear();
        ui->anime11->setStyleSheet("background: transparent;");
        ui->anime11_2->setStyleSheet("background: transparent;");
        ui->anime11_2->clear();
    }
    if(i+11 < tamanhoLista){
        ui->anime12->setScaledContents(true);
        ui->anime12_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime12_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+11], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime12->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+11], configuracoes->diretorioImagensMedio), "png")){
            ui->anime12->setPixmap(pix);
        }
    }
    else{
        ui->anime12->clear();
        ui->anime12->setStyleSheet("background: transparent;");
        ui->anime12_2->setStyleSheet("background: transparent;");
        ui->anime12_2->clear();
    }
    if(i+12 < tamanhoLista){
        ui->anime13->setScaledContents(true);
        ui->anime13_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime13_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+12], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime13->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+12], configuracoes->diretorioImagensMedio), "png")){
            ui->anime13->setPixmap(pix);
        }
    }
    else{
        ui->anime13->clear();
        ui->anime13->setStyleSheet("background: transparent;");
        ui->anime13_2->setStyleSheet("background: transparent;");
        ui->anime13_2->clear();
    }
    if(i+13 < tamanhoLista){
        ui->anime14->setScaledContents(true);
        ui->anime14_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime14_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+13], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime14->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+13], configuracoes->diretorioImagensMedio), "png")){
            ui->anime14->setPixmap(pix);
        }
    }
    else{
        ui->anime14->clear();
        ui->anime14->setStyleSheet("background: transparent;");
        ui->anime14_2->setStyleSheet("background: transparent;");
        ui->anime14_2->clear();
    }
    if(i+14 < tamanhoLista){
        ui->anime15->setScaledContents(true);
        ui->anime15_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime15_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+14], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime15->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+14], configuracoes->diretorioImagensMedio), "png")){
            ui->anime15->setPixmap(pix);
        }
    }
    else{
        ui->anime15->clear();
        ui->anime15->setStyleSheet("background: transparent;");
        ui->anime15_2->setStyleSheet("background: transparent;");
        ui->anime15_2->clear();
    }
    if(i+15 < tamanhoLista){
        ui->anime16_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime16_2->setText(leitorA->retornaNome(i));
        ui->anime16->setScaledContents(true);
        if(pix.load(leitorA->imagem(vetorAnimes[i+15], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime16->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+15], configuracoes->diretorioImagensMedio), "png")){
            ui->anime16->setPixmap(pix);
        }
    }
    else{
        ui->anime16->clear();
        ui->anime16->setStyleSheet("background: transparent;");
        ui->anime16_2->setStyleSheet("background: transparent;");
        ui->anime16_2->clear();
    }
    if(i+16 < tamanhoLista){
        ui->anime17->setScaledContents(true);
        ui->anime17_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime17_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+16], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime17->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+16], configuracoes->diretorioImagensMedio), "png")){
            ui->anime17->setPixmap(pix);
        }
    }
    else{
        ui->anime17->clear();
        ui->anime17->setStyleSheet("background: transparent;");
        ui->anime17_2->setStyleSheet("background: transparent;");
        ui->anime17_2->clear();
    }
    if(i+17 < tamanhoLista){
        ui->anime18->setScaledContents(true);
        ui->anime18_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime18_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+17], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime18->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+17], configuracoes->diretorioImagensMedio), "png")){
            ui->anime18->setPixmap(pix);
        }
    }
    else{
        ui->anime18->clear();
        ui->anime18->setStyleSheet("background: transparent;");
        ui->anime18_2->setStyleSheet("background: transparent;");
        ui->anime18_2->clear();
    }
    if(i+18 < tamanhoLista){
        ui->anime19->setScaledContents(true);
        ui->anime19_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime19_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+18], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime19->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+18], configuracoes->diretorioImagensMedio), "png")){
            ui->anime19->setPixmap(pix);
        }
    }
    else{
        ui->anime19->clear();
        ui->anime19->setStyleSheet("background: transparent;");
        ui->anime19_2->setStyleSheet("background: transparent;");
        ui->anime19_2->clear();
    }
    if(i+19 < tamanhoLista){
        ui->anime20->setScaledContents(true);
        ui->anime20_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime20_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+19], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime20->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+19], configuracoes->diretorioImagensMedio), "png")){
            ui->anime20->setPixmap(pix);
        }
    }
    else{
        ui->anime20->clear();
        ui->anime20->setStyleSheet("background: transparent;");
        ui->anime20_2->setStyleSheet("background: transparent;");
        ui->anime20_2->clear();
    }
    if(i+20 < tamanhoLista){
        ui->anime21->setScaledContents(true);
        ui->anime21_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime21_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+20], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime21->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+20], configuracoes->diretorioImagensMedio), "png")){
            ui->anime21->setPixmap(pix);
        }
    }
    else{
        ui->anime21->clear();
        ui->anime21->setStyleSheet("background: transparent;");
        ui->anime21_2->setStyleSheet("background: transparent;");
        ui->anime21_2->clear();
    }
    if(i+21 < tamanhoLista){
        ui->anime22->setScaledContents(true);
        ui->anime22_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime22_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+21], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime22->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+21], configuracoes->diretorioImagensMedio), "png")){
            ui->anime22->setPixmap(pix);
        }
    }
    else{
        ui->anime22->clear();
        ui->anime22->setStyleSheet("background: transparent;");
        ui->anime22_2->setStyleSheet("background: transparent;");
        ui->anime22_2->clear();
    }
    if(i+22 < tamanhoLista){
        ui->anime23->setScaledContents(true);
        ui->anime23_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime23_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+22], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime23->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+22], configuracoes->diretorioImagensMedio), "png")){
            ui->anime23->setPixmap(pix);
        }
    }
    else{
        ui->anime23->clear();
        ui->anime23->setStyleSheet("background: transparent;");
        ui->anime23_2->setStyleSheet("background: transparent;");
        ui->anime23_2->clear();
    }
    if(i+23 < tamanhoLista){
        ui->anime24->setScaledContents(true);
        ui->anime24_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime24_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+23], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime24->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+23], configuracoes->diretorioImagensMedio), "png")){
            ui->anime24->setPixmap(pix);
        }
    }
    else{
        ui->anime24->clear();
        ui->anime24->setStyleSheet("background: transparent;");
        ui->anime24_2->setStyleSheet("background: transparent;");
        ui->anime24_2->clear();
    }
    if(i+24 < tamanhoLista){
        ui->anime25->setScaledContents(true);
        ui->anime25_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime25_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+24], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime25->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+24], configuracoes->diretorioImagensMedio), "png")){
            ui->anime25->setPixmap(pix);
        }
    }
    else{
        ui->anime25->clear();
        ui->anime25->setStyleSheet("background: transparent;");
        ui->anime25_2->setStyleSheet("background: transparent;");
        ui->anime25_2->clear();
    }
    if(i+25 < tamanhoLista){
        ui->anime26->setScaledContents(true);
        ui->anime26_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime26_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+25], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime26->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+25], configuracoes->diretorioImagensMedio), "png")){
            ui->anime26->setPixmap(pix);
        }
    }
    else{
        ui->anime26->clear();
        ui->anime26->setStyleSheet("background: transparent;");
        ui->anime26_2->setStyleSheet("background: transparent;");
        ui->anime26_2->clear();
    }
    if(i+26 < tamanhoLista){
        ui->anime27->setScaledContents(true);
        ui->anime27_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime27_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+26], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime27->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+26], configuracoes->diretorioImagensMedio), "png")){
            ui->anime27->setPixmap(pix);
        }
    }
    else{
        ui->anime27->clear();
        ui->anime27->setStyleSheet("background: transparent;");
        ui->anime27_2->setStyleSheet("background: transparent;");
        ui->anime27_2->clear();
    }
    if(i+27 < tamanhoLista){
        ui->anime28->setScaledContents(true);
        ui->anime28_2->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime28_2->setText(leitorA->retornaNome(i));
        if(pix.load(leitorA->imagem(vetorAnimes[i+27], configuracoes->diretorioImagensMedio), "jpg")){
            ui->anime28->setPixmap(pix);
        }
        else if(pix.load(leitorA->imagem(vetorAnimes[i+27], configuracoes->diretorioImagensMedio), "png")){
            ui->anime28->setPixmap(pix);
        }
    }
    else{
        ui->anime28->clear();
        ui->anime28->setStyleSheet("background: transparent;");
        ui->anime28_2->setStyleSheet("background: transparent;");
        ui->anime28_2->clear();
    }
}

void MainWindow::on_NotaMais_clicked()
{
    if(ui->Nota->text().toInt() < 100){
        ui->Nota->setText(QString::number(ui->Nota->text().toInt()+10));
        ui->Nota_2->setText(ui->Nota->text());
    }
}

void MainWindow::on_NotaMenos_clicked()
{
    if(ui->Nota->text().toInt() > 0){
        ui->Nota->setText(QString::number(ui->Nota->text().toInt()-10));
        ui->Nota_2->setText(ui->Nota->text());
    }
}

void MainWindow::on_ProgressoMais_clicked()
{
    QStringList progressoQuebrado = ui->ProxEpi->text().split('/');
    if(progressoQuebrado.at(0).toInt() < leitorA->retornaNumEpi(anime0).toInt()){
        ui->ProxEpi->setText(QString::number(progressoQuebrado.at(0).toInt()+1) + "/" + progressoQuebrado.at(1));
        ui->Progresso_2->setText(ui->ProxEpi->text());
    }

}

void MainWindow::on_ProgressoMenos_clicked()
{
    QStringList progressoQuebrado = ui->ProxEpi->text().split('/');
    if(progressoQuebrado.at(0).toInt() > 0){
        ui->ProxEpi->setText(QString::number(progressoQuebrado.at(0).toInt()-1) + "/" + progressoQuebrado.at(1));
        ui->Progresso_2->setText(ui->ProxEpi->text());
    }
}

void MainWindow::on_Lista_clicked()
{
    ui->Watching->blockSignals(false);
    ui->Completed->blockSignals(false);
    ui->Dropped->blockSignals(false);
    ui->OnHold->blockSignals(false);
    ui->PlanToWatch->blockSignals(false);
    ui->Refresh->blockSignals(false);
    ui->MudarListaBotao->blockSignals(false);
    ui->MudarLista->blockSignals(false);
    ui->OrdemAnime->blockSignals(false);
    ui->Busca->blockSignals(false);

    ui->Watching->setStyleSheet("background: white;");
    ui->Completed->setStyleSheet("background: white;");
    ui->Dropped->setStyleSheet("background: white;");
    ui->OnHold->setStyleSheet("background: white;");
    ui->PlanToWatch->setStyleSheet("background: white;");
    ui->Refresh->setStyleSheet("background: white;");
    ui->MudarListaBotao->setStyleSheet("background: white;");
    ui->Busca->setStyleSheet("background: white;");

    if(lista == "watching")
        ui->Watching->setStyleSheet("background: red;");
    else if(lista == "completed")
        ui->Completed->setStyleSheet("background: red;");
    else if(lista == "onhold")
        ui->OnHold->setStyleSheet("background: red;");
    else if(lista == "dropped")
        ui->Dropped->setStyleSheet("background: red;");
    else if(lista == "plantowatch")
        ui->PlanToWatch->setStyleSheet("background: red;");
    ui->janela->setCurrentIndex(0);
}
