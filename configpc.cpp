#include "configpc.h"

configPC::configPC()
{
//    arquivo = new leitorarquivos;
    anitomy = new anitomy::Anitomy;
    diretorioImagensPequenas = "Configurações/Imagens/Pequeno/";
    diretorioImagensMedio = "Configurações/Imagens/Medio/";
    diretorioImagensGrandes = "Configurações/Imagens/Grande/";
//    diretorioAnimes.append("E:/Animes");
//    diretorioAnimes.append(QDir::homePath() + "/Downloads/Animes");
    diretorioTaiga = QDir::homePath() + "/AppData/Roaming/Taiga/data";
//    CriaPastasBase();
}

configPC::~configPC(){
    delete anitomy;
    delete arquivo;
    delete jconfig;
}

void configPC::recebeJConfig(JanelaConfiguracao *JanelaConfg){
    jconfig = JanelaConfg;
    setUser();
    this->ordemLista = jconfig->returnOrdemLista();

    connect(jconfig, SIGNAL(user()), this, SLOT(setUser()));
    connect(jconfig, SIGNAL(dirAdd(QString)), this, SLOT(addDir(QString)));
    connect(jconfig, SIGNAL(dirRem(QString)), this, SLOT(rmvDir(QString)));
    connect(jconfig, SIGNAL(detec(int)), this, SLOT(setDetection(int)));
    connect(jconfig, SIGNAL(dListas(int)), this, SLOT(setDownloadListas(int)));
    connect(jconfig, SIGNAL(tDownload(int)), this, SLOT(setTempoDownload(int)));
    connect(jconfig, SIGNAL(dAutomatico(int)), this, SLOT(setDownloadAutomatico(int)));
    connect(jconfig, SIGNAL(dFolder(QString)), this, SLOT(setDownloadFolder(QString)));
    connect(jconfig, SIGNAL(fBusca(QString)), this, SLOT(setFeedBusca(QString)));
    connect(jconfig, SIGNAL(fSub(QString)), this, SLOT(setFansub(QString)));
    connect(jconfig, SIGNAL(quality(QString)), this, SLOT(setQualidade(QString)));
    connect(jconfig, SIGNAL(tPadrao(QString)), this, SLOT(setTorrentPadrao(QString)));
}

void configPC::addDir(QString dir){
    diretorioAnimes.append(dir);
    EscreveArquivo();
}

void configPC::rmvDir(QString dir){
    for(int i = 0; i < diretorioAnimes.length(); i++){
        if(diretorioAnimes[i] == dir){
            diretorioAnimes.remove(i);
            break;
        }
    }
    EscreveArquivo();
}

void configPC::CriaPastasBase(){
    if(!QDir("Configurações").exists()){
        QDir().mkdir("Configurações");
    }
    if(!QDir("Configurações/Imagens").exists()){
        QDir().mkdir("Configurações/Imagens");
    }
    if(!QDir("Configurações/Imagens/Pequeno").exists()){
        QDir().mkdir("Configurações/Imagens/Pequeno");
    }
    if(!QDir("Configurações/Imagens/Medio").exists()){
        QDir().mkdir("Configurações/Imagens/Medio");
    }
    if(!QDir("Configurações/Imagens/Grande").exists()){
        QDir().mkdir("Configurações/Imagens/Grande");
    }
    if(!QDir("Configurações/Temp/").exists()){
        QDir().mkdir("Configurações/Temp/");
    }
    if(!QDir("Configurações/Temp/Torrents").exists()){
        QDir().mkdir("Configurações/Temp/Torrents");
    }
}

void configPC::setUser(){
    usernameAnilist = jconfig->returnUserAnilist();
    diretorioAnimes = jconfig->retornaDiretorioAnime();
    detection = jconfig->returnDetection();
    downloadListas = jconfig->returnDownloadListas();
    tempoDownload = jconfig->returnTempoDownload();
    downloadAutomatico = jconfig->returnDownloadAutomatico();
    downloadFolder = jconfig->returnDownloadFolder();
    QStringList temp = jconfig->returnFeedBusca().split("-");
    feedTorrentGeral = temp.at(0);
    feedTorrentEspecifico = temp.at(1);
    preFfansub = jconfig->returnFansub();
    prefQualidade = jconfig->returnQualidade();
    prefTorrent = jconfig->returnTorrentPadrao();
}

void configPC::setFeedBusca(QString jfb){
    QStringList temp = jfb.split("-");
    feedTorrentGeral = temp.at(0);
    feedTorrentEspecifico = temp.at(1);
}

void configPC::setOrdem(QString ordem){
    ordemLista = ordem;
}

void configPC::setDetection(int jdet){
    detection = jdet;
}

void configPC::setDownloadListas(int jdl){
    downloadListas = jdl;
}

void configPC::setTempoDownload(int jtd){
    tempoDownload = jtd;
}

void configPC::setDownloadAutomatico(int jda){
    downloadAutomatico = jda;
}

void configPC::setDownloadFolder(QString jdf){
    downloadFolder = jdf;
}

void configPC::setFansub(QString jfs){
    preFfansub = jfs;
}

void configPC::setQualidade(QString jql){
    prefQualidade = jql;
}

void configPC::setTorrentPadrao(QString jtp){
    prefTorrent = jtp;
}

QString configPC::getOrdem(){
    return ordemLista;
}

QString configPC::retornaUser(){
    return usernameAnilist;
}

void configPC::EscreveArquivo(){
    QString filename = "Configurações/conf.txt";
    QFile file(filename);
    file.remove();
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        for(int i = 0; i < diretorioAnimes.length(); i++){
            stream << "D01>diretorioAnime[" << i << "]>" << diretorioAnimes[i] << endl;
        }
        for(int i = 0; i < diretorioAnimeEspecifico.length(); i++){
            stream << "D02>" << idAnime[i] << ">" << diretorioAnimeEspecifico[i] << endl;
        }
    }
}

void configPC::EscreveConfig(){
    QString filename = "Configurações/user.txt";
    QFile file(filename);
    file.remove();
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << "user>" << usernameAnilist << endl;
        stream << "ordem>" << ordemLista << endl;
        stream << "feedgeral>" << feedTorrentGeral << endl;
        stream << "feedespecifico>" << feedTorrentEspecifico << endl;
        stream << "detection>" << detection << endl;
        stream << "downloadlistas>" << downloadListas << endl;
        stream << "tempodownload>" << tempoDownload << endl;
        stream << "downloadautomatico>" << downloadAutomatico << endl;
        stream << "downloadfolder>" << downloadFolder << endl;
        stream << "fansub>" << preFfansub << endl;
        stream << "qualidade>" << prefQualidade << endl;
        stream << "torrent>" << prefTorrent << endl;
    }
}

std::string configPC::RetornaDiretorioTaiga(){
    return diretorioTaiga.toStdString();
}

int configPC::BuscaPastasAnimesEspecificos(QString nome, QString nomeAlternativo, QString id){
    //Busca todos os diretórios posíveis onde tem animes
    //pro caso de ter mais de um diretório
    //Ex: downloads/animes/ + e:/animes/
    QString nomeAnime = nome.toLower();
    QString nome2Anime = nomeAlternativo.toLower();
    QString nomeArquivoPelado;
//    nomeAnime.remove("-");
    nomeAnime.remove(".");
    nomeAnime.remove("?");
    nome2Anime.remove(".");
    nome2Anime.remove("?");
    QString pelado1, pelado2;
    for(int i = 0; i < diretorioAnimes.length(); i++){
        //Busca diretórios apenas
        QDirIterator it(diretorioAnimes[i], QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        while(it.hasNext()){
            //Iterador
            it.next();
            //Parse no nome da pasta
            anitomy->Parse(it.fileName().toStdWString());
            const auto& elements = anitomy->elements();
            std::wstring a = elements.get(anitomy::kElementAnimeTitle);
            nomeArquivoPelado = QString::fromStdWString(a).toLower();
            nomeArquivoPelado.remove(".");
            nomeArquivoPelado.remove("?");
            nomeArquivoPelado.remove("S1");
            nomeArquivoPelado.remove("S2");
            nomeArquivoPelado.remove("s1");
            nomeArquivoPelado.remove("s2");
            nomeArquivoPelado.remove("s01");
            nomeArquivoPelado.remove("s02");
            nomeArquivoPelado.remove("S01");
            nomeArquivoPelado.remove("S02");
            nomeArquivoPelado = nomeArquivoPelado.simplified();
            //Compara o nome da pasta com o nome do anime
            //Compara os dois nomes em letras minusculas, pra evitar erros
            if(nomeAnime.contains(nomeArquivoPelado) == true || nome2Anime.contains(nomeArquivoPelado) == true){// || pelado2.compare(nomeAnime) == 0 || pelado2.compare(nome2Anime) == 0){
                idAnime.append(id);
                diretorioAnimeEspecifico.append(it.filePath());
                return 0;
            }
        }
    }
    return 0;
}

//USAR ESSA FUNÇÃO NA HORA DE ABRIR O EPISÓDIO
//SEMPRE CHECAR SE EXISTE DIRETORIO ESPECIFICO ANTES DE PROCURAR
QString configPC::RetornaDiretorioAnimeEspecifico(int id){
    QFile inputFile("Configurações/conf.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       QStringList linha;
       while (!in.atEnd())
       {
           linha = in.readLine().split('>');
           if(linha.at(0).compare("D02") == 0)
               if(linha.at(1).toInt() == id){
                   return linha.at(2);
               }
       }
       inputFile.close();
    }
    return "0";
}

int configPC::RetornaTamanhoVector(){
    return diretorioAnimeEspecifico.length();
}
