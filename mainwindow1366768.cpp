#include "mainwindow1366768.h"
#include "ui_mainwindow1366768.h"

mainwindow1366768::mainwindow1366768(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mainwindow1366768)
{
    anime0 = -1;
    idAnime = 0;
    pagina = 1;
    downl = 0;
    primeiraLeitura = false;
    baixaQualidade = false;

    lista = "watching";
    leitorA = new leitorarquivos;
    configuracoes = new configPC();
    configuracoes->recebeJConfig(&jConfig);
    configuracoes->CriaPastasBase();

    runArquivo = new Config();

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
    jtorrent.getLeitorArquivos(leitorA);
    jtorrent.getJConfig(&jConfig);
    ui->StringBusca->setMaximumBlockCount(1);

    ui->label->setText("Carregando lista de animes");
    if(configuracoes->retornaUser() != ""){
        setUser();
    }

    Botoes();
    QTimer::singleShot(600000, this, SLOT(mandaRefresh()));
}

mainwindow1366768::~mainwindow1366768()
{
//    configuracoes->EscreveConfig();
    delete leitorA;
//    runArquivo->deleteLater();
//    configuracoes->deleteLater();

//    delete DownImagemPequenaListas;
//    delete DownImagemGrandeListas;
//    delete DownImagemListas;
//    delete organiza;
    delete ui;
}

void mainwindow1366768::keyPressEvent(QKeyEvent * event){
    if(event->key() == Qt::Key_U){
//        mudaImagem();
    }
    else if(event->key() == Qt::Key_X){
        AbreEpisodio();
    }
}

void mainwindow1366768::Botoes(){
    connect(ui->Refresh, SIGNAL(clicked()), this,SLOT(mandaRefresh()));
    connect(ui->BotaoTorrent, SIGNAL(clicked()),this,SLOT(Torrent()));
    connect(ui->BotaoConfig, SIGNAL(clicked()),this,SLOT(Configurar()));

    connect(ui->clickAnime1_3, SIGNAL(clicked()),this,SLOT(carregaAnime1()));
    connect(ui->clickAnime2_3, SIGNAL(clicked()),this,SLOT(carregaAnime2()));
    connect(ui->clickAnime3_3, SIGNAL(clicked()),this,SLOT(carregaAnime3()));
    connect(ui->clickAnime4_3, SIGNAL(clicked()),this,SLOT(carregaAnime4()));
    connect(ui->clickAnime5_3, SIGNAL(clicked()),this,SLOT(carregaAnime5()));
    connect(ui->clickAnime6_3, SIGNAL(clicked()),this,SLOT(carregaAnime6()));
    connect(ui->clickAnime7_3, SIGNAL(clicked()),this,SLOT(carregaAnime7()));
    connect(ui->clickAnime8_3, SIGNAL(clicked()),this,SLOT(carregaAnime8()));
    connect(ui->clickAnime9_3, SIGNAL(clicked()),this,SLOT(carregaAnime9()));
    connect(ui->clickAnime10_3, SIGNAL(clicked()),this,SLOT(carregaAnime10()));
    connect(ui->clickAnime11_3, SIGNAL(clicked()),this,SLOT(carregaAnime11()));
    connect(ui->clickAnime12_3, SIGNAL(clicked()),this,SLOT(carregaAnime12()));
    connect(ui->clickAnime13_3, SIGNAL(clicked()),this,SLOT(carregaAnime13()));
    connect(ui->clickAnime14_3, SIGNAL(clicked()),this,SLOT(carregaAnime14()));
    connect(ui->clickAnime15_3, SIGNAL(clicked()),this,SLOT(carregaAnime15()));
    connect(ui->clickAnime16_3, SIGNAL(clicked()),this,SLOT(carregaAnime16()));
    connect(ui->clickAnime17_3, SIGNAL(clicked()),this,SLOT(carregaAnime17()));
    connect(ui->clickAnime18_3, SIGNAL(clicked()),this,SLOT(carregaAnime18()));
    connect(ui->clickAnime19_3, SIGNAL(clicked()),this,SLOT(carregaAnime19()));
    connect(ui->clickAnime20_3, SIGNAL(clicked()),this,SLOT(carregaAnime20()));
    connect(ui->clickAnime21_3, SIGNAL(clicked()),this,SLOT(carregaAnime21()));
    connect(ui->clickAnime22_3, SIGNAL(clicked()),this,SLOT(carregaAnime22()));
    connect(ui->clickAnime23_3, SIGNAL(clicked()),this,SLOT(carregaAnime23()));
    connect(ui->clickAnime24_3, SIGNAL(clicked()),this,SLOT(carregaAnime24()));
    connect(ui->clickAnime25_3, SIGNAL(clicked()),this,SLOT(carregaAnime25()));
    connect(ui->clickAnime26_3, SIGNAL(clicked()),this,SLOT(carregaAnime26()));
    connect(ui->clickAnime27_3, SIGNAL(clicked()),this,SLOT(carregaAnime27()));
    connect(ui->clickAnime28_3, SIGNAL(clicked()),this,SLOT(carregaAnime28()));

    connect(ui->PaginaProxima_3, SIGNAL(clicked()),this,SLOT(proximaPagina()));
    connect(ui->PaginaAnterior_3, SIGNAL(clicked()),this,SLOT(voltaPagina()));
    connect(ui->AbreEpi_3, SIGNAL(clicked()),this,SLOT(AbreEpisodio()));
    connect(ui->OrdemAnime, SIGNAL(currentIndexChanged(int)), this,SLOT(OrdenaVetor()));
    connect(ui->Busca, SIGNAL(clicked()), this,SLOT(BotaoBusca()));
    connect(ui->BotaoPasta_3, SIGNAL(clicked()), this,SLOT(abrePasta()));
    connect(ui->BotaoAnilist_3, SIGNAL(clicked()), this,SLOT(abreAnilist()));
    connect(ui->DownloadAnime_3, SIGNAL(clicked()),this,SLOT(BuscaTorrentAnimeEspecifico()));

    connect(&jConfig, SIGNAL(user()), this, SLOT(setUser()));
    connect(&jConfig, SIGNAL(bDownload(int)), this, SLOT(mudouQualidade(int)));
    connect(&jConfig, SIGNAL(tPadrao(QString)), &jtorrent, SLOT(mudaTorrentPadrao(QString)));
}

void mainwindow1366768::setUser(){
    ui->nomeUsuario->setText(configuracoes->retornaUser());
    runArquivo->setConfigs(configuracoes);
    runArquivo->IniciaThread(cThread);
    runArquivo->moveToThread(&cThread);
    cThread.start();
}

void mainwindow1366768::LiberaBotaoCompleted(){
    ui->Completed->setStyleSheet("background: white;");
    connect(ui->Completed, SIGNAL(clicked()),this,SLOT(BotaoCompleted()));
}
void mainwindow1366768::LiberaBotaoOnHold(){
    ui->OnHold->setStyleSheet("background: white;");
    connect(ui->OnHold, SIGNAL(clicked()),this,SLOT(BotaoOnHold()));
}
void mainwindow1366768::LiberaBotaoDropped(){
    ui->Dropped->setStyleSheet("background: white;");
    connect(ui->Dropped, SIGNAL(clicked()),this,SLOT(BotaoDropped()));
}
void mainwindow1366768::LiberaBotaoPlanToWatch(){
    ui->PlanToWatch->setStyleSheet("background: white;");
    connect(ui->PlanToWatch, SIGNAL(clicked()),this,SLOT(BotaoPlanToWatch()));
    if(primeiraLeitura == false){
        ui->label->setText("tudo pronto! - Se as imagens não carregarem, estão sendo baixadas. Dê um refresh que talvez elas apareçam!");
        primeiraLeitura = true;
    }
    baixaImagens();
}

void mainwindow1366768::InstauraPrimeiraJanela(){
    anime0 = 0;

    ui->label->setText("Carregando!");

    leitorA->leLinha("watching");
    ordemVetorWatching = configuracoes->getOrdem();
    leitorA->OrdenaVetor(ordemVetorWatching);

    organiza = new Organizador(configuracoes);

    QFuture<void> future = QtConcurrent::run(this, &mainwindow1366768::ConfiguraArquivos);
    tamanhoLista = leitorA->retornaTamanhoLista();
    if(idAnime >= tamanhoLista){
        idAnime = tamanhoLista - 28;
    }

    for(int w = 0; w < tamanhoLista; w++){
        vetorAnimes.append(w);
    }

    ui->Watching->setStyleSheet("background: red;");
    jtorrent.getOrganizador(organiza);
    carregaInfo();
    connect(ui->Watching, SIGNAL(clicked()),this,SLOT(BotaoWatching()));
}

void mainwindow1366768::BotaoWatching(){
    delete leitorA;

    ui->OrdemAnime->setCurrentIndex(0);

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
}

void mainwindow1366768::BotaoCompleted(){
    delete leitorA;

    ui->OrdemAnime->setCurrentIndex(0);
    lista = "completed";

    ui->Completed->setStyleSheet("background: red;");
    ui->Dropped->setStyleSheet("background: white;");
    ui->PlanToWatch->setStyleSheet("background: white;");
    ui->OnHold->setStyleSheet("background: white;");
    ui->Watching->setStyleSheet("background: white;");

    leitorA = new leitorarquivos;
    leitorA->leLinha(lista);
    tamanhoLista = leitorA->retornaTamanhoLista();

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
}

void mainwindow1366768::BotaoOnHold(){
    delete leitorA;

    ui->OrdemAnime->setCurrentIndex(0);

    lista = "onhold";

    ui->OnHold->setStyleSheet("background: red;");
    ui->Dropped->setStyleSheet("background: white;");
    ui->Completed->setStyleSheet("background: white;");
    ui->PlanToWatch->setStyleSheet("background: white;");
    ui->Watching->setStyleSheet("background: white;");

    leitorA = new leitorarquivos;
    leitorA->leLinha(lista);
    tamanhoLista = leitorA->retornaTamanhoLista();

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
}
void mainwindow1366768::BotaoDropped(){
    delete leitorA;

    ui->OrdemAnime->setCurrentIndex(0);
    lista = "dropped";

    ui->Dropped->setStyleSheet("background: red;");
    ui->Completed->setStyleSheet("background: white;");
    ui->PlanToWatch->setStyleSheet("background: white;");
    ui->OnHold->setStyleSheet("background: white;");
    ui->Watching->setStyleSheet("background: white;");

    leitorA = new leitorarquivos;
    leitorA->leLinha(lista);
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

    carregaInfo();
}
void mainwindow1366768::BotaoPlanToWatch(){
    delete leitorA;

    ui->OrdemAnime->setCurrentIndex(0);

    lista = "plantowatch";

    ui->PlanToWatch->setStyleSheet("background: red;");
    ui->Dropped->setStyleSheet("background: white;");
    ui->Completed->setStyleSheet("background: white;");
    ui->OnHold->setStyleSheet("background: white;");
    ui->Watching->setStyleSheet("background: white;");

    leitorA = new leitorarquivos;
    leitorA->leLinha(lista);
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

    carregaInfo();
}

void mainwindow1366768::BotaoBusca(){
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

void mainwindow1366768::OrdenaVetor(){
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

void mainwindow1366768::mandaRefresh(){
    ui->label->setText("Atuaizando lista");
    runArquivo->botaoRefresh();
}

void mainwindow1366768::refreshArquivo(){
    //O download de imagens só acontece caso a lista tenha novos animes
    //e isso vai garantir que a função vai rodar toda vez que der refresh
    QString currentList = lista;
    baixaImagens();
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
    QTimer::singleShot(600000, this, SLOT(mandaRefresh()));
}

void mainwindow1366768::mudouQualidade(int Qualidade){
    if(Qualidade == 0){
        baixaImagensPequenas();
        baixaQualidade = true;
    }
    else{
        baixaImagens();
        baixaQualidade = false;
    }
}

void mainwindow1366768::proximaPagina(){
     if(idAnime+28 < tamanhoLista){
        idAnime = idAnime+28;
        pagina++;
    }
    carregaInfo();
}

void mainwindow1366768::voltaPagina(){
    if(idAnime-28 >= 0){
        idAnime = idAnime-28;
        pagina--;
    }
    carregaInfo();
}

void mainwindow1366768::abrePasta(){
    ui->label->clear();
    if(organiza->abrePasta(configuracoes->RetornaDiretorioAnimeEspecifico(leitorA->retornaId(anime0).toInt())) == 1)
        ui->label->setText("Pasta não encontrada");
}

void mainwindow1366768::abreAnilist(){
    QDesktopServices::openUrl(QUrl("https://anilist.co/anime/"+ leitorA->retornaId(anime0),QUrl::TolerantMode));
}

void mainwindow1366768::AbreEpisodio()
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
        QTimer::singleShot(240000, this, SLOT(mandaRefresh()));
    }
}

void mainwindow1366768::baixaImagens()
{
    DownImagemListas = new QDownloader();
    DownImagemListas->setNext();
    connect(DownImagemListas, SIGNAL(listaMensagem(QString)), this, SLOT(mensagemFimDownload(QString)));
}

void mainwindow1366768::baixaImagensGrandes(){
    DownImagemGrandeListas = new QDownloader();
    DownImagemGrandeListas->setNextBig();
//    connect(DownImagemGrandeListas, SIGNAL(listaMensagem(QString)), this, SLOT(mensagemFimDownload(QString)));
}

void mainwindow1366768::baixaImagensPequenas(){
    QDownloader *DownImagemPequenaListas = new QDownloader();
    DownImagemPequenaListas->setNextSmall();
//    connect(DownImagemGrandeListas, SIGNAL(listaMensagem(QString)), this, SLOT(mensagemFimDownload(QString)));
}

void mainwindow1366768::mensagemFimDownload(QString mensagem){
    if(mensagem == "all")
        ui->label->setText("Todas as imagens foram carregadas");
    else
        ui->label->setText("As imagens da lista " + mensagem + " foram carregadas.");
//    delete DownImagemListas;
    if(mensagem == "all")
        ui->label->setText("Todas as imagens foram carregadas");
    else
        ui->label->setText("As imagens da lista " + mensagem + " foram carregadas.");
//    delete DownImagemListas;
    if(jConfig.returnImagemBaixaQualidade() == 0){
        baixaQualidade = true;
        baixaImagensPequenas();
    }
    else{
        baixaImagensGrandes();
    }
}

void mainwindow1366768::BuscaTorrentAnimeEspecifico(){
    QString nomeAnime = leitorA->retornaNome(anime0);
    nomeAnime.remove(".");
    nomeAnime.remove("?");
    nomeAnime.remove("S1");
    nomeAnime.remove("S2");
    nomeAnime.remove("s1");
    nomeAnime.remove("s2");
    nomeAnime.remove("s01");
    nomeAnime.remove("s02");
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
    jtorrent.getRSSEspecifico(nomeAnime);
}

void mainwindow1366768::ConfiguraArquivos(){
    ui->label->setText("Buscando animes");
    leitorarquivos *leitorconf = new leitorarquivos;
    leitorconf->leLinha("watching");
    for(int i = 0; i < leitorconf->retornaTamanhoLista(); i++){
        configuracoes->BuscaPastasAnimesEspecificos(leitorconf->retornaNome(i), leitorconf->retornaNomeIngles(i), leitorconf->retornaId(i));
    }
    leitorconf->leLinha("onhold");
    for(int i = 0; i < leitorconf->retornaTamanhoLista(); i++){
        configuracoes->BuscaPastasAnimesEspecificos(leitorconf->retornaNome(i), leitorconf->retornaNomeIngles(i), leitorconf->retornaId(i));
    }
    leitorconf->leLinha("dropped");
    for(int i = 0; i < leitorconf->retornaTamanhoLista(); i++){
        configuracoes->BuscaPastasAnimesEspecificos(leitorconf->retornaNome(i), leitorconf->retornaNomeIngles(i), leitorconf->retornaId(i));
    }
    leitorconf->leLinha("completed");
    for(int i = 0; i < leitorconf->retornaTamanhoLista(); i++){
        configuracoes->BuscaPastasAnimesEspecificos(leitorconf->retornaNome(i), leitorconf->retornaNomeIngles(i), leitorconf->retornaId(i));
    }
    leitorconf->leLinha("plantowatch");
    for(int i = 0; i < leitorconf->retornaTamanhoLista(); i++){
        configuracoes->BuscaPastasAnimesEspecificos(leitorconf->retornaNome(i), leitorconf->retornaNomeIngles(i), leitorconf->retornaId(i));
    }
    configuracoes->EscreveArquivo();
    leitorconf->leLinha("watching");
    ui->label->setText("Animes baixados encontrados! Checando imagens");
    delete leitorconf;
}

void mainwindow1366768::Configurar(){
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

void mainwindow1366768::Torrent(){
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

void mainwindow1366768::carregaAnime1(){
    if(idAnime <= tamanhoLista){
        anime0 = idAnime;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime2(){
    if(idAnime+1 < tamanhoLista){
        anime0 = idAnime+1;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime3(){
    if(idAnime+2 < tamanhoLista){
        anime0 = idAnime+2;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime4(){
    if(idAnime+3 < tamanhoLista){
        anime0 = idAnime+3;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime5(){
    if(idAnime+4 < tamanhoLista){
        anime0 = idAnime+4;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime6(){
    if(idAnime+5 < tamanhoLista){
        anime0 = idAnime+5;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime7(){
    if(idAnime+6 < tamanhoLista){
        anime0 = idAnime+6;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime8(){
    if(idAnime+7 < tamanhoLista){
        anime0 = idAnime+7;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime9(){
    if(idAnime+8 < tamanhoLista){
        anime0 = idAnime+8;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime10(){
    if(idAnime+9 < tamanhoLista){
        anime0 = idAnime+9;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime11(){
    if(idAnime+10 < tamanhoLista){
        anime0 = idAnime+10;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime12(){
    if(idAnime+11 < tamanhoLista){
        anime0 = idAnime+11;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime13(){
    if(idAnime+12 < tamanhoLista){
        anime0 = idAnime+12;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime14(){
    if(idAnime+13 < tamanhoLista){
        anime0 = idAnime+13;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime15(){
    if(idAnime+14 < tamanhoLista){
        anime0 = idAnime+14;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime16(){
    if(idAnime+15 < tamanhoLista){
        anime0 = idAnime+15;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime17(){
    if(idAnime+16 < tamanhoLista){
        anime0 = idAnime+16;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime18(){
    if(idAnime+17 < tamanhoLista){
        anime0 = idAnime+17;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime19(){
    if(idAnime+18 < tamanhoLista){
        anime0 = idAnime+18;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime20(){
    if(idAnime+19 < tamanhoLista){
        anime0 = idAnime+19;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime21(){
    if(idAnime+20 < tamanhoLista){
        anime0 = idAnime+20;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime22(){
    if(idAnime+21 < tamanhoLista){
        anime0 = idAnime+21;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime23(){
    if(idAnime+22 < tamanhoLista){
        anime0 = idAnime+22;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime24(){
    if(idAnime+23 < tamanhoLista){
        anime0 = idAnime+23;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime25(){
    if(idAnime+24 < tamanhoLista){
        anime0 = idAnime+24;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime26(){
    if(idAnime+25 < tamanhoLista){
        anime0 = idAnime+25;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime27(){
    if(idAnime+26 < tamanhoLista){
        anime0 = idAnime+26;
        carregaInfo();
    }
}
void mainwindow1366768::carregaAnime28(){
    if(idAnime+27 < tamanhoLista){
        anime0 = idAnime+27;
        carregaInfo();
    }
}

void mainwindow1366768::carregaInfo(){
    cThread.requestInterruption();

    leitorA->retornaNumEpisodiosLancados(anime0);
    ui->NumPagina_3->setText("Página "+ QString::number(pagina) + "/" + QString::number(((tamanhoLista-1)/28)+1));
    ui->Nome_3->setWordWrap(true);
    ui->Nome_3->setText(leitorA->retornaNome(anime0));
    ui->NomeIngles_3->setWordWrap(true);
    ui->NomeIngles_3->setText(leitorA->retornaNomeIngles(anime0));
    ui->Status_9->setWordWrap(true);
    ui->Status_9->setText(leitorA->retornaStatus(anime0));
    ui->NotaGeral_3->setText(leitorA->retornaNotaGeral(anime0));
    ui->Season_6->setWordWrap(true);
    ui->Season_6->setText(leitorA->retornaSeason(anime0));
    ui->sinopse_3->setText(leitorA->retornaSinopse(anime0));
    ui->sinopse_3->setWordWrap(true);
    numEpisodios = 0;
    numEpisodios = organiza->retornaNumEpiNaPasta(numEpisodios, leitorA->retornaId(anime0).toInt(), leitorA->retornaNumEpi(anime0).toInt());
    qEpiDisponivel = organiza->retornaEpisodiosDisponiveis();
    //Caso tenham episódios disponíveis na pasta
    if(qEpiDisponivel.empty() == false){
        //Caso todos os episódios estejam disponíveis, organiza o texto e mostra no label
        //Isso poupa memória, já que não tem que passar pelo loop
        if(qEpiDisponivel.length() == leitorA->retornaNumEpi(anime0).toInt()){
            //Mostra como EPI1 to ULTIMO EPI
            ui->qEpiDisponivel_3->setText("1-" + leitorA->retornaNumEpi(anime0));
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
            ui->qEpiDisponivel_3->setText(epidis);
        }
    }
    //Caso não tenham episódios na pasta, o vetor está vazio
    else{
        ui->qEpiDisponivel_3->clear();//setText("Nenhum episódio disponível");
    }
    //Le a string do próximo episódio e dá o display no label
    ui->ProxEpi_3->setText(QString::number(leitorA->retornaProgresso(anime0)) + "/" + leitorA->retornaNumEpi(anime0));
    ui->Progresso_4->setText(QString::number(leitorA->retornaProgresso(anime0)) + "/" + leitorA->retornaNumEpi(anime0));
    if(numEpisodios > 1){
        ui->EpiDisponivel_3->setText(QString::number(numEpisodios) + " episódios baixados");
    }
    else if(numEpisodios - leitorA->retornaProgresso(anime0) == 1){
        ui->EpiDisponivel_3->setText("1 episódio baixado");
    }
    else{
        ui->EpiDisponivel_3->setText("Nenhum episódio baixado");
    }
    ui->Nota_5->setText(QString::number(leitorA->retornaScore(anime0)));
    ui->Nota_6->setText(QString::number(leitorA->retornaScore(anime0)));
    int i = idAnime;
    if(i <= tamanhoLista && tamanhoLista > 0){
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(anime0, configuracoes->diretorioImagensMedio), "jpg")){
                ui->picBig_3->setScaledContents(true);
                ui->picBig_3->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(anime0, configuracoes->diretorioImagensMedio), "png")){
                ui->picBig_3->setScaledContents(true);
                ui->picBig_3->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(anime0, configuracoes->diretorioImagensGrandes), "jpg")){
                ui->picBig_3->setScaledContents(true);
                ui->picBig_3->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(anime0, configuracoes->diretorioImagensGrandes), "png")){
                ui->picBig_3->setScaledContents(true);
                ui->picBig_3->setPixmap(pix);
            }
            else{
                if(pix.load(leitorA->imagem(anime0, configuracoes->diretorioImagensMedio), "jpg")){
                    ui->picBig_3->setScaledContents(true);
                    ui->picBig_3->setPixmap(pix);
                }
                else if(pix.load(leitorA->imagem(anime0, configuracoes->diretorioImagensMedio), "png")){
                    ui->picBig_3->setScaledContents(true);
                    ui->picBig_3->setPixmap(pix);
                }
            }
        }
        ui->anime1_6->setScaledContents(true);
        ui->anime1_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime1_5->setText(leitorA->retornaNome(i));
        ui->anime1_5->setAlignment(Qt::AlignCenter);
        ui->anime1_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime1_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime1_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime1_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i], configuracoes->diretorioImagensMedio), "png")){
                ui->anime1_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime1_5->clear();
        ui->anime1_5->setStyleSheet("background: transparent;");
        ui->anime1_6->setStyleSheet("background: transparent;");
        ui->anime1_6->clear();
    }
    if(i+1 < tamanhoLista){
        ui->anime2_6->setScaledContents(true);
        ui->anime2_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime2_5->setText(leitorA->retornaNome(i));
        ui->anime2_5->setAlignment(Qt::AlignCenter);
        ui->anime2_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+1], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime2_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+1], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime2_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+1], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime2_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+1], configuracoes->diretorioImagensMedio), "png")){
                ui->anime2_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime6_6->clear();
        ui->anime6_6->setStyleSheet("background: transparent;");
        ui->anime2_5->setStyleSheet("background: transparent;");
        ui->anime2_5->clear();
    }
    if(i+2 < tamanhoLista){
        ui->anime3_6->setScaledContents(true);
        ui->anime3_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime3_5->setText(leitorA->retornaNome(i));
        ui->anime3_5->setAlignment(Qt::AlignCenter);
        ui->anime3_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+2], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime3_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+2], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime3_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+2], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime3_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+2], configuracoes->diretorioImagensMedio), "png")){
                ui->anime3_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime3_6->clear();
        ui->anime3_6->setStyleSheet("background: transparent;");
        ui->anime3_5->setStyleSheet("background: transparent;");
        ui->anime3_5->clear();
    }
    if(i+3 < tamanhoLista){
        ui->anime4_6->setScaledContents(true);
        ui->anime4_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime4_5->setText(leitorA->retornaNome(i));        ui->anime1_5->setAlignment(Qt::AlignCenter);
        ui->anime4_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+3], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime4_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+3], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime4_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+3], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime4_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+3], configuracoes->diretorioImagensMedio), "png")){
                ui->anime4_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime4_6->clear();
        ui->anime4_6->setStyleSheet("background: transparent;");
        ui->anime4_5->setStyleSheet("background: transparent;");
        ui->anime4_5->clear();
    }
    if(i+4 < tamanhoLista){
        ui->anime5_6->setScaledContents(true);
        ui->anime5_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime5_5->setText(leitorA->retornaNome(i));
        ui->anime5_5->setAlignment(Qt::AlignCenter);
        ui->anime5_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+4], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime5_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+4], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime5_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+4], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime5_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+4], configuracoes->diretorioImagensMedio), "png")){
                ui->anime5_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime5_6->clear();
        ui->anime5_6->setStyleSheet("background: transparent;");
        ui->anime5_5->setStyleSheet("background: transparent;");
        ui->anime5_5->clear();
    }
    if(i+5 < tamanhoLista){
        ui->anime6_6->setScaledContents(true);
        ui->anime6_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime6_5->setText(leitorA->retornaNome(i));
        ui->anime6_5->setAlignment(Qt::AlignCenter);
        ui->anime6_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+5], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime6_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+5], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime6_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+5], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime6_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+5], configuracoes->diretorioImagensMedio), "png")){
                ui->anime6_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime6_6->clear();
        ui->anime6_6->setStyleSheet("background: transparent;");
        ui->anime6_5->setStyleSheet("background: transparent;");
        ui->anime6_5->clear();
    }
    if(i+6 < tamanhoLista){
        ui->anime7_6->setScaledContents(true);
        ui->anime7_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime7_5->setText(leitorA->retornaNome(i));
        ui->anime7_5->setAlignment(Qt::AlignCenter);
        ui->anime7_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+6], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime7_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+6], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime7_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+6], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime7_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+6], configuracoes->diretorioImagensMedio), "png")){
                ui->anime7_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime7_6->clear();
        ui->anime7_6->setStyleSheet("background: transparent;");
        ui->anime7_5->setStyleSheet("background: transparent;");
        ui->anime7_5->clear();
    }
    if(i+7 < tamanhoLista){
        ui->anime8_6->setScaledContents(true);
        ui->anime8_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime8_5->setText(leitorA->retornaNome(i));
        ui->anime8_5->setAlignment(Qt::AlignCenter);
        ui->anime8_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+7], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime8_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+7], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime8_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+7], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime8_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+7], configuracoes->diretorioImagensMedio), "png")){
                ui->anime8_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime8_6->clear();
        ui->anime8_6->setStyleSheet("background: transparent;");
        ui->anime8_5->setStyleSheet("background: transparent;");
        ui->anime8_5->clear();
    }
    if(i+8 < tamanhoLista){
        ui->anime9_6->setScaledContents(true);
        ui->anime9_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime9_5->setText(leitorA->retornaNome(i));
        ui->anime9_5->setAlignment(Qt::AlignCenter);
        ui->anime9_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+8], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime9_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+8], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime9_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+8], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime9_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+8], configuracoes->diretorioImagensMedio), "png")){
                ui->anime9_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime9_6->clear();
        ui->anime9_6->setStyleSheet("background: transparent;");
        ui->anime9_5->setStyleSheet("background: transparent;");
        ui->anime9_5->clear();
    }
    if(i+9 < tamanhoLista){
        ui->anime10_6->setScaledContents(true);
        ui->anime10_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime10_5->setText(leitorA->retornaNome(i));
        ui->anime10_5->setAlignment(Qt::AlignCenter);
        ui->anime10_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+9], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime10_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+9], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime10_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+9], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime10_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+9], configuracoes->diretorioImagensMedio), "png")){
                ui->anime10_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime10_6->clear();
        ui->anime10_6->setStyleSheet("background: transparent;");
        ui->anime10_5->setStyleSheet("background: transparent;");
        ui->anime10_5->clear();
    }
    if(i+10 < tamanhoLista){
        ui->anime11_6->setScaledContents(true);
        ui->anime11_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime11_5->setText(leitorA->retornaNome(i));
        ui->anime11_5->setAlignment(Qt::AlignCenter);
        ui->anime11_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+10], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime11_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+10], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime11_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+10], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime11_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+10], configuracoes->diretorioImagensMedio), "png")){
                ui->anime11_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime11_6->clear();
        ui->anime11_6->setStyleSheet("background: transparent;");
        ui->anime11_5->setStyleSheet("background: transparent;");
        ui->anime11_5->clear();
    }
    if(i+11 < tamanhoLista){
        ui->anime12_6->setScaledContents(true);
        ui->anime12_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime12_5->setText(leitorA->retornaNome(i));
        ui->anime12_5->setAlignment(Qt::AlignCenter);
        ui->anime12_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+11], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime12_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+11], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime12_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+11], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime12_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+11], configuracoes->diretorioImagensMedio), "png")){
                ui->anime12_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime16_6->clear();
        ui->anime16_6->setStyleSheet("background: transparent;");
        ui->anime12_5->setStyleSheet("background: transparent;");
        ui->anime12_5->clear();
    }
    if(i+12 < tamanhoLista){
        ui->anime13_6->setScaledContents(true);
        ui->anime13_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime13_5->setText(leitorA->retornaNome(i));
        ui->anime13_5->setAlignment(Qt::AlignCenter);
        ui->anime13_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+12], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime13_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+12], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime13_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+12], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime13_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+12], configuracoes->diretorioImagensMedio), "png")){
                ui->anime13_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime13_6->clear();
        ui->anime13_6->setStyleSheet("background: transparent;");
        ui->anime13_5->setStyleSheet("background: transparent;");
        ui->anime13_5->clear();
    }
    if(i+13 < tamanhoLista){
        ui->anime14_6->setScaledContents(true);
        ui->anime14_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime14_5->setText(leitorA->retornaNome(i));
        ui->anime14_5->setAlignment(Qt::AlignCenter);
        ui->anime14_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+13], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime14_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+13], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime14_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+13], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime14_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+13], configuracoes->diretorioImagensMedio), "png")){
                ui->anime14_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime14_6->clear();
        ui->anime14_6->setStyleSheet("background: transparent;");
        ui->anime14_5->setStyleSheet("background: transparent;");
        ui->anime14_5->clear();
    }
    if(i+14 < tamanhoLista){
        ui->anime15_6->setScaledContents(true);
        ui->anime15_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime15_5->setText(leitorA->retornaNome(i));
        ui->anime15_5->setAlignment(Qt::AlignCenter);
        ui->anime15_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+14], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime15_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+14], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime15_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+14], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime15_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+14], configuracoes->diretorioImagensMedio), "png")){
                ui->anime15_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime15_6->clear();
        ui->anime15_6->setStyleSheet("background: transparent;");
        ui->anime15_5->setStyleSheet("background: transparent;");
        ui->anime15_5->clear();
    }
    if(i+15 < tamanhoLista){
        ui->anime16_6->setScaledContents(true);
        ui->anime16_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime16_5->setText(leitorA->retornaNome(i));
        ui->anime16_5->setAlignment(Qt::AlignCenter);
        ui->anime16_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+15], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime16_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+15], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime16_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+15], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime16_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+15], configuracoes->diretorioImagensMedio), "png")){
                ui->anime16_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime16_6->clear();
        ui->anime16_6->setStyleSheet("background: transparent;");
        ui->anime16_5->setStyleSheet("background: transparent;");
        ui->anime16_5->clear();
    }
    if(i+16 < tamanhoLista){
        ui->anime17_6->setScaledContents(true);
        ui->anime17_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime17_5->setText(leitorA->retornaNome(i));
        ui->anime17_5->setAlignment(Qt::AlignCenter);
        ui->anime17_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+16], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime17_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+16], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime17_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+16], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime17_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+16], configuracoes->diretorioImagensMedio), "png")){
                ui->anime17_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime17_6->clear();
        ui->anime17_6->setStyleSheet("background: transparent;");
        ui->anime17_5->setStyleSheet("background: transparent;");
        ui->anime17_5->clear();
    }
    if(i+17 < tamanhoLista){
        ui->anime18_6->setScaledContents(true);
        ui->anime18_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime18_5->setText(leitorA->retornaNome(i));
        ui->anime18_5->setAlignment(Qt::AlignCenter);
        ui->anime18_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+17], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime18_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+17], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime18_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+17], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime18_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+17], configuracoes->diretorioImagensMedio), "png")){
                ui->anime18_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime18_6->clear();
        ui->anime18_6->setStyleSheet("background: transparent;");
        ui->anime18_5->setStyleSheet("background: transparent;");
        ui->anime18_5->clear();
    }
    if(i+18 < tamanhoLista){
        ui->anime19_6->setScaledContents(true);
        ui->anime19_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime19_5->setText(leitorA->retornaNome(i));
        ui->anime19_5->setAlignment(Qt::AlignCenter);
        ui->anime19_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+18], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime19_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+18], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime19_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+18], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime19_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+18], configuracoes->diretorioImagensMedio), "png")){
                ui->anime19_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime19_6->clear();
        ui->anime19_6->setStyleSheet("background: transparent;");
        ui->anime19_5->setStyleSheet("background: transparent;");
        ui->anime19_5->clear();
    }
    if(i+19 < tamanhoLista){
        ui->anime20_6->setScaledContents(true);
        ui->anime20_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime20_5->setText(leitorA->retornaNome(i));
        ui->anime20_5->setAlignment(Qt::AlignCenter);
        ui->anime20_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+19], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime20_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+19], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime20_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+19], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime20_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+19], configuracoes->diretorioImagensMedio), "png")){
                ui->anime20_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime20_6->clear();
        ui->anime20_6->setStyleSheet("background: transparent;");
        ui->anime20_5->setStyleSheet("background: transparent;");
        ui->anime20_5->clear();
    }
    if(i+20 < tamanhoLista){
        ui->anime21_6->setScaledContents(true);
        ui->anime21_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime21_5->setText(leitorA->retornaNome(i));
        ui->anime21_5->setAlignment(Qt::AlignCenter);
        ui->anime21_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+20], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime21_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+20], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime21_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+20], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime21_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+20], configuracoes->diretorioImagensMedio), "png")){
                ui->anime21_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime21_6->clear();
        ui->anime21_6->setStyleSheet("background: transparent;");
        ui->anime21_5->setStyleSheet("background: transparent;");
        ui->anime21_5->clear();
    }
    if(i+21 < tamanhoLista){
        ui->anime22_6->setScaledContents(true);
        ui->anime22_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime22_5->setText(leitorA->retornaNome(i));
        ui->anime22_5->setAlignment(Qt::AlignCenter);
        ui->anime22_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+21], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime22_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+21], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime22_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+21], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime22_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+21], configuracoes->diretorioImagensMedio), "png")){
                ui->anime22_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime26_6->clear();
        ui->anime26_6->setStyleSheet("background: transparent;");
        ui->anime22_5->setStyleSheet("background: transparent;");
        ui->anime22_5->clear();
    }
    if(i+22 < tamanhoLista){
        ui->anime23_6->setScaledContents(true);
        ui->anime23_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime23_5->setText(leitorA->retornaNome(i));
        ui->anime23_5->setAlignment(Qt::AlignCenter);
        ui->anime23_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+22], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime23_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+22], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime23_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+22], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime23_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+22], configuracoes->diretorioImagensMedio), "png")){
                ui->anime23_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime23_6->clear();
        ui->anime23_6->setStyleSheet("background: transparent;");
        ui->anime23_5->setStyleSheet("background: transparent;");
        ui->anime23_5->clear();
    }
    if(i+23 < tamanhoLista){
        ui->anime24_6->setScaledContents(true);
        ui->anime24_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime24_5->setText(leitorA->retornaNome(i));
        ui->anime24_5->setAlignment(Qt::AlignCenter);
        ui->anime24_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+23], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime24_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+23], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime24_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+23], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime24_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+23], configuracoes->diretorioImagensMedio), "png")){
                ui->anime24_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime24_6->clear();
        ui->anime24_6->setStyleSheet("background: transparent;");
        ui->anime24_5->setStyleSheet("background: transparent;");
        ui->anime24_5->clear();
    }
    if(i+24 < tamanhoLista){
        ui->anime25_6->setScaledContents(true);
        ui->anime25_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime25_5->setText(leitorA->retornaNome(i));
        ui->anime25_5->setAlignment(Qt::AlignCenter);
        ui->anime25_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+24], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime25_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+24], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime25_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+24], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime25_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+24], configuracoes->diretorioImagensMedio), "png")){
                ui->anime25_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime25_6->clear();
        ui->anime25_6->setStyleSheet("background: transparent;");
        ui->anime25_5->setStyleSheet("background: transparent;");
        ui->anime25_5->clear();
    }
    if(i+25 < tamanhoLista){
        ui->anime26_6->setScaledContents(true);
        ui->anime26_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime26_5->setText(leitorA->retornaNome(i));
        ui->anime26_5->setAlignment(Qt::AlignCenter);
        ui->anime26_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+25], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime26_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+25], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime26_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+25], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime26_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+25], configuracoes->diretorioImagensMedio), "png")){
                ui->anime26_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime26_6->clear();
        ui->anime26_6->setStyleSheet("background: transparent;");
        ui->anime26_5->setStyleSheet("background: transparent;");
        ui->anime26_5->clear();
    }
    if(i+26 < tamanhoLista){
        ui->anime27_6->setScaledContents(true);
        ui->anime27_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime27_5->setText(leitorA->retornaNome(i));
        ui->anime27_5->setAlignment(Qt::AlignCenter);
        ui->anime27_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+26], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime27_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+26], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime27_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+26], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime27_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+26], configuracoes->diretorioImagensMedio), "png")){
                ui->anime27_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime27_6->clear();
        ui->anime27_6->setStyleSheet("background: transparent;");
        ui->anime27_5->setStyleSheet("background: transparent;");
        ui->anime27_5->clear();
    }
    if(i+27 < tamanhoLista){
        ui->anime28_6->setScaledContents(true);
        ui->anime28_5->setStyleSheet("background-color : rgb(181, 181, 181);");
        ui->anime28_5->setText(leitorA->retornaNome(i));
        ui->anime28_5->setAlignment(Qt::AlignCenter);
        ui->anime28_5->setWordWrap(true);
        if(baixaQualidade == true){
            if(pix.load(leitorA->imagem(vetorAnimes[i+27], configuracoes->diretorioImagensPequenas), "jpg")){
                ui->anime28_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+27], configuracoes->diretorioImagensPequenas), "png")){
                ui->anime28_6->setPixmap(pix);
            }
        }
        else{
            if(pix.load(leitorA->imagem(vetorAnimes[i+27], configuracoes->diretorioImagensMedio), "jpg")){
                ui->anime28_6->setPixmap(pix);
            }
            else if(pix.load(leitorA->imagem(vetorAnimes[i+27], configuracoes->diretorioImagensMedio), "png")){
                ui->anime28_6->setPixmap(pix);
            }
        }
    }
    else{
        ui->anime28_6->clear();
        ui->anime28_6->setStyleSheet("background: transparent;");
        ui->anime28_5->setStyleSheet("background: transparent;");
        ui->anime28_5->clear();
    }
}

void mainwindow1366768::on_NotaMais_3_clicked()
{
    if(ui->Nota_5->text().toInt() < 100){
        ui->Nota_5->setText(QString::number(ui->Nota_5->text().toInt()+10));
        ui->Nota_6->setText(ui->Nota_5->text());
    }
}

void mainwindow1366768::on_NotaMenos_3_clicked()
{
    if(ui->Nota_5->text().toInt() > 0){
        ui->Nota_5->setText(QString::number(ui->Nota_5->text().toInt()-10));
        ui->Nota_6->setText(ui->Nota_5->text());
    }
}

void mainwindow1366768::on_ProgressoMais_3_clicked()
{
    QStringList progressoQuebrado = ui->ProxEpi_3->text().split('/');
    if(progressoQuebrado.at(0).toInt() < leitorA->retornaNumEpi(anime0).toInt()){
        ui->ProxEpi_3->setText(QString::number(progressoQuebrado.at(0).toInt()+1) + "/" + progressoQuebrado.at(1));
        ui->Progresso_4->setText(ui->ProxEpi_3->text());
    }

}

void mainwindow1366768::on_ProgressoMenos_3_clicked()
{
    QStringList progressoQuebrado = ui->ProxEpi_3->text().split('/');
    if(progressoQuebrado.at(0).toInt() > 0){
        ui->ProxEpi_3->setText(QString::number(progressoQuebrado.at(0).toInt()-1) + "/" + progressoQuebrado.at(1));
        ui->Progresso_4->setText(ui->ProxEpi_3->text());
    }
}

void mainwindow1366768::on_Lista_clicked()
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




