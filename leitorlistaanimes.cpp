#include "leitorlistaanimes.h"

leitorlistaanimes::leitorlistaanimes(QObject *parent) : QObject(parent)
{

}

leitorlistaanimes::~leitorlistaanimes(){
//    fdeletaListaAnimes();
    qDeleteAll(vlistaWatching.begin(),vlistaWatching.end()); //Deletar todas as listas
    qDeleteAll(vlistaCompleted.begin(),vlistaCompleted.end()); //Deletar todas as listas
    qDeleteAll(vlistaDropped.begin(),vlistaDropped.end()); //Deletar todas as listas
    qDeleteAll(vlistaOnHold.begin(),vlistaOnHold.end()); //Deletar todas as listas
    qDeleteAll(vlistaPlanToWatch.begin(),vlistaPlanToWatch.end()); //Deletar todas as listas
    qDeleteAll(vlistaBusca.begin(),vlistaBusca.end()); //Deletar todas as listas
}

bool leitorlistaanimes::fleJson(){
//    fdeletaListaAnimes();
    QFile lleJson("Configurações/Temp/animeList.txt");
    if(lleJson.size() == 0)
        return false;
    QTextStream json(&lleJson);
    json.setCodec("UTF-8");
    QString lid;
    QString lnome;
    QString lnomeIngles;
    QString lnomeJapones;
    QStringList lnomeAlternativo; //StringList por que pode ter lários nomes alternativos
    QString lnumEpisodiosTotais;
    QString lvnumEpisodiosAssistidos;
    QString lnotaMediaSite;
    QString lLinkImagemMedia;
    QString lnotaMediaPessoal;
    QString lsinopse;
    QString lseason;
    QString lstatus;
    QString llista;
    QString lformato;
    QString lstreamCrunchyroll;
    QString lsiteAnilist;
    QString lproximoEpisodio;
    QString lano;
    QString lmes;
    QString lchapters;
    QString lvolumes;
    QString ldataEpisodioFinal;
    int ltemporadaAnime = 1;
    QDateTime ldataEpisodio;
    QTime lhoraLancamentoEpisodio;
    QDate ldataEpisodioConvertida;
    QDate ldataEstreia;///Data de estréia do próximo episódio

    if(lleJson.open(QIODevice::ReadOnly)){
        while(!json.atEnd()){
            QString llinha = json.readLine();
            if(llinha.contains("null")){
                llinha.replace("null", "?");
                llinha.replace("null,", "?");
            }
            //Procura pelo primeiro dado do anime, a capa
            if(llinha.contains("\"large\":")){
                llinha.remove("\",");
                llinha.remove("\"");
                llinha.remove("large: ");
                lLinkImagemMedia = llinha.trimmed();
            }
            else if(llinha.contains("\"description\":")){
                llinha.remove("\",");
                llinha.remove("\\n");
                llinha.remove("\\r");
                llinha.remove("\\");
                llinha.remove("\"description\": \"");
                llinha = llinha.trimmed();
                lsinopse = llinha.toUtf8();
            }
            else if(llinha.contains("\"episodes\":")){
                llinha.remove(",");
                llinha.remove("\"");
                llinha.remove("episodes: ");
                lnumEpisodiosTotais = llinha.trimmed();
            }
            else if(llinha.contains("\"format\":")){
                llinha.remove("\",");
                llinha.remove("\"");
                llinha.replace("_", " ");
                llinha.remove("format: ");
                lformato = llinha.trimmed();
            }
            else if(llinha.contains("\"id\":")){
                llinha.remove(",");
                llinha.remove("\"");
                llinha.remove("id: ");
                lid = llinha.trimmed();
            }
            else if(llinha.contains("\"url\":")){
                if(llinha.contains("crunchyroll")){
                    llinha.remove(",");
                    llinha.remove("\"");
                    llinha.remove("url: ");
                    if(llinha.contains("?"))
                        llinha.replace("?", "null");
                    lstreamCrunchyroll = llinha.left(llinha.lastIndexOf("episode-")).trimmed();
                }
            }
            else if(llinha.contains("\"chapters\":")){
                llinha.remove(",");
                llinha.remove("\"");
                llinha.remove("chapters: ");
                lchapters = llinha.trimmed();
            }
            else if(llinha.contains("\"volumes\":")){
                llinha.remove(",");
                llinha.remove("\"");
                llinha.remove("volumes: ");
                lvolumes = llinha.trimmed();
            }
            else if(llinha.contains("\"siteUrl\":")){
                llinha.remove(",");
                llinha.remove("\"");
                llinha.remove("siteUrl: ");
                lsiteAnilist = llinha.trimmed();
            }
            else if(llinha.contains("\"nextAiringEpisode\":")){
                if(llinha.contains("?"))
                    ldataEpisodioFinal = "Not      Airing";
            }
            else if(llinha.contains("\"airingAt\":")){
                llinha.remove(",");
                llinha.remove("\"");
                llinha.remove("airingAt: ");
                llinha = llinha.trimmed();
                ldataEpisodio.setSecsSinceEpoch(llinha.toInt());
                ldataEpisodioConvertida = ldataEpisodio.date();
                lhoraLancamentoEpisodio = ldataEpisodio.time();
                if(ldataEpisodioConvertida.dayOfWeek() == 1)
                    ldataEpisodioFinal = "Monday ";
                else if(ldataEpisodioConvertida.dayOfWeek() == 2)
                    ldataEpisodioFinal = "Tuesday ";
                else if(ldataEpisodioConvertida.dayOfWeek() == 3)
                    ldataEpisodioFinal = "Wednesday ";
                else if(ldataEpisodioConvertida.dayOfWeek() == 4)
                    ldataEpisodioFinal = "Thursday ";
                else if(ldataEpisodioConvertida.dayOfWeek() == 5)
                    ldataEpisodioFinal = "Friday  ";
                else if(ldataEpisodioConvertida.dayOfWeek() == 6)
                    ldataEpisodioFinal = "Saturday ";
                else if(ldataEpisodioConvertida.dayOfWeek() == 7)
                    ldataEpisodioFinal = "Sunday ";
                ldataEpisodioFinal.append(lhoraLancamentoEpisodio.toString("hh:mm"));
            }
            else if(llinha.contains("\"episode\":")){
                llinha.remove("\",");
                llinha.remove("\"");
                llinha.remove("episode: ");
                lproximoEpisodio = llinha.trimmed();
            }
            else if(llinha.contains("\"season\":")){
                llinha.remove("\",");
                llinha.remove("\"");
                llinha.remove("season: ");
                llinha = llinha.trimmed();
                llinha = llinha.toLower();
                llinha[0] = llinha.at(0).toUpper();
                if(llinha.contains("?"))
                    lseason = "-";
                else
                    lseason = llinha;
            }
            else if(llinha.contains("\"month\":")){
                llinha.remove(",");
                llinha.remove("\"");
                llinha.remove("month: ");
                lmes = llinha.trimmed();
            }
            else if(llinha.contains("\"year\":")){
                llinha.remove("\",");
                llinha.remove("\"");
                llinha.remove("year: ");
                if(lseason == "-")
                    lano = "";
                else
                    lano = llinha.trimmed();
            }
            else if(llinha.contains("\"status\":")){
                llinha.remove("\",");
                llinha.remove("\"");
                llinha.remove("status: ");
                llinha = llinha.trimmed();
                if(llinha == "FINISHED")
                    lstatus = "Finished Airing";
                else if(llinha == "RELEASING")
                    lstatus = "Ongoing";
                else
                    lstatus = "Not Aired Yet";
            }
            else if(llinha.contains("\"synonyms\":")){
                llinha = json.readLine();
                while(!llinha.contains("],")){
                    llinha.remove("\",");
                    llinha.remove("\"");
                    lnomeAlternativo.append(llinha.trimmed());
                    llinha = json.readLine();
                }
            }
            else if(llinha.contains("\"english\":")){
                llinha.remove(",");
                llinha.remove("\"");
                llinha.remove("english:");
                lnomeIngles = llinha.trimmed();
            }
            else if(llinha.contains("\"romaji\":")){
                llinha.remove(",");
                llinha.remove("\"");
                llinha.remove("romaji:");
                lnome = llinha.trimmed();
                if(lnome.endsWith("2"))
                    ltemporadaAnime = 2;
                else if(lnome.endsWith("3"))
                    ltemporadaAnime = 3;
                else if(lnome.endsWith("4"))
                    ltemporadaAnime = 4;
                else
                    ltemporadaAnime = 1;

            }
            else if(llinha.contains("\"progress\":")){
                llinha.remove(",");
                llinha.remove("\"");
                llinha.remove("progress: ");
                lvnumEpisodiosAssistidos = llinha.trimmed();
            }
            else if(llinha.contains("\"averageScore\":")){
                llinha.remove(",");
                llinha.remove("\"");
                llinha.remove("averageScore: ");
                lnotaMediaSite = llinha.trimmed();
            }
            else if(llinha.contains("\"score\":")){
                llinha.remove(",");
                llinha.remove("\"");
                llinha.remove("score: ");
                lnotaMediaPessoal = llinha.trimmed();
                llinha = json.readLine();
                llinha.remove("\",");
                llinha.remove("\"");
                llinha.remove("status: ");
                llista = llinha.trimmed();
//                QPointer<anime> lnovoAnime(new anime);
                anime* lnovoAnime = new anime;
                if(lnome == "?")
                    lnome = lnomeIngles;
                else if (lnomeIngles == "?")
                    lnomeIngles = lnome;
                lnovoAnime->vnome = lnome;
                lnovoAnime->vnomeIngles = lnomeIngles;
                if(!lnomeAlternativo.isEmpty()){
                    lnovoAnime->vnomeAlternativo = lnomeAlternativo;
                    lnomeAlternativo.empty();
                }
                lnovoAnime->vnumEpisodiosTotais = lnumEpisodiosTotais;
                if(!lnotaMediaSite.isEmpty())
                    lnovoAnime->vnotaMediaSite = lnotaMediaSite;
                else
                    lnovoAnime->vnotaMediaSite = "-";
                if(lano != "?" && lmes != "?"){
                    ldataEstreia.setDate(lano.toInt(), lmes.toInt(), 1);
                }
                else if(lmes != "?"){
                    if(lseason == "Winter")
                        lmes = "3";
                    else if(lseason == "Spring")
                        lmes = "6";
                    else if(lseason == "Summer")
                        lmes = "9";
                    else
                        lmes = "12";
                    if(lmes == "?")
                        lmes = "12";
                    ldataEstreia.setDate(lano.toInt(), lmes.toInt(), 31);
                }
                else if(lano != "?"){
                    if(lmes == "?")
                        lmes = "12";
                    ldataEstreia.setDate(2050, lmes.toInt(), 31);
                }
                lnovoAnime->vLinkImagemMedia = lLinkImagemMedia;
                lnovoAnime->vnotaMediaPessoal = lnotaMediaPessoal;
                lnovoAnime->vstatus = lstatus;
                lnovoAnime->vsinopse = lsinopse;
                lnovoAnime->vid = lid;
                lnovoAnime->vseason = lseason + " " + lano;
                lnovoAnime->vnumProximoEpisodioLancado = lproximoEpisodio;
                lnovoAnime->vnumEpisodiosAssistidos = lvnumEpisodiosAssistidos;
                lnovoAnime->vformato = lformato;
                lnovoAnime->vdataEstreia = ldataEstreia;
                lnovoAnime->vdataEpisodio = ldataEpisodioFinal;
                lnovoAnime->vsiteAnilist = lsiteAnilist;
                lnovoAnime->vstreamCrunchyroll = lstreamCrunchyroll;
                lnovoAnime->vtemporada = ltemporadaAnime;
                if(lformato != "MANGA" && lformato != "NOVEL" && lformato != "ONE SHOT"){
                    if(llinha.trimmed() == "CURRENT"){
                        lnovoAnime->vlista = "Watching";
                        vlistaWatching.append(lnovoAnime);
                    }
                    else if(llinha.trimmed() == "COMPLETED"){
                        lnovoAnime->vlista = "Completed";
                        vlistaCompleted.append(lnovoAnime);
                    }
                    else if(llinha.trimmed() == "PLANNING"){
                        lnovoAnime->vlista = "Plan to Watch";
                        vlistaPlanToWatch.append(lnovoAnime);
                    }
                    else if(llinha.trimmed() == "DROPPED"){
                        lnovoAnime->vlista = "Dropped";
                        vlistaDropped.append(lnovoAnime);
                    }
                    else if(llinha.trimmed() == "PAUSED"){
                        lnovoAnime->vlista = "On Hold";
                        vlistaOnHold.append(lnovoAnime);
                    }
                    sAnimeAdicionadoNaLista(lnovoAnime->vid);
                }
                else if(lformato == "MANGA" || lformato == "ONE SHOT"){
                    if(lnovoAnime->vstatus.contains("Air", Qt::CaseInsensitive))
                        lnovoAnime->vstatus.replace("Air","Releas");
                    lnovoAnime->vnumEpisodiosTotais = lchapters;
                    if(llinha.trimmed() == "CURRENT"){
                        lnovoAnime->vlista = "Reading(Manga)";
                        vlistaMangaReading.append(lnovoAnime);
                    }
                    else if(llinha.trimmed() == "COMPLETED"){
                        lnovoAnime->vlista = "Completed(Manga)";
                        vlistaMangaCompleted.append(lnovoAnime);
                    }
                    else if(llinha.trimmed() == "PLANNING"){
                        lnovoAnime->vlista = "Plan to Read(Manga)";
                        vlistaMangaPlanToRead.append(lnovoAnime);
                    }
                    else if(llinha.trimmed() == "DROPPED"){
                        lnovoAnime->vlista = "Dropped(Manga)";
                        vlistaMangaDropped.append(lnovoAnime);
                    }
                    else if(llinha.trimmed() == "PAUSED"){
                        lnovoAnime->vlista = "On Hold(Manga)";
                        vlistaMangaOnHold.append(lnovoAnime);
                    }
                    sMangaAdicionadoNaLista(lnovoAnime->vid);
                }
                else if(lformato == "NOVEL"){
                    lnovoAnime->vnumEpisodiosTotais = lvolumes;
                    if(llinha.trimmed() == "CURRENT"){
                        lnovoAnime->vlista = "Reading(Novel)";
                        vlistaNovelReading.append(lnovoAnime);
                    }
                    else if(llinha.trimmed() == "COMPLETED"){
                        lnovoAnime->vlista = "Completed(Novel)";
                        vlistaNovelCompleted.append(lnovoAnime);
                    }
                    else if(llinha.trimmed() == "PLANNING"){
                        lnovoAnime->vlista = "Plan to Read(Novel)";
                        vlistaNovelPlanToRead.append(lnovoAnime);
                    }
                    else if(llinha.trimmed() == "DROPPED"){
                        lnovoAnime->vlista = "Dropped(Novel)";
                        vlistaNovelDropped.append(lnovoAnime);
                    }
                    else if(llinha.trimmed() == "PAUSED"){
                        lnovoAnime->vlista = "On Hold(Novel)";
                        vlistaNovelOnHold.append(lnovoAnime);
                    }
                    sNovelAdicionadoNaLista(lnovoAnime->vid);
                }
                lnome.clear();
                lnomeIngles.clear();
                lnomeAlternativo.clear();
                lnumEpisodiosTotais.clear();
                lnotaMediaSite.clear();
                lLinkImagemMedia.clear();
                lnotaMediaPessoal.clear();
                lstatus.clear();
                lsinopse.clear();
                lid.clear();
                lseason.clear();
                lproximoEpisodio.clear();
                lvnumEpisodiosAssistidos.clear();
                lformato.clear();
                lformato.clear();
                lstreamCrunchyroll.clear();
                lsiteAnilist.clear();
            }
        }
        lleJson.close();
    }
    //Caso o arquivo não possa ser lido, retorna false
    else
        return false;
    return true;
}


void leitorlistaanimes::fdeletaListaAnimes(){
    if(!vlistaWatching.isEmpty()){
        qDeleteAll(vlistaWatching.begin(),vlistaWatching.end()); //Deletar todas as listas
        vlistaWatching.clear();
    }
    if(!vlistaCompleted.isEmpty()){
        qDeleteAll(vlistaCompleted.begin(),vlistaCompleted.end()); //Deletar todas as listas
        vlistaCompleted.clear();
    }
    if(!vlistaDropped.isEmpty()){
        qDeleteAll(vlistaDropped.begin(),vlistaDropped.end()); //Deletar todas as listas
        vlistaDropped.clear();
    }
    if(!vlistaOnHold.isEmpty()){
        qDeleteAll(vlistaOnHold.begin(),vlistaOnHold.end()); //Deletar todas as listas
        vlistaOnHold.clear();
    }
    if(!vlistaPlanToWatch.isEmpty()){
        qDeleteAll(vlistaPlanToWatch.begin(),vlistaPlanToWatch.end()); //Deletar todas as listas
        vlistaPlanToWatch.clear();
    }
    if(!vlistaBusca.isEmpty()){
        qDeleteAll(vlistaBusca.begin(),vlistaBusca.end()); //Deletar todas as listas
        vlistaBusca.clear();
    }
    if(!vlistaMangaReading.isEmpty()){
        qDeleteAll(vlistaMangaReading.begin(),vlistaMangaReading.end()); //Deletar todas as listas
        vlistaMangaReading.clear();
    }
    if(!vlistaMangaCompleted.isEmpty()){
        qDeleteAll(vlistaMangaCompleted.begin(),vlistaMangaCompleted.end()); //Deletar todas as listas
        vlistaMangaCompleted.clear();
    }
    if(!vlistaMangaDropped.isEmpty()){
        qDeleteAll(vlistaMangaDropped.begin(),vlistaMangaDropped.end()); //Deletar todas as listas
        vlistaMangaDropped.clear();
    }
    if(!vlistaMangaOnHold.isEmpty()){
        qDeleteAll(vlistaMangaOnHold.begin(),vlistaMangaOnHold.end()); //Deletar todas as listas
        vlistaMangaOnHold.clear();
    }
    if(!vlistaMangaPlanToRead.isEmpty()){
        qDeleteAll(vlistaMangaPlanToRead.begin(),vlistaMangaPlanToRead.end()); //Deletar todas as listas
        vlistaMangaPlanToRead.clear();
    }
    if(!vlistaNovelReading.isEmpty()){
        qDeleteAll(vlistaNovelReading.begin(),vlistaNovelReading.end()); //Deletar todas as listas
        vlistaNovelReading.clear();
    }
    if(!vlistaNovelCompleted.isEmpty()){
        qDeleteAll(vlistaNovelCompleted.begin(),vlistaNovelCompleted.end()); //Deletar todas as listas
        vlistaNovelCompleted.clear();
    }
    if(!vlistaNovelDropped.isEmpty()){
        qDeleteAll(vlistaNovelDropped.begin(),vlistaNovelDropped.end()); //Deletar todas as listas
        vlistaNovelDropped.clear();
    }
    if(!vlistaNovelOnHold.isEmpty()){
        qDeleteAll(vlistaNovelOnHold.begin(),vlistaNovelOnHold.end()); //Deletar todas as listas
        vlistaNovelOnHold.clear();
    }
    if(!vlistaNovelPlanToRead.isEmpty()){
        qDeleteAll(vlistaNovelPlanToRead.begin(),vlistaNovelPlanToRead.end()); //Deletar todas as listas
        vlistaNovelPlanToRead.clear();
    }
}

bool leitorlistaanimes::fdeletedaLista(QString rid, QString rlista)
{
    if(rlista == "Watching"){
        for(int i = 0; i < vlistaWatching.size(); i++){
            if(vlistaWatching[i]->vid == rid){
                vlistaWatching.remove(i);
                return true;
            }
        }
    }
    else if(rlista == "Completed"){
        for(int i = 0; i < vlistaCompleted.size(); i++){
            if(vlistaCompleted[i]->vid == rid){
                vlistaCompleted.remove(i);
                return true;
            }
        }
    }
    else if(rlista == "On Hold"){
        for(int i = 0; i < vlistaOnHold.size(); i++){
            if(vlistaOnHold[i]->vid == rid){
                vlistaOnHold.remove(i);
                return true;
            }
        }
    }
    else if(rlista == "Dropped"){
        for(int i = 0; i < vlistaDropped.size(); i++){
            if(vlistaDropped[i]->vid == rid){
                vlistaDropped.remove(i);
                return true;
            }
        }
    }
    else if(rlista == "Plan to Watch"){
        for(int i = 0; i < vlistaPlanToWatch.size(); i++){
            if(vlistaPlanToWatch[i]->vid == rid){
                vlistaPlanToWatch.remove(i);
                return true;
            }
        }
    }
    else if(rlista == "Reading(Manga)"){
        for(int i = 0; i < vlistaMangaReading.size(); i++){
            if(vlistaMangaReading[i]->vid == rid){
                vlistaMangaReading.remove(i);
                return true;
            }
        }
    }
    else if(rlista == "Completed(Manga)"){
        for(int i = 0; i < vlistaMangaCompleted.size(); i++){
            if(vlistaMangaCompleted[i]->vid == rid){
                vlistaMangaCompleted.remove(i);
                return true;
            }
        }
    }
    else if(rlista == "On Hold(Manga)"){
        for(int i = 0; i < vlistaMangaOnHold.size(); i++){
            if(vlistaMangaOnHold[i]->vid == rid){
                vlistaMangaOnHold.remove(i);
                return true;
            }
        }
    }
    else if(rlista == "Dropped(Manga)"){
        for(int i = 0; i < vlistaMangaDropped.size(); i++){
            if(vlistaMangaDropped[i]->vid == rid){
                vlistaMangaDropped.remove(i);
                return true;
            }
        }
    }
    else if(rlista == "Plan to Read(Manga)"){
        for(int i = 0; i < vlistaMangaPlanToRead.size(); i++){
            if(vlistaMangaPlanToRead[i]->vid == rid){
                vlistaMangaPlanToRead.remove(i);
                return true;
            }
        }
    }
    else if(rlista == "Reading(Novel)"){
        for(int i = 0; i < vlistaNovelReading.size(); i++){
            if(vlistaNovelReading[i]->vid == rid){
                vlistaNovelReading.remove(i);
                return true;
            }
        }
    }
    else if(rlista == "Completed(Novel)"){
        for(int i = 0; i < vlistaNovelCompleted.size(); i++){
            if(vlistaNovelCompleted[i]->vid == rid){
                vlistaNovelCompleted.remove(i);
                return true;
            }
        }
    }
    else if(rlista == "On Hold(Novel)"){
        for(int i = 0; i < vlistaNovelOnHold.size(); i++){
            if(vlistaNovelOnHold[i]->vid == rid){
                vlistaNovelOnHold.remove(i);
                return true;
            }
        }
    }
    else if(rlista == "Dropped(Novel)"){
        for(int i = 0; i < vlistaNovelDropped.size(); i++){
            if(vlistaNovelDropped[i]->vid == rid){
                vlistaNovelDropped.remove(i);
                return true;
            }
        }
    }
    else if(rlista == "Plan to Watch(Novel)"){
        for(int i = 0; i < vlistaNovelPlanToRead.size(); i++){
            if(vlistaNovelPlanToRead[i]->vid == rid){
                vlistaNovelPlanToRead.remove(i);
                return true;
            }
        }
    }
    return false;

}

QVector<anime *> leitorlistaanimes::sortLista(QString rordem, QString rlista){
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
    else if(rlista == "mangareading")
        llistaTemp = vlistaMangaReading;
    else if(rlista == "mangacompleted")
        llistaTemp = vlistaMangaCompleted;
    else if(rlista == "mangaonhold")
        llistaTemp = vlistaMangaOnHold;
    else if(rlista == "mangadropped")
        llistaTemp = vlistaMangaDropped;
    else if(rlista == "mangaplantoread")
        llistaTemp = vlistaMangaPlanToRead;
    else if(rlista == "novelreading")
        llistaTemp = vlistaNovelReading;
    else if(rlista == "novelcompleted")
        llistaTemp = vlistaNovelCompleted;
    else if(rlista == "novelonhold")
        llistaTemp = vlistaNovelOnHold;
    else if(rlista == "noveldropped")
        llistaTemp = vlistaNovelDropped;
    else if(rlista == "novelplantoread")
        llistaTemp = vlistaNovelPlanToRead;
    else
        llistaTemp.clear();


    if(!rordem.isEmpty())
    {
        if(rordem.startsWith("c")){
            if(rordem.contains("nome", Qt::CaseInsensitive)){
                std::sort(llistaTemp.begin(),llistaTemp.end(),[](anime* a, anime* b)->bool{
                    return a->vnome < b->vnome ;});
            }
            else if(rordem.contains("data", Qt::CaseInsensitive)){
                std::sort(llistaTemp.begin(),llistaTemp.end(),[](anime* a, anime* b)->bool{
                    return a->vdataEstreia < b->vdataEstreia ;});
            }
            else if(rordem.contains("progresso", Qt::CaseInsensitive)){
                std::sort(llistaTemp.begin(),llistaTemp.end(),[](anime* a, anime* b)->bool{return
                            a->vnumEpisodiosAssistidos.toInt()
                            < b->vnumEpisodiosAssistidos.toInt() ;});
            }
            else if(rordem.contains("nota", Qt::CaseInsensitive)){
                std::sort(llistaTemp.begin(),llistaTemp.end(),[](anime* a, anime* b)->bool{
                    return a->vnotaMediaPessoal.toInt()
                            < b->vnotaMediaPessoal.toInt() ;});
            }
            else if(rordem.contains("formato", Qt::CaseInsensitive)){
                std::sort(llistaTemp.begin(),llistaTemp.end(),[](anime* a, anime* b)->bool{
                    return a->vformato < b->vformato ;});
            }
        }
        else{
            if(rordem.contains("nome", Qt::CaseInsensitive)){
                std::sort(llistaTemp.begin(),llistaTemp.end(),[](anime* a, anime* b)->bool{
                    return a->vnome > b->vnome ;});
            }
            else if(rordem.contains("data", Qt::CaseInsensitive)){
                std::sort(llistaTemp.begin(),llistaTemp.end(),[](anime* a, anime* b)->bool{
                    return a->vdataEstreia > b->vdataEstreia ;});
            }
            else if(rordem.contains("progresso", Qt::CaseInsensitive)){
                std::sort(llistaTemp.begin(),llistaTemp.end(),[](anime* a, anime* b)->bool{return
                            static_cast<float>(a->vnumEpisodiosAssistidos.toInt())
                            > static_cast<float>(b->vnumEpisodiosAssistidos.toInt()) ;});
            }
            else if(rordem.contains("nota", Qt::CaseInsensitive)){
                std::sort(llistaTemp.begin(),llistaTemp.end(),[](anime* a, anime* b)->bool{
                    return a->vnotaMediaPessoal.toInt()
                            > b->vnotaMediaPessoal.toInt() ;});
            }
            else if(rordem.contains("formato", Qt::CaseInsensitive)){
                std::sort(llistaTemp.begin(),llistaTemp.end(),[](anime* a, anime* b)->bool{
                    return a->vformato > b->vformato ;});
            }
        }
    }
    return llistaTemp;
}

QVector<anime *> leitorlistaanimes::fbuscaLista(QString rnome, QString rtipoMidia){
    vlistaBusca.clear();
    if(rtipoMidia.compare("anime", Qt::CaseInsensitive) == 0){
        for(int i = 0; i < vlistaWatching.size(); i++){
            if(vlistaWatching[i]->vnome.contains(rnome, Qt::CaseInsensitive) == true ||
                    vlistaWatching[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive) == true){
                vlistaBusca.append(vlistaWatching[i]);
            }
            else if(vlistaWatching[i]->vnomeAlternativo.size() != 0){
                for(int w = 0; w < vlistaWatching[i]->vnomeAlternativo.size(); w++){
                    if(vlistaWatching[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive)){
                        vlistaBusca.append(vlistaWatching[i]);
                        break;
                    }
                }
            }
        }
        for(int i = 0; i < vlistaCompleted.size(); i++){
            if(vlistaCompleted[i]->vnome.contains(rnome, Qt::CaseInsensitive) == true ||
                    vlistaCompleted[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive) == true)
                vlistaBusca.append(vlistaCompleted[i]);
            else if(vlistaCompleted[i]->vnomeAlternativo.size() != 0){
                for(int w = 0; w < vlistaCompleted[i]->vnomeAlternativo.size(); w++){
                    if(vlistaCompleted[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive)){
                        vlistaBusca.append(vlistaCompleted[i]);
                        break;
                    }
                }
            }
        }
        for(int i = 0; i < vlistaOnHold.size(); i++){
            if(vlistaOnHold[i]->vnome.contains(rnome, Qt::CaseInsensitive) == true ||
                    vlistaOnHold[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive) == true)
                vlistaBusca.append(vlistaOnHold[i]);
            else if(vlistaOnHold[i]->vnomeAlternativo.size() != 0){
                for(int w = 0; w < vlistaOnHold[i]->vnomeAlternativo.size(); w++){
                    if(vlistaOnHold[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive)){
                        vlistaBusca.append(vlistaOnHold[i]);
                        break;
                    }
                }
            }
        }
        for(int i = 0; i < vlistaDropped.size(); i++){
            if(vlistaDropped[i]->vnome.contains(rnome, Qt::CaseInsensitive) == true ||
                    vlistaDropped[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive) == true)
                vlistaBusca.append(vlistaDropped[i]);
            else if(vlistaDropped[i]->vnomeAlternativo.size() != 0){
                for(int w = 0; w < vlistaDropped[i]->vnomeAlternativo.size(); w++){
                    if(vlistaDropped[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive)){
                        vlistaBusca.append(vlistaDropped[i]);
                        break;
                    }
                }
            }
        }
        for(int i = 0; i < vlistaPlanToWatch.size(); i++){
            if(vlistaPlanToWatch[i]->vnome.contains(rnome, Qt::CaseInsensitive) == true ||
                    vlistaPlanToWatch[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive) == true)
                vlistaBusca.append(vlistaPlanToWatch[i]);
            else if(vlistaPlanToWatch[i]->vnomeAlternativo.size() != 0){
                for(int w = 0; w < vlistaPlanToWatch[i]->vnomeAlternativo.size(); w++){
                    if(vlistaPlanToWatch[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive)){
                        vlistaBusca.append(vlistaPlanToWatch[i]);
                        break;
                    }
                }
            }
        }
    }
    else if(rtipoMidia.compare("manga", Qt::CaseInsensitive) == 0){
        for(int i = 0; i < vlistaMangaReading.size(); i++){
            if(vlistaMangaReading[i]->vnome.contains(rnome, Qt::CaseInsensitive) == true ||
                    vlistaMangaReading[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive) == true){
                vlistaBusca.append(vlistaMangaReading[i]);
            }
            else if(vlistaMangaReading[i]->vnomeAlternativo.size() != 0){
                for(int w = 0; w < vlistaMangaReading[i]->vnomeAlternativo.size(); w++){
                    if(vlistaMangaReading[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive)){
                        vlistaBusca.append(vlistaMangaReading[i]);
                        break;
                    }
                }
            }
        }
        for(int i = 0; i < vlistaMangaCompleted.size(); i++){
            if(vlistaMangaCompleted[i]->vnome.contains(rnome, Qt::CaseInsensitive) == true ||
                    vlistaMangaCompleted[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive) == true)
                vlistaBusca.append(vlistaMangaCompleted[i]);
            else if(vlistaMangaCompleted[i]->vnomeAlternativo.size() != 0){
                for(int w = 0; w < vlistaMangaCompleted[i]->vnomeAlternativo.size(); w++){
                    if(vlistaMangaCompleted[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive)){
                        vlistaBusca.append(vlistaMangaCompleted[i]);
                        break;
                    }
                }
            }
        }
        for(int i = 0; i < vlistaMangaOnHold.size(); i++){
            if(vlistaMangaOnHold[i]->vnome.contains(rnome, Qt::CaseInsensitive) == true ||
                    vlistaMangaOnHold[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive) == true)
                vlistaBusca.append(vlistaMangaOnHold[i]);
            else if(vlistaMangaOnHold[i]->vnomeAlternativo.size() != 0){
                for(int w = 0; w < vlistaMangaOnHold[i]->vnomeAlternativo.size(); w++){
                    if(vlistaMangaOnHold[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive)){
                        vlistaBusca.append(vlistaMangaOnHold[i]);
                        break;
                    }
                }
            }
        }
        for(int i = 0; i < vlistaMangaDropped.size(); i++){
            if(vlistaMangaDropped[i]->vnome.contains(rnome, Qt::CaseInsensitive) == true ||
                    vlistaMangaDropped[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive) == true)
                vlistaBusca.append(vlistaMangaDropped[i]);
            else if(vlistaMangaDropped[i]->vnomeAlternativo.size() != 0){
                for(int w = 0; w < vlistaMangaDropped[i]->vnomeAlternativo.size(); w++){
                    if(vlistaMangaDropped[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive)){
                        vlistaBusca.append(vlistaMangaDropped[i]);
                        break;
                    }
                }
            }
        }
        for(int i = 0; i < vlistaMangaPlanToRead.size(); i++){
            if(vlistaMangaPlanToRead[i]->vnome.contains(rnome, Qt::CaseInsensitive) == true ||
                    vlistaMangaPlanToRead[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive) == true)
                vlistaBusca.append(vlistaMangaPlanToRead[i]);
            else if(vlistaMangaPlanToRead[i]->vnomeAlternativo.size() != 0){
                for(int w = 0; w < vlistaMangaPlanToRead[i]->vnomeAlternativo.size(); w++){
                    if(vlistaMangaPlanToRead[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive)){
                        vlistaBusca.append(vlistaMangaPlanToRead[i]);
                        break;
                    }
                }
            }
        }
    }
    else if(rtipoMidia.compare("novel", Qt::CaseInsensitive) == 0){
        for(int i = 0; i < vlistaNovelReading.size(); i++){
            if(vlistaNovelReading[i]->vnome.contains(rnome, Qt::CaseInsensitive) == true ||
                    vlistaNovelReading[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive) == true){
                vlistaBusca.append(vlistaNovelReading[i]);
            }
            else if(vlistaNovelReading[i]->vnomeAlternativo.size() != 0){
                for(int w = 0; w < vlistaNovelReading[i]->vnomeAlternativo.size(); w++){
                    if(vlistaNovelReading[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive)){
                        vlistaBusca.append(vlistaNovelReading[i]);
                        break;
                    }
                }
            }
        }
        for(int i = 0; i < vlistaNovelCompleted.size(); i++){
            if(vlistaNovelCompleted[i]->vnome.contains(rnome, Qt::CaseInsensitive) == true ||
                    vlistaNovelCompleted[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive) == true)
                vlistaBusca.append(vlistaNovelCompleted[i]);
            else if(vlistaNovelCompleted[i]->vnomeAlternativo.size() != 0){
                for(int w = 0; w < vlistaNovelCompleted[i]->vnomeAlternativo.size(); w++){
                    if(vlistaNovelCompleted[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive)){
                        vlistaBusca.append(vlistaNovelCompleted[i]);
                        break;
                    }
                }
            }
        }
        for(int i = 0; i < vlistaNovelOnHold.size(); i++){
            if(vlistaNovelOnHold[i]->vnome.contains(rnome, Qt::CaseInsensitive) == true ||
                    vlistaNovelOnHold[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive) == true)
                vlistaBusca.append(vlistaNovelOnHold[i]);
            else if(vlistaNovelOnHold[i]->vnomeAlternativo.size() != 0){
                for(int w = 0; w < vlistaNovelOnHold[i]->vnomeAlternativo.size(); w++){
                    if(vlistaNovelOnHold[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive)){
                        vlistaBusca.append(vlistaNovelOnHold[i]);
                        break;
                    }
                }
            }
        }
        for(int i = 0; i < vlistaNovelDropped.size(); i++){
            if(vlistaNovelDropped[i]->vnome.contains(rnome, Qt::CaseInsensitive) == true ||
                    vlistaNovelDropped[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive) == true)
                vlistaBusca.append(vlistaNovelDropped[i]);
            else if(vlistaNovelDropped[i]->vnomeAlternativo.size() != 0){
                for(int w = 0; w < vlistaNovelDropped[i]->vnomeAlternativo.size(); w++){
                    if(vlistaNovelDropped[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive)){
                        vlistaBusca.append(vlistaNovelDropped[i]);
                        break;
                    }
                }
            }
        }
        for(int i = 0; i < vlistaNovelPlanToRead.size(); i++){
            if(vlistaNovelPlanToRead[i]->vnome.contains(rnome, Qt::CaseInsensitive) == true ||
                    vlistaNovelPlanToRead[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive) == true)
                vlistaBusca.append(vlistaNovelPlanToRead[i]);
            else if(vlistaNovelPlanToRead[i]->vnomeAlternativo.size() != 0){
                for(int w = 0; w < vlistaNovelPlanToRead[i]->vnomeAlternativo.size(); w++){
                    if(vlistaNovelPlanToRead[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive)){
                        vlistaBusca.append(vlistaNovelPlanToRead[i]);
                        break;
                    }
                }
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

QVector<anime*> leitorlistaanimes::retornaListaMangaReading()
{
    return vlistaMangaReading;
}

QVector<anime *> leitorlistaanimes::retornaListaMangaCompleted()
{
    return vlistaMangaCompleted;
}

QVector<anime *> leitorlistaanimes::retornaListaMangaOnHold()
{
    return vlistaMangaOnHold;
}

QVector<anime *> leitorlistaanimes::retornaListaMangaDropped()
{
    return vlistaMangaDropped;
}

QVector<anime *> leitorlistaanimes::retornaListaMangaPlanToRead()
{
    return vlistaMangaPlanToRead;
}

QVector<anime*> leitorlistaanimes::retornaListaNovelReading()
{
    return vlistaNovelReading;
}

QVector<anime *> leitorlistaanimes::retornaListaNovelCompleted()
{
    return vlistaNovelCompleted;
}

QVector<anime *> leitorlistaanimes::retornaListaNovelOnHold()
{
    return vlistaNovelOnHold;
}

QVector<anime *> leitorlistaanimes::retornaListaNovelDropped()
{
    return vlistaNovelDropped;
}

QVector<anime *> leitorlistaanimes::retornaListaNovelPlanToRead()
{
    return vlistaNovelPlanToRead;
}

bool leitorlistaanimes::fmudaLista(QString rid, QString rlista, QString rtipo){
    if(rtipo.compare("anime", Qt::CaseInsensitive) == 0){
        if(!vlistaWatching.isEmpty()){
            for(int i = 0; i < vlistaWatching.size(); i++){
                if(vlistaWatching[i]->vid == rid){
                    QPointer<anime> lnovoAnime(new anime);
                    lnovoAnime = vlistaWatching[i];
                    if(rlista == "Completed")
                        vlistaCompleted.append(lnovoAnime);
                    else if(rlista == "Dropped")
                        vlistaDropped.append(lnovoAnime);
                    else if(rlista == "On Hold")
                        vlistaOnHold.append(lnovoAnime);
                    else if(rlista == "Plan to Watch")
                        vlistaPlanToWatch.append(lnovoAnime);
                    vlistaWatching.remove(i);
                    return true;
                }
            }
        }
        if(!vlistaCompleted.isEmpty()){
            for(int i = 0; i < vlistaCompleted.size(); i++){
                if(vlistaCompleted[i]->vid == rid){
                    QPointer<anime> lnovoAnime(new anime);
                    lnovoAnime = vlistaCompleted[i];
                    if(rlista == "Watching")
                        vlistaWatching.append(lnovoAnime);
                    else if(rlista == "Dropped")
                        vlistaDropped.append(lnovoAnime);
                    else if(rlista == "On Hold")
                        vlistaOnHold.append(lnovoAnime);
                    else if(rlista == "Plan to Watch")
                        vlistaPlanToWatch.append(lnovoAnime);
                    vlistaCompleted.remove(i);
                    return true;
                }
            }
        }
        if(!vlistaOnHold.isEmpty()){
            for(int i = 0; i < vlistaOnHold.size(); i++){
                if(vlistaOnHold[i]->vid == rid){
                    QPointer<anime> lnovoAnime(new anime);
                    lnovoAnime = vlistaOnHold[i];
                    if(rlista == "Watching")
                        vlistaWatching.append(lnovoAnime);
                    else if(rlista == "Completed")
                        vlistaCompleted.append(lnovoAnime);
                    else if(rlista == "Dropped")
                        vlistaDropped.append(lnovoAnime);
                    else if(rlista == "Plan to Watch")
                        vlistaPlanToWatch.append(lnovoAnime);
                    vlistaOnHold.remove(i);
                    return true;
                }
            }
        }
        if(!vlistaDropped.isEmpty()){
            for(int i = 0; i < vlistaDropped.size(); i++){
                if(vlistaDropped[i]->vid == rid){
                    QPointer<anime> lnovoAnime(new anime);
                    lnovoAnime = vlistaDropped[i];
                    if(rlista == "Watching")
                        vlistaWatching.append(lnovoAnime);
                    else if(rlista == "Completed")
                        vlistaCompleted.append(lnovoAnime);
                    else if(rlista == "On Hold")
                        vlistaOnHold.append(lnovoAnime);
                    else if(rlista == "Plan to Watch")
                        vlistaPlanToWatch.append(lnovoAnime);
                    vlistaDropped.remove(i);
                    return true;
                }
            }
        }
        if(!vlistaPlanToWatch.isEmpty()){
            for(int i = 0; i < vlistaPlanToWatch.size(); i++){
                if(vlistaPlanToWatch[i]->vid == rid){
                    QPointer<anime> lnovoAnime(new anime);
                    lnovoAnime = vlistaPlanToWatch[i];
                    if(rlista == "Watching")
                        vlistaWatching.append(lnovoAnime);
                    else if(rlista == "Completed")
                        vlistaCompleted.append(lnovoAnime);
                    else if(rlista == "On Hold")
                        vlistaOnHold.append(lnovoAnime);
                    else if(rlista == "Dropped")
                        vlistaDropped.append(lnovoAnime);
                    vlistaPlanToWatch.remove(i);
                    return true;
                }
            }
        }
    }
    else if(rtipo.compare("novel", Qt::CaseInsensitive) == 0){
        if(!vlistaNovelReading.isEmpty()){
            for(int i = 0; i < vlistaNovelReading.size(); i++){
                if(vlistaNovelReading[i]->vid == rid){
                    QPointer<anime> lnovoAnime(new anime);
                    lnovoAnime = vlistaNovelReading[i];
                    if(rlista == "Completed(Novel)")
                        vlistaNovelCompleted.append(lnovoAnime);
                    else if(rlista == "Dropped(Novel)")
                        vlistaNovelDropped.append(lnovoAnime);
                    else if(rlista == "On Hold(Novel)")
                        vlistaNovelOnHold.append(lnovoAnime);
                    else if(rlista == "Plan to Read(Novel)")
                        vlistaNovelPlanToRead.append(lnovoAnime);
                    vlistaNovelReading.remove(i);
                    return true;
                }
            }
        }
        if(!vlistaNovelCompleted.isEmpty()){
            for(int i = 0; i < vlistaNovelCompleted.size(); i++){
                if(vlistaNovelCompleted[i]->vid == rid){
                    QPointer<anime> lnovoAnime(new anime);
                    lnovoAnime = vlistaNovelCompleted[i];
                    if(rlista == "Reading(Novel)")
                        vlistaNovelReading.append(lnovoAnime);
                    else if(rlista == "Dropped(Novel)")
                        vlistaNovelDropped.append(lnovoAnime);
                    else if(rlista == "On Hold(Novel)")
                        vlistaNovelOnHold.append(lnovoAnime);
                    else if(rlista == "Plan to Read(Novel)")
                        vlistaNovelPlanToRead.append(lnovoAnime);
                    vlistaNovelCompleted.remove(i);
                    return true;
                }
            }
        }
        if(!vlistaNovelOnHold.isEmpty()){
            for(int i = 0; i < vlistaNovelOnHold.size(); i++){
                if(vlistaNovelOnHold[i]->vid == rid){
                    QPointer<anime> lnovoAnime(new anime);
                    lnovoAnime = vlistaNovelOnHold[i];
                    if(rlista == "Reading(Novel)")
                        vlistaNovelReading.append(lnovoAnime);
                    else if(rlista == "Completed(Novel)")
                        vlistaNovelCompleted.append(lnovoAnime);
                    else if(rlista == "Dropped(Novel)")
                        vlistaNovelDropped.append(lnovoAnime);
                    else if(rlista == "Plan to Read(Novel)")
                        vlistaNovelPlanToRead.append(lnovoAnime);
                    vlistaNovelOnHold.remove(i);
                    return true;
                }
            }
        }
        if(!vlistaNovelDropped.isEmpty()){
            for(int i = 0; i < vlistaNovelDropped.size(); i++){
                if(vlistaNovelDropped[i]->vid == rid){
                    QPointer<anime> lnovoAnime(new anime);
                    lnovoAnime = vlistaNovelDropped[i];
                    if(rlista == "Watching(Novel)")
                        vlistaNovelReading.append(lnovoAnime);
                    else if(rlista == "Completed(Novel)")
                        vlistaNovelCompleted.append(lnovoAnime);
                    else if(rlista == "On Hold(Novel)")
                        vlistaNovelOnHold.append(lnovoAnime);
                    else if(rlista == "Plan to Read(Novel)")
                        vlistaNovelPlanToRead.append(lnovoAnime);
                    vlistaNovelDropped.remove(i);
                    return true;
                }
            }
        }
        if(!vlistaNovelPlanToRead.isEmpty()){
            for(int i = 0; i < vlistaNovelPlanToRead.size(); i++){
                if(vlistaNovelPlanToRead[i]->vid == rid){
                    QPointer<anime> lnovoAnime(new anime);
                    lnovoAnime = vlistaNovelPlanToRead[i];
                    if(rlista == "Reading(Novel)")
                        vlistaNovelReading.append(lnovoAnime);
                    else if(rlista == "Completed(Novel)")
                        vlistaNovelCompleted.append(lnovoAnime);
                    else if(rlista == "On Hold(Novel)")
                        vlistaNovelOnHold.append(lnovoAnime);
                    else if(rlista == "Dropped(Novel)")
                        vlistaNovelDropped.append(lnovoAnime);
                    vlistaNovelPlanToRead.remove(i);
                    return true;
                }
            }
        }
    }
    else if(rtipo.compare("manga", Qt::CaseInsensitive) == 0){
        if(!vlistaMangaReading.isEmpty()){
            for(int i = 0; i < vlistaMangaReading.size(); i++){
                if(vlistaMangaReading[i]->vid == rid){
                    QPointer<anime> lnovoAnime(new anime);
                    lnovoAnime = vlistaMangaReading[i];
                    if(rlista == "Completed(Manga)")
                        vlistaMangaCompleted.append(lnovoAnime);
                    else if(rlista == "Dropped(Manga)")
                        vlistaMangaDropped.append(lnovoAnime);
                    else if(rlista == "On Hold(Manga)")
                        vlistaMangaOnHold.append(lnovoAnime);
                    else if(rlista == "Plan to Read(Manga)")
                        vlistaMangaPlanToRead.append(lnovoAnime);
                    vlistaMangaReading.remove(i);
                    return true;
                }
            }
        }
        if(!vlistaMangaCompleted.isEmpty()){
            for(int i = 0; i < vlistaMangaCompleted.size(); i++){
                if(vlistaMangaCompleted[i]->vid == rid){
                    QPointer<anime> lnovoAnime(new anime);
                    lnovoAnime = vlistaMangaCompleted[i];
                    if(rlista == "Reading(Manga)")
                        vlistaMangaReading.append(lnovoAnime);
                    else if(rlista == "Dropped(Manga)")
                        vlistaMangaDropped.append(lnovoAnime);
                    else if(rlista == "On Hold(Manga)")
                        vlistaMangaOnHold.append(lnovoAnime);
                    else if(rlista == "Plan to Read(Manga)")
                        vlistaMangaPlanToRead.append(lnovoAnime);
                    vlistaMangaCompleted.remove(i);
                    return true;
                }
            }
        }
        if(!vlistaMangaOnHold.isEmpty()){
            for(int i = 0; i < vlistaMangaOnHold.size(); i++){
                if(vlistaMangaOnHold[i]->vid == rid){
                    QPointer<anime> lnovoAnime(new anime);
                    lnovoAnime = vlistaMangaOnHold[i];
                    if(rlista == "Reading(Manga)")
                        vlistaMangaReading.append(lnovoAnime);
                    else if(rlista == "Completed(Manga)")
                        vlistaMangaCompleted.append(lnovoAnime);
                    else if(rlista == "Dropped(Manga)")
                        vlistaMangaDropped.append(lnovoAnime);
                    else if(rlista == "Plan to Read(Manga)")
                        vlistaMangaPlanToRead.append(lnovoAnime);
                    vlistaMangaOnHold.remove(i);
                    return true;
                }
            }
        }
        if(!vlistaMangaDropped.isEmpty()){
            for(int i = 0; i < vlistaMangaDropped.size(); i++){
                if(vlistaMangaDropped[i]->vid == rid){
                    QPointer<anime> lnovoAnime(new anime);
                    lnovoAnime = vlistaMangaDropped[i];
                    if(rlista == "Reading(Manga)")
                        vlistaMangaReading.append(lnovoAnime);
                    else if(rlista == "Completed(Manga)")
                        vlistaMangaCompleted.append(lnovoAnime);
                    else if(rlista == "On Hold(Manga)")
                        vlistaMangaOnHold.append(lnovoAnime);
                    else if(rlista == "Plan to Read(Manga)")
                        vlistaMangaPlanToRead.append(lnovoAnime);
                    vlistaMangaDropped.remove(i);
                    return true;
                }
            }
        }
        if(!vlistaMangaPlanToRead.isEmpty()){
            for(int i = 0; i < vlistaMangaPlanToRead.size(); i++){
                if(vlistaMangaPlanToRead[i]->vid == rid){
                    QPointer<anime> lnovoAnime(new anime);
                    lnovoAnime = vlistaMangaPlanToRead[i];
                    if(rlista == "Reading(Manga)")
                        vlistaMangaReading.append(lnovoAnime);
                    else if(rlista == "Completed(Manga)")
                        vlistaMangaCompleted.append(lnovoAnime);
                    else if(rlista == "On Hold(Manga)")
                        vlistaMangaOnHold.append(lnovoAnime);
                    else if(rlista == "Dropped(Manga)")
                        vlistaMangaDropped.append(lnovoAnime);
                    vlistaMangaPlanToRead.remove(i);
                    return true;
                }
            }
        }
    }
    return false;
}
