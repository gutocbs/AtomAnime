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
    QString ldataEpisodioFinal;
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
                QPointer<anime> lnovoAnime(new anime);
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
            }
        }
        lleJson.close();
    }
    else
        //Caso o arquivo não possa ser lido, retorna false
        return false;
    return true;
}


void leitorlistaanimes::fdeletaListaAnimes(){
    if(!vlistaWatching.isEmpty()){
        vlistaWatching.clear();
    }
    if(!vlistaCompleted.isEmpty()){
        vlistaCompleted.clear();
    }
    if(!vlistaDropped.isEmpty()){
        vlistaDropped.clear();
    }
    if(!vlistaOnHold.isEmpty()){
        vlistaOnHold.clear();
    }
    if(!vlistaPlanToWatch.isEmpty()){
        vlistaPlanToWatch.clear();
    }
    if(!vlistaBusca.isEmpty()){
        vlistaBusca.clear();
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
    return false;

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
                        int lnumEpiTotali = llistaTemp[i]->vnumEpisodiosTotais.toInt();
                        int lnumEpiTotalw = llistaTemp[w]->vnumEpisodiosTotais.toInt();
                        if(lnumEpiTotali == 0)
                            lnumEpiTotali = 1000000;
                        if(lnumEpiTotalw == 0)
                            lnumEpiTotalw = 1000000;
                        if(static_cast<float>(llistaTemp[i]->vnumEpisodiosAssistidos.toInt())/lnumEpiTotali
                                > static_cast<float>(llistaTemp[w]->vnumEpisodiosAssistidos.toInt())/lnumEpiTotalw){
                            llistaTemp.move(i,w);
                            llistaTemp.move(w-1,i);
                        }
                    }
                    else if(rordem == "cnota"){
                        if(llistaTemp[i]->vnotaMediaPessoal.toInt() > llistaTemp[w]->vnotaMediaPessoal.toInt()){
                            llistaTemp.move(i,w);
                            llistaTemp.move(w-1,i);
                        }
                    }
                    else if(rordem == "cformato"){
                        if(llistaTemp[i]->vformato > llistaTemp[w]->vformato){
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
                        int lnumEpiTotali = llistaTemp[i]->vnumEpisodiosTotais.toInt();
                        int lnumEpiTotalw = llistaTemp[w]->vnumEpisodiosTotais.toInt();
                        if(lnumEpiTotali == 0)
                            lnumEpiTotali = 1000000;
                        if(lnumEpiTotalw == 0)
                            lnumEpiTotalw = 1000000;
                        if(static_cast<float>(llistaTemp[i]->vnumEpisodiosAssistidos.toInt())/lnumEpiTotali
                                < static_cast<float>(llistaTemp[w]->vnumEpisodiosAssistidos.toInt())/lnumEpiTotalw){
                            llistaTemp.move(i,w);
                            llistaTemp.move(w-1,i);
                        }
                    }
                    else if(rordem == "dnota"){
                        if(llistaTemp[i]->vnotaMediaPessoal.toInt() < llistaTemp[w]->vnotaMediaPessoal.toInt()){
                            llistaTemp.move(i,w);
                            llistaTemp.move(w-1,i);
                        }
                    }
                    else if(rordem == "dformato"){
                        if(llistaTemp[i]->vformato < llistaTemp[w]->vformato){
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
    //Essa função faz crashar. Se eu deleto os animes e tento procurar por eles, eles não existem mais no endereço certo e causam bad alloc
//    qDeleteAll(vlistaBusca.begin(),vlistaBusca.end()); //Deletar todas as listas
    vlistaBusca.clear();
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

bool leitorlistaanimes::fmudaLista(QString rid, QString rlista){
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
    return false;
}
