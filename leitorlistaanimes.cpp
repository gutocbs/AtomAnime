#include "leitorlistaanimes.h"

leitorlistaanimes::leitorlistaanimes(QObject *parent) : QObject(parent)
{

}

leitorlistaanimes::~leitorlistaanimes(){
    fdeletaListaAnimes();
}

bool leitorlistaanimes::fadicionaAnimes(){
    QString arquivoLer1 = QDir::homePath() + "/AppData/Roaming/Taiga/data/user/gutocbs@anilist/anime.xml";
    QString arquivoLer2 = QDir::homePath() + "/AppData/Roaming/Taiga/data/db/anime.xml";
    QFile lerUser(arquivoLer1);
    QFile lerDB(arquivoLer2);

    bool larquivo1Lido = false; //Checa se já encontrou todos os dados de um anime em uma das listas
    bool lAnimeId = false; //Chega se o id do anime lido é o id do anime procurado
    bool lStarted = false; //Checa se o anime já terminou ou ainda está lançando episódios
    bool lFinished = false; //Checa se o anime já terminou ou ainda está lançando episódios
    bool lmaisSinopse = false; //Checa se a sinopse tem mais de uma linha
    QString llista; //Diz em que lista o anime se encontra

    QString lid;
    QString lnome;
    QString lnomeIngles;
    QString lnomeJapones;
    QStringList lnomeAlternativo; //StringList por que pode ter lários nomes alternativos
    QString lnumEpisodiosTotais;
    QString lnumEpisodiosAssistidos;
    QString lnotaMediaSite;
    QString lLinkImagemMedia;
    QString lnotaMediaPessoal;
    QString lsinopse;
    QString lseason;
    QDate lestreia;

    //Checa se todos os arquivos do taiga podem ser abertos
    if (lerUser.open(QIODevice::ReadOnly) && lerDB.open(QIODevice::ReadOnly))
    {
        //Cria variáveis pra ler os arquivos do taiga
        QTextStream usr(&lerUser);
        QTextStream db(&lerDB);
        //Checa se é possível escrever no arquivo de configuração
        //Lê o arquivo usr até o fim
        while (!usr.atEnd())
        {
            QString llinha = usr.readLine();
            //Checa se a llinha tem o dado que procuro //ID no caso
            if(llinha.contains("<id>"))
            {
                //Remove dados desnecessários da llinha e salva no vetor
                llinha.remove("<id>");
                llinha.remove("</id>");
                //Remove espaços inúteis
                llinha = llinha.simplified();
                //Salva na variável certa
                 lid = llinha;
            }
            else if(llinha.contains("<progress>"))
            {
                //Remove dados desnecessários da llinha e salva no vetor
                llinha.remove("<progress>");
                llinha.remove("</progress>");
                //Remove espaços inúteis
                llinha = llinha.simplified();
                //Salva na variável certa
                 lnumEpisodiosAssistidos = llinha;
            }
            else if(llinha.contains("<score>"))
            {
                //Remove dados desnecessários da llinha e salva no vetor
                llinha.remove("<score>");
                llinha.remove("</score>");
                //Remove espaços inúteis
                llinha = llinha.simplified();
                //Salva na variável certa
                 lnotaMediaPessoal = llinha;
            }
            else if(llinha.contains("<status>"))
            {
                //Remove dados desnecessários da llinha e salva no vetor
                llinha.remove("<status>");
                llinha.remove("</status>");
                //Remove espaços inúteis
                llinha = llinha.simplified();
                //Checa em que lista o anime de encaixa
                llista = llinha;
                larquivo1Lido = true;
            }
            //Caso larquivo1lido seja true, encontrou um anime à ser lido. Então vamos entrar nos outros arquivos para procurar outras informações
            if(larquivo1Lido == true){
                lnome.clear();
                lnomeIngles.clear();
                lnomeJapones.clear();
                lnomeAlternativo.clear(); //StringList por que pode ter lários nomes alternativos
                lnumEpisodiosTotais.clear();
                lnotaMediaSite.clear();
                lLinkImagemMedia.clear();
                lsinopse.clear();
                while(!db.atEnd()){
                    QString llinha2 = db.readLine();
                    if(llinha2.contains("<id name=\"taiga\">"))
                    {
                        //Remove dados desnecessários da llinha e salva no vetor
                        llinha2.remove("<id name=\"taiga\">");
                        llinha2.remove("</id>");
                        //Remove espaços inúteis
                        llinha2 = llinha2.simplified();
                        //Checa se o id coincide com o anime procurado
                        if(lid == llinha2){
                            lAnimeId = true;
                        }
                    }
                    //Caso tenha achado o id certo, começa a buscar as informações
                    if(lAnimeId == true){
                        if(llinha2.contains("<title>"))
                        {
                            //Remove dados desnecessários da llinha e salva no vetor
                            llinha2.remove("<title><![CDATA[");
                            llinha2.remove("]]></title>");
                            //Remove espaços inúteis
                            llinha2 = llinha2.simplified();
                            //Salva nome do anime
                            lnome = llinha2;
                            //É necessário colocar todos os nomes para o caso de não ter não diferente em inglês
                             lnomeIngles = llinha2;
                        }
                        else if(llinha2.contains("<english>"))
                        {
                            //Remove dados desnecessários da llinha e salva no vetor
                            llinha2.remove("<english><![CDATA[");
                            llinha2.remove("]]></english>");
                            //Remove espaços inúteis
                            llinha2 = llinha2.simplified();
                            //Salva nome do anime
                             lnomeIngles = llinha2;
                        }
                        else if(llinha2.contains("<episode_count>"))
                        {
                            //Remove dados desnecessários da llinha e salva no vetor
                            llinha2.remove("<episode_count>");
                            llinha2.remove("</episode_count>");
                            //Remove espaços inúteis
                            llinha2 = llinha2.simplified();
                            //Salva nome do anime
                             lnumEpisodiosTotais = llinha2;
                        }
                        else if(llinha2.contains("<date_start>"))
                        {
                            //Remove dados desnecessários da llinha e salva no vetor
                            llinha2.remove("<date_start>");
                            llinha2.remove("</date_start>");
                            //Remove espaços inúteis
                            llinha2 = llinha2.simplified();
                            QStringList ldata = llinha2.split('-');
                            lestreia.setDate(ldata.at(0).toInt(),ldata.at(1).toInt(),ldata.at(2).toInt());
                            if(lestreia.month() == 1 || lestreia.month() == 2 || lestreia.month() == 3)
                                lseason = "Winter ";
                            else if(lestreia.month() == 4 || lestreia.month() == 5 || lestreia.month() == 6)
                                lseason = "Spring ";
                            else if(lestreia.month() == 7 || lestreia.month() == 8 || lestreia.month() == 9)
                                lseason = "Summer ";
                            else if(lestreia.month() == 10 || lestreia.month() == 11 || lestreia.month() == 12)
                                lseason = "Fall ";
                            lseason.append(QString::number(lestreia.year()));
                            if(lestreia <= QDate::currentDate())
                                lStarted = true;
                        }
                        else if(llinha2.contains("<date_end>"))
                        {
                             lFinished = true;
                        }
                        else if(llinha2.contains("<image>"))
                        {
                            //Remove dados desnecessários da llinha e salva no vetor
                            llinha2.remove("<image>");
                            llinha2.remove("</image>");
                            //Remove espaços inúteis
                            llinha2 = llinha2.simplified();
                            //Salva link da imagem
                            lLinkImagemMedia = llinha2;
                        }
                        else if(llinha2.contains("<score>"))//ALGUNS ANIMES NÃO ENTRAM AQUI
                        {
                            //Remove dados desnecessários da llinha e salva no vetor
                            llinha2.remove("<score>");
                            llinha2.remove("</score>");
                            //Remove espaços inúteis
                            llinha2 = llinha2.simplified();
                            //Pega os 4 primeiros caracteres, pra evitar números demais
                            llinha2.resize(4);
                            //Salva nome do anime
                             lnotaMediaSite = llinha2;
                        }
                        else if(llinha2.contains("<synopsis>")){//ALGUNS ANIMES NÃO ENTRAM AQUI
                            //Remove dados desnecessários da llinha e salva no vetor
                            llinha2.remove("<synopsis><![CDATA[");
                            if(!llinha2.contains("</synopsis>"))
                                lmaisSinopse = true;
                            else
                                llinha2.remove("]]></synopsis>");
                            //Remove espaços inúteis
                            llinha2 = llinha2.simplified();
                            //Salva nome do anime
                            lsinopse = llinha2;
                        }
                        else if(lmaisSinopse == true){
                            if(llinha2.contains("</synopsis>")){
                                lmaisSinopse = false;
                                llinha2.remove("]]></synopsis>");
                            }
                            lsinopse.append("\n");
                            lsinopse.append(llinha2);
                        }
                        else if(llinha2.contains("</anime>")){
                            //Para de ler o arquivo, já não temos mais nada o que fazer aqui
                            lAnimeId = false;
                            break;
                        }
                    }
                }
                //Após ler tudo, começa a escrever os dados no arquivo
                if(larquivo1Lido == true){
//                    qDebug() << lid << " " << llista;
                    QPointer<anime> lnovoAnime(new anime);
                    lnovoAnime->vnome = lnome;
                    lnovoAnime->vnomeIngles = lnomeIngles;
                    if(!lnumEpisodiosTotais.isEmpty())
                        lnovoAnime->vnumEpisodiosTotais = lnumEpisodiosTotais;
                    else
                        lnovoAnime->vnumEpisodiosTotais = "?";
                    lnovoAnime->vnumEpisodiosAssistidos = lnumEpisodiosAssistidos;
                    if(!lnotaMediaSite.isEmpty())
                        lnovoAnime->vnotaMediaSite = lnotaMediaSite;
                    else
                        lnovoAnime->vnotaMediaSite = "-";
                    lnovoAnime->vLinkImagemMedia = lLinkImagemMedia;
                    lnovoAnime->vnotaMediaPessoal = lnotaMediaPessoal;
                    if(lStarted == true && lFinished == true)
                        lnovoAnime->vstatus = "Finished Airing";
                    else if(lStarted == true)
                        lnovoAnime->vstatus = "Ongoing";
                    else
                        lnovoAnime->vstatus = "Not aired yet";
                    lnovoAnime->vsinopse = lsinopse;
                    lnovoAnime->vid = lid;
                    lnovoAnime->vseason = lseason;
                    lnovoAnime->vdataEstreia = lestreia;
                    if(llista == "1"){
                        lnovoAnime->vlista = "watching";
                        vlistaWatching.append(lnovoAnime);
                    }
                    else if(llista == "2"){
                        lnovoAnime->vlista = "completed";
                        vlistaCompleted.append(lnovoAnime);
                    }
                    else if(llista == "3"){
                        lnovoAnime->vlista = "onhold";
                        vlistaOnHold.append(lnovoAnime);
                    }
                    else if(llista == "4"){
                        lnovoAnime->vlista = "dropped";
                        vlistaDropped.append(lnovoAnime);
                    }
                    else if(llista == "5"){
                        lnovoAnime->vlista = "plantowatch";
                        vlistaPlanToWatch.append(lnovoAnime);
                    }
                }
                larquivo1Lido = false;
            }
            lFinished = false;
            lStarted = false;
        }
        //Após terminar de ler e escrever os arquivos, começa a salvar e fechar
        lerDB.close();
        lerUser.close();
    }
    return true;
}

void leitorlistaanimes::fdeletaListaAnimes(){
    qDeleteAll(vlistaWatching.begin(),vlistaWatching.end()); //Deletar todas as listas
    vlistaWatching.clear();
    qDeleteAll(vlistaCompleted.begin(),vlistaCompleted.end()); //Deletar todas as listas
    vlistaCompleted.clear();
    qDeleteAll(vlistaDropped.begin(),vlistaDropped.end()); //Deletar todas as listas
    vlistaDropped.clear();
    qDeleteAll(vlistaOnHold.begin(),vlistaOnHold.end()); //Deletar todas as listas
    vlistaOnHold.clear();
    qDeleteAll(vlistaPlanToWatch.begin(),vlistaPlanToWatch.end()); //Deletar todas as listas
    vlistaPlanToWatch.clear();
    qDeleteAll(vlistaBusca.begin(),vlistaBusca.end()); //Deletar todas as listas
    vlistaBusca.clear();
}

QVector<anime *> leitorlistaanimes::sortLista(QString rordem, QString rlista){
    QVector<anime*> llistaTemp;
    if(rlista == "watching")
        llistaTemp = vlistaWatching;
    else if(rlista == "completed")
        llistaTemp = vlistaCompleted;
    else if(rlista == "onhold")
        llistaTemp = vlistaOnHold;
    else if(rlista == "dropped")
        llistaTemp = vlistaDropped;
    else if(rlista == "plantowatch")
        llistaTemp = vlistaPlanToWatch;
    else if(rlista == "busca")
        llistaTemp = vlistaBusca;
    if(rordem != "")
    {
        for(int i = 0; i < llistaTemp.size(); i++){
            for(int w = i+1; w < llistaTemp.size(); w++){
                if(rordem[0] == "c"){
                    if(rordem == "cnome"){
                        if(llistaTemp[i]->vnome > llistaTemp[w]->vnome){
                            llistaTemp.move(i,w);
                            llistaTemp.move(w-1,i);
                        }
                    }
                    else if(rordem == "cdata"){
                        if(llistaTemp[i]->vdataEstreia > llistaTemp[w]->vdataEstreia){
                            llistaTemp.move(i,w);
                            llistaTemp.move(w-1,i);
                        }
                    }
                    else if(rordem == "cprogresso"){
                        if(llistaTemp[i]->vnumEpisodiosAssistidos > llistaTemp[w]->vnumEpisodiosAssistidos){
                            llistaTemp.move(i,w);
                            llistaTemp.move(w-1,i);
                        }
                    }
                    else if(rordem == "cnota"){
                        if(llistaTemp[i]->vnotaMediaPessoal > llistaTemp[w]->vnotaMediaPessoal){
                            llistaTemp.move(i,w);
                            llistaTemp.move(w-1,i);
                        }
                    }
                }
                else{
                    if(rordem == "dnome"){
                        if(llistaTemp[i]->vnome < llistaTemp[w]->vnome){
                            llistaTemp.move(i,w);
                            llistaTemp.move(w-1,i);
                        }
                    }
                    else if(rordem == "ddata"){
                        if(llistaTemp[i]->vdataEstreia < llistaTemp[w]->vdataEstreia){
                            llistaTemp.move(i,w);
                            llistaTemp.move(w-1,i);
                        }
                    }
                    else if(rordem == "dprogresso"){
                        if(llistaTemp[i]->vnumEpisodiosAssistidos < llistaTemp[w]->vnumEpisodiosAssistidos){
                            llistaTemp.move(i,w);
                            llistaTemp.move(w-1,i);
                        }
                    }
                    else if(rordem == "dnota"){
                        if(llistaTemp[i]->vnotaMediaPessoal < llistaTemp[w]->vnotaMediaPessoal){
                            llistaTemp.move(i,w);
                            llistaTemp.move(w-1,i);
                        }
                    }
                }
            }
        }
    }
    return llistaTemp;
}

QVector<anime *> leitorlistaanimes::fbuscaLista(QString rnome){
    qDeleteAll(vlistaBusca.begin(),vlistaBusca.end()); //Deletar todas as listas
    vlistaBusca.clear();
    for(int i = 0; i < vlistaWatching.size(); i++){
        if(vlistaWatching[i]->vnome.contains(rnome, Qt::CaseInsensitive) == true ||
                vlistaWatching[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive) == true){
            vlistaBusca.append(vlistaWatching[i]);
        }
        else if(vlistaWatching[i]->vnomeAlternativo.size() != 0){
            for(int w = 0; w < vlistaWatching[i]->vnomeAlternativo.size(); w++){
                if(vlistaWatching[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive))
                    vlistaBusca.append(vlistaWatching[i]);
            }
        }
    }
    for(int i = 0; i < vlistaCompleted.size(); i++){
        if(vlistaCompleted[i]->vnome.contains(rnome, Qt::CaseInsensitive) == true ||
                vlistaCompleted[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive) == true)
            vlistaBusca.append(vlistaCompleted[i]);
        else if(vlistaCompleted[i]->vnomeAlternativo.size() != 0){
            for(int w = 0; w < vlistaCompleted[i]->vnomeAlternativo.size(); w++){
                if(vlistaCompleted[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive))
                    vlistaBusca.append(vlistaCompleted[i]);
            }
        }
    }
    for(int i = 0; i < vlistaOnHold.size(); i++){
        if(vlistaOnHold[i]->vnome.contains(rnome, Qt::CaseInsensitive) == true ||
                vlistaOnHold[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive) == true)
            vlistaBusca.append(vlistaOnHold[i]);
        else if(vlistaOnHold[i]->vnomeAlternativo.size() != 0){
            for(int w = 0; w < vlistaOnHold[i]->vnomeAlternativo.size(); w++){
                if(vlistaOnHold[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive))
                    vlistaBusca.append(vlistaOnHold[i]);
            }
        }
    }
    for(int i = 0; i < vlistaDropped.size(); i++){
        if(vlistaDropped[i]->vnome.contains(rnome, Qt::CaseInsensitive) == true ||
                vlistaDropped[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive) == true)
            vlistaBusca.append(vlistaDropped[i]);
        else if(vlistaDropped[i]->vnomeAlternativo.size() != 0){
            for(int w = 0; w < vlistaDropped[i]->vnomeAlternativo.size(); w++){
                if(vlistaDropped[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive))
                    vlistaBusca.append(vlistaDropped[i]);
            }
        }
    }
    for(int i = 0; i < vlistaPlanToWatch.size(); i++){
        if(vlistaPlanToWatch[i]->vnome.contains(rnome, Qt::CaseInsensitive) == true ||
                vlistaPlanToWatch[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive) == true)
            vlistaBusca.append(vlistaPlanToWatch[i]);
        else if(vlistaPlanToWatch[i]->vnomeAlternativo.size() != 0){
            for(int w = 0; w < vlistaPlanToWatch[i]->vnomeAlternativo.size(); w++){
                if(vlistaPlanToWatch[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive))
                    vlistaBusca.append(vlistaPlanToWatch[i]);
            }
        }
    }
    return vlistaBusca;
}

QVector<anime*> leitorlistaanimes::retornaListaWatching()
{
    return vlistaWatching;
}

QVector<anime *> leitorlistaanimes::retornaListaCompleted()
{
    return vlistaCompleted;
}

QVector<anime *> leitorlistaanimes::retornaListaOnHold()
{
    return vlistaOnHold;
}

QVector<anime *> leitorlistaanimes::retornaListaDropped()
{
    return vlistaDropped;
}

QVector<anime *> leitorlistaanimes::retornaListaPlanToWatch()
{
    return vlistaPlanToWatch;
}
