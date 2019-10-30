#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    canilist = new anilist();

    logger::fattachLogger();
    qInfo() << QDateTime::currentDateTime().toString();
    cconfBase = new confBase;
    cconfBase->fcriaDiretoriosBase();
    qDebug() << "Main configuration is up";
    carquivos = new arquivos;
    qDebug() << "File system is up";
    cconfUsuario = new confUsuario;
    qDebug() << "User configuration is up";

    cleitorListaAnimes = new leitorlistaanimes;
    qDebug() << "Anime list is up";
//    if(cleitorListaAnimes->fadicionaAnimes())
    if(cleitorListaAnimes->fleJson())
        qDebug() << "The anime list have been read successfully";
    else
        qCritical() << "There was a problem reading the anime list";
    cfiledownloader = new filedownloader;
    qDebug() << "The download system is up";
    cfiledownloader->fsetConfBase(cconfBase);
    cfiledownloader->fsetLeitorListaAnimes(cleitorListaAnimes);
    //Baixa imagens ao abrir o programa
    cfiledownloader->fsetNext();
    qDebug() << "Images ready";

    // Já aciona a lista de modo ordenado
    vordem = "";
    vlistaSelecionada = cleitorListaAnimes->sortLista(vordem, "watching");
    qDebug() << "Opening Watching List";
    vlistaAtual = "watching";
    qDebug() << "Searching for animes in the computer";
    cconfUsuario->frecebeListaAnime(cleitorListaAnimes->sortLista(vordem, "watching"));
    cconfUsuario->fbuscaDiretoriosAnimes();
    cconfUsuario->frecebeListaAnime(cleitorListaAnimes->sortLista(vordem, "completed"));
    cconfUsuario->fbuscaDiretoriosAnimes();
    cconfUsuario->frecebeListaAnime(cleitorListaAnimes->sortLista(vordem, "onhold"));
    cconfUsuario->fbuscaDiretoriosAnimes();
    cconfUsuario->frecebeListaAnime(cleitorListaAnimes->sortLista(vordem, "dropped"));
    cconfUsuario->fbuscaDiretoriosAnimes();
    cconfUsuario->frecebeListaAnime(cleitorListaAnimes->sortLista(vordem, "plantowatch"));
    cconfUsuario->fbuscaDiretoriosAnimes();
    qDebug() << "All animes in the computer were found";
    carquivos->frecebeDiretorios(cconfUsuario);

    //Após configurar tudo, a tela abre
    ui->setupUi(this);
    ui->janelaRotativa->addWidget(&jconfig);

    //QFuture vai rodar a função fcarregaImagensBackground em uma thread separada, para carregar todas as imagens enquanto o usuário mexe no
    //programa, para deixar o programa mais fluído
    vfuture = QtConcurrent::run(this, &MainWindow::fcarregaImagensBackground);
    //E o timer para atualizar a lista automaticamente
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::on_botaoRefresh_clicked));
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

    vanimeSelecionado = 0;
    vpagina = 1;

    ui->NumPagina->setText("Watching - Page "+QString::number(vpagina)+"/"+QString::number(((vlistaSelecionada.size()-1)/12)+1));

    finfoAnimeSelecionado();
}

MainWindow::~MainWindow()
{
    vrefreshAcontecendo = true;
    vfuture.waitForFinished();
    qDebug() << "Deleting pointers";
    cconfUsuario->deleteLater();
    qDebug() << "cconfUsuario deleted";
    carquivos->deleteLater();
    qDebug() << "carquivos deleted";
    cfiledownloader->deleteLater();
    qDebug() << "cfiledownloader deleted";
    cleitorListaAnimes->deleteLater();
    qDebug() << "cleitorListaAnimes deleted";
    canilist->deleteLater();
    qDebug() << "canilist deleted";
    qInfo() << "Software closed successfully";
    timer->deleteLater();
    delete ui;
}

///ASSIM QUE O DESIGN ESTIVER PRONTO, CHECAR TAMBÉM SE A SINOPSE É MAIOR QUE O LABEL E AJEITAR ISSO
void MainWindow::finfoAnimeSelecionado(){
    if(!vlistaSelecionada.isEmpty()){
        ui->labelNomeAnime->setText(vlistaSelecionada[vanimeSelecionado]->vnome);
        ui->labelNomeAnimeIngles->setText(vlistaSelecionada[vanimeSelecionado]->vnomeIngles);
        ui->labelSinopse->setText(vlistaSelecionada[vanimeSelecionado]->vsinopse);
        ui->labelStatus->setText(vlistaSelecionada[vanimeSelecionado]->vstatus);
        ui->labelSeason->setText(vlistaSelecionada[vanimeSelecionado]->vseason);
        ui->labelMediaSite->setText(QString::number(vlistaSelecionada[vanimeSelecionado]->vnotaMediaSite.toFloat()/10, 'f',2));
        ui->labelNota->setText(vlistaSelecionada[vanimeSelecionado]->vnotaMediaPessoal);
        fcarregaImagensLista();
    }
    else{
        qWarning() << "Anime list is empty!";
    }
}

void MainWindow::fcarregaImagensLista(){
    qDebug() << "Loading anime list info";
    QPixmap pix;
    QString lstyleSheet;
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
    ui->imagemAnime00->setScaledContents(true);
    //Checa tamanho do título pra sempre caber no frame
    lstyleSheet = ui->labelAnime00->styleSheet();
    if(vlistaSelecionada[0+(12*(vpagina-1))]->vnome.size() < 47)
        ui->labelAnime00->setStyleSheet("background: transparent; font: 75 8pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
    else if(vlistaSelecionada[0+(12*(vpagina-1))]->vnome.size() < 58)
        ui->labelAnime00->setStyleSheet("background: transparent; font: 75 7pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
    else
        ui->labelAnime00->setStyleSheet("background: transparent; font: 75 6pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
    ui->labelAnime00->setText(vlistaSelecionada[0+(12*(vpagina-1))]->vnome);
    ui->labelAnime00->setAlignment(Qt::AlignCenter);
    ui->labelAnime00->setWordWrap(true);
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
        ui->labelAnimeLista00->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnimeLista00->setText(vlistaSelecionada[0+(12*(vpagina-1))]->vlista);
    }
    else{
        ui->labelAnimeLista00->clear();
        ui->labelAnimeLista00->setStyleSheet("background: transparent");
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
    if(vlistaSelecionada.size() > 1+(12*(vpagina-1))){
        ui->imagemAnime01->setScaledContents(true);
        ui->labelFundoAnime01Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime01Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime01Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(vlistaSelecionada[1+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime01->setStyleSheet("background: transparent; font: 75 8pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[1+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime01->setStyleSheet("background: transparent; font: 75 7pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime01->setStyleSheet("background: transparent; font: 75 6pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime01->setText(vlistaSelecionada[1+(12*(vpagina-1))]->vnome);
        ui->labelAnime01->setAlignment(Qt::AlignCenter);
        ui->labelAnime01->setWordWrap(true);
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
            ui->labelAnimeLista01->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelAnimeLista01->setText(vlistaSelecionada[1+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelAnimeLista01->clear();
            ui->labelAnimeLista01->setStyleSheet("background: transparent");
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
        ui->labelAnime01->clear();
        ui->labelAnime01Progresso->clear();
        ui->labelAnime01Nota->clear();
        ui->labelFundoAnime01Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime01Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime01Titulo->setStyleSheet("background: transparent;");
        ui->labelAnimeLista01->clear();
        ui->labelAnimeLista01->setStyleSheet("background: transparent");///Por isso em todos
    }
    if(vlistaSelecionada.size() > 2+(12*(vpagina-1))){
        ui->imagemAnime02->setScaledContents(true);
        ui->labelFundoAnime02Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime02Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime02Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(vlistaSelecionada[2+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime02->setStyleSheet("background: transparent; font: 75 8pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[2+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime02->setStyleSheet("background: transparent; font: 75 7pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime02->setStyleSheet("background: transparent; font: 75 6pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime02->setText(vlistaSelecionada[2+(12*(vpagina-1))]->vnome);
        ui->labelAnime02->setAlignment(Qt::AlignCenter);
        ui->labelAnime02->setWordWrap(true);
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
            ui->labelAnimeLista02->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelAnimeLista02->setText(vlistaSelecionada[2+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelAnimeLista02->clear();
            ui->labelAnimeLista02->setStyleSheet("background: transparent");
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
        ui->labelAnime02->clear();
        ui->labelAnime02Progresso->clear();
        ui->labelAnime02Nota->clear();
        ui->labelFundoAnime02Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime02Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime02Titulo->setStyleSheet("background: transparent;");
    }
    if(vlistaSelecionada.size() > 3+(12*(vpagina-1))){
        ui->imagemAnime03->setScaledContents(true);
        ui->labelFundoAnime03Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime03Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime03Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(vlistaSelecionada[3+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime03->setStyleSheet("background: transparent; font: 75 8pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[3+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime03->setStyleSheet("background: transparent; font: 75 7pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime03->setStyleSheet("background: transparent; font: 75 6pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime03->setText(vlistaSelecionada[3+(12*(vpagina-1))]->vnome);
        ui->labelAnime03->setAlignment(Qt::AlignCenter);
        ui->labelAnime03->setWordWrap(true);
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
            ui->labelAnimeLista03->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelAnimeLista03->setText(vlistaSelecionada[3+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelAnimeLista03->clear();
            ui->labelAnimeLista03->setStyleSheet("background: transparent");
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
        ui->labelAnime03->clear();
        ui->labelAnime03Progresso->clear();
        ui->labelAnime03Nota->clear();
        ui->labelFundoAnime03Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime03Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime03Titulo->setStyleSheet("background: transparent;");
    }
    if(vlistaSelecionada.size() > 4+(12*(vpagina-1))){
        ui->imagemAnime04->setScaledContents(true);
        ui->labelFundoAnime04Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime04Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime04Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(vlistaSelecionada[4+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime04->setStyleSheet("background: transparent; font: 75 8pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[4+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime04->setStyleSheet("background: transparent; font: 75 7pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime04->setStyleSheet("background: transparent; font: 75 6pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime04->setText(vlistaSelecionada[4+(12*(vpagina-1))]->vnome);
        ui->labelAnime04->setAlignment(Qt::AlignCenter);
        ui->labelAnime04->setWordWrap(true);
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
            ui->labelAnimeLista04->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelAnimeLista04->setText(vlistaSelecionada[4+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelAnimeLista04->clear();
            ui->labelAnimeLista04->setStyleSheet("background: transparent");
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
        ui->labelAnime04->clear();
        ui->labelAnime04Progresso->clear();
        ui->labelAnime04Nota->clear();
        ui->labelFundoAnime04Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime04Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime04Titulo->setStyleSheet("background: transparent;");
    }
    if(vlistaSelecionada.size() > 5+(12*(vpagina-1))){
        ui->imagemAnime05->setScaledContents(true);
        ui->labelFundoAnime05Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime05Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime05Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(vlistaSelecionada[5+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime05->setStyleSheet("background: transparent; font: 75 8pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[5+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime05->setStyleSheet("background: transparent; font: 75 7pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime05->setStyleSheet("background: transparent; font: 75 6pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime05->setText(vlistaSelecionada[5+(12*(vpagina-1))]->vnome);
        ui->labelAnime05->setAlignment(Qt::AlignCenter);
        ui->labelAnime05->setWordWrap(true);
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
            ui->labelAnimeLista05->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelAnimeLista05->setText(vlistaSelecionada[5+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelAnimeLista05->clear();
            ui->labelAnimeLista05->setStyleSheet("background: transparent");
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
        ui->labelAnime05->clear();
        ui->labelAnime05Progresso->clear();
        ui->labelAnime05Nota->clear();
        ui->labelFundoAnime05Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime05Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime05Titulo->setStyleSheet("background: transparent;");
    }
    if(vlistaSelecionada.size() > 6+(12*(vpagina-1))){
        ui->imagemAnime06->setScaledContents(true);
        ui->labelFundoAnime06Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime06Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime06Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(vlistaSelecionada[6+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime06->setStyleSheet("background: transparent; font: 75 8pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[6+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime06->setStyleSheet("background: transparent; font: 75 7pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime06->setStyleSheet("background: transparent; font: 75 6pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime06->setText(vlistaSelecionada[6+(12*(vpagina-1))]->vnome);
        ui->labelAnime06->setAlignment(Qt::AlignCenter);
        ui->labelAnime06->setWordWrap(true);
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
            ui->labelAnimeLista06->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelAnimeLista06->setText(vlistaSelecionada[6+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelAnimeLista06->clear();
            ui->labelAnimeLista06->setStyleSheet("background: transparent");
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
        ui->labelAnime06->clear();
        ui->labelAnime06Progresso->clear();
        ui->labelAnime06Nota->clear();
        ui->labelFundoAnime06Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime06Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime06Titulo->setStyleSheet("background: transparent;");
    }
    if(vlistaSelecionada.size() > 7+(12*(vpagina-1))){
        ui->imagemAnime07->setScaledContents(true);
        ui->labelFundoAnime07Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime07Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime07Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(vlistaSelecionada[7+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime07->setStyleSheet("background: transparent; font: 75 8pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[7+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime07->setStyleSheet("background: transparent; font: 75 7pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime07->setStyleSheet("background: transparent; font: 75 6pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime07->setText(vlistaSelecionada[7+(12*(vpagina-1))]->vnome);
        ui->labelAnime07->setAlignment(Qt::AlignCenter);
        ui->labelAnime07->setWordWrap(true);
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
            ui->labelAnimeLista07->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelAnimeLista07->setText(vlistaSelecionada[7+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelAnimeLista07->clear();
            ui->labelAnimeLista07->setStyleSheet("background: transparent");
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
        ui->labelAnime07->clear();
        ui->labelAnime07Progresso->clear();
        ui->labelAnime07Nota->clear();
        ui->labelFundoAnime07Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime07Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime07Titulo->setStyleSheet("background: transparent;");
    }
    if(vlistaSelecionada.size() > 8+(12*(vpagina-1))){
        ui->imagemAnime08->setScaledContents(true);
        ui->labelFundoAnime08Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime08Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime08Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(vlistaSelecionada[8+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime08->setStyleSheet("background: transparent; font: 75 8pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[8+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime08->setStyleSheet("background: transparent; font: 75 7pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime08->setStyleSheet("background: transparent; font: 75 6pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime08->setText(vlistaSelecionada[8+(12*(vpagina-1))]->vnome);
        ui->labelAnime08->setAlignment(Qt::AlignCenter);
        ui->labelAnime08->setWordWrap(true);
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
            ui->labelAnimeLista08->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelAnimeLista08->setText(vlistaSelecionada[8+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelAnimeLista08->clear();
            ui->labelAnimeLista08->setStyleSheet("background: transparent");
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
        ui->labelAnime08->clear();
        ui->labelAnime08Progresso->clear();
        ui->labelAnime08Nota->clear();
        ui->labelFundoAnime08Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime08Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime08Titulo->setStyleSheet("background: transparent;");
    }
    if(vlistaSelecionada.size() > 9+(12*(vpagina-1))){
        ui->imagemAnime09->setScaledContents(true);
        ui->labelFundoAnime09Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime09Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime09Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(vlistaSelecionada[9+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime09->setStyleSheet("background: transparent; font: 75 8pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[9+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime09->setStyleSheet("background: transparent; font: 75 7pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime09->setStyleSheet("background: transparent; font: 75 6pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime09->setText(vlistaSelecionada[9+(12*(vpagina-1))]->vnome);
        ui->labelAnime09->setAlignment(Qt::AlignCenter);
        ui->labelAnime09->setWordWrap(true);
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
            ui->labelAnimeLista09->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelAnimeLista09->setText(vlistaSelecionada[9+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelAnimeLista09->clear();
            ui->labelAnimeLista09->setStyleSheet("background: transparent");
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
        ui->labelAnime09->clear();
        ui->labelAnime09Progresso->clear();
        ui->labelAnime09Nota->clear();
        ui->labelFundoAnime09Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime09Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime09Titulo->setStyleSheet("background: transparent;");
    }
    if(vlistaSelecionada.size() > 10+(12*(vpagina-1))){
        ui->imagemAnime10->setScaledContents(true);
        ui->labelFundoAnime10Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime10Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime10Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(vlistaSelecionada[10+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime10->setStyleSheet("background: transparent; font: 75 8pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[10+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime10->setStyleSheet("background: transparent; font: 75 7pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime10->setStyleSheet("background: transparent; font: 75 6pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime10->setText(vlistaSelecionada[10+(12*(vpagina-1))]->vnome);
        ui->labelAnime10->setAlignment(Qt::AlignCenter);
        ui->labelAnime10->setWordWrap(true);
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
            ui->labelAnimeLista10->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelAnimeLista10->setText(vlistaSelecionada[10+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelAnimeLista10->clear();
            ui->labelAnimeLista10->setStyleSheet("background: transparent");
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
        ui->labelAnime10->clear();
        ui->labelAnime10Progresso->clear();
        ui->labelAnime10Nota->clear();
        ui->labelFundoAnime10Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime10Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime10Titulo->setStyleSheet("background: transparent;");
    }
    if(vlistaSelecionada.size() > 11+(12*(vpagina-1))){
        ui->imagemAnime11->setScaledContents(true);
        ui->labelFundoAnime11Nota->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime11Progresso->setStyleSheet("background: rgb(185,201,250);");
        ui->labelFundoAnime11Titulo->setStyleSheet("background: rgb(185,201,250);");
        if(vlistaSelecionada[11+(12*(vpagina-1))]->vnome.size() < 47)
            ui->labelAnime11->setStyleSheet("background: transparent; font: 75 8pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        else if(vlistaSelecionada[11+(12*(vpagina-1))]->vnome.size() < 58)
            ui->labelAnime11->setStyleSheet("background: transparent; font: 75 7pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        else
            ui->labelAnime11->setStyleSheet("background: transparent; font: 75 6pt \"MS Shell Dlg 2\"; font-weight: bold; color: rgb(20, 20, 20);");
        ui->labelAnime11->setText(vlistaSelecionada[11+(12*(vpagina-1))]->vnome);
        ui->labelAnime11->setAlignment(Qt::AlignCenter);
        ui->labelAnime11->setWordWrap(true);
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
            ui->labelAnimeLista11->setStyleSheet("background: rgb(185, 202, 250); font: 75 8pt \"Calibri\"; font-weight: bold; color: rgb(20, 20, 20);");
            ui->labelAnimeLista11->setText(vlistaSelecionada[11+(12*(vpagina-1))]->vlista);
        }
        else{
            ui->labelAnimeLista11->clear();
            ui->labelAnimeLista11->setStyleSheet("background: transparent");
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
        ui->labelAnime11->clear();
        ui->labelAnime11Progresso->clear();
        ui->labelAnime11Nota->clear();
        ui->labelFundoAnime11Nota->setStyleSheet("background: transparent;");
        ui->labelFundoAnime11Progresso->setStyleSheet("background: transparent;");
        ui->labelFundoAnime11Titulo->setStyleSheet("background: transparent;");
    }
}

bool MainWindow::fcarregaImagensBackground(){
    ui->labelMensagem->setText("Carregando imagens");
    qDebug() << "Carregando imagens";
    QPixmap lpix;
    //Quando a lista estiver sendo atualizada, os animes podem mudar de lugar ou sumir da lista.
    //Se existiam 10 animes na lista antes de atualizar e apenas 1 depois da atualização, tentar acessar a memória desses animes
    //Pode crashar o programa. Por esse motivo, esse if serve como uma saída de emergência pra esses casos
    if(vrefreshAcontecendo == true)
        return false;
    vcarregaListaBackground = cleitorListaAnimes->sortLista(vordem, "watching");
    //Vamos sempre carregar a primeira imagem da lista em formato grande, pra acelerar na hora de clicar no botão das listas
    if(lpix.load(cconfBase->vdiretorioImagensGrandes+vcarregaListaBackground[0]->vid+".jpg", "jpg")){
        ui->labelImagemBackground->setPixmap(lpix);
    }
    else if(lpix.load(cconfBase->vdiretorioImagensGrandes+vcarregaListaBackground[0]->vid+".png", "png")){
        ui->labelImagemBackground->setPixmap(lpix);
    }
    //Depois carregar todas as imagens na memória
    for(int i = 0; i < vcarregaListaBackground.size(); i++){
        if(lpix.load(cconfBase->vdiretorioImagensMedio+vcarregaListaBackground[i]->vid+".jpg", "jpg")){
            ui->labelImagemBackground->setPixmap(lpix);
        }
        else if(lpix.load(cconfBase->vdiretorioImagensMedio+vcarregaListaBackground[i]->vid+".png", "png")){
            ui->labelImagemBackground->setPixmap(lpix);
        }
    }
    vcarregaListaBackground.clear();
    if(vrefreshAcontecendo == true)
        return false;
    vcarregaListaBackground = cleitorListaAnimes->sortLista(vordem, "completed");
    if(lpix.load(cconfBase->vdiretorioImagensGrandes+vcarregaListaBackground[0]->vid+".jpg", "jpg")){
        ui->labelImagemBackground->setPixmap(lpix);
    }
    else if(lpix.load(cconfBase->vdiretorioImagensGrandes+vcarregaListaBackground[0]->vid+".png", "png")){
        ui->labelImagemBackground->setPixmap(lpix);
    }
    for(int i = 0; i < vcarregaListaBackground.size(); i++){
        if(lpix.load(cconfBase->vdiretorioImagensMedio+vcarregaListaBackground[i]->vid+".jpg", "jpg")){
            ui->labelImagemBackground->setPixmap(lpix);
        }
        else if(lpix.load(cconfBase->vdiretorioImagensMedio+vcarregaListaBackground[i]->vid+".png", "png")){
            ui->labelImagemBackground->setPixmap(lpix);
        }
    }
    vcarregaListaBackground.clear();
    if(vrefreshAcontecendo == true)
        return false;
    vcarregaListaBackground = cleitorListaAnimes->sortLista(vordem, "onhold");
    if(lpix.load(cconfBase->vdiretorioImagensGrandes+vcarregaListaBackground[0]->vid+".jpg", "jpg")){
        ui->labelImagemBackground->setPixmap(lpix);
    }
    else if(lpix.load(cconfBase->vdiretorioImagensGrandes+vcarregaListaBackground[0]->vid+".png", "png")){
        ui->labelImagemBackground->setPixmap(lpix);
    }
    for(int i = 0; i < vcarregaListaBackground.size(); i++){
        if(lpix.load(cconfBase->vdiretorioImagensMedio+vcarregaListaBackground[i]->vid+".jpg", "jpg")){
            ui->labelImagemBackground->setPixmap(lpix);
        }
        else if(lpix.load(cconfBase->vdiretorioImagensMedio+vcarregaListaBackground[i]->vid+".png", "png")){
            ui->labelImagemBackground->setPixmap(lpix);
        }
    }
    vcarregaListaBackground.clear();
    if(vrefreshAcontecendo == true)
        return false;
    vcarregaListaBackground = cleitorListaAnimes->sortLista(vordem, "dropped");
    if(lpix.load(cconfBase->vdiretorioImagensGrandes+vcarregaListaBackground[0]->vid+".jpg", "jpg")){
        ui->labelImagemBackground->setPixmap(lpix);
    }
    else if(lpix.load(cconfBase->vdiretorioImagensGrandes+vcarregaListaBackground[0]->vid+".png", "png")){
        ui->labelImagemBackground->setPixmap(lpix);
    }
    for(int i = 0; i < vcarregaListaBackground.size(); i++){
        if(lpix.load(cconfBase->vdiretorioImagensMedio+vcarregaListaBackground[i]->vid+".jpg", "jpg")){
            ui->labelImagemBackground->setPixmap(lpix);
        }
        else if(lpix.load(cconfBase->vdiretorioImagensMedio+vcarregaListaBackground[i]->vid+".png", "png")){
            ui->labelImagemBackground->setPixmap(lpix);
        }
    }
    vcarregaListaBackground.clear();
    if(vrefreshAcontecendo == true)
        return false;
    vcarregaListaBackground = cleitorListaAnimes->sortLista(vordem, "plantowatch");
    if(lpix.load(cconfBase->vdiretorioImagensGrandes+vcarregaListaBackground[0]->vid+".jpg", "jpg")){
        ui->labelImagemBackground->setPixmap(lpix);
    }
    else if(lpix.load(cconfBase->vdiretorioImagensGrandes+vcarregaListaBackground[0]->vid+".png", "png")){
        ui->labelImagemBackground->setPixmap(lpix);
    }
    for(int i = 0; i < vcarregaListaBackground.size(); i++){
        if(lpix.load(cconfBase->vdiretorioImagensGrandes+vcarregaListaBackground[i]->vid+".jpg", "jpg")){
            ui->labelImagemBackground->setPixmap(lpix);
        }
        else if(lpix.load(cconfBase->vdiretorioImagensGrandes+vcarregaListaBackground[i]->vid+".png", "png")){
            ui->labelImagemBackground->setPixmap(lpix);
        }
    }
    vcarregaListaBackground.clear();
    qDebug() << "Imagens carregadas com sucesso";
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
    qDebug() << "Trying to open episode";
    if(!carquivos->fprocuraEpisodio(vlistaSelecionada[vanimeSelecionado]).isEmpty()){
        if(carquivos->fabreEpisodio(carquivos->fprocuraEpisodio(vlistaSelecionada[vanimeSelecionado])))
            ui->labelMensagem->setText("Abrindo episódio!");
        else
            ui->labelMensagem->setText("O episódio não foi encontrado no computador");
    }
}

void MainWindow::on_botaoAbrePasta_clicked()
{
    qDebug() << "Trying to open anime folder";
    //Pega o caminho da pasta onde o episódio está e converte para o caminho nativo do OS (só pra evitar erros com o nome)
    QString lpathSeparadorNativo = QDir::toNativeSeparators(carquivos->fprocuraEpisodio(vlistaSelecionada[vanimeSelecionado]));
    int lseparadorPasta = lpathSeparadorNativo.lastIndexOf(QDir::separator());
    QString lcaminhoPasta = carquivos->fprocuraEpisodio(vlistaSelecionada[vanimeSelecionado]).mid(0,lseparadorPasta);
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

void MainWindow::on_boxOrdemLista_activated(const QString &arg1)
{
    qDebug() << "Trying to ordenate list";
    vordem = "";
    if(arg1.contains("reverse", Qt::CaseInsensitive))
        vordem.append("d");
    else
        vordem.append("c");
    if(arg1.contains("alphabetical", Qt::CaseInsensitive))
        vordem.append("nome");
    else if(arg1.contains("score", Qt::CaseInsensitive))
        vordem.append("nota");
    else if(arg1.contains("release", Qt::CaseInsensitive))
        vordem.append("data");
    else
        vordem.append("progresso");
    vlistaSelecionada = cleitorListaAnimes->sortLista(vordem, vlistaAtual);
    if(!vlistaSelecionada.isEmpty()){
        vanimeSelecionado = 0;
        vpagina = 1;
        ui->NumPagina->setText("Page "+QString::number(vpagina)+"/"+QString::number(((vlistaSelecionada.size()-1)/12)+1));
        finfoAnimeSelecionado();
    }
}

void MainWindow::on_botaoRefresh_clicked()
{
    fbloqueiaSinaisBotoes();
    ui->labelMensagem->setText("Atualizando lista...");
    timer->start(600000);
    vtimerSegundos = 59;
    vrefreshAcontecendo = true;
    qDebug() << "Refreshing list...";
    cleitorListaAnimes->fdeletaListaAnimes(); //Será que não buga?
    canilist->fgetList();
    if(cleitorListaAnimes->fleJson())
        qDebug() << "The anime list have been read successfully";
    else{
        qCritical() << "There was a problem reading the anime list";
        return;
    }
    vlistaSelecionada = cleitorListaAnimes->sortLista(vordem, vlistaAtual);
    cfiledownloader->fsetLeitorListaAnimes(cleitorListaAnimes);
    cfiledownloader->fsetNext();
    qDebug() << "Images ready";
    finfoAnimeSelecionado();
    ui->labelMensagem->setText("As listas foram atualizadas!");
    fliberaSinaisBotoes();
    vrefreshAcontecendo = false;
    fcarregaImagensBackground();
}

void MainWindow::fbloqueiaSinaisBotoes(){
    ui->botaoBusca->blockSignals(true);
    ui->botaoRefresh->blockSignals(true);
    ui->botaoProximaPagina->blockSignals(true);
    ui->botaoPaginaAnterior->blockSignals(true);
    ui->botaoProximoEpisodio->blockSignals(true);
    ui->Watching->blockSignals(true);
    ui->Completed->blockSignals(true);
    ui->Dropped->blockSignals(true);
    ui->OnHold->blockSignals(true);
    ui->PlanToWatch->blockSignals(true);
    ui->botaoAnilist->blockSignals(true);
    ui->botaoCrunchyroll->blockSignals(true);
    ui->botaoDownloadAnime->blockSignals(true);
    ui->botaoAbrePasta->blockSignals(true);
    //ui->botaoConfiguracao->blockSignals(true);
    //ui->botaoHome->blockSignals(true);
    //ui->botaoTorrent->blockSignals(true);
    ui->botaoAnime00->blockSignals(true);
    ui->botaoAnime01->blockSignals(true);
    ui->botaoAnime02->blockSignals(true);
    ui->botaoAnime03->blockSignals(true);
    ui->botaoAnime04->blockSignals(true);
    ui->botaoAnime05->blockSignals(true);
    ui->botaoAnime06->blockSignals(true);
    ui->botaoAnime07->blockSignals(true);
    ui->botaoAnime08->blockSignals(true);
    ui->botaoAnime09->blockSignals(true);
    ui->botaoAnime10->blockSignals(true);
    ui->botaoAnime11->blockSignals(true);
    ui->boxOrdemLista->blockSignals(true);
    //ui->botaoMudarLista->blockSignals(true);
}

void MainWindow::fliberaSinaisBotoes(){
   ui->botaoBusca->blockSignals(false);
   ui->botaoRefresh->blockSignals(false);
   ui->botaoProximaPagina->blockSignals(false);
   ui->botaoPaginaAnterior->blockSignals(false);
   ui->botaoProximoEpisodio->blockSignals(false);
   ui->Watching->blockSignals(false);
   ui->Completed->blockSignals(false);
   ui->Dropped->blockSignals(false);
   ui->OnHold->blockSignals(false);
   ui->PlanToWatch->blockSignals(false);
   ui->botaoAnilist->blockSignals(false);
   ui->botaoCrunchyroll->blockSignals(false);
   ui->botaoDownloadAnime->blockSignals(false);
   ui->botaoAbrePasta->blockSignals(false);
   //ui->botaoConfiguracao->blockSignals(false);
   //ui->botaoHome->blockSignals(false);
   //ui->botaoTorrent->blockSignals(false);
   ui->botaoAnime00->blockSignals(false);
   ui->botaoAnime01->blockSignals(false);
   ui->botaoAnime02->blockSignals(false);
   ui->botaoAnime03->blockSignals(false);
   ui->botaoAnime04->blockSignals(false);
   ui->botaoAnime05->blockSignals(false);
   ui->botaoAnime06->blockSignals(false);
   ui->botaoAnime07->blockSignals(false);
   ui->botaoAnime08->blockSignals(false);
   ui->botaoAnime09->blockSignals(false);
   ui->botaoAnime10->blockSignals(false);
   ui->botaoAnime11->blockSignals(false);
   ui->boxOrdemLista->blockSignals(false);
   //ui->botaoMudarLista->blockSignals(false);
}

void MainWindow::fatualizaRefreshTimer()
{
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
    vlistaSelecionada[vanimeSelecionado]->vnotaMediaPessoal = QString::number(vlistaSelecionada[vanimeSelecionado]->vnotaMediaPessoal.toInt()+1);
    ui->labelNota->setText(vlistaSelecionada[vanimeSelecionado]->vnotaMediaPessoal);
    QString lacao = "nota:" + vlistaSelecionada[vanimeSelecionado]->vid + ":" + QString::number(vlistaSelecionada[vanimeSelecionado]->vnotaMediaPessoal.toInt()+1);
    QStringList lstringListAcao = lacao.split(':');
    vlistaAcoes.append(lstringListAcao);
}

void MainWindow::on_botaoNotaMenos_clicked()
{
    vlistaSelecionada[vanimeSelecionado]->vnotaMediaPessoal = QString::number(vlistaSelecionada[vanimeSelecionado]->vnotaMediaPessoal.toInt()-1);
    ui->labelNota->setText(vlistaSelecionada[vanimeSelecionado]->vnotaMediaPessoal);
    QString lacao = "nota:" + vlistaSelecionada[vanimeSelecionado]->vid + ":" + QString::number(vlistaSelecionada[vanimeSelecionado]->vnotaMediaPessoal.toInt()-1);
    QStringList lstringListAcao = lacao.split(':');
    vlistaAcoes.append(lstringListAcao);
}

void MainWindow::on_botaoProgressoMais_clicked()
{
    vlistaSelecionada[vanimeSelecionado]->vnumEpisodiosAssistidos = QString::number(vlistaSelecionada[vanimeSelecionado]->vnumEpisodiosAssistidos.toInt()+1);
    finfoAnimeSelecionado();
    QString lacao = "progresso:" + vlistaSelecionada[vanimeSelecionado]->vid + ":" + QString::number(vlistaSelecionada[vanimeSelecionado]->vnumEpisodiosAssistidos.toInt()+1);
    QStringList lstringListAcao = lacao.split(':');
    vlistaAcoes.append(lstringListAcao);
}

void MainWindow::on_botaoProgressoMenos_clicked()
{
    vlistaSelecionada[vanimeSelecionado]->vnumEpisodiosAssistidos = QString::number(vlistaSelecionada[vanimeSelecionado]->vnumEpisodiosAssistidos.toInt()-1);
    finfoAnimeSelecionado();
    QString lacao = "progresso:" + vlistaSelecionada[vanimeSelecionado]->vid + ":" + QString::number(vlistaSelecionada[vanimeSelecionado]->vnumEpisodiosAssistidos.toInt()-1);
    QStringList lstringListAcao = lacao.split(':');
    vlistaAcoes.append(lstringListAcao);
}

void MainWindow::fatualizaAnilist(){
    int limite = 90;
    if(vlistaAcoes.isEmpty())
        return;
    else if(vlistaAcoes.size() < 90){
        limite = vlistaAcoes.size();
    }
    for(int i = 0; i < limite; i++){
        if(vlistaAcoes[i].at(0) == "nota"){
            if(canilist->fmudaNota(vlistaAcoes[i].at(1).toInt(), vlistaAcoes[i].at(2).toInt()))
                    vlistaAcoes.remove(i);
        }
        else if(vlistaAcoes[i].at(0) == "progresso"){
            if(canilist->fmudaProgresso(vlistaAcoes[i].at(1).toInt(), vlistaAcoes[i].at(2).toInt()))
                    vlistaAcoes.remove(i);
        }
        else if(vlistaAcoes[i].at(0) == "lista"){
            if(canilist->fmudaLista(vlistaAcoes[i].at(1).toInt(), vlistaAcoes[i].at(2)))
                    vlistaAcoes.remove(i);
        }
    }
}
