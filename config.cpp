#include "config.h"
#include "filedownloader.h"

Config::Config(QObject *parent) : QObject(parent)
//Config::Config(QObject *parent, configPC *configura) : QObject(parent)
{
    pastas = new configPC;
    qWatching = false;
    qAnimeId = false;
    qFinished = false;
    qStartDateOk = false;
//    qPrimeiraEntrada = true;
    breakLoop = false;
}

Config::~Config(){
    delete pastas;
}

void Config::quebraloop(){
    breakLoop = true;
}

void Config::IniciaThread(QThread &cThread){
    connect(&cThread, SIGNAL(started()), this, SLOT(run()));
}

void Config::setConfigs(configPC *Configura){
    pastas = Configura;
    user = pastas->retornaUser();
//    user = "oi";
}


void Config::ParseTaiga(){
    QString arquivoEscrever = "Configurações/confTaiga.txt";
    QString arquivoLer1 = QString::fromStdString(pastas->RetornaDiretorioTaiga())+ "/user/" + pastas->retornaUser() + "@anilist/anime.xml";
    QString arquivoLer2 = QString::fromStdString(pastas->RetornaDiretorioTaiga())+ "/db/anime.xml";
    QFile lerUser(arquivoLer1);
    QFile lerDB(arquivoLer2);
    QFile escrever(arquivoEscrever);
    //Checa se todos os arquivos do taiga podem ser abertos
    escrever.remove();
    if (lerUser.open(QIODevice::ReadOnly) && lerDB.open(QIODevice::ReadOnly))
    {
        //Cria variáveis pra ler os arquivos do taiga
        QTextStream usr(&lerUser);
        QTextStream db(&lerDB);
        //Checa se é possível escrever no arquivo de configuração
        if (escrever.open(QIODevice::ReadWrite)) {
            QTextStream stream(&escrever);
            //Lê o arquivo usr até o fim
            while (!usr.atEnd())
            {
                //le cada linha do arquivo
                QString linha = usr.readLine();
                //Checa se a linha tem o dado que procuro //ID no caso
                if(linha.contains("<id>"))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<id>");
                    linha.remove("</id>");
                    //Remove espaços inúteis
                    linha = linha.simplified();
                    //Salva na variável certa
                    qId = linha;
                }
                else if(linha.contains("<progress>"))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<progress>");
                    linha.remove("</progress>");
                    //Remove espaços inúteis
                    linha = linha.simplified();
                    //Salva na variável certa
                    qProgresso = linha;
                }
                else if(linha.contains("<score>"))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<score>");
                    linha.remove("</score>");
                    //Remove espaços inúteis
                    linha = linha.simplified();
                    //Salva na variável certa
                    qScore = linha;
                }
                else if(linha.contains("<status>"))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<status>");
                    linha.remove("</status>");
                    //Remove espaços inúteis
                    linha = linha.simplified();
                    //Checa se está assistindo o anime
                    if(linha == "1"){
                        qWatching = true;
                    }
                }
                //Caso qWatching seja true, está vendo o anime. Então vamos entrar nos outros arquivos para procurar outras informações
                if(qWatching == true){
                    while(!db.atEnd()){
                        QString linha2 = db.readLine();
                        if(linha2.contains("<id name=\"anilist\">"))
                        {
                            //Remove dados desnecessários da linha e salva no vetor
//                            qDebug() << linha2;
                            linha2.remove("<id name=\"anilist\">");
                            linha2.remove("</id>");
                            //Remove espaços inúteis
                            linha2 = linha2.simplified();
                            //Checa se o id coincide com o anime procurado
                            if(qId == linha2){
                                qAnimeId = true;
                            }
                        }
                        //Caso tenha achado o id certo, começa a buscar as informações
                        if(qAnimeId == true){
//                            qSeason = '0';
                            if(linha2.contains("<title>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<title><![CDATA[");
                                linha2.remove("]]></title>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qNome = linha2;
                                //É necessário colocar todos os nomes para o caso de não ter não diferente em inglês
                                qNomeIngles = linha2;
                            }
                            else if(linha2.contains("<english>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<english><![CDATA[");
                                linha2.remove("]]></english>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qNomeIngles = linha2;
                            }
                            else if(linha2.contains("<episode_count>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<episode_count>");
                                linha2.remove("</episode_count>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qEpiCount = linha2;
                            }
                            else if(linha2.contains("<date_start>"))
                            {
//                                qDebug() << "oi";
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<date_start>");
                                linha2.remove("</date_start>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva link da imagem
                                qSeason = linha2;
                                qStartDateOk = true;
                            }
                            else if(linha2.contains("<date_end>"))
                            {
                                qFinished = true;
                            }
                            else if(linha2.contains("<image>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<image>");
                                linha2.remove("</image>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva link da imagem
                                qImagem = linha2;
                            }
                            else if(linha2.contains("<genres>"))
                            {
                                //Remove da dos desnecessários da linha e salva no vetor
                                linha2.remove("<genres>");
                                linha2.remove("</genres>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qGenero = linha2;
                            }
                            else if(linha2.contains("<producers>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<producers>");
                                linha2.remove("</producers>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qProdutores = linha2;
                            }
                            else if(linha2.contains("<score>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<score>");
                                linha2.remove("</score>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Pega os 4 primeiros caracteres, pra evitar números demais
                                linha2.resize(4);
                                //Salva nome do anime
                                qScoreGeral = linha2;
                            }
                            else if(linha2.contains("<synopsis>")){
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<synopsis><![CDATA[");
                                linha2.remove("]]></synopsis>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qSinopse = linha2;
                                //Para de ler o arquivo, já não temos mais nada o que fazer aqui
                                qAnimeId = false;
                                break;
                            }
                        }
                    }
                }
                //Após ler tudo, começa a escrever os dados no arquivo
                if(qWatching == true){
                    stream << qNome << endl;
                    stream << qNomeIngles << endl;
                    stream << qProgresso << endl;
                    stream << qEpiCount << endl;
                    stream << qScore << endl;
                    if(qStartDateOk == true){
                        stream << qScoreGeral << endl;
                        stream << qSeason << endl;
                    }
                    else{
                        stream << "0" << endl;
                        qSeason = "2050-01-01";
                        stream << qSeason << endl;
                    }
                    stream << arrumaSeason(qSeason) << endl;
                    if(qFinished == false){
                        if(comparaData(qSeason) == true){
                            stream << "Ongoing" << endl;
                        }
                        else{
                            stream << "Not aired yet" << endl;
                        }
                    }
                    else{
                        stream << "Finished Airing" << endl;
                    }
                    stream << qProdutores << endl;
                    stream << qImagem << endl;
                    stream << qGenero << endl;
                    stream << qSinopse << endl;
                    stream << qId << endl;
                }
                qWatching = false;
                qFinished = false;
                qStartDateOk = false;
            }
            //Após terminar de ler e escrever os arquivos, começa a salvar e fechar
            escrever.close();
        }
        lerDB.close();
        lerUser.close();
    }
}


void Config::ParseCompleted(){
    QString arquivoEscrever = "Configurações/confCompleted.txt";
    QString arquivoLer1 = QString::fromStdString(pastas->RetornaDiretorioTaiga())+ "/user/" + pastas->retornaUser() + "@anilist/anime.xml";
    QString arquivoLer2 = QString::fromStdString(pastas->RetornaDiretorioTaiga())+ "/db/anime.xml";
    QFile lerUser(arquivoLer1);
    QFile lerDB(arquivoLer2);
    QFile escrever(arquivoEscrever);
    escrever.remove();
    //Checa se todos os arquivos do taiga podem ser abertos
    if (lerUser.open(QIODevice::ReadOnly) && lerDB.open(QIODevice::ReadOnly))
    {
        //Cria variáveis pra ler os arquivos do taiga
        QTextStream usr(&lerUser);
        QTextStream db(&lerDB);
        //Checa se é possível escrever no arquivo de configuração
        if (escrever.open(QIODevice::ReadWrite)) {
            QTextStream stream(&escrever);
            //Lê o arquivo usr até o fim
            while (!usr.atEnd())
            {
                //le cada linha do arquivo
                QString linha = usr.readLine();
                //Checa se a linha tem o dado que procuro //ID no caso
                if(linha.contains("<id>"))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<id>");
                    linha.remove("</id>");
                    //Remove espaços inúteis
                    linha = linha.simplified();
                    //Salva na variável certa
                    qId = linha;
                }
                else if(linha.contains("<progress>"))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<progress>");
                    linha.remove("</progress>");
                    //Remove espaços inúteis
                    linha = linha.simplified();
                    //Salva na variável certa
                    qProgresso = linha;
                }
                else if(linha.contains("<score>"))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<score>");
                    linha.remove("</score>");
                    //Remove espaços inúteis
                    linha = linha.simplified();
                    //Salva na variável certa
                    qScore = linha;
                }
                else if(linha.contains("<status>"))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<status>");
                    linha.remove("</status>");
                    //Remove espaços inúteis
                    linha = linha.simplified();
                    //Checa se está assistindo o anime
                    if(linha == "2"){
                        qWatching = true;
                    }
                }
                //Caso qWatching seja true, está vendo o anime. Então vamos entrar nos outros arquivos para procurar outras informações
                if(qWatching == true){
                    while(!db.atEnd()){
                        QString linha2 = db.readLine();
                        if(linha2.contains("<id name=\"anilist\">"))
                        {
                            //Remove dados desnecessários da linha e salva no vetor
//                            qDebug() << linha2;
                            linha2.remove("<id name=\"anilist\">");
                            linha2.remove("</id>");
                            //Remove espaços inúteis
                            linha2 = linha2.simplified();
                            //Checa se o id coincide com o anime procurado
                            if(qId == linha2){
                                qAnimeId = true;
                            }
                        }
                        //Caso tenha achado o id certo, começa a buscar as informações
                        if(qAnimeId == true){
//                            qSeason = '0';
                            if(linha2.contains("<title>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<title><![CDATA[");
                                linha2.remove("]]></title>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qNome = linha2;
                                //É necessário colocar todos os nomes para o caso de não ter não diferente em inglês
                                qNomeIngles = linha2;
                            }
                            else if(linha2.contains("<english>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<english><![CDATA[");
                                linha2.remove("]]></english>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qNomeIngles = linha2;
                            }
                            else if(linha2.contains("<episode_count>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<episode_count>");
                                linha2.remove("</episode_count>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qEpiCount = linha2;
                            }
                            else if(linha2.contains("<date_start>"))
                            {
//                                qDebug() << "oi";
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<date_start>");
                                linha2.remove("</date_start>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva link da imagem
                                qSeason = linha2;
                                qStartDateOk = true;
                            }
                            else if(linha2.contains("<date_end>"))
                            {
                                qFinished = true;
                            }
                            else if(linha2.contains("<image>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<image>");
                                linha2.remove("</image>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva link da imagem
                                qImagem = linha2;
                            }
                            else if(linha2.contains("<genres>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<genres>");
                                linha2.remove("</genres>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qGenero = linha2;
                            }
                            else if(linha2.contains("<producers>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<producers>");
                                linha2.remove("</producers>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qProdutores = linha2;
                            }
                            else if(linha2.contains("<score>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<score>");
                                linha2.remove("</score>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Pega os 4 primeiros caracteres, pra evitar números demais
                                linha2.resize(4);
                                //Salva nome do anime
                                qScoreGeral = linha2;
                            }
                            else if(linha2.contains("<synopsis>")){
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<synopsis><![CDATA[");
                                linha2.remove("]]></synopsis>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qSinopse = linha2;
                                //Para de ler o arquivo, já não temos mais nada o que fazer aqui
                                qAnimeId = false;
                                break;
                            }
                        }
                    }
                }
                //Após ler tudo, começa a escrever os dados no arquivo
                if(qWatching == true){
                    stream << qNome << endl;
                    stream << qNomeIngles << endl;
                    stream << qProgresso << endl;
                    stream << qEpiCount << endl;
                    stream << qScore << endl;
                    if(qStartDateOk == true){
                        stream << qScoreGeral << endl;
                        stream << qSeason << endl;
                    }
                    else{
                        stream << "0" << endl;
                        qSeason = "2050-01-01";
                        stream << qSeason << endl;
                    }
                    stream << arrumaSeason(qSeason) << endl;
                    if(qFinished == false){
                        if(comparaData(qSeason) == true){
                            stream << "Ongoing" << endl;
                        }
                        else{
                            stream << "Not aired yet" << endl;
                        }
                    }
                    else{
                        stream << "Finished Airing" << endl;
                    }
                    stream << qProdutores << endl;
                    stream << qImagem << endl;
                    stream << qGenero << endl;
                    stream << qSinopse << endl;
                    stream << qId << endl;
                }
                qWatching = false;
                qFinished = false;
                qStartDateOk = false;
            }
            //Após terminar de ler e escrever os arquivos, começa a salvar e fechar
            escrever.close();
        }
        lerDB.close();
        lerUser.close();
    }
}
void Config::ParseOnHold(){
    QString arquivoEscrever = "Configurações/confOnHold.txt";
    QString arquivoLer1 = QString::fromStdString(pastas->RetornaDiretorioTaiga())+ "/user/" + pastas->retornaUser() + "@anilist/anime.xml";
    QString arquivoLer2 = QString::fromStdString(pastas->RetornaDiretorioTaiga())+ "/db/anime.xml";
    QFile lerUser(arquivoLer1);
    QFile lerDB(arquivoLer2);
    QFile escrever(arquivoEscrever);
    escrever.remove();
    //Checa se todos os arquivos do taiga podem ser abertos
    if (lerUser.open(QIODevice::ReadOnly) && lerDB.open(QIODevice::ReadOnly))
    {
        //Cria variáveis pra ler os arquivos do taiga
        QTextStream usr(&lerUser);
        QTextStream db(&lerDB);
        //Checa se é possível escrever no arquivo de configuração
        if (escrever.open(QIODevice::ReadWrite)) {
            QTextStream stream(&escrever);
            //Lê o arquivo usr até o fim
            while (!usr.atEnd())
            {
                //le cada linha do arquivo
                QString linha = usr.readLine();
                //Checa se a linha tem o dado que procuro //ID no caso
                if(linha.contains("<id>"))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<id>");
                    linha.remove("</id>");
                    //Remove espaços inúteis
                    linha = linha.simplified();
                    //Salva na variável certa
                    qId = linha;
                }
                else if(linha.contains("<progress>"))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<progress>");
                    linha.remove("</progress>");
                    //Remove espaços inúteis
                    linha = linha.simplified();
                    //Salva na variável certa
                    qProgresso = linha;
                }
                else if(linha.contains("<score>"))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<score>");
                    linha.remove("</score>");
                    //Remove espaços inúteis
                    linha = linha.simplified();
                    //Salva na variável certa
                    qScore = linha;
                }
                else if(linha.contains("<status>"))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<status>");
                    linha.remove("</status>");
                    //Remove espaços inúteis
                    linha = linha.simplified();
                    //Checa se está assistindo o anime
                    if(linha == "3"){
                        qWatching = true;
                    }
                }
                //Caso qWatching seja true, está vendo o anime. Então vamos entrar nos outros arquivos para procurar outras informações
                if(qWatching == true){
                    while(!db.atEnd()){
                        QString linha2 = db.readLine();
                        if(linha2.contains("<id name=\"anilist\">"))
                        {
                            //Remove dados desnecessários da linha e salva no vetor
//                            qDebug() << linha2;
                            linha2.remove("<id name=\"anilist\">");
                            linha2.remove("</id>");
                            //Remove espaços inúteis
                            linha2 = linha2.simplified();
                            //Checa se o id coincide com o anime procurado
                            if(qId == linha2){
                                qAnimeId = true;
                            }
                        }
                        //Caso tenha achado o id certo, começa a buscar as informações
                        if(qAnimeId == true){
//                            qSeason = '0';
                            if(linha2.contains("<title>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<title><![CDATA[");
                                linha2.remove("]]></title>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qNome = linha2;
                                //É necessário colocar todos os nomes para o caso de não ter não diferente em inglês
                                qNomeIngles = linha2;
                            }
                            else if(linha2.contains("<english>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<english><![CDATA[");
                                linha2.remove("]]></english>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qNomeIngles = linha2;
                            }
                            else if(linha2.contains("<episode_count>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<episode_count>");
                                linha2.remove("</episode_count>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qEpiCount = linha2;
                            }
                            else if(linha2.contains("<date_start>"))
                            {
//                                qDebug() << "oi";
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<date_start>");
                                linha2.remove("</date_start>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva link da imagem
                                qSeason = linha2;
                                qStartDateOk = true;
                            }
                            else if(linha2.contains("<date_end>"))
                            {
                                qFinished = true;
                            }
                            else if(linha2.contains("<image>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<image>");
                                linha2.remove("</image>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva link da imagem
                                qImagem = linha2;
                            }
                            else if(linha2.contains("<genres>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<genres>");
                                linha2.remove("</genres>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qGenero = linha2;
                            }
                            else if(linha2.contains("<producers>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<producers>");
                                linha2.remove("</producers>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qProdutores = linha2;
                            }
                            else if(linha2.contains("<score>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<score>");
                                linha2.remove("</score>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Pega os 4 primeiros caracteres, pra evitar números demais
                                linha2.resize(4);
                                //Salva nome do anime
                                qScoreGeral = linha2;
                            }
                            else if(linha2.contains("<synopsis>")){
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<synopsis><![CDATA[");
                                linha2.remove("]]></synopsis>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qSinopse = linha2;
                                //Para de ler o arquivo, já não temos mais nada o que fazer aqui
                                qAnimeId = false;
                                break;
                            }
                        }
                    }
                }
                //Após ler tudo, começa a escrever os dados no arquivo
                if(qWatching == true){
                    stream << qNome << endl;
                    stream << qNomeIngles << endl;
                    stream << qProgresso << endl;
                    stream << qEpiCount << endl;
                    stream << qScore << endl;
                    if(qStartDateOk == true){
                        stream << qScoreGeral << endl;
                        stream << qSeason << endl;
                    }
                    else{
                        stream << "0" << endl;
                        qSeason = "2050-01-01";
                        stream << qSeason << endl;
                    }
                    stream << arrumaSeason(qSeason) << endl;
                    if(qFinished == false){
                        if(comparaData(qSeason) == true){
                            stream << "Ongoing" << endl;
                        }
                        else{
                            stream << "Not aired yet" << endl;
                        }
                    }
                    else{
                        stream << "Finished Airing" << endl;
                    }
                    stream << qProdutores << endl;
                    stream << qImagem << endl;
                    stream << qGenero << endl;
                    stream << qSinopse << endl;
                    stream << qId << endl;
                }
                qWatching = false;
                qFinished = false;
                qStartDateOk = false;
            }
            //Após terminar de ler e escrever os arquivos, começa a salvar e fechar
            escrever.close();
        }
        lerDB.close();
        lerUser.close();
    }
}
void Config::ParseDropped(){
    QString arquivoEscrever = "Configurações/confDropped.txt";
    QString arquivoLer1 = QString::fromStdString(pastas->RetornaDiretorioTaiga())+ "/user/" + pastas->retornaUser() + "@anilist/anime.xml";
    QString arquivoLer2 = QString::fromStdString(pastas->RetornaDiretorioTaiga())+ "/db/anime.xml";
    QFile lerUser(arquivoLer1);
    QFile lerDB(arquivoLer2);
    QFile escrever(arquivoEscrever);
    escrever.remove();
    //Checa se todos os arquivos do taiga podem ser abertos
    if (lerUser.open(QIODevice::ReadOnly) && lerDB.open(QIODevice::ReadOnly))
    {
        //Cria variáveis pra ler os arquivos do taiga
        QTextStream usr(&lerUser);
        QTextStream db(&lerDB);
        //Checa se é possível escrever no arquivo de configuração
        if (escrever.open(QIODevice::ReadWrite)) {
            QTextStream stream(&escrever);
            //Lê o arquivo usr até o fim
            while (!usr.atEnd())
            {
                //le cada linha do arquivo
                QString linha = usr.readLine();
                //Checa se a linha tem o dado que procuro //ID no caso
                if(linha.contains("<id>"))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<id>");
                    linha.remove("</id>");
                    //Remove espaços inúteis
                    linha = linha.simplified();
                    //Salva na variável certa
                    qId = linha;
                }
                else if(linha.contains("<progress>"))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<progress>");
                    linha.remove("</progress>");
                    //Remove espaços inúteis
                    linha = linha.simplified();
                    //Salva na variável certa
                    qProgresso = linha;
                }
                else if(linha.contains("<score>"))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<score>");
                    linha.remove("</score>");
                    //Remove espaços inúteis
                    linha = linha.simplified();
                    //Salva na variável certa
                    qScore = linha;
                }
                else if(linha.contains("<status>"))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<status>");
                    linha.remove("</status>");
                    //Remove espaços inúteis
                    linha = linha.simplified();
                    //Checa se está assistindo o anime
                    if(linha == "4"){
                        qWatching = true;
                    }
                }
                //Caso qWatching seja true, está vendo o anime. Então vamos entrar nos outros arquivos para procurar outras informações
                if(qWatching == true){
                    while(!db.atEnd()){
                        QString linha2 = db.readLine();
                        if(linha2.contains("<id name=\"anilist\">"))
                        {
                            //Remove dados desnecessários da linha e salva no vetor
//                            qDebug() << linha2;
                            linha2.remove("<id name=\"anilist\">");
                            linha2.remove("</id>");
                            //Remove espaços inúteis
                            linha2 = linha2.simplified();
                            //Checa se o id coincide com o anime procurado
                            if(qId == linha2){
                                qAnimeId = true;
                            }
                        }
                        //Caso tenha achado o id certo, começa a buscar as informações
                        if(qAnimeId == true){
//                            qSeason = '0';
                            if(linha2.contains("<title>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<title><![CDATA[");
                                linha2.remove("]]></title>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qNome = linha2;
                                //É necessário colocar todos os nomes para o caso de não ter não diferente em inglês
                                qNomeIngles = linha2;
                            }
                            else if(linha2.contains("<english>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<english><![CDATA[");
                                linha2.remove("]]></english>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qNomeIngles = linha2;
                            }
                            else if(linha2.contains("<episode_count>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<episode_count>");
                                linha2.remove("</episode_count>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qEpiCount = linha2;
                            }
                            else if(linha2.contains("<date_start>"))
                            {
//                                qDebug() << "oi";
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<date_start>");
                                linha2.remove("</date_start>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva link da imagem
                                qSeason = linha2;
                            }
                            else if(linha2.contains("<date_end>"))
                            {
                                qFinished = true;
                            }
                            else if(linha2.contains("<image>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<image>");
                                linha2.remove("</image>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva link da imagem
                                qImagem = linha2;
                            }
                            else if(linha2.contains("<genres>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<genres>");
                                linha2.remove("</genres>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qGenero = linha2;
                            }
                            else if(linha2.contains("<producers>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<producers>");
                                linha2.remove("</producers>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qProdutores = linha2;
                            }
                            else if(linha2.contains("<score>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<score>");
                                linha2.remove("</score>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Pega os 4 primeiros caracteres, pra evitar números demais
                                linha2.resize(4);
                                //Salva nome do anime
                                qScoreGeral = linha2;
                            }
                            else if(linha2.contains("<synopsis>")){
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<synopsis><![CDATA[");
                                linha2.remove("]]></synopsis>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qSinopse = linha2;
                                //Para de ler o arquivo, já não temos mais nada o que fazer aqui
                                qAnimeId = false;
                                break;
                            }
                        }
                    }
                }
                //Após ler tudo, começa a escrever os dados no arquivo
                if(qWatching == true){
                    stream << qNome << endl;
                    stream << qNomeIngles << endl;
                    stream << qProgresso << endl;
                    stream << qEpiCount << endl;
                    stream << qScore << endl;
                    stream << qScoreGeral << endl;
                    if(qStartDateOk == true){
                        stream << qSeason << endl;
                    }
                    else{
                        qSeason = "2050-01-01";
                        stream << qSeason << endl;
                    }
                    stream << arrumaSeason(qSeason) << endl;
                    if(qFinished == false){
                        if(comparaData(qSeason) == true){
                            stream << "Ongoing" << endl;
                        }
                        else{
                            stream << "Not aired yet" << endl;
                        }
                    }
                    else{
                        stream << "Finished Airing" << endl;
                    }
                    stream << qProdutores << endl;
                    stream << qImagem << endl;
                    stream << qGenero << endl;
                    stream << qSinopse << endl;
                    stream << qId << endl;
                }
                qWatching = false;
                qFinished = false;
                qStartDateOk = false;
            }
            //Após terminar de ler e escrever os arquivos, começa a salvar e fechar
            escrever.close();
        }
        lerDB.close();
        lerUser.close();
    }
}
void Config::ParsePlanToWatch(){
    QString arquivoEscrever = "Configurações/confPlanToWatch.txt";
    QString arquivoLer1 = QString::fromStdString(pastas->RetornaDiretorioTaiga())+ "/user/" + pastas->retornaUser() + "@anilist/anime.xml";
    QString arquivoLer2 = QString::fromStdString(pastas->RetornaDiretorioTaiga())+ "/db/anime.xml";
    QFile lerUser(arquivoLer1);
    QFile lerDB(arquivoLer2);
    QFile escrever(arquivoEscrever);
    escrever.remove();
    //Checa se todos os arquivos do taiga podem ser abertos
    if (lerUser.open(QIODevice::ReadOnly) && lerDB.open(QIODevice::ReadOnly))
    {
        //Cria variáveis pra ler os arquivos do taiga
        QTextStream usr(&lerUser);
        QTextStream db(&lerDB);
        //Checa se é possível escrever no arquivo de configuração
        if (escrever.open(QIODevice::ReadWrite)) {
            QTextStream stream(&escrever);
            //Lê o arquivo usr até o fim
            while (!usr.atEnd())
            {
                //le cada linha do arquivo
                QString linha = usr.readLine();
                //Checa se a linha tem o dado que procuro //ID no caso
                if(linha.contains("<id>"))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<id>");
                    linha.remove("</id>");
                    //Remove espaços inúteis
                    linha = linha.simplified();
                    //Salva na variável certa
                    qId = linha;
                }
                else if(linha.contains("<progress>"))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<progress>");
                    linha.remove("</progress>");
                    //Remove espaços inúteis
                    linha = linha.simplified();
                    //Salva na variável certa
                    qProgresso = linha;
                }
                else if(linha.contains("<score>"))
                {
                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<score>");
                    linha.remove("</score>");
                    //Remove espaços inúteis
                    linha = linha.simplified();
                    //Salva na variável certa
                    qScore = linha;
                }
                else if(linha.contains("<status>"))
                {

                    //Remove dados desnecessários da linha e salva no vetor
                    linha.remove("<status>");
                    linha.remove("</status>");
                    //Remove espaços inúteis
                    linha = linha.simplified();
                    //Checa se está assistindo o anime
                    if(linha == "5"){
                        qWatching = true;
                    }
                }
                //Caso qWatching seja true, está vendo o anime. Então vamos entrar nos outros arquivos para procurar outras informações
                if(qWatching == true){
                    while(!db.atEnd()){
                        QString linha2 = db.readLine();
                        if(linha2.contains("<id name=\"anilist\">"))
                        {
                            //Remove dados desnecessários da linha e salva no vetor
//                            qDebug() << linha2;
                            linha2.remove("<id name=\"anilist\">");
                            linha2.remove("</id>");
                            //Remove espaços inúteis
                            linha2 = linha2.simplified();
                            //Checa se o id coincide com o anime procurado
                            if(qId == linha2){
                                qAnimeId = true;
                            }
                        }
                        //Caso tenha achado o id certo, começa a buscar as informações
                        if(qAnimeId == true){
//                            qSeason = '0';
                            if(linha2.contains("<title>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<title><![CDATA[");
                                linha2.remove("]]></title>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qNome = linha2;
                                //É necessário colocar todos os nomes para o caso de não ter não diferente em inglês
                                qNomeIngles = linha2;
                            }
                            else if(linha2.contains("<english>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<english><![CDATA[");
                                linha2.remove("]]></english>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qNomeIngles = linha2;
                            }
                            else if(linha2.contains("<episode_count>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<episode_count>");
                                linha2.remove("</episode_count>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qEpiCount = linha2;
                            }
                            else if(linha2.contains("<date_start>"))
                            {
//                                qDebug() << "oi";
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<date_start>");
                                linha2.remove("</date_start>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva link da imagem
                                qSeason = linha2;
                                qStartDateOk = true;
                            }
                            else if(linha2.contains("<date_end>"))
                            {
                                qFinished = true;
                            }
                            else if(linha2.contains("<image>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<image>");
                                linha2.remove("</image>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva link da imagem
                                qImagem = linha2;
                            }
                            else if(linha2.contains("<genres>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<genres>");
                                linha2.remove("</genres>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qGenero = linha2;
                            }
                            else if(linha2.contains("<producers>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<producers>");
                                linha2.remove("</producers>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qProdutores = linha2;
                            }
                            else if(linha2.contains("<score>"))
                            {
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<score>");
                                linha2.remove("</score>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Pega os 4 primeiros caracteres, pra evitar números demais
                                linha2.resize(4);
                                //Salva nome do anime
                                qScoreGeral = linha2;
                            }
                            else if(linha2.contains("<synopsis>")){
                                //Remove dados desnecessários da linha e salva no vetor
                                linha2.remove("<synopsis><![CDATA[");
                                linha2.remove("]]></synopsis>");
                                //Remove espaços inúteis
                                linha2 = linha2.simplified();
                                //Salva nome do anime
                                qSinopse = linha2;
                                //Para de ler o arquivo, já não temos mais nada o que fazer aqui
                                qAnimeId = false;
                                break;
                            }
                        }
                    }
                }
                //Após ler tudo, começa a escrever os dados no arquivo
                if(qWatching == true){
                    stream << qNome << endl;
                    stream << qNomeIngles << endl;
                    stream << qProgresso << endl;
                    stream << qEpiCount << endl;
                    stream << qScore << endl;
                    if(qStartDateOk == true){
                        stream << qScoreGeral << endl;
                        stream << qSeason << endl;
                    }
                    else{
                        stream << "0" << endl;
                        qSeason = "2050-01-01";
                        stream << qSeason << endl;
                    }
                    stream << arrumaSeason(qSeason) << endl;
                    if(qFinished == false){
                        if(comparaData(qSeason) == true){
                            stream << "Ongoing" << endl;
                        }
                        else{
                            stream << "Not aired yet" << endl;
                        }
                    }
                    else{
                        stream << "Finished Airing" << endl;
                    }
                    stream << qProdutores << endl;
                    stream << qImagem << endl;
                    stream << qGenero << endl;
                    stream << qSinopse << endl;
                    stream << qId << endl;
                }
                qWatching = false;
                qFinished = false;
                qStartDateOk = false;
            }
            //Após terminar de ler e escrever os arquivos, começa a salvar e fechar
            escrever.close();
        }
        lerDB.close();
        lerUser.close();
    }
}

QString Config::arrumaSeason(QString season){
    if(season == "2050-01-01"){
        return "Not defined";
    }
    QStringList listaSeason = season.split('-');

    if(listaSeason.at(1) == "01" || listaSeason.at(1) == "02" || listaSeason.at(1) == "03")
        season = "Winter ";
    else if(listaSeason.at(1) == "04" || listaSeason.at(1) == "05" || listaSeason.at(1) == "06")
        season = "Spring ";
    else if(listaSeason.at(1) == "07" || listaSeason.at(1) == "08" || listaSeason.at(1) == "09")
        season = "Summer ";
    else if(listaSeason.at(1) == "10" || listaSeason.at(1) == "11" || listaSeason.at(1) == "12")
        season = "Fall ";
    season.append(listaSeason.at(0));
    return season;
}

bool Config::comparaData(QString season){
    QStringList data = season.split('-');
    QDate estreia(data.at(0).toInt(), data.at(1).toInt(), data.at(2).toInt());
    if(estreia <= QDate::currentDate()){
        return true;
    }
    return false;
}

void Config::botaoRefresh(){
    ParseTaiga();
    ParseCompleted();
    ParseOnHold();
    ParseDropped();
    qWatching = false;
    qAnimeId = false;
    ParsePlanToWatch();
    emit refresh();
}

void Config::run(){
    QDir diretorio;
    while(true){
        if(pastas->retornaUser().compare(user) == 0 && pastas->retornaUser() != ""){
            ParseTaiga();
            emit terminouSetArquivo();
            while(true){
                ParseCompleted();
                emit terminouCompleted();
                ParseOnHold();
                emit terminouOnHold();
                ParseDropped();
                qWatching = false;
                qAnimeId = false;
                emit terminouDropped();
                ParsePlanToWatch();
                emit terminouPlanToWatch();
                this->thread()->sleep(300);
                ParseTaiga();
                emit refresh();
                if(breakLoop == true)
                    break;
            }
        }
        else if(pastas->retornaUser().compare(user) != 0 && pastas->retornaUser() != ""){
            diretorio.setPath(QString::fromStdString(pastas->RetornaDiretorioTaiga())+ "/user/" + pastas->retornaUser() + "@anilist");
            if(diretorio.exists()){
                user = pastas->retornaUser();
                emit mensagemConfig("AUsr");
            }
            else{
                emit mensagemConfig("EUsr");
            }
        }
        if(breakLoop == true)
            break;
    }
    qDebug() << "olha, quebrou";
}
