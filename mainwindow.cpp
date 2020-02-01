#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ///um dia ainda faço qml disso
    //fmudaResolucao();
    logger::fattachLogger();
    qInfo() << QDateTime::currentDateTime().toString();
    canilist = new anilist(nullptr);
    canilist->frecebeAutorizacao(jconfig.fretornaUsuario(),jconfig.fretornaCodigoAutorizacao());
    cconfBase = new confBase(nullptr);
    cconfBase->fcriaDiretoriosBase();
    dirGrande = cconfBase->vdiretorioImagensGrandes;
    dirMedio = cconfBase->vdiretorioImagensMedio;
    dirPequeno = cconfBase->vdiretorioImagensPequenas;
    if(dirGrande.isEmpty())
        dirGrande = "0";
    if(dirPequeno.isEmpty())
        dirPequeno = "0";
    if(dirMedio.isEmpty())
        dirMedio = "0";
    qDebug() << "Main configuration is up";
    carquivos = new arquivos(this);
    qDebug() << "File system is up";
    cconfUsuario = new confUsuario(nullptr);
    qDebug() << "User configuration is up";

    //Função que passa os diretórios
    qDebug() << jconfig.fretornaDiretorios();

    ///Essa é a parte onde rola a busca das pastas
    cconfUsuario->flePastasArquivos();
    fmandaDiretoriosArquivos();

    cleitorListaAnimes = new leitorlistaanimes(nullptr);

    //Já iniciaram todas as variáveis, então tenta conexão com o anilist.
    //Tenho que fazer o seguinte:
    //Se conseguir ler o código, tentar uma conexão e baixar a lista.
    //Se der, eu continuo o programa. Se não der, eu tento ler a lista que tem ali e ir com ela, tentando conexão
    //de um em um minuto
    //Sempre que apertar o botão de salvar, dar reset nas funções de procurar pastas
    //Entao. basicamente, por um sinal na função de salvar pra fazer reler os diretórios
    finiciaPrograma();

    //Tem que ter uma função que vai chamar o if apenas. Caso o if seja falso, ele chama a função novamente
    //Daqui 60 segundos

    //Nenhuma função dependente do arquivo deve rodar se o arquivo não puder ser lido
    if(cleitorListaAnimes->fleJson()){
        vlistaLidaSucesso = true;
        qDebug() << "The anime list have been read successfully";
        cfiledownloader = new filedownloader(nullptr);
        qDebug() << "The download system is up";
        cfiledownloader->fsetConfBase(cconfBase);
        cfiledownloader->fsetLeitorListaAnimes(cleitorListaAnimes);
        //Baixa imagens ao abrir o programa
        cfiledownloader->fsetNext();
        connect(cfiledownloader,&filedownloader::slistaMensagem,this,&MainWindow::finfoAnimeSelecionado, Qt::QueuedConnection);
        qDebug() << "Images ready";
        // Já aciona a lista de modo ordenado
        vordem = "";
        vlistaSelecionada = cleitorListaAnimes->sortLista(vordem, "watching");
        qDebug() << "Opening Watching List";
        vlistaAtual = "watching";
        qDebug() << "Searching for animes in the computer";

        //QFuture vai rodar a função fcarregaImagensBackground em uma thread separada, para carregar todas as imagens enquanto o usuário mexe no
        //programa, para deixar o programa mais fluído
        vfuture = QtConcurrent::run(this, &MainWindow::fcarregaImagensBackground);
    }
    else{
        qCritical() << "There was a problem reading the anime list";
        ui->labelMensagem->setText("There was a problem reading the anime list. Trying again in one minute");
        tryTimer = new QTimer(this);
        connect(tryTimer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::on_botaoRefresh_clicked));
        tryTimer->start(60000); //1 minutos
        vfalhaconexao = true;
    }
    //E o timer para atualizar a lista automaticamente
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::on_botaoRefresh_clicked));
//    timer->start(60000); //1 MINUTO PRA TESTES DE DEBUG
    timer->start(600000); //10 minutos
    //O segundo timer é uma contagem de segundo em segundo pra mostrar quanto tempo falta para o timer de cima chegar à 0
    timerRefresh = new QTimer(this);
    connect(timerRefresh, &QTimer::timeout, this, QOverload<>::of(&MainWindow::fatualizaRefreshTimer));
    timerRefresh->start(1000);
    vtimerSegundos = 59;
    //Timer que atualiza o anilist, pro caso de falhas ou assistir offline
    timerAcao = new QTimer(this);
    connect(timerAcao, &QTimer::timeout, this, QOverload<>::of(&MainWindow::fatualizaAnilist));
    timerAcao->start(60000); //1 minuto

    ui->janelaRotativa->addWidget(&jconfig);
    ui->janelaRotativa->addWidget(&jtorrent);
    jtorrent.fpassaPonteiros(cleitorListaAnimes, &jconfig, carquivos);
    cconfUsuario->frecebeConfigs(jconfig.fretornaDiretorios());
    carquivos->frecebeAnimes(cleitorListaAnimes->sortLista("cnome", "watching"), cleitorListaAnimes->sortLista("cnome", "completed"),
                             cleitorListaAnimes->sortLista("cnome", "onhold"), cleitorListaAnimes->sortLista("cnome", "dropped"),
                             cleitorListaAnimes->sortLista("cnome", "plantowatch"));
    vanimeSelecionado = 0;
    vpagina = 1;

    ui->NumPagina->setText("Watching - Page "+QString::number(vpagina)+"/"+QString::number(((vlistaSelecionada.size()-1)/12)+1));

    connect(&jtorrent, &janelatorrent::error, this, &MainWindow::favisoErro);
    QFile lfile("Configurações/Temp/ListUpdates.txt");
    if(lfile.exists() && lfile.size() != 0){
        if(lfile.open(QIODevice::ReadOnly)) {
            QTextStream lstreamTexto(&lfile);
            while(!lstreamTexto.atEnd()){
                QStringList linha = lstreamTexto.readLine().split("-");
                QString act = linha.last();
                linha.removeLast();
                vlistaAcoes.insert(linha, act);
            }
            lfile.close();
        }
    }
    finfoAnimeSelecionado();
}

void MainWindow::finiciaPrograma(){
    this->setWindowTitle("Atom - " + jconfig.fretornaUsuario());
    ui->labelUsername->setText(jconfig.fretornaUsuario());
    //O programa tenta fazer a conexão logo de cara sem uma thread pelo simples fato de que ainda não carregou e, portanto, não vai ficar
    //travado. Caso ele esteja carregado, ele irá travar ao baixar a lista
    qDebug() << "Checking connection with Anilist";
    ui->labelMensagem->setText("Baixando listas de animes");
    canilist->fbaixaListaThread(cThread);
    canilist->moveToThread(&cThread);
    cThread.start();
    connect(canilist, &anilist::sterminouDownload, this, &MainWindow::frefreshListas, Qt::QueuedConnection);
    connect(&jconfig, &janeladeconfig::sauthcodesave, this, &MainWindow::fretryAnilist);
    //Basicamente, sempre que terminar um download ele chama a função duas vezes. Ter dois sinais diferentes?
    //Ter dois ponteiros, e deletar o primeiro?
    //Com esse bool, podemos controlar outras funções que rodam ao mesmo tempo, como a de carregar imagens no background
    vrefreshAcontecendo = true;
    //fcarregouListaTeste();
}

//void MainWindow::fcarregouListaTeste(){
//    if(cleitorListaAnimes->fleJson())
//        fcarregouListaSucesso(); //Continua fazendo o que tem no construtor
//    else
//        fcarregouListaFalha(); //Que chama fcarregouListateste a cada 1 min outra vez em thread, até dar certo.
//    //É importante ter certeza de que mesmo falhando, ele ainda vai tentar ler a lista offline pra mostrar os animes
//}

MainWindow::~MainWindow()
{
    if(!vlistaAcoes.isEmpty()){
        QFile lfile("Configurações/Temp/ListUpdates.txt");
        if(lfile.open(QIODevice::WriteOnly | QIODevice::Append)) {
            QTextStream lstreamTexto(&lfile);
            foreach (QStringList key, vlistaAcoes.keys()){
                lstreamTexto << key.at(0) << "-" << key.at(1) << "-" << vlistaAcoes[key] << endl;
                vlistaAcoes.remove(key);
            }
            lstreamTexto.flush();
            lfile.close();
        }
    }
    vrefreshAcontecendo = true;
    vfuture.waitForFinished();
    cThread.requestInterruption();
    cThread.wait();
    dThread.requestInterruption();
    dThread.wait();
    vfuture.waitForFinished();
    qDebug() << "Deleting pointers";
    cconfUsuario->deleteLater();
    qDebug() << "cconfUsuario deleted";
    carquivos->deleteLater();
    qDebug() << "carquivos deleted";
    if(vlistaLidaSucesso == true){
        timer->deleteLater();
        timerRefresh->deleteLater();
        timerAcao->deleteLater();
        cfiledownloader->deleteLater();
        qDebug() << "cfiledownloader deleted";
    }
    cleitorListaAnimes->deleteLater();
    qDebug() << "cleitorListaAnimes deleted";
    canilist->deleteLater();
    qDebug() << "canilist deleted";
    qInfo() << "Software closed successfully";
    delete ui;
}

///ASSIM QUE O DESIGN ESTIVER PRONTO, CHECAR TAMBÉM SE A SINOPSE É MAIOR QUE O LABEL E AJEITAR ISSO
void MainWindow::finfoAnimeSelecionado(){
    if(!vlistaSelecionada.isEmpty()){
        ui->labelInfoNomeAnime->setText(vlistaSelecionada[vanimeSelecionado]->vnome);
        ui->labelInfoNomeAnimeIngles->setText(vlistaSelecionada[vanimeSelecionado]->vnomeIngles);
        ui->labelInfoSinopse->setText(vlistaSelecionada[vanimeSelecionado]->vsinopse);
        ui->labelInfoStatus->setText(vlistaSelecionada[vanimeSelecionado]->vstatus);
        ui->labelInfoSeason->setText(vlistaSelecionada[vanimeSelecionado]->vseason);
        ui->labelInfoMediaSite->setText(QString::number(vlistaSelecionada[vanimeSelecionado]->vnotaMediaSite.toDouble()/10, 'f',2));
        ui->labelInfoNota->setText(vlistaSelecionada[vanimeSelecionado]->vnotaMediaPessoal);
        ui->labelInfoRelease->setText(vlistaSelecionada[vanimeSelecionado]->vdataEpisodio);
        if(vlistaSelecionada[vanimeSelecionado]->vnumProximoEpisodioLancado == "" && vlistaSelecionada[vanimeSelecionado]->vstatus != "Finished Airing")
            ui->labelInfoEpisodiosLancados->setText("-");
        else if(vlistaSelecionada[vanimeSelecionado]->vnumProximoEpisodioLancado.toInt() == 0)
            ui->labelInfoEpisodiosLancados->setText("All Episodes");
        else
            ui->labelInfoEpisodiosLancados->setText(QString::number(vlistaSelecionada[vanimeSelecionado]->vnumProximoEpisodioLancado.toInt() - 1) + " Episodes");
        ui->labelInfoTipo->setText(vlistaSelecionada[vanimeSelecionado]->vformato);
    }
    else{
        qWarning() << "Anime list is empty!";
        ui->labelInfoNomeAnime->clear();
        ui->labelInfoNomeAnimeIngles->clear();
        ui->labelInfoSinopse->clear();
        ui->labelInfoStatus->clear();
        ui->labelInfoSeason->clear();
        ui->labelInfoMediaSite->clear();
        ui->labelInfoNota->clear();
        ui->labelInfoRelease->clear();
        ui->labelInfoEpisodiosLancados->clear();
        ui->labelInfoTipo->clear();
    }
    fcarregaImagensLista();
}

///BOTAR ISSO NUM LOOP, PFV, TEM 675 LINHAS SÓ NESSA FUNÇÃO!
/// //tentei e não deu ::sad face::
/// Mas tudo bem, vou pensar em algo
void MainWindow::fcarregaImagensLista(){
    QPixmap pix;
    QString lstyleSheet;
    //Tenta carrgar o avatar antes de tudo
    ui->labelAvatar->setScaledContents(true);
    if(pix.load("Configurações/Temp/Imagens/avatar.jpg", "jpg")){
        ui->labelAvatar->setPixmap(pix);
    }
    if(!vlistaSelecionada.isEmpty()){
        ui->imagemAnime00->setScaledContents(true);
        ui->imagemAnimeGrande->setScaledContents(true);
        //Tenta carregar a imagem como jpg, se possível
        if(pix.load(cconfBase->vdiretorioImagensGrandes+vlistaSelecionada[vanimeSelecionado]->vid+".jpg", "jpg")){
            ui->imagemAnimeGrande->setPixmap(pix);
        }
        //Se não conseguir carregar como jpg, tenta carregar como png
        else if(pix.load(cconfBase->vdiretorioImagensGrandes+vlistaSelecionada[vanimeSelecionado]->vid+".png", "png")){
            ui->imagemAnimeGrande->setPixmap(pix);
        }
        //Se a imagem grande não existir, vai carregar a imagem média no lugar
        else if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[vanimeSelecionado]->vid+".jpg", "jpg")){
            ui->imagemAnimeGrande->setPixmap(pix);
        }
        else if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[vanimeSelecionado]->vid+".png", "png")){
            ui->imagemAnimeGrande->setPixmap(pix);
        }
        //Checa tamanho do título pra sempre caber no frame
        lstyleSheet = ui->labelAnime00Titulo->styleSheet();
        if(vlistaSelecionada[0+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime00Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[0+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime00Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime00Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelFundoAnime00Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime00Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime00Titulo->setStyleSheet("background: rgb(185,201,250);");
        ui->labelAnime00Titulo->setText(vlistaSelecionada[0+(12*(vpagina-1))]->vnome);
        ui->labelAnime00Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime00Titulo->setWordWrap(true);
        ui->labelAnime00Progresso->setAlignment(Qt::AlignCenter);
        if(!carquivos->fprocuraEpisodio(vlistaSelecionada[0+(12*(vpagina-1))]).isEmpty())
            ui->labelAnime00Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(53, 120, 56);");
        else
            ui->labelAnime00Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime00Progresso->setText("Progresso: " + vlistaSelecionada[0+(12*(vpagina-1))]->vnumEpisodiosAssistidos +
                    "/" + vlistaSelecionada[0+(12*(vpagina-1))]->vnumEpisodiosTotais);
        ui->labelAnime00Nota->setAlignment(Qt::AlignCenter);
        ui->labelAnime00Nota->setText("Nota: " + vlistaSelecionada[0+(12*(vpagina-1))]->vnotaMediaPessoal + "/10");
        if(vlistaAtual == "busca"){
            ui->labelFundoAnime00Lista->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelFundoAnime00Lista->setText(vlistaSelecionada[0+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelFundoAnime00Lista->clear();
            ui->labelFundoAnime00Lista->setStyleSheet("background: transparent");
        }
        if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[0+(12*(vpagina-1))]->vid+".jpg", "jpg")){
            ui->imagemAnime00->setPixmap(pix);
        }
        else if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[0+(12*(vpagina-1))]->vid+".png", "png")){
            ui->imagemAnime00->setPixmap(pix);
        }
        else{
            ui->imagemAnime00->clear();
            ui->imagemAnime00->setStyleSheet("background: black;");
        }
    }
    else{
        //PRO LOOP
        //remove imagemAnime remove labelAnime remove labelFundoAnime remove nota remove progresso remove titulo
        //Ai pega o valor que sobrar e joga na condição
        //if valor == 0 && vlistaSelecionada.size() != 0
        //if valor != 0 && vlistaSelecionada.size() > VALOR+(12*(vpagina-1))
        ui->imagemAnimeGrande->clear();
        ui->imagemAnime00->clear();
        ui->imagemAnime00->setStyleSheet("background: transparent;");
        ui->labelAnime00Titulo->clear();
        ui->labelAnime00Progresso->clear();
        ui->labelAnime00Nota->clear();
        ui->labelFundoAnime00Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime00Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime00Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime00Lista->clear();
        ui->labelFundoAnime00Lista->setStyleSheet("background: transparent");
    }
    if(vlistaSelecionada.size() > 1+(12*(vpagina-1))){
        ui->imagemAnime01->setScaledContents(true);
        ui->labelFundoAnime01Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime01Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime01Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(vlistaSelecionada[1+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime01Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[1+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime01Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime01Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime01Titulo->setText(vlistaSelecionada[1+(12*(vpagina-1))]->vnome);
        ui->labelAnime01Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime01Titulo->setWordWrap(true);
        if(!carquivos->fprocuraEpisodio(vlistaSelecionada[1+(12*(vpagina-1))]).isEmpty())
            ui->labelAnime01Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(53, 120, 56);");
        else
            ui->labelAnime01Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime01Progresso->setAlignment(Qt::AlignCenter);
        ui->labelAnime01Progresso->setText("Progresso: " + vlistaSelecionada[1+(12*(vpagina-1))]->vnumEpisodiosAssistidos +
                    "/" + vlistaSelecionada[1+(12*(vpagina-1))]->vnumEpisodiosTotais);
        ui->labelAnime01Nota->setAlignment(Qt::AlignCenter);
        ui->labelAnime01Nota->setText("Nota: " + vlistaSelecionada[1+(12*(vpagina-1))]->vnotaMediaPessoal + "/10");
        if(vlistaAtual == "busca"){
            ui->labelFundoAnime01Lista->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelFundoAnime01Lista->setText(vlistaSelecionada[1+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelFundoAnime01Lista->clear();
            ui->labelFundoAnime01Lista->setStyleSheet("background: transparent");
        }
        if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[1+(12*(vpagina-1))]->vid+".jpg", "jpg")){
            ui->imagemAnime01->setPixmap(pix);
        }
        else if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[1+(12*(vpagina-1))]->vid+".png", "png")){
            ui->imagemAnime01->setPixmap(pix);
        }
        else{
            ui->imagemAnime01->clear();//Trocar isso por imagem coringa pro caso de não ter imagem do anime
            ui->imagemAnime01->setStyleSheet("background: black;");
        }
    }
    else{
        ui->imagemAnime01->clear();
        ui->imagemAnime01->setStyleSheet("background: transparent;");
        ui->labelAnime01Titulo->clear();
        ui->labelAnime01Progresso->clear();
        ui->labelAnime01Nota->clear();
        ui->labelFundoAnime01Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime01Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime01Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime01Lista->clear();
        ui->labelFundoAnime01Lista->setStyleSheet("background: transparent");
    }
    if(vlistaSelecionada.size() > 2+(12*(vpagina-1))){
        ui->imagemAnime02->setScaledContents(true);
        ui->labelFundoAnime02Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime02Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime02Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(vlistaSelecionada[2+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime02Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[2+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime02Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime02Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime02Titulo->setText(vlistaSelecionada[2+(12*(vpagina-1))]->vnome);
        ui->labelAnime02Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime02Titulo->setWordWrap(true);
        ui->labelAnime02Progresso->setAlignment(Qt::AlignCenter);
        if(!carquivos->fprocuraEpisodio(vlistaSelecionada[2+(12*(vpagina-1))]).isEmpty())
            ui->labelAnime02Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(53, 120, 56);");
        else
            ui->labelAnime02Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime02Progresso->setText("Progresso: " + vlistaSelecionada[2+(12*(vpagina-1))]->vnumEpisodiosAssistidos +
                    "/" + vlistaSelecionada[2+(12*(vpagina-1))]->vnumEpisodiosTotais);
        ui->labelAnime02Nota->setAlignment(Qt::AlignCenter);
        ui->labelAnime02Nota->setText("Nota: " + vlistaSelecionada[2+(12*(vpagina-1))]->vnotaMediaPessoal + "/10");
        if(vlistaAtual == "busca"){
            ui->labelFundoAnime02Lista->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelFundoAnime02Lista->setText(vlistaSelecionada[2+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelFundoAnime02Lista->clear();
            ui->labelFundoAnime02Lista->setStyleSheet("background: transparent");
        }
        if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[2+(12*(vpagina-1))]->vid+".jpg", "jpg")){
            ui->imagemAnime02->setPixmap(pix);
        }
        else if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[2+(12*(vpagina-1))]->vid+".png", "png")){
            ui->imagemAnime02->setPixmap(pix);
        }
        else{
            ui->imagemAnime02->clear();
            ui->imagemAnime02->setStyleSheet("background: black;");
        }
    }
    else{
        ui->imagemAnime02->clear();
        ui->imagemAnime02->setStyleSheet("background: transparent;");
        ui->labelAnime02Titulo->clear();
        ui->labelAnime02Progresso->clear();
        ui->labelAnime02Nota->clear();
        ui->labelFundoAnime02Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime02Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime02Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime02Lista->clear();
        ui->labelFundoAnime02Lista->setStyleSheet("background: transparent");
    }
    if(vlistaSelecionada.size() > 3+(12*(vpagina-1))){
        ui->imagemAnime03->setScaledContents(true);
        ui->labelFundoAnime03Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime03Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime03Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(vlistaSelecionada[3+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime03Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[3+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime03Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime03Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime03Titulo->setText(vlistaSelecionada[3+(12*(vpagina-1))]->vnome);
        ui->labelAnime03Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime03Titulo->setWordWrap(true);
        ui->labelAnime03Progresso->setAlignment(Qt::AlignCenter);
        if(!carquivos->fprocuraEpisodio(vlistaSelecionada[3+(12*(vpagina-1))]).isEmpty())
            ui->labelAnime03Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(53, 120, 56);");
        else
            ui->labelAnime03Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime03Progresso->setText("Progresso: " + vlistaSelecionada[3+(12*(vpagina-1))]->vnumEpisodiosAssistidos +
                    "/" + vlistaSelecionada[3+(12*(vpagina-1))]->vnumEpisodiosTotais);
        ui->labelAnime03Nota->setAlignment(Qt::AlignCenter);
        ui->labelAnime03Nota->setText("Nota: " + vlistaSelecionada[3+(12*(vpagina-1))]->vnotaMediaPessoal + "/10");
        if(vlistaAtual == "busca"){
            ui->labelFundoAnime03Lista->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelFundoAnime03Lista->setText(vlistaSelecionada[3+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelFundoAnime03Lista->clear();
            ui->labelFundoAnime03Lista->setStyleSheet("background: transparent");
        }
        if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[3+(12*(vpagina-1))]->vid+".jpg", "jpg")){
            ui->imagemAnime03->setPixmap(pix);
        }
        else if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[3+(12*(vpagina-1))]->vid+".png", "png")){
            ui->imagemAnime03->setPixmap(pix);
        }
        else{
            ui->imagemAnime03->clear();
            ui->imagemAnime03->setStyleSheet("background: black;");
        }
    }
    else{
        ui->imagemAnime03->clear();
        ui->imagemAnime03->setStyleSheet("background: transparent;");
        ui->labelAnime03Titulo->clear();
        ui->labelAnime03Progresso->clear();
        ui->labelAnime03Nota->clear();
        ui->labelFundoAnime03Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime03Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime03Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime03Lista->clear();
        ui->labelFundoAnime03Lista->setStyleSheet("background: transparent");
    }
    if(vlistaSelecionada.size() > 4+(12*(vpagina-1))){
        ui->imagemAnime04->setScaledContents(true);
        ui->labelFundoAnime04Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime04Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime04Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(vlistaSelecionada[4+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime04Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[4+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime04Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime04Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime04Titulo->setText(vlistaSelecionada[4+(12*(vpagina-1))]->vnome);
        ui->labelAnime04Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime04Titulo->setWordWrap(true);
        ui->labelAnime04Progresso->setAlignment(Qt::AlignCenter);
        if(!carquivos->fprocuraEpisodio(vlistaSelecionada[4+(12*(vpagina-1))]).isEmpty())
            ui->labelAnime04Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(53, 120, 56);");
        else
            ui->labelAnime04Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime04Progresso->setText("Progresso: " + vlistaSelecionada[4+(12*(vpagina-1))]->vnumEpisodiosAssistidos +
                    "/" + vlistaSelecionada[4+(12*(vpagina-1))]->vnumEpisodiosTotais);
        ui->labelAnime04Nota->setAlignment(Qt::AlignCenter);
        ui->labelAnime04Nota->setText("Nota: " + vlistaSelecionada[4+(12*(vpagina-1))]->vnotaMediaPessoal + "/10");
        if(vlistaAtual == "busca"){
            ui->labelFundoAnime04Lista->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelFundoAnime04Lista->setText(vlistaSelecionada[4+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelFundoAnime04Lista->clear();
            ui->labelFundoAnime04Lista->setStyleSheet("background: transparent");
        }
        if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[4+(12*(vpagina-1))]->vid+".jpg", "jpg")){
            ui->imagemAnime04->setPixmap(pix);
        }
        else if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[4+(12*(vpagina-1))]->vid+".png", "png")){
            ui->imagemAnime04->setPixmap(pix);
        }
        else{
            ui->imagemAnime04->clear();
            ui->imagemAnime04->setStyleSheet("background: black;");
        }
    }
    else{
        ui->imagemAnime04->clear();
        ui->imagemAnime04->setStyleSheet("background: transparent;");
        ui->labelAnime04Titulo->clear();
        ui->labelAnime04Progresso->clear();
        ui->labelAnime04Nota->clear();
        ui->labelFundoAnime04Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime04Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime04Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime04Lista->clear();
        ui->labelFundoAnime04Lista->setStyleSheet("background: transparent");
    }
    if(vlistaSelecionada.size() > 5+(12*(vpagina-1))){
        ui->imagemAnime05->setScaledContents(true);
        ui->labelFundoAnime05Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime05Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime05Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(vlistaSelecionada[5+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime05Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[5+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime05Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime05Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime05Titulo->setText(vlistaSelecionada[5+(12*(vpagina-1))]->vnome);
        ui->labelAnime05Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime05Titulo->setWordWrap(true);
        ui->labelAnime05Progresso->setAlignment(Qt::AlignCenter);
        ui->labelAnime05Progresso->setText("Progresso: " + vlistaSelecionada[5+(12*(vpagina-1))]->vnumEpisodiosAssistidos +
                    "/" + vlistaSelecionada[5+(12*(vpagina-1))]->vnumEpisodiosTotais);
        ui->labelAnime05Nota->setAlignment(Qt::AlignCenter);
        if(!carquivos->fprocuraEpisodio(vlistaSelecionada[5+(12*(vpagina-1))]).isEmpty())
            ui->labelAnime05Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(53, 120, 56);");
        else
            ui->labelAnime05Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime05Nota->setText("Nota: " + vlistaSelecionada[5+(12*(vpagina-1))]->vnotaMediaPessoal + "/10");
        if(vlistaAtual == "busca"){
            ui->labelFundoAnime05Lista->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelFundoAnime05Lista->setText(vlistaSelecionada[5+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelFundoAnime05Lista->clear();
            ui->labelFundoAnime05Lista->setStyleSheet("background: transparent");
        }
        if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[5+(12*(vpagina-1))]->vid+".jpg", "jpg")){
            ui->imagemAnime05->setPixmap(pix);
        }
        else if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[5+(12*(vpagina-1))]->vid+".png", "png")){
            ui->imagemAnime05->setPixmap(pix);
        }
        else{
            ui->imagemAnime05->clear();
            ui->imagemAnime05->setStyleSheet("background: black;");
        }
    }
    else{
        ui->imagemAnime05->clear();
        ui->imagemAnime05->setStyleSheet("background: transparent;");
        ui->labelAnime05Titulo->clear();
        ui->labelAnime05Progresso->clear();
        ui->labelAnime05Nota->clear();
        ui->labelFundoAnime05Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime05Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime05Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime05Lista->clear();
        ui->labelFundoAnime05Lista->setStyleSheet("background: transparent");
    }
    if(vlistaSelecionada.size() > 6+(12*(vpagina-1))){
        ui->imagemAnime06->setScaledContents(true);
        ui->labelFundoAnime06Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime06Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime06Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(vlistaSelecionada[6+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime06Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[6+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime06Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime06Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime06Titulo->setText(vlistaSelecionada[6+(12*(vpagina-1))]->vnome);
        ui->labelAnime06Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime06Titulo->setWordWrap(true);
        ui->labelAnime06Progresso->setAlignment(Qt::AlignCenter);
        ui->labelAnime06Progresso->setText("Progresso: " + vlistaSelecionada[6+(12*(vpagina-1))]->vnumEpisodiosAssistidos +
                    "/" + vlistaSelecionada[6+(12*(vpagina-1))]->vnumEpisodiosTotais);
        ui->labelAnime06Nota->setAlignment(Qt::AlignCenter);
        if(!carquivos->fprocuraEpisodio(vlistaSelecionada[6+(12*(vpagina-1))]).isEmpty())
            ui->labelAnime06Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(53, 120, 56);");
        else
            ui->labelAnime06Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime06Nota->setText("Nota: " + vlistaSelecionada[6+(12*(vpagina-1))]->vnotaMediaPessoal + "/10");
        if(vlistaAtual == "busca"){
            ui->labelFundoAnime06Lista->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelFundoAnime06Lista->setText(vlistaSelecionada[6+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelFundoAnime06Lista->clear();
            ui->labelFundoAnime06Lista->setStyleSheet("background: transparent");
        }
        if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[6+(12*(vpagina-1))]->vid+".jpg", "jpg")){
            ui->imagemAnime06->setPixmap(pix);
        }
        else if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[6+(12*(vpagina-1))]->vid+".png", "png")){
            ui->imagemAnime06->setPixmap(pix);
        }
        else{
            ui->imagemAnime06->clear();
            ui->imagemAnime06->setStyleSheet("background: black;");
        }
    }
    else{
        ui->imagemAnime06->clear();
        ui->imagemAnime06->setStyleSheet("background: transparent;");
        ui->labelAnime06Titulo->clear();
        ui->labelAnime06Progresso->clear();
        ui->labelAnime06Nota->clear();
        ui->labelFundoAnime06Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime06Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime06Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime06Lista->clear();
        ui->labelFundoAnime06Lista->setStyleSheet("background: transparent");
    }
    if(vlistaSelecionada.size() > 7+(12*(vpagina-1))){
        ui->imagemAnime07->setScaledContents(true);
        ui->labelFundoAnime07Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime07Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime07Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(vlistaSelecionada[7+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime07Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[7+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime07Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime07Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime07Titulo->setText(vlistaSelecionada[7+(12*(vpagina-1))]->vnome);
        ui->labelAnime07Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime07Titulo->setWordWrap(true);
        ui->labelAnime07Progresso->setAlignment(Qt::AlignCenter);
        if(!carquivos->fprocuraEpisodio(vlistaSelecionada[7+(12*(vpagina-1))]).isEmpty())
            ui->labelAnime07Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(53, 120, 56);");
        else
            ui->labelAnime07Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime07Progresso->setText("Progresso: " + vlistaSelecionada[7+(12*(vpagina-1))]->vnumEpisodiosAssistidos +
                    "/" + vlistaSelecionada[7+(12*(vpagina-1))]->vnumEpisodiosTotais);
        ui->labelAnime07Nota->setAlignment(Qt::AlignCenter);
        ui->labelAnime07Nota->setText("Nota: " + vlistaSelecionada[7+(12*(vpagina-1))]->vnotaMediaPessoal + "/10");
        if(vlistaAtual == "busca"){
            ui->labelFundoAnime07Lista->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelFundoAnime07Lista->setText(vlistaSelecionada[7+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelFundoAnime07Lista->clear();
            ui->labelFundoAnime07Lista->setStyleSheet("background: transparent");
        }
        if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[7+(12*(vpagina-1))]->vid+".jpg", "jpg")){
            ui->imagemAnime07->setPixmap(pix);
        }
        else if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[7+(12*(vpagina-1))]->vid+".png", "png")){
            ui->imagemAnime07->setPixmap(pix);
        }
        else{
            ui->imagemAnime07->clear();
            ui->imagemAnime07->setStyleSheet("background: black;");
        }
    }
    else{
        ui->imagemAnime07->clear();
        ui->imagemAnime07->setStyleSheet("background: transparent;");
        ui->labelAnime07Titulo->clear();
        ui->labelAnime07Progresso->clear();
        ui->labelAnime07Nota->clear();
        ui->labelFundoAnime07Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime07Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime07Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime07Lista->clear();
        ui->labelFundoAnime07Lista->setStyleSheet("background: transparent");
    }
    if(vlistaSelecionada.size() > 8+(12*(vpagina-1))){
        ui->imagemAnime08->setScaledContents(true);
        ui->labelFundoAnime08Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime08Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime08Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(vlistaSelecionada[8+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime08Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[8+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime08Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime08Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime08Titulo->setText(vlistaSelecionada[8+(12*(vpagina-1))]->vnome);
        ui->labelAnime08Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime08Titulo->setWordWrap(true);
        ui->labelAnime08Progresso->setAlignment(Qt::AlignCenter);
        if(!carquivos->fprocuraEpisodio(vlistaSelecionada[8+(12*(vpagina-1))]).isEmpty())
            ui->labelAnime08Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(53, 120, 56);");
        else
            ui->labelAnime08Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime08Progresso->setText("Progresso: " + vlistaSelecionada[8+(12*(vpagina-1))]->vnumEpisodiosAssistidos +
                    "/" + vlistaSelecionada[8+(12*(vpagina-1))]->vnumEpisodiosTotais);
        ui->labelAnime08Nota->setAlignment(Qt::AlignCenter);
        ui->labelAnime08Nota->setText("Nota: " + vlistaSelecionada[8+(12*(vpagina-1))]->vnotaMediaPessoal + "/10");
        if(vlistaAtual == "busca"){
            ui->labelFundoAnime08Lista->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelFundoAnime08Lista->setText(vlistaSelecionada[8+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelFundoAnime08Lista->clear();
            ui->labelFundoAnime08Lista->setStyleSheet("background: transparent");
        }
        if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[8+(12*(vpagina-1))]->vid+".jpg", "jpg")){
            ui->imagemAnime08->setPixmap(pix);
        }
        else if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[8+(12*(vpagina-1))]->vid+".png", "png")){
            ui->imagemAnime08->setPixmap(pix);
        }
        else{
            ui->imagemAnime08->clear();
            ui->imagemAnime08->setStyleSheet("background: black;");
        }
    }
    else{
        ui->imagemAnime08->clear();
        ui->imagemAnime08->setStyleSheet("background: transparent;");
        ui->labelAnime08Titulo->clear();
        ui->labelAnime08Progresso->clear();
        ui->labelAnime08Nota->clear();
        ui->labelFundoAnime08Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime08Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime08Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime08Lista->clear();
        ui->labelFundoAnime08Lista->setStyleSheet("background: transparent");
    }
    if(vlistaSelecionada.size() > 9+(12*(vpagina-1))){
        ui->imagemAnime09->setScaledContents(true);
        ui->labelFundoAnime09Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime09Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime09Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(vlistaSelecionada[9+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime09Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[9+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime09Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime09Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime09Titulo->setText(vlistaSelecionada[9+(12*(vpagina-1))]->vnome);
        ui->labelAnime09Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime09Titulo->setWordWrap(true);
        ui->labelAnime09Progresso->setAlignment(Qt::AlignCenter);
        if(!carquivos->fprocuraEpisodio(vlistaSelecionada[9+(12*(vpagina-1))]).isEmpty())
            ui->labelAnime09Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(53, 120, 56);");
        else
            ui->labelAnime09Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime09Progresso->setText("Progresso: " + vlistaSelecionada[9+(12*(vpagina-1))]->vnumEpisodiosAssistidos +
                    "/" + vlistaSelecionada[9+(12*(vpagina-1))]->vnumEpisodiosTotais);
        ui->labelAnime09Nota->setAlignment(Qt::AlignCenter);
        ui->labelAnime09Nota->setText("Nota: " + vlistaSelecionada[9+(12*(vpagina-1))]->vnotaMediaPessoal + "/10");
        if(vlistaAtual == "busca"){
            ui->labelFundoAnime09Lista->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelFundoAnime09Lista->setText(vlistaSelecionada[9+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelFundoAnime09Lista->clear();
            ui->labelFundoAnime09Lista->setStyleSheet("background: transparent");
        }
        if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[9+(12*(vpagina-1))]->vid+".jpg", "jpg")){
            ui->imagemAnime09->setPixmap(pix);
        }
        else if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[9+(12*(vpagina-1))]->vid+".png", "png")){
            ui->imagemAnime09->setPixmap(pix);
        }
        else{
            ui->imagemAnime09->clear();
            ui->imagemAnime09->setStyleSheet("background: black;");
        }
    }
    else{
        ui->imagemAnime09->clear();
        ui->imagemAnime09->setStyleSheet("background: transparent;");
        ui->labelAnime09Titulo->clear();
        ui->labelAnime09Progresso->clear();
        ui->labelAnime09Nota->clear();
        ui->labelFundoAnime09Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime09Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime09Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime09Lista->clear();
        ui->labelFundoAnime09Lista->setStyleSheet("background: transparent");
    }
    if(vlistaSelecionada.size() > 10+(12*(vpagina-1))){
        ui->imagemAnime10->setScaledContents(true);
        ui->labelFundoAnime10Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime10Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime10Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(vlistaSelecionada[10+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime10Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[10+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime10Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime10Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime10Titulo->setText(vlistaSelecionada[10+(12*(vpagina-1))]->vnome);
        ui->labelAnime10Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime10Titulo->setWordWrap(true);
        ui->labelAnime10Progresso->setAlignment(Qt::AlignCenter);
        if(!carquivos->fprocuraEpisodio(vlistaSelecionada[10+(12*(vpagina-1))]).isEmpty())
            ui->labelAnime10Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(53, 120, 56);");
        else
            ui->labelAnime10Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime10Progresso->setText("Progresso: " + vlistaSelecionada[10+(12*(vpagina-1))]->vnumEpisodiosAssistidos +
                    "/" + vlistaSelecionada[10+(12*(vpagina-1))]->vnumEpisodiosTotais);
        ui->labelAnime10Nota->setAlignment(Qt::AlignCenter);
        ui->labelAnime10Nota->setText("Nota: " + vlistaSelecionada[10+(12*(vpagina-1))]->vnotaMediaPessoal + "/10");
        if(vlistaAtual == "busca"){
            ui->labelFundoAnime10Lista->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelFundoAnime10Lista->setText(vlistaSelecionada[10+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelFundoAnime10Lista->clear();
            ui->labelFundoAnime10Lista->setStyleSheet("background: transparent");
        }
        if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[10+(12*(vpagina-1))]->vid+".jpg", "jpg")){
            ui->imagemAnime10->setPixmap(pix);
        }
        else if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[10+(12*(vpagina-1))]->vid+".png", "png")){
            ui->imagemAnime10->setPixmap(pix);
        }
        else{
            ui->imagemAnime10->clear();
            ui->imagemAnime10->setStyleSheet("background: black;");
        }
    }
    else{
        ui->imagemAnime10->clear();
        ui->imagemAnime10->setStyleSheet("background: transparent;");
        ui->labelAnime10Titulo->clear();
        ui->labelAnime10Progresso->clear();
        ui->labelAnime10Nota->clear();
        ui->labelFundoAnime10Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime10Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime10Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime10Lista->clear();
        ui->labelFundoAnime10Lista->setStyleSheet("background: transparent");
    }
    if(vlistaSelecionada.size() > 11+(12*(vpagina-1))){
        ui->imagemAnime11->setScaledContents(true);
        ui->labelFundoAnime11Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime11Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime11Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(vlistaSelecionada[11+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime11Titulo->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[11+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime11Titulo->setStyleSheet("background: transparent; font: 75 7pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime11Titulo->setStyleSheet("background: transparent; font: 75 6pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime11Titulo->setText(vlistaSelecionada[11+(12*(vpagina-1))]->vnome);
        ui->labelAnime11Titulo->setAlignment(Qt::AlignCenter);
        ui->labelAnime11Titulo->setWordWrap(true);
        ui->labelAnime11Progresso->setAlignment(Qt::AlignCenter);
        if(!carquivos->fprocuraEpisodio(vlistaSelecionada[11+(12*(vpagina-1))]).isEmpty())
            ui->labelAnime11Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(53, 120, 56);");
        else
            ui->labelAnime11Progresso->setStyleSheet("background: transparent; font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime11Progresso->setText("Progresso: " + vlistaSelecionada[11+(12*(vpagina-1))]->vnumEpisodiosAssistidos +
                    "/" + vlistaSelecionada[11+(12*(vpagina-1))]->vnumEpisodiosTotais);
        ui->labelAnime11Nota->setAlignment(Qt::AlignCenter);
        ui->labelAnime11Nota->setText("Nota: " + vlistaSelecionada[11+(12*(vpagina-1))]->vnotaMediaPessoal + "/10");
        if(vlistaAtual == "busca"){
            ui->labelFundoAnime11Lista->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelFundoAnime11Lista->setText(vlistaSelecionada[11+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelFundoAnime11Lista->clear();
            ui->labelFundoAnime11Lista->setStyleSheet("background: transparent");
        }
        if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[11+(12*(vpagina-1))]->vid+".jpg", "jpg")){
            ui->imagemAnime11->setPixmap(pix);
        }
        else if(pix.load(cconfBase->vdiretorioImagensMedio+vlistaSelecionada[11+(12*(vpagina-1))]->vid+".png", "png")){
            ui->imagemAnime11->setPixmap(pix);
        }
        else{
            ui->imagemAnime11->clear();
            ui->imagemAnime11->setStyleSheet("background: black;");
        }
    }
    else{
        ui->imagemAnime11->clear();
        ui->imagemAnime11->setStyleSheet("background: transparent;");
        ui->labelAnime11Titulo->clear();
        ui->labelAnime11Progresso->clear();
        ui->labelAnime11Nota->clear();
        ui->labelFundoAnime11Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime11Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime11Titulo->setStyleSheet("background: transparent;");
        ui->labelFundoAnime11Lista->clear();
        ui->labelFundoAnime11Lista->setStyleSheet("background: transparent");
    }
}

bool MainWindow::fcarregaImagensBackground(){
    if(vlistaBaixada == true)
        ui->labelMensagem->setText("Carregando imagens");
    qDebug() << "Carregando imagens";
    QPixmap lpix;
    QString file;
    for(int j = 0; j < 5; j++){
        if(vrefreshAcontecendo == true)
            return false;
        switch(j){
        case 0:
            vcarregaListaBackground = cleitorListaAnimes->sortLista(vordem, "watching");
            break;
        case 1:
            vcarregaListaBackground = cleitorListaAnimes->sortLista(vordem, "completed");
            break;
        case 2:
            vcarregaListaBackground = cleitorListaAnimes->sortLista(vordem, "onhold");
            break;
        case 3:
            vcarregaListaBackground = cleitorListaAnimes->sortLista(vordem, "dropped");
            break;
        case 4:
            vcarregaListaBackground = cleitorListaAnimes->sortLista(vordem, "plantowatch");
            break;
        }
        if(!vcarregaListaBackground.isEmpty()){
            //Depois carregar todas as imagens na memória
            for(int i = 0; i < vcarregaListaBackground.size(); i++){
                //Inicialmente carrega apenas as primeiras imagens grandes de cada lista, pra agilizar a mudança de
                //lista
                file = dirGrande+vcarregaListaBackground[i]->vid;
                if(QFile::exists(file+".jpg")){
                    file.append(".jpg");
                    if(lpix.load(file, "jpg"))
                        ui->labelImagemBackground->setPixmap(lpix);
                }
                else if(QFile::exists(file+".png")){
                    file.append(".png");
                    if(lpix.load(file, "png"))
                        ui->labelImagemBackground->setPixmap(lpix);
                }
                file = dirMedio+vcarregaListaBackground[i]->vid;
                if(QFile::exists(file+".jpg")){
                    file.append(".jpg");
                    if(lpix.load(file, "jpg"))
                        ui->labelImagemBackground->setPixmap(lpix);
                }
                else if(QFile::exists(file+".png")){
                    file.append(".png");
                    if(lpix.load(file, "png"))
                        ui->labelImagemBackground->setPixmap(lpix);
                }
            }
            vcarregaListaBackground.clear();
        }
    }
    ui->labelImagemBackground->clear();
    if(vlistaBaixada == true)
        ui->labelMensagem->setText("Todas as imagens foram carregadas com sucesso!");
    return true;
}

void MainWindow::on_botaoAnime00_clicked()
{
    if(vlistaSelecionada.size() > 0+(12*(vpagina-1))){
        vanimeSelecionado = 0+(12*(vpagina-1));
        finfoAnimeSelecionado();
    }
}

void MainWindow::on_botaoAnime01_clicked()
{
    if(vlistaSelecionada.size() > 1+(12*(vpagina-1))){
        vanimeSelecionado = 1+(12*(vpagina-1));
        finfoAnimeSelecionado();
    }
}

void MainWindow::on_botaoAnime02_clicked()
{
    if(vlistaSelecionada.size() > 2+(12*(vpagina-1))){
        vanimeSelecionado = 2+(12*(vpagina-1));
        finfoAnimeSelecionado();
    }
}

void MainWindow::on_botaoAnime03_clicked()
{
    if(vlistaSelecionada.size() > 3+(12*(vpagina-1))){
        vanimeSelecionado = 3+(12*(vpagina-1));
        finfoAnimeSelecionado();
    }
}

void MainWindow::on_botaoAnime04_clicked()
{
    if(vlistaSelecionada.size() > 4+(12*(vpagina-1))){
        vanimeSelecionado = 4+(12*(vpagina-1));
        finfoAnimeSelecionado();
    }
}

void MainWindow::on_botaoAnime05_clicked()
{
    if(vlistaSelecionada.size() > 5+(12*(vpagina-1))){
        vanimeSelecionado = 5+(12*(vpagina-1));
        finfoAnimeSelecionado();
    }
}

void MainWindow::on_botaoAnime06_clicked()
{
    if(vlistaSelecionada.size() > 6+(12*(vpagina-1))){
        vanimeSelecionado = 6+(12*(vpagina-1));
        finfoAnimeSelecionado();
    }
}

void MainWindow::on_botaoAnime07_clicked()
{
    if(vlistaSelecionada.size() > 7+(12*(vpagina-1))){
        vanimeSelecionado = 7+(12*(vpagina-1));
        finfoAnimeSelecionado();
    }
}

void MainWindow::on_botaoAnime08_clicked()
{
    if(vlistaSelecionada.size() > 8+(12*(vpagina-1))){
        vanimeSelecionado = 8+(12*(vpagina-1));
        finfoAnimeSelecionado();
    }
}

void MainWindow::on_botaoAnime09_clicked()
{
    if(vlistaSelecionada.size() > 9+(12*(vpagina-1))){
        vanimeSelecionado = 9+(12*(vpagina-1));
        finfoAnimeSelecionado();
    }
}

void MainWindow::on_botaoAnime10_clicked()
{
    if(vlistaSelecionada.size() > 10+(12*(vpagina-1))){
        vanimeSelecionado = 10+(12*(vpagina-1));
        finfoAnimeSelecionado();
    }
}

void MainWindow::on_botaoAnime11_clicked()
{
    if(vlistaSelecionada.size() > 11+(12*(vpagina-1))){
        vanimeSelecionado = 11+(12*(vpagina-1));
        finfoAnimeSelecionado();
    }
}

void MainWindow::on_botaoProximaPagina_clicked()
{
    qDebug() << "Trying to go to page " << vpagina+1 << " from " << vpagina;
    if(vlistaSelecionada.size() > 12+(12*(vpagina-1))){
        vpagina++;
        QString llistaAtual;
        if(vlistaAtual == "watching")
            llistaAtual = "Watching";
        else if(vlistaAtual == "completed")
            llistaAtual = "Completed";
        else if(vlistaAtual == "onhold")
            llistaAtual = "On Hold";
        else if(vlistaAtual == "dropped")
            llistaAtual = "Dropped";
        else if(vlistaAtual == "plantowatch")
            llistaAtual = "Plan to Watch";
        else if(vlistaAtual == "busca")
            llistaAtual = "Busca";
        ui->NumPagina->setText(llistaAtual + " - Page "+QString::number(vpagina)+"/"+QString::number(((vlistaSelecionada.size()-1)/12)+1));
        fcarregaImagensLista();
    }
}

void MainWindow::on_botaoPaginaAnterior_clicked()
{
    qDebug() << "Trying to go to page " << vpagina-1 << " from " << vpagina;
    if(vpagina > 1){
        vpagina--;
        QString llistaAtual;
        if(vlistaAtual == "watching")
            llistaAtual = "Watching";
        else if(vlistaAtual == "completed")
            llistaAtual = "Completed";
        else if(vlistaAtual == "onhold")
            llistaAtual = "On Hold";
        else if(vlistaAtual == "dropped")
            llistaAtual = "Dropped";
        else if(vlistaAtual == "plantowatch")
            llistaAtual = "Plan to Watch";
        else if(vlistaAtual == "busca")
            llistaAtual = "Busca";
        ui->NumPagina->setText(llistaAtual + " - Page "+QString::number(vpagina)+"/"+QString::number(((vlistaSelecionada.size()-1)/12)+1));
        fcarregaImagensLista();
    }
}

void MainWindow::on_Watching_clicked()
{
    qDebug() << "Selected list: Watching";
    vlistaSelecionada = cleitorListaAnimes->sortLista(vordem, "watching");
    vlistaAtual = "watching";
    vanimeSelecionado = 0;
    vpagina = 1;
    ui->NumPagina->setText("Watching - Page "+QString::number(vpagina)+"/"+QString::number(((vlistaSelecionada.size()-1)/12)+1));
    finfoAnimeSelecionado();
}

void MainWindow::on_Completed_clicked()
{
    qDebug() << "Selected list: Completed";
    vlistaAtual = "completed";
    vlistaSelecionada = cleitorListaAnimes->sortLista(vordem, "completed");
    vanimeSelecionado = 0;
    vpagina = 1;
    ui->NumPagina->setText("Completed - Page "+QString::number(vpagina)+"/"+QString::number(((vlistaSelecionada.size()-1)/12)+1));
    finfoAnimeSelecionado();
}

void MainWindow::on_OnHold_clicked()
{
    qDebug() << "Selected list: On Hold";
    vlistaSelecionada = cleitorListaAnimes->sortLista(vordem, "onhold");
    vlistaAtual = "onhold";
    vanimeSelecionado = 0;
    vpagina = 1;
    ui->NumPagina->setText("On Hold - Page "+QString::number(vpagina)+"/"+QString::number(((vlistaSelecionada.size()-1)/12)+1));
    finfoAnimeSelecionado();
}

void MainWindow::on_Dropped_clicked()
{
    qDebug() << "Selected list: Dropped";
    vlistaSelecionada = cleitorListaAnimes->sortLista(vordem, "dropped");
    vlistaAtual = "dropped";
    vanimeSelecionado = 0;
    vpagina = 1;
    ui->NumPagina->setText("Dropped - Page "+QString::number(vpagina)+"/"+QString::number(((vlistaSelecionada.size()-1)/12)+1));
    finfoAnimeSelecionado();
}

void MainWindow::on_PlanToWatch_clicked()
{
    qDebug() << "Selected list: Plan to Watch";
    vlistaSelecionada = cleitorListaAnimes->sortLista(vordem, "plantowatch");
    vlistaAtual = "plantowatch";
    vanimeSelecionado = 0;
    vpagina = 1;
    ui->NumPagina->setText("Plan to Watch - Page "+QString::number(vpagina)+"/"+QString::number(((vlistaSelecionada.size()-1)/12)+1));
    finfoAnimeSelecionado();
}

void MainWindow::on_botaoProximoEpisodio_clicked()
{
    ui->labelMensagem->setText("Buscando episódio...");
    if(!carquivos->fprocuraEpisodio(vlistaSelecionada[vanimeSelecionado]).isEmpty()){
        if(carquivos->fabreEpisodio(carquivos->fprocuraEpisodio(vlistaSelecionada[vanimeSelecionado])))
            ui->labelMensagem->setText("Abrindo episódio!");
    }
    else
        ui->labelMensagem->setText("O episódio não foi encontrado no computador");
}

void MainWindow::on_botaoAbrePasta_clicked()
{
    qDebug() << "Trying to open anime folder";
    //Pega o caminho da pasta onde o episódio está e converte para o caminho nativo do OS (só pra evitar erros com o nome)
    QString lcaminhoPasta = cconfUsuario->fretornaDiretorioEspecifico(vlistaSelecionada[vanimeSelecionado]->vid.toInt(  ));
    if(!lcaminhoPasta.isEmpty())
        carquivos->fabreEpisodio(lcaminhoPasta);
    else
        ui->labelMensagem->setText("A pasta com este anime não foi encontrada");
}

void MainWindow::on_botaoBusca_clicked()
{
    if(!ui->barraBusca->toPlainText().isEmpty()){
        qDebug() << "Trying to search anime " << ui->barraBusca->toPlainText();
        vlistaSelecionada = cleitorListaAnimes->fbuscaLista(ui->barraBusca->toPlainText());
        if(!vlistaSelecionada.isEmpty()){
            vlistaAtual = "busca";
            vanimeSelecionado = 0;
            vpagina = 1;
            ui->NumPagina->setText("Busca - Page "+QString::number(vpagina)+"/"+QString::number(((vlistaSelecionada.size()-1)/12)+1));
            finfoAnimeSelecionado();
        }
    }
}

void MainWindow::on_botaoRefresh_clicked()
{
    if(vrefreshAcontecendo == false){
        vrefreshAcontecendo = true;
        //Quando clicar no botão, o programa vai rodar a função de baixar as listas em uma thread e quando a lista for baixada, será
        //atualizada na função frefreshListas();
//        canilist->fbaixaListaThread(cThread);
//        canilist->moveToThread(&cThread);
        cThread.start();
        //Com esse bool, podemos controlar outras funções que rodam ao mesmo tempo, como a de carregar imagens no background
        ui->labelRefreshTimer->setText("Refreshing,,,");
    }
}

void MainWindow::frefreshListas(bool rcheckDownload){
    //Checamos se a conexão com o anilist foi um sucesso. No caso de ter falhado, não rodamos a função de refresh
    if(rcheckDownload == false){
        vrefreshAcontecendo = false;
        ui->labelMensagem->setText("Failed to connect to AniList!");
        return;
    }
    if(vfalhaconexao == true){
        tryTimer->stop();
        vfalhaconexao = false;
    }
    ui->labelMensagem->setText("Anime lists downloaded");

    //Atualiza o avatar do usuário também.
    cfiledownloader->fdownloadAvatarUsuario(canilist->fretornaAvatar());
    connect(cfiledownloader, &filedownloader::savatar, this, &MainWindow::fcarregaImagensLista, Qt::QueuedConnection);

    //Quando rolar a busca, a lista vai resetar. Esse index será usado pra voltar na info do anime que a pessoa estava vendo quando
    //rolou o refresh
    int lanimeAtual = 0;
    int lpaginaAtual = 0;
    bool lbusca = false;
    //Vamos bloquear todos os botões para evitar que o usuário tente ler um anime que saiu da lista
    ui->labelMensagem->setText("Loading new animes. It will be working again in a few seconds");
    fbloqueiaSinaisBotoes();

    qDebug() << "Refreshing list...";

    //Apagamos a lista antiga para poder reescrevela
    cleitorListaAnimes->fdeletaListaAnimes();
    //Se não houver nenhum problema no download ou no caso do usuário deletar o arquivo das listas, elas vão estar sempre prontas
    if(cleitorListaAnimes->fleJson()){
//        timer->start(60000); //1 MINUTO PRA TESTES
        timer->start(600000);
        vtimerSegundos = 59;
        qDebug() << "The anime list have been read successfully";
    }
    else{
        qCritical() << "There was a problem reading the anime list";
        ui->labelMensagem->setText("Failed to refresh!");
        fliberaSinaisBotoes();
        vrefreshAcontecendo = false;
        return;
    }
    qDebug() << "Checking anime folders";
    ui->labelMensagem->setText("Searching for animes in the computer");
    cconfUsuario->frecebeListaAnime(cleitorListaAnimes);
    if(vprimeiraVezThread == true){
        vprimeiraVezThread = false;
        cconfUsuario->fbuscaPastasThread(dThread);
        cconfUsuario->moveToThread(&dThread);
        dThread.start();
    }
    else
        dThread.start();

    connect(cconfUsuario, &confUsuario::schecouPastas, this, &MainWindow::fmandaDiretoriosArquivos, Qt::QueuedConnection);

    cfiledownloader->fsetLeitorListaAnimes(cleitorListaAnimes);
    cfiledownloader->fsetNext();
    qDebug() << "Images ready";
    //Crashando daqui pra frente

    //Caso o usuário esteja no meio de uma busca quando atualiza a lista, precisamos salvar o index do anime que ele estava vendo.
    lpaginaAtual = vpagina;
    lanimeAtual = vanimeSelecionado;
    if(vlistaAtual.compare("busca", Qt::CaseInsensitive) == 0){
        //Possivel que seja na ordenação
        vlistaSelecionada = cleitorListaAnimes->sortLista(vordem, "watching");
        lbusca = true;
    }
    else
        vlistaSelecionada = cleitorListaAnimes->sortLista(vordem, vlistaAtual);
    if(lbusca){
        on_botaoBusca_clicked();
        for(int i = 0; i < lpaginaAtual-1; i++){
            on_botaoProximaPagina_clicked();
        }
    }
    if(lanimeAtual < vlistaSelecionada.size() && lanimeAtual > 0){
        ///Possivelmente aqui. Se lanimeAtual for -1 por algum motivo, poderia bugar
        vanimeSelecionado = lanimeAtual;
    }
    qDebug() << "The lists were updated";
    ui->labelMensagem->setText("As listas foram atualizadas!");
    fliberaSinaisBotoes();
    finfoAnimeSelecionado();
    vrefreshAcontecendo = false;
    QFuture<void> lcarregaImagens = QtConcurrent::run(this, &MainWindow::fcarregaImagensBackground);
}

void MainWindow::fmandaDiretoriosArquivos()
{
    ui->labelMensagem->setText("All animes in the computer were found");
    carquivos->frecebeDiretorios(cconfUsuario);
}

void MainWindow::favisoErro(QString rerro)
{
    ui->labelMensagem->setText(rerro);
}

void MainWindow::fbloqueiaSinaisBotoes(){
    QList<QPushButton *> listaBotoes = ui->listaAnimes->findChildren<QPushButton *>();
    foreach(QPushButton *psh, listaBotoes){
        psh->blockSignals(true);
    }
}

void MainWindow::fliberaSinaisBotoes(){
    QList<QPushButton *> listaBotoes = ui->listaAnimes->findChildren<QPushButton *>();
    foreach(QPushButton *psh, listaBotoes){
        psh->blockSignals(false);
    }
}

void MainWindow::fatualizaRefreshTimer()
{
    vlistaBaixada = true;
    if(vrefreshAcontecendo == false){
        int lminutos = timer->remainingTime()/60000;
        if(vtimerSegundos == 0)
            ui->labelRefreshTimer->setText(QString::number(lminutos) + ":00");
        else if(vtimerSegundos < 10)
            ui->labelRefreshTimer->setText(QString::number(lminutos) + ":0" + QString::number(vtimerSegundos));
        else
            ui->labelRefreshTimer->setText(QString::number(lminutos) + ":" + QString::number(vtimerSegundos));
        if(vtimerSegundos == 0)
            vtimerSegundos = 59;
        else
            vtimerSegundos--;
    }
}

void MainWindow::on_botaoHome_clicked()
{
    ui->janelaRotativa->setCurrentIndex(0);
}

void MainWindow::on_botaoConfiguracao_clicked()
{
    ui->janelaRotativa->setCurrentIndex(1);
}

void MainWindow::on_botaoNotaMais_clicked()
{
    if(vlistaSelecionada[vanimeSelecionado]->vnotaMediaPessoal.toInt() < 10){
        vlistaSelecionada[vanimeSelecionado]->vnotaMediaPessoal = QString::number(vlistaSelecionada[vanimeSelecionado]->vnotaMediaPessoal.toInt()+1);
        ui->labelInfoNota->setText(vlistaSelecionada[vanimeSelecionado]->vnotaMediaPessoal);
        finfoAnimeSelecionado();
        QString lacao = "nota:" + vlistaSelecionada[vanimeSelecionado]->vid;
        QStringList lstringListAcao = lacao.split(':');
        vlistaAcoes.insert(lstringListAcao, QString::number(vlistaSelecionada[vanimeSelecionado]->vnotaMediaPessoal.toInt()));
    }
}

void MainWindow::on_botaoNotaMenos_clicked()
{
    if(vlistaSelecionada[vanimeSelecionado]->vnotaMediaPessoal.toInt() > 0){
        vlistaSelecionada[vanimeSelecionado]->vnotaMediaPessoal = QString::number(vlistaSelecionada[vanimeSelecionado]->vnotaMediaPessoal.toInt()-1);
        ui->labelInfoNota->setText(vlistaSelecionada[vanimeSelecionado]->vnotaMediaPessoal);
        finfoAnimeSelecionado();
        QString lacao = "nota:" + vlistaSelecionada[vanimeSelecionado]->vid;
        QStringList lstringListAcao = lacao.split(':');
        vlistaAcoes.insert(lstringListAcao, QString::number(vlistaSelecionada[vanimeSelecionado]->vnotaMediaPessoal.toInt()));
    }
}

void MainWindow::on_botaoProgressoMais_clicked()
{
    int lepisodiosTotais = 0;
    //Caso o número máximo de episódios não seja conhecido, não deve existir um limite;
    if(vlistaSelecionada[vanimeSelecionado]->vnumEpisodiosTotais != "?")
        lepisodiosTotais = vlistaSelecionada[vanimeSelecionado]->vnumEpisodiosTotais.toInt();
    else
        lepisodiosTotais = INT_MAX;
    if(vlistaSelecionada[vanimeSelecionado]->vnumEpisodiosAssistidos.toInt() < lepisodiosTotais){
        vlistaSelecionada[vanimeSelecionado]->vnumEpisodiosAssistidos = QString::number(vlistaSelecionada[vanimeSelecionado]->vnumEpisodiosAssistidos.toInt()+1);
        finfoAnimeSelecionado();
        QString lacao = "progresso:" + vlistaSelecionada[vanimeSelecionado]->vid;
        QStringList lstringListAcao = lacao.split(':');
        vlistaAcoes.insert(lstringListAcao, QString::number(vlistaSelecionada[vanimeSelecionado]->vnumEpisodiosAssistidos.toInt()));
        if(vlistaSelecionada[vanimeSelecionado]->vnumEpisodiosAssistidos.toInt() == vlistaSelecionada[vanimeSelecionado]->vnumEpisodiosTotais.toInt()){
            ui->boxMudarPraLista->setCurrentIndex(1);
            on_botaoMudarPraLista_clicked();
        }
    }
}

void MainWindow::on_botaoProgressoMenos_clicked()
{
    if(vlistaSelecionada[vanimeSelecionado]->vnumEpisodiosAssistidos.toInt() > 0){
        vlistaSelecionada[vanimeSelecionado]->vnumEpisodiosAssistidos = QString::number(vlistaSelecionada[vanimeSelecionado]->vnumEpisodiosAssistidos.toInt()-1);
        finfoAnimeSelecionado();
        QString lacao = "progresso:" + vlistaSelecionada[vanimeSelecionado]->vid;
        QStringList lstringListAcao = lacao.split(':');
        vlistaAcoes.insert(lstringListAcao, QString::number(vlistaSelecionada[vanimeSelecionado]->vnumEpisodiosAssistidos.toInt()));
    }
}

void MainWindow::fatualizaAnilist(){
    int i = 0;
    if(vlistaAcoes.isEmpty())
        return;
    foreach (QStringList key, vlistaAcoes.keys()) {
        if(i == 90)
            return;
        if(key.at(0) == "nota"){
            if(canilist->fmudaNota(key.at(1).toInt(), vlistaAcoes[key].toInt())){
                vlistaAcoes.remove(key);
            }
            else
                qDebug() << "Não foi possível acessar a base de dados do anilist";
        }
        else if(key.at(0) == "progresso"){
            if(canilist->fmudaProgresso(key.at(1).toInt(), vlistaAcoes[key].toInt())){
                vlistaAcoes.remove(key);
            }
            else
                qDebug() << "Não foi possível acessar a base de dados do anilist";
        }
        else if(key.at(0) == "lista"){
            if(canilist->fmudaLista(key.at(1).toInt(), vlistaAcoes[key])){
                vlistaAcoes.remove(key);
            }
            else
                qDebug() << "Não foi possível acessar a base de dados do anilist";
        }
        else if(key.at(0) == "remove"){
            if(canilist->fexcluiAnime(key.at(1).toInt()))
                vlistaAcoes.remove(key);
            else
                qDebug() << "Não foi possível acessar a base de dados do anilist";
        }
        i++;
    }
}

void MainWindow::on_botaoMudarPraLista_clicked()
{
    qDebug() << "Updating anime list";
    vordem = "";
    if(ui->boxMudarPraLista->currentText().compare(vlistaAtual, Qt::CaseInsensitive) == 0)
        ui->labelMensagem->setText("Já está na lista correta!");
    else if(ui->boxMudarPraLista->currentText().compare("Watching") == 0){
        vlistaSelecionada[vanimeSelecionado]->vlista = "Watching";
        //Atualiza o anilist e a lista local de animes
        canilist->fmudaLista(vlistaSelecionada[vanimeSelecionado]->vid.toInt(), "CURRENT");
        cleitorListaAnimes->fmudaLista(vlistaSelecionada[vanimeSelecionado]->vid, vlistaSelecionada[vanimeSelecionado]->vlista);
    }
    else if(ui->boxMudarPraLista->currentText().compare("Completed") == 0){
        vlistaSelecionada[vanimeSelecionado]->vlista = "Completed";
        canilist->fmudaLista(vlistaSelecionada[vanimeSelecionado]->vid.toInt(), "COMPLETED");
        cleitorListaAnimes->fmudaLista(vlistaSelecionada[vanimeSelecionado]->vid, vlistaSelecionada[vanimeSelecionado]->vlista);
    }
    else if(ui->boxMudarPraLista->currentText().compare("On Hold") == 0){
        vlistaSelecionada[vanimeSelecionado]->vlista = "On Hold";
        canilist->fmudaLista(vlistaSelecionada[vanimeSelecionado]->vid.toInt(), "PAUSED");
        cleitorListaAnimes->fmudaLista(vlistaSelecionada[vanimeSelecionado]->vid, vlistaSelecionada[vanimeSelecionado]->vlista);
    }
    else if(ui->boxMudarPraLista->currentText().compare("Dropped") == 0){
        vlistaSelecionada[vanimeSelecionado]->vlista = "Dropped";
        canilist->fmudaLista(vlistaSelecionada[vanimeSelecionado]->vid.toInt(), "DROPPED");
        cleitorListaAnimes->fmudaLista(vlistaSelecionada[vanimeSelecionado]->vid, vlistaSelecionada[vanimeSelecionado]->vlista);
    }
    else if(ui->boxMudarPraLista->currentText().compare("Plan to Watch") == 0){
        vlistaSelecionada[vanimeSelecionado]->vlista = "Plan to Watch";
        canilist->fmudaLista(vlistaSelecionada[vanimeSelecionado]->vid.toInt(), "PLANNING");
        cleitorListaAnimes->fmudaLista(vlistaSelecionada[vanimeSelecionado]->vid, vlistaSelecionada[vanimeSelecionado]->vlista);
    }
    vlistaSelecionada = cleitorListaAnimes->sortLista(vordem, vlistaAtual);
    if(vlistaSelecionada.size() <= vanimeSelecionado){
        vanimeSelecionado = 0;
        vpagina = 1;
    }
    finfoAnimeSelecionado();
    fatualizaAnilist();
}

void MainWindow::on_botaoAnilist_clicked()
{
    QDesktopServices::openUrl(QUrl(vlistaSelecionada[vanimeSelecionado]->vsiteAnilist));
}

void MainWindow::on_botaoCrunchyroll_clicked()
{
    if(!QDesktopServices::openUrl(QUrl(vlistaSelecionada[vanimeSelecionado]->vstreamCrunchyroll)))
        ui->labelMensagem->setText("A página do Crunchyroll não foi encontrada");
}

void MainWindow::on_botaoTorrent_clicked()
{
    ui->janelaRotativa->setCurrentIndex(2);
}

void MainWindow::on_botaoRemoverdaLista_clicked()
{
    qDebug() << "Removing anime from list";
    vordem = "";
    QString lacao = "remove:" + vlistaSelecionada[vanimeSelecionado]->vid;
    QStringList lstringListAcao = lacao.split(':');
    vlistaAcoes.insert(lstringListAcao, "null");
    cleitorListaAnimes->fdeletedaLista(vlistaSelecionada[vanimeSelecionado]->vid, vlistaSelecionada[vanimeSelecionado]->vlista);
    vlistaSelecionada = cleitorListaAnimes->sortLista(vordem, vlistaAtual);
    if(vlistaSelecionada.size() <= vanimeSelecionado){
        vanimeSelecionado = 0;
        vpagina = 1;
    }
    finfoAnimeSelecionado();
    fatualizaAnilist();
}

void MainWindow::fretryAnilist()
{
    canilist->frecebeAutorizacao(jconfig.fretornaUsuario(),jconfig.fretornaCodigoAutorizacao());
    canilist->fbaixaListaThread(cThread);
    canilist->moveToThread(&cThread);
    cThread.start();
}

void MainWindow::on_botaoOrdemAlfabetica_clicked()
{
    qDebug() << "Trying to sort list by alphabetical order";
    if(vordem.compare("cnome", Qt::CaseInsensitive) == 0)
        vordem = "dnome";
    else
        vordem = "cnome";
    vlistaSelecionada = cleitorListaAnimes->sortLista(vordem, vlistaAtual);
    if(!vlistaSelecionada.isEmpty()){
        vanimeSelecionado = 0;
        vpagina = 1;
        ui->NumPagina->setText("Page "+QString::number(vpagina)+"/"+QString::number(((vlistaSelecionada.size()-1)/12)+1));
        finfoAnimeSelecionado();
    }
}

void MainWindow::on_botaoOrdemProgresso_clicked()
{
    qDebug() << "Trying to sort list by progress order";
    if(vordem.compare("cprogresso", Qt::CaseInsensitive) == 0)
        vordem = "dprogresso";
    else
        vordem = "cprogresso";
    vlistaSelecionada = cleitorListaAnimes->sortLista(vordem, vlistaAtual);
    if(!vlistaSelecionada.isEmpty()){
        vanimeSelecionado = 0;
        vpagina = 1;
        ui->NumPagina->setText("Page "+QString::number(vpagina)+"/"+QString::number(((vlistaSelecionada.size()-1)/12)+1));
        finfoAnimeSelecionado();
    }
}

void MainWindow::on_botaoOrdemScore_clicked()
{
    qDebug() << "Trying to sort list by score order";
    if(vordem.compare("cnota", Qt::CaseInsensitive) == 0)
        vordem = "dnota";
    else
        vordem = "cnota";
    vlistaSelecionada = cleitorListaAnimes->sortLista(vordem, vlistaAtual);
    if(!vlistaSelecionada.isEmpty()){
        vanimeSelecionado = 0;
        vpagina = 1;
        ui->NumPagina->setText("Page "+QString::number(vpagina)+"/"+QString::number(((vlistaSelecionada.size()-1)/12)+1));
        finfoAnimeSelecionado();
    }
}

void MainWindow::on_botaoOrdemRelease_clicked()
{
    qDebug() << "Trying to sort list by release order";
    if(vordem.compare("cdata", Qt::CaseInsensitive) == 0)
        vordem = "ddata";
    else
        vordem = "cdata";
    vlistaSelecionada = cleitorListaAnimes->sortLista(vordem, vlistaAtual);
    if(!vlistaSelecionada.isEmpty()){
        vanimeSelecionado = 0;
        vpagina = 1;
        ui->NumPagina->setText("Page "+QString::number(vpagina)+"/"+QString::number(((vlistaSelecionada.size()-1)/12)+1));
        finfoAnimeSelecionado();
    }
}

void MainWindow::on_botaoOrdemType_clicked()
{
    qDebug() << "Trying to sort list by type order";
    if(vordem.compare("cformato", Qt::CaseInsensitive) == 0)
        vordem = "dformato";
    else
        vordem = "cformato";
    vlistaSelecionada = cleitorListaAnimes->sortLista(vordem, vlistaAtual);
    if(!vlistaSelecionada.isEmpty()){
        vanimeSelecionado = 0;
        vpagina = 1;
        ui->NumPagina->setText("Page "+QString::number(vpagina)+"/"+QString::number(((vlistaSelecionada.size()-1)/12)+1));
        finfoAnimeSelecionado();
    }
}


//Eu sei que seria muito melhor usar qml pra fazer isso. Seria 10x mais fácil e mais prático de programar.
//Mas ainda não aprendi QML e estou para aprender num futuro próximo. Assim que começar a aprender, isso vai sumir
//e iriei mudar o visual do programa
void MainWindow::fmudaResolucao(){
    QPointer<QScreen> screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();
    QObjectList objectList = ui->centralwidget->children();
    //1000 é a altura da janela e 1080 a altura do monitor, em pixels. Preciso fazer isso para não deixar a janela ficar atrás do menu
    //iniciar. O windows aceita a janela ficar atrás, mas imagino que terão OS que não irão aceitar.
    int objheight = qCeil(static_cast<int>(height*(static_cast<double>(1000)/1080)));
    //Comparamos o tamanho da página original com a nova resolução, para definir em quantos % deveremos aumentar ou diminuir os
    //widgets
    double compHeight = static_cast<double>(objheight)/1000;
    double compWidth = static_cast<double>(width)/1920;
    foreach(QObject *wid, objectList){
        //Pego a posição relativa dos objetos
        double a = static_cast<double>(this->findChild<QWidget *>(wid->objectName())->pos().rx())/1920;
        double b = static_cast<double>(this->findChild<QWidget *>(wid->objectName())->pos().ry())/1000;
        //Variáveis que não precisavam existir. Posso não usar elas, mas deixaria o código mais difícil de se ler
        double objWid = static_cast<double>(this->findChild<QWidget *>(wid->objectName())->width());
        double objHei = static_cast<double>(this->findChild<QWidget *>(wid->objectName())->height());
        //Multiplico o tamanho real do objeto pelo tamanho relativo do objeto.
        //Ex: Se o objeto ocupa, originalmente, 5% do tamanho da página, faço o novo objeto ocupar 5% do tamanho da página com a
        //Nova resolução
        this->findChild<QWidget *>(wid->objectName())->resize(qCeil(static_cast<int>(compWidth*objWid)),
                                                              qCeil(static_cast<int>(compHeight*objHei)));
        //Faço os objetos entrarem na posição certa;
        this->findChild<QWidget *>(wid->objectName())->move(qCeil(static_cast<int>(a*width)), qCeil(static_cast<int>(b*objheight)));
    }
    //Essa função funciona muito bem, mas algumas coisas ficam com 1 ou 2 pixels fora do lugar
    int widheight = qCeil(static_cast<int>(height*(static_cast<double>(821)/1080)));
//    QWidgetList widgetList = ui->listaAnimes->findChildren<QWidget *>();
//    QObjectList widgetList = ui->listaAnimes->findChildren();
//    foreach(QObject *wid, widgetList){
//        double a = static_cast<double>(this->findChild<QWidget *>(wid->objectName())->pos().rx())/1920;
//        double b = static_cast<double>(this->findChild<QWidget *>(wid->objectName())->pos().ry())/821;
//        double compHeight = static_cast<double>(widheight)/821;
//        double compWidth = static_cast<double>(width)/1920;
//        double objWid = static_cast<double>(this->findChild<QWidget *>(wid->objectName())->width());
//        double objHei = static_cast<double>(this->findChild<QWidget *>(wid->objectName())->height());
//        qDebug() << wid->objectName() << this->findChild<QWidget *>(wid->objectName())->pos().rx()
//                 << this->findChild<QWidget *>(wid->objectName())->pos().ry()
//                 << a*width << b*widheight;
//        this->findChild<QWidget *>(wid->objectName())->resize(qCeil(static_cast<int>(compWidth*objWid)),
//                                                              qCeil(static_cast<int>(compHeight*objHei)));
//        this->findChild<QWidget *>(wid->objectName())->move(qCeil(static_cast<int>(a*width)), qCeil(static_cast<int>(b*widheight)));

//    }
    //Dou resize na janela
    double comprimentoJanela = static_cast<double>(width)/1920;
    double alturaJanela = static_cast<double>(widheight)/821;
    //Pego a posição e o tamanho do label que vou usar como referência, já com a nova resolução
    double posxLabelAnime = static_cast<double>(ui->imagemAnime00->pos().rx())/1920;
    double posyLabelAnime = static_cast<double>(ui->imagemAnime00->pos().ry())/821;
    double larguraLabelAnime = static_cast<double>(ui->imagemAnime00->width());
    double alturaLabelAnime = static_cast<double>(ui->imagemAnime00->height());
    //Dou resize nos labeis base pra nova resolução
    ui->imagemAnime00->resize(static_cast<int>(comprimentoJanela*larguraLabelAnime),
                             static_cast<int>(alturaJanela*alturaLabelAnime));
    ui->imagemAnime00->move(static_cast<int>(posxLabelAnime*width),
                           static_cast<int>(posyLabelAnime*widheight));
    ui->labelFundoAnime00Titulo->resize(static_cast<int>(comprimentoJanela*ui->labelFundoAnime00Titulo->width()),
                             static_cast<int>(alturaJanela*ui->labelFundoAnime00Titulo->height()));
    ui->labelFundoAnime00Titulo->move(static_cast<int>((static_cast<double>(ui->labelFundoAnime00Titulo->pos().rx())/1920)*width),
                           static_cast<int>((static_cast<double>(ui->labelFundoAnime00Titulo->pos().ry())/821)*widheight));
    ui->labelAnime00Titulo->resize(static_cast<int>(comprimentoJanela*ui->labelAnime00Titulo->width()),
                             static_cast<int>(alturaJanela*ui->labelAnime00Titulo->height()));
    ui->labelAnime00Titulo->move(static_cast<int>((static_cast<double>(ui->labelAnime00Titulo->pos().rx())/1920)*width),
                           static_cast<int>((static_cast<double>(ui->labelAnime00Titulo->pos().ry())/821)*widheight));
    ui->imagemAnimeGrande->resize(static_cast<int>(comprimentoJanela*ui->imagemAnimeGrande->width()),
                             static_cast<int>(alturaJanela*ui->imagemAnimeGrande->height()));
    ui->imagemAnimeGrande->move(static_cast<int>((static_cast<double>(ui->imagemAnimeGrande->pos().rx())/1920)*width),
                           static_cast<int>((static_cast<double>(ui->imagemAnimeGrande->pos().ry())/821)*widheight));

    //Começo a aplicar isso aos outros labels. Pra isso, preciso fazer uma lista com todos os labels disponíveis
    QWidgetList widgetList = ui->listaAnimes->findChildren<QWidget *>();
    //Criar um loop para iterá-los
    foreach(QObject *wid, widgetList){
        //E ver qual label é qual, pra saber como mudá-los
        //No caso, se eles forem labeis da mesma categoria que o que escolhi pra referência, eles devem ter sempre o mesmo tamanho
        //Mas diferentes posições
        if(this->findChild<QWidget *>(wid->objectName())->objectName().contains("botaoAnime", Qt::CaseInsensitive)
                || (this->findChild<QWidget *>(wid->objectName())->objectName().contains("imagemAnime", Qt::CaseInsensitive)
                && !this->findChild<QWidget *>(wid->objectName())->objectName().contains("grande", Qt::CaseInsensitive))){
            this->findChild<QWidget *>(wid->objectName())->resize(static_cast<int>(comprimentoJanela*larguraLabelAnime),
                                                                  static_cast<int>(alturaJanela*alturaLabelAnime));
            QString vposicaoGrade;
            if(this->findChild<QWidget *>(wid->objectName())->objectName().contains("botaoAnime", Qt::CaseInsensitive))
                vposicaoGrade = this->findChild<QWidget *>(wid->objectName())->objectName().remove("botaoAnime");
            else if(this->findChild<QWidget *>(wid->objectName())->objectName().contains("imagemAnime", Qt::CaseInsensitive))
                vposicaoGrade = this->findChild<QWidget *>(wid->objectName())->objectName().remove("imagemAnime");
            //Se for um múltiplo de 3, fica na primeira coluna
            if(vposicaoGrade.toInt()%3 == 0)
                this->findChild<QWidget *>(wid->objectName())->move(ui->imagemAnime00->x(),
                    static_cast<int>(ui->imagemAnime00->y()+((ui->imagemAnime00->height()+2)*vposicaoGrade.toInt()/3)));
            else if((vposicaoGrade.toInt()-1)%3 == 0)
                this->findChild<QWidget *>(wid->objectName())->move(ui->labelFundoAnime00Titulo->x()
                                                                    +ui->labelFundoAnime00Titulo->width()+10,
                    static_cast<int>(ui->imagemAnime00->y()+((ui->imagemAnime00->height()+2)*(vposicaoGrade.toInt()-1)/3)));
            else if((vposicaoGrade.toInt()-2)%3 == 0)
                this->findChild<QWidget *>(wid->objectName())->move((ui->labelFundoAnime00Titulo->x() + ui->imagemAnime00->width()
                                                                    +(ui->labelFundoAnime00Titulo->width()+10)*2),
                    static_cast<int>(ui->imagemAnime00->y()+((ui->imagemAnime00->height()+2)*(vposicaoGrade.toInt()-2)/3)));
        }
        //Labelfundo e label anime são separados por motivos de tamanho do label
        else if(this->findChild<QWidget *>(wid->objectName())->objectName().contains("labelFundoAnime", Qt::CaseInsensitive)){
            if(this->findChild<QWidget *>(wid->objectName())->objectName().contains("titulo", Qt::CaseInsensitive))
                this->findChild<QWidget *>(wid->objectName())->resize(ui->labelFundoAnime00Titulo->width(),
                                                                      ui->labelFundoAnime00Titulo->height());
            else
                this->findChild<QWidget *>(wid->objectName())->resize(ui->labelFundoAnime00Titulo->width(),
                                                                      ui->labelFundoAnime00Titulo->height()/2);

            QString vposicaoGrade = this->findChild<QWidget *>(wid->objectName())->objectName().remove("labelFundoAnime");
            int ypos = 0;
            if(this->findChild<QWidget *>(wid->objectName())->objectName().contains("titulo", Qt::CaseInsensitive)){
                vposicaoGrade = vposicaoGrade.remove("titulo", Qt::CaseInsensitive);
                ypos = ui->labelFundoAnime00Titulo->y();
            }
            else if(this->findChild<QWidget *>(wid->objectName())->objectName().contains("progresso", Qt::CaseInsensitive)){
                vposicaoGrade = vposicaoGrade.remove("progresso", Qt::CaseInsensitive);
                ypos = ui->labelFundoAnime00Titulo->y()+ui->labelFundoAnime00Titulo->height()+1;
            }
            else if(this->findChild<QWidget *>(wid->objectName())->objectName().contains("nota", Qt::CaseInsensitive)){
                vposicaoGrade = vposicaoGrade.remove("nota", Qt::CaseInsensitive);
                ypos = static_cast<int>(ui->labelFundoAnime00Titulo->y()+(ui->labelFundoAnime00Titulo->height()*1.5)+2);
            }
            else if(this->findChild<QWidget *>(wid->objectName())->objectName().contains("lista", Qt::CaseInsensitive)){
                vposicaoGrade = vposicaoGrade.remove("lista", Qt::CaseInsensitive);
                ypos = ui->labelFundoAnime00Titulo->y()+ui->labelFundoAnime00Titulo->height()+
                        ((ui->labelFundoAnime00Progresso->height())*2)+3;
            }
            else{
                ypos = ui->labelFundoAnime00Titulo->y();
            }
            if(vposicaoGrade.toInt()%3 == 0){
                this->findChild<QWidget *>(wid->objectName())->move(ui->labelFundoAnime00Titulo->x(),
                    static_cast<int>(ypos+((2+ui->imagemAnime00->height())*
                        (vposicaoGrade.toInt()/3))));
            }
            else if((vposicaoGrade.toInt()-1)%3 == 0)
                this->findChild<QWidget *>(wid->objectName())->move(ui->labelFundoAnime00Titulo->x()
                                                                    +ui->labelFundoAnime00Titulo->width()+10
                                                                    +ui->imagemAnime00->width(),
                    static_cast<int>(ypos+((2+ui->imagemAnime00->height())*
                                           (vposicaoGrade.toInt()/3))));
            else if((vposicaoGrade.toInt()-2)%3 == 0)
                this->findChild<QWidget *>(wid->objectName())->move(ui->labelFundoAnime00Titulo->x()
                                                                    +((ui->labelFundoAnime00Titulo->width()+10
                                                                    +ui->imagemAnime00->width())*2),
                    static_cast<int>(ypos+((2+ui->imagemAnime00->height())*
                                           (vposicaoGrade.toInt()/3))));
        }
        //Agora começa a mudar os textos
        else if(this->findChild<QWidget *>(wid->objectName())->objectName().contains("labelAnime", Qt::CaseInsensitive)){
            if(this->findChild<QWidget *>(wid->objectName())->objectName().contains("titulo", Qt::CaseInsensitive))
                this->findChild<QWidget *>(wid->objectName())->resize(ui->labelAnime00Titulo->width(),
                                                                      ui->labelAnime00Titulo->height());
            else
                this->findChild<QWidget *>(wid->objectName())->resize(ui->labelAnime00Titulo->width(),
                                                                      ui->labelAnime00Titulo->height()/2);

            QString vposicaoGrade = this->findChild<QWidget *>(wid->objectName())->objectName().remove("labelAnime");
            int ypos = 0;
            if(this->findChild<QWidget *>(wid->objectName())->objectName().contains("titulo", Qt::CaseInsensitive)){
                vposicaoGrade = vposicaoGrade.remove("titulo", Qt::CaseInsensitive);
                ypos = ui->labelAnime00Titulo->y();
            }
            else if(this->findChild<QWidget *>(wid->objectName())->objectName().contains("progresso", Qt::CaseInsensitive)){
                vposicaoGrade = vposicaoGrade.remove("progresso", Qt::CaseInsensitive);
                ypos = ui->labelAnime00Titulo->y()+ui->labelAnime00Titulo->height()+2;
            }
            else if(this->findChild<QWidget *>(wid->objectName())->objectName().contains("nota", Qt::CaseInsensitive)){
                vposicaoGrade = vposicaoGrade.remove("nota", Qt::CaseInsensitive);
                ypos = static_cast<int>(ui->labelAnime00Titulo->y()+(ui->labelAnime00Titulo->height()*1.5)+4);
            }
            else if(this->findChild<QWidget *>(wid->objectName())->objectName().contains("lista", Qt::CaseInsensitive)){
                vposicaoGrade = vposicaoGrade.remove("lista", Qt::CaseInsensitive);
                ypos = ui->labelAnime00Titulo->y()+ui->labelAnime00Titulo->height()+
                        ((ui->labelAnime00Progresso->height())*2)+3;
            }
            else{
                ypos = ui->labelAnime00Titulo->y();
            }
            if(vposicaoGrade.toInt()%3 == 0){
                this->findChild<QWidget *>(wid->objectName())->move(ui->labelAnime00Titulo->x(),
                    static_cast<int>(ypos+((2+ui->imagemAnime00->height())*
                        (vposicaoGrade.toInt()/3))));
            }
            else if((vposicaoGrade.toInt()-1)%3 == 0)
                this->findChild<QWidget *>(wid->objectName())->move(ui->labelAnime00Titulo->x()
                                                                    +ui->labelAnime00Titulo->width()+13
                                                                    +ui->imagemAnime00->width(),
                    static_cast<int>(ypos+((2+ui->imagemAnime00->height())*
                                           (vposicaoGrade.toInt()/3))));
            else if((vposicaoGrade.toInt()-2)%3 == 0)
                this->findChild<QWidget *>(wid->objectName())->move(ui->labelAnime00Titulo->x()
                                                                    +((ui->labelAnime00Titulo->width()+13
                                                                    +ui->imagemAnime00->width())*2),
                    static_cast<int>(ypos+((2+ui->imagemAnime00->height())*
                                           (vposicaoGrade.toInt()/3))));
        }
        //Muda as informações do anime
        //Tenho que mudar manualmente mas isso quebra o galho por enquanto
//        else if(this->findChild<QWidget *>(wid->objectName())->objectName().contains("labelInfo", Qt::CaseInsensitive)){
        else if(!this->findChild<QWidget *>(wid->objectName())->objectName().contains("imagemanimegrande", Qt::CaseInsensitive)){
            qDebug() << this->findChild<QWidget *>(wid->objectName())->size();
            this->findChild<QWidget *>(wid->objectName())->resize(
                        static_cast<int>(comprimentoJanela*this->findChild<QWidget *>(wid->objectName())->width()),
                        static_cast<int>(alturaJanela*this->findChild<QWidget *>(wid->objectName())->height())+3);
            qDebug() << this->findChild<QWidget *>(wid->objectName())->size();
            this->findChild<QWidget *>(wid->objectName())->move(
                        static_cast<int>((static_cast<double>(this->findChild<QWidget *>(wid->objectName())->x())/1920)*width),
                        static_cast<int>((static_cast<double>(this->findChild<QWidget *>(wid->objectName())->y())/821)*widheight));
//            int ypos = 0;
//            if(this->findChild<QWidget *>(wid->objectName())->objectName().contains("titulo", Qt::CaseInsensitive)){
//                vposicaoGrade = vposicaoGrade.remove("titulo", Qt::CaseInsensitive);
//                ypos = ui->labelAnime00Titulo->y();
//            }
//            else if(this->findChild<QWidget *>(wid->objectName())->objectName().contains("progresso", Qt::CaseInsensitive)){
//                vposicaoGrade = vposicaoGrade.remove("progresso", Qt::CaseInsensitive);
//                ypos = ui->labelAnime00Titulo->y()+ui->labelAnime00Titulo->height()+2;
//            }
//            else if(this->findChild<QWidget *>(wid->objectName())->objectName().contains("nota", Qt::CaseInsensitive)){
//                vposicaoGrade = vposicaoGrade.remove("nota", Qt::CaseInsensitive);
//                ypos = static_cast<int>(ui->labelAnime00Titulo->y()+(ui->labelAnime00Titulo->height()*1.5)+4);
//            }
//            else if(this->findChild<QWidget *>(wid->objectName())->objectName().contains("lista", Qt::CaseInsensitive)){
//                vposicaoGrade = vposicaoGrade.remove("lista", Qt::CaseInsensitive);
//                ypos = ui->labelAnime00Titulo->y()+ui->labelAnime00Titulo->height()+
//                        ((ui->labelAnime00Progresso->height())*2)+3;
//            }
//            else{
//                ypos = ui->labelAnime00Titulo->y();
//            }
//            if(vposicaoGrade.toInt()%3 == 0){
//                this->findChild<QWidget *>(wid->objectName())->move(ui->labelAnime00Titulo->x(),
//                    static_cast<int>(ypos+((2+ui->imagemAnime00->height())*
//                        (vposicaoGrade.toInt()/3))));
//            }
//            else if((vposicaoGrade.toInt()-1)%3 == 0)
//                this->findChild<QWidget *>(wid->objectName())->move(ui->labelAnime00Titulo->x()
//                                                                    +ui->labelAnime00Titulo->width()+13
//                                                                    +ui->imagemAnime00->width(),
//                    static_cast<int>(ypos+((2+ui->imagemAnime00->height())*
//                                           (vposicaoGrade.toInt()/3))));
//            else if((vposicaoGrade.toInt()-2)%3 == 0)
//                this->findChild<QWidget *>(wid->objectName())->move(ui->labelAnime00Titulo->x()
//                                                                    +((ui->labelAnime00Titulo->width()+13
//                                                                    +ui->imagemAnime00->width())*2),
//                    static_cast<int>(ypos+((2+ui->imagemAnime00->height())*
//                                           (vposicaoGrade.toInt()/3))));
        }
    }
}
