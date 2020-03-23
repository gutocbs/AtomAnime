#include "leitorlistaanimes.h"

leitorlistaanimes::leitorlistaanimes(QObject *parent) : QObject(parent)
{
    vcarregaListasAnoAcabou = true;

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
                QStringList tempNomeAnime;
                if(lformato != "MANGA" && lformato != "NOVEL" && lformato != "ONE SHOT"){
                    if(llinha.trimmed() == "CURRENT"){
                        lnovoAnime->vlista = "Watching";
                        vlistaWatching.append(lnovoAnime);
                        if(!vHashPosicaoAnimesPorId.contains(lnovoAnime->vid))
                            vHashPosicaoAnimesPorId.insert(lnovoAnime->vid, vlistaWatching.size()-1);
                        else if(vHashPosicaoAnimesPorId[lnovoAnime->vid] != vlistaWatching.size()-1)
                            vHashPosicaoAnimesPorId.insert(lnovoAnime->vid, vlistaWatching.size()-1);
                        if(!vHashNomeAnimesPorId.contains(lnovoAnime->vid)){
                            tempNomeAnime.append(lnovoAnime->vnome);
                            tempNomeAnime.append(lnovoAnime->vnomeIngles);
                            tempNomeAnime.append(lnovoAnime->vnomeAlternativo);
                            vHashNomeAnimesPorId.insert(lnovoAnime->vid, tempNomeAnime);
                        }
                    }
                    else if(llinha.trimmed() == "COMPLETED"){
                        lnovoAnime->vlista = "Completed";
                        vlistaCompleted.append(lnovoAnime);
                        if(!vHashPosicaoAnimesPorId.contains(lnovoAnime->vid))
                            vHashPosicaoAnimesPorId.insert(lnovoAnime->vid, vlistaCompleted.size()-1);
                        else if(vHashPosicaoAnimesPorId[lnovoAnime->vid] != vlistaCompleted.size()-1)
                            vHashPosicaoAnimesPorId.insert(lnovoAnime->vid, vlistaCompleted.size()-1);
                        if(!vHashNomeAnimesPorId.contains(lnovoAnime->vid)){
                            tempNomeAnime.append(lnovoAnime->vnome);
                            tempNomeAnime.append(lnovoAnime->vnomeIngles);
                            tempNomeAnime.append(lnovoAnime->vnomeAlternativo);
                            vHashNomeAnimesPorId.insert(lnovoAnime->vid, tempNomeAnime);
                        }
                    }
                    else if(llinha.trimmed() == "PLANNING"){
                        lnovoAnime->vlista = "Plan to Watch";
                        vlistaPlanToWatch.append(lnovoAnime);
                        if(!vHashPosicaoAnimesPorId.contains(lnovoAnime->vid))
                            vHashPosicaoAnimesPorId.insert(lnovoAnime->vid, vlistaPlanToWatch.size()-1);
                        else if(vHashPosicaoAnimesPorId[lnovoAnime->vid] != vlistaPlanToWatch.size()-1)
                            vHashPosicaoAnimesPorId.insert(lnovoAnime->vid, vlistaPlanToWatch.size()-1);
                        if(!vHashNomeAnimesPorId.contains(lnovoAnime->vid)){
                            tempNomeAnime.append(lnovoAnime->vnome);
                            tempNomeAnime.append(lnovoAnime->vnomeIngles);
                            tempNomeAnime.append(lnovoAnime->vnomeAlternativo);
                            vHashNomeAnimesPorId.insert(lnovoAnime->vid, tempNomeAnime);
                        }
                    }
                    else if(llinha.trimmed() == "DROPPED"){
                        lnovoAnime->vlista = "Dropped";
                        vlistaDropped.append(lnovoAnime);
                        if(!vHashPosicaoAnimesPorId.contains(lnovoAnime->vid))
                            vHashPosicaoAnimesPorId.insert(lnovoAnime->vid, vlistaDropped.size()-1);
                        else if(vHashPosicaoAnimesPorId[lnovoAnime->vid] != vlistaDropped.size()-1)
                            vHashPosicaoAnimesPorId.insert(lnovoAnime->vid, vlistaDropped.size()-1);
                        if(!vHashNomeAnimesPorId.contains(lnovoAnime->vid)){
                            tempNomeAnime.append(lnovoAnime->vnome);
                            tempNomeAnime.append(lnovoAnime->vnomeIngles);
                            tempNomeAnime.append(lnovoAnime->vnomeAlternativo);
                            vHashNomeAnimesPorId.insert(lnovoAnime->vid, tempNomeAnime);
                        }
                    }
                    else if(llinha.trimmed() == "PAUSED"){
                        lnovoAnime->vlista = "On Hold";
                        vlistaOnHold.append(lnovoAnime);
                        if(!vHashPosicaoAnimesPorId.contains(lnovoAnime->vid))
                            vHashPosicaoAnimesPorId.insert(lnovoAnime->vid, vlistaOnHold.size()-1);
                        else if(vHashPosicaoAnimesPorId[lnovoAnime->vid] != vlistaOnHold.size()-1)
                            vHashPosicaoAnimesPorId.insert(lnovoAnime->vid, vlistaOnHold.size()-1);
                        if(!vHashNomeAnimesPorId.contains(lnovoAnime->vid)){
                            tempNomeAnime.append(lnovoAnime->vnome);
                            tempNomeAnime.append(lnovoAnime->vnomeIngles);
                            tempNomeAnime.append(lnovoAnime->vnomeAlternativo);
                            vHashNomeAnimesPorId.insert(lnovoAnime->vid, tempNomeAnime);
                        }
                    }
                    sAnimeAdicionadoNaLista(lnovoAnime->vid);
                    if(!vHashListaAnimesPorId.contains(lnovoAnime->vid))
                        vHashListaAnimesPorId.insert(lnovoAnime->vid, lnovoAnime->vlista);
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

QVector<anime*> leitorlistaanimes::fleListaAno(int ano)
{
    QVector<anime*> vetorTemporario;
    QFile lleJson("Configurações/Temp/Lists/animeList"+QString::number(ano)+".txt");
    if(lleJson.size() == 0)
        return vetorTemporario;
    if(!vHashSizeListasPorAno.contains(ano))
        vHashSizeListasPorAno.insert(ano, QString::number(lleJson.size()));
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

                 lnotaMediaPessoal = "";
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
                lnovoAnime->vlista = fbuscaAnimePorIDERetornaLista(lid);
                lnovoAnime->vnumEpisodiosAssistidos = fbuscaAnimePorIDERetornaEpisodio(lid);
                lnovoAnime->vnotaMediaPessoal = fbuscaAnimePorIDERetornaNota(lid);
                lnovoAnime->vLinkImagemMedia = lLinkImagemMedia;
                lnovoAnime->vstatus = lstatus;
                lnovoAnime->vsinopse = lsinopse;
                lnovoAnime->vid = lid;
                lnovoAnime->vseason = lseason + " " + lano;
                lnovoAnime->vnumProximoEpisodioLancado = lproximoEpisodio;
                lnovoAnime->vformato = lformato;
                lnovoAnime->vdataEstreia = ldataEstreia;
                lnovoAnime->vdataEpisodio = ldataEpisodioFinal;
                lnovoAnime->vsiteAnilist = lsiteAnilist;
                lnovoAnime->vstreamCrunchyroll = lstreamCrunchyroll;
                lnovoAnime->vtemporada = ltemporadaAnime;
                if(ldataEstreia.year() == ano){
                    vetorTemporario.append(lnovoAnime);
                    sAnimeAdicionadoNaLista(lid);
                }
                QStringList tempNomeAnime;
                if(!vHashNomeAnimesPorId.contains(lnovoAnime->vid)){
                    tempNomeAnime.append(lnovoAnime->vnome);
                    tempNomeAnime.append(lnovoAnime->vnomeIngles);
                    tempNomeAnime.append(lnovoAnime->vnomeAlternativo);
                    vHashNomeAnimesPorId.insert(lnovoAnime->vid, tempNomeAnime);
                }
                if(!vHashPosicaoAnimesPorId.contains(lnovoAnime->vid))
                    vHashPosicaoAnimesPorId.insert(lnovoAnime->vid, vetorTemporario.size()-1);
                if(!vHashListaAnimesPorId.contains(lnovoAnime->vid))
                    vHashListaAnimesPorId.insert(lnovoAnime->vid, QString::number(ano));

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
    return vetorTemporario;
}

QVector<anime *> leitorlistaanimes::fleListaAnoSeason(int ano, QString seasonEscolhida)
{
    QVector<anime*> vetorTemporario;
    QFile lleJson("Configurações/Temp/Lists/animeList"+QString::number(ano)+".txt");
    if(lleJson.size() == 0)
        return vetorTemporario;
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

                 lnotaMediaPessoal = "";
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
                lnovoAnime->vlista = fbuscaAnimePorIDERetornaLista(lid);
                lnovoAnime->vnumEpisodiosAssistidos = fbuscaAnimePorIDERetornaEpisodio(lid);
                lnovoAnime->vnotaMediaPessoal = fbuscaAnimePorIDERetornaNota(lid);
                lnovoAnime->vLinkImagemMedia = lLinkImagemMedia;
                lnovoAnime->vstatus = lstatus;
                lnovoAnime->vsinopse = lsinopse;
                lnovoAnime->vid = lid;
                lnovoAnime->vseason = lseason + " " + lano;
                lnovoAnime->vnumProximoEpisodioLancado = lproximoEpisodio;
                lnovoAnime->vformato = lformato;
                lnovoAnime->vdataEstreia = ldataEstreia;
                lnovoAnime->vdataEpisodio = ldataEpisodioFinal;
                lnovoAnime->vsiteAnilist = lsiteAnilist;
                lnovoAnime->vstreamCrunchyroll = lstreamCrunchyroll;
                lnovoAnime->vtemporada = ltemporadaAnime;
                if(lseason.compare(seasonEscolhida, Qt::CaseInsensitive) == 0){
                    vetorTemporario.append(lnovoAnime);
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
    return vetorTemporario;
}

void leitorlistaanimes::fsetListasPorAnoEmThread()
{
    if(!vcarregaListas.isRunning() && vcarregaListasAnoAcabou == true)
        vcarregaListas = QtConcurrent::run(this, &leitorlistaanimes::fcarregaListaAnoEmThread);
}

void leitorlistaanimes::fleListaIdsNomesAnos()
{
    QFile larquivo("Configurações/idNomeLista.txt");
    if(larquivo.size() == 0)
        return;
    if(larquivo.open(QIODevice::ReadOnly)){
        while(!larquivo.atEnd()){
            QString lstreamTexto = larquivo.readLine();
            QStringList lidListaNome = lstreamTexto.split(";");
            lidListaNome.last() = lidListaNome.last().trimmed();
            QString id = lidListaNome.takeFirst();
            QString lista = lidListaNome.takeFirst();
            QString posicaoLista = lidListaNome.takeFirst();
            vHashListaAnimesPorId.insert(id,lista);
            vHashPosicaoAnimesPorId.insert(id,posicaoLista.toInt());
            vHashNomeAnimesPorId.insert(id,lidListaNome);
        }
        larquivo.close();
    }
    QFile larquivoSizePorAno("Configurações/Temp/sizePerYear.txt");
    if(larquivoSizePorAno.size() == 0)
        return;
    if(larquivoSizePorAno.open(QIODevice::ReadOnly)){
        while(!larquivoSizePorAno.atEnd()){
            QString lstreamTexto = larquivoSizePorAno.readLine();
            QStringList lidAnoSize = lstreamTexto.split(";");
            lidAnoSize.last() = lidAnoSize.last().trimmed();
            QString ano = lidAnoSize.takeFirst();
            QString size = lidAnoSize.takeFirst();
            vHashSizeListasPorAno.insert(ano.toInt(),size);
        }
        larquivoSizePorAno.close();
    }
}

void leitorlistaanimes::fsalvaListaIdsNomesAnos()
{
    QFile larquivo("Configurações/idListaNome.txt");
    if(larquivo.open(QIODevice::WriteOnly)){
        QTextStream lstreamTexto(&larquivo);
        foreach(QString key, vHashNomeAnimesPorId.keys()){
            lstreamTexto << key << ";" << vHashListaAnimesPorId[key] << ";" << vHashPosicaoAnimesPorId[key] << ";" <<
                            vHashNomeAnimesPorId[key].join(";").trimmed() << endl;
        }
        larquivo.close();
    }
    QFile larquivoSizePorAno("Configurações/Temp/sizePerYear.txt");
    if(larquivoSizePorAno.open(QIODevice::WriteOnly)){
        QTextStream lstreamTexto(&larquivoSizePorAno);
        foreach(int key, vHashSizeListasPorAno.keys()){
            lstreamTexto << QString::number(key) << ";" << vHashSizeListasPorAno[key] << endl;
        }
        larquivoSizePorAno.close();
    }
}

void leitorlistaanimes::fcarregaListaAnoEmThread()
{
    vcarregaListasAnoAcabou = false;
    for(int i = 0; i < QDate::currentDate().year() - 1999; i++){
        QFile lleJson("Configurações/Temp/Lists/animeList"+QString::number(2000+i)+".txt");
        if(!vHashSizeListasPorAno.contains(2000+i))
            fleListaAno(2000+i);
    }
    vcarregaListasAnoAcabou = true;
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

QString leitorlistaanimes::fprocuraAnimeNasListas(QString rnomeAnime)
{
    foreach(QString key, vHashNomeAnimesPorId.keys()){
        for(int i = 0; i < vHashNomeAnimesPorId[key].size(); i++){
            if(vHashNomeAnimesPorId[key].at(i) == rnomeAnime)
                return key;
        }
    }
    QStringList tempNomeAnime;
    for(int i = 0; i < vlistaWatching.size(); i++){
        if(formatador.fcomparaNomes(vlistaWatching[i]->vnome, rnomeAnime) ||
                formatador.fcomparaNomes(vlistaWatching[i]->vnomeIngles, rnomeAnime)){
            tempNomeAnime.append(rnomeAnime);
            if(!vHashNomeAnimesPorId.contains(vlistaWatching[i]->vid))
                vHashNomeAnimesPorId.insert(vlistaWatching[i]->vid, tempNomeAnime);
            else
                vHashNomeAnimesPorId[vlistaWatching[i]->vid].append(tempNomeAnime);
            return vlistaWatching[i]->vid;
        }
        else if(vlistaWatching[i]->vnomeAlternativo.size() != 0){
            for(int w = 0; w < vlistaWatching[i]->vnomeAlternativo.size(); w++){
                if(formatador.fcomparaNomes(vlistaWatching[i]->vnomeAlternativo.at(w), rnomeAnime)){
                    tempNomeAnime.append(rnomeAnime);
                    if(!vHashNomeAnimesPorId.contains(vlistaWatching[i]->vid))
                        vHashNomeAnimesPorId.insert(vlistaWatching[i]->vid, tempNomeAnime);
                    else
                        vHashNomeAnimesPorId[vlistaWatching[i]->vid].append(tempNomeAnime);
                    return vlistaWatching[i]->vid;
                }
            }
        }
    }
    for(int i = 0; i < vlistaPlanToWatch.size(); i++){
        if(formatador.fcomparaNomes(vlistaPlanToWatch[i]->vnome, rnomeAnime) ||
                formatador.fcomparaNomes(vlistaPlanToWatch[i]->vnomeIngles, rnomeAnime)){
            tempNomeAnime.append(rnomeAnime);
            if(!vHashNomeAnimesPorId.contains(vlistaPlanToWatch[i]->vid))
                vHashNomeAnimesPorId.insert(vlistaPlanToWatch[i]->vid, tempNomeAnime);
            else
                vHashNomeAnimesPorId[vlistaPlanToWatch[i]->vid].append(tempNomeAnime);
            return vlistaPlanToWatch[i]->vid;
        }
        else if(vlistaPlanToWatch[i]->vnomeAlternativo.size() != 0){
            for(int w = 0; w < vlistaPlanToWatch[i]->vnomeAlternativo.size(); w++){
                if(formatador.fcomparaNomes(vlistaPlanToWatch[i]->vnomeAlternativo.at(w), rnomeAnime)){
                    tempNomeAnime.append(rnomeAnime);
                    if(!vHashNomeAnimesPorId.contains(vlistaPlanToWatch[i]->vid))
                        vHashNomeAnimesPorId.insert(vlistaPlanToWatch[i]->vid, tempNomeAnime);
                    else
                        vHashNomeAnimesPorId[vlistaPlanToWatch[i]->vid].append(tempNomeAnime);
                    return vlistaPlanToWatch[i]->vid;
                }
            }
        }
    }
    for(int i = 0; i < vlistaOnHold.size(); i++){
        if(formatador.fcomparaNomes(vlistaOnHold[i]->vnome, rnomeAnime) ||
                formatador.fcomparaNomes(vlistaOnHold[i]->vnomeIngles, rnomeAnime)){
            tempNomeAnime.append(rnomeAnime);
            if(!vHashNomeAnimesPorId.contains(vlistaOnHold[i]->vid))
                vHashNomeAnimesPorId.insert(vlistaOnHold[i]->vid, tempNomeAnime);
            else
                vHashNomeAnimesPorId[vlistaOnHold[i]->vid].append(tempNomeAnime);
            return vlistaOnHold[i]->vid;
        }
        else if(vlistaOnHold[i]->vnomeAlternativo.size() != 0){
            for(int w = 0; w < vlistaOnHold[i]->vnomeAlternativo.size(); w++){
                if(formatador.fcomparaNomes(vlistaOnHold[i]->vnomeAlternativo.at(w), rnomeAnime)){
                    tempNomeAnime.append(rnomeAnime);
                    if(!vHashNomeAnimesPorId.contains(vlistaOnHold[i]->vid))
                        vHashNomeAnimesPorId.insert(vlistaOnHold[i]->vid, tempNomeAnime);
                    else
                        vHashNomeAnimesPorId[vlistaOnHold[i]->vid].append(tempNomeAnime);
                    return vlistaOnHold[i]->vid;
                }
            }
        }
    }
    for(int i = 0; i < vlistaDropped.size(); i++){
        if(formatador.fcomparaNomes(vlistaDropped[i]->vnome, rnomeAnime) ||
                formatador.fcomparaNomes(vlistaDropped[i]->vnomeIngles, rnomeAnime)){
            tempNomeAnime.append(rnomeAnime);
            if(!vHashNomeAnimesPorId.contains(vlistaDropped[i]->vid))
                vHashNomeAnimesPorId.insert(vlistaDropped[i]->vid, tempNomeAnime);
            else
                vHashNomeAnimesPorId[vlistaDropped[i]->vid].append(tempNomeAnime);
            return vlistaDropped[i]->vid;
        }
        else if(vlistaDropped[i]->vnomeAlternativo.size() != 0){
            for(int w = 0; w < vlistaDropped[i]->vnomeAlternativo.size(); w++){
                if(formatador.fcomparaNomes(vlistaDropped[i]->vnomeAlternativo.at(w), rnomeAnime)){
                    tempNomeAnime.append(rnomeAnime);
                    if(!vHashNomeAnimesPorId.contains(vlistaDropped[i]->vid))
                        vHashNomeAnimesPorId.insert(vlistaDropped[i]->vid, tempNomeAnime);
                    else
                        vHashNomeAnimesPorId[vlistaDropped[i]->vid].append(tempNomeAnime);
                    return vlistaDropped[i]->vid;
                }
            }
        }
    }
    for(int i = 0; i < vlistaCompleted.size(); i++){
        if(formatador.fcomparaNomes(vlistaCompleted[i]->vnome, rnomeAnime) ||
                formatador.fcomparaNomes(vlistaCompleted[i]->vnomeIngles, rnomeAnime)){
            tempNomeAnime.append(rnomeAnime);
            if(!vHashNomeAnimesPorId.contains(vlistaCompleted[i]->vid))
                vHashNomeAnimesPorId.insert(vlistaCompleted[i]->vid, tempNomeAnime);
            else
                vHashNomeAnimesPorId[vlistaCompleted[i]->vid].append(tempNomeAnime);
            return vlistaCompleted[i]->vid;
        }
        else if(vlistaCompleted[i]->vnomeAlternativo.size() != 0){
            for(int w = 0; w < vlistaCompleted[i]->vnomeAlternativo.size(); w++){
                if(formatador.fcomparaNomes(vlistaCompleted[i]->vnomeAlternativo.at(w), rnomeAnime)){
                    tempNomeAnime.append(rnomeAnime);
                    if(!vHashNomeAnimesPorId.contains(vlistaCompleted[i]->vid))
                        vHashNomeAnimesPorId.insert(vlistaCompleted[i]->vid, tempNomeAnime);
                    else
                        vHashNomeAnimesPorId[vlistaCompleted[i]->vid].append(tempNomeAnime);
                    return vlistaCompleted[i]->vid;
                }
            }
        }
    }
    return "";
}

QString leitorlistaanimes::fprocuraIdNasListasRapido(QString rnomeAnime)
{
    foreach(QString key, vHashNomeAnimesPorId.keys()){
        for(int i = 0; i < vHashNomeAnimesPorId[key].size(); i++){
            if(vHashNomeAnimesPorId[key].at(i) == rnomeAnime)
                return key;
        }
    }
    return "";
}

void leitorlistaanimes::finsereNomeAlternativo(QString rid, QStringList rlistaNomesAlternativos)
{
    QString llista = fbuscaAnimePorIDERetornaLista(rid);
    int lposicao = fbuscaAnimePorIDERetornaPosicao(rid);
    if(lposicao == -1)
        return;

    if(llista.compare("Watching", Qt::CaseInsensitive) == 0){
        vlistaWatching[lposicao]->vnomeAlternativo.append(rlistaNomesAlternativos);
    }
    else if(llista.compare("Plan to Watch", Qt::CaseInsensitive) == 0){
        vlistaPlanToWatch[lposicao]->vnomeAlternativo.append(rlistaNomesAlternativos);
    }
    else if(llista.compare("On Hold", Qt::CaseInsensitive) == 0){
        vlistaOnHold[lposicao]->vnomeAlternativo.append(rlistaNomesAlternativos);
    }
    else if(llista.compare("Dropped", Qt::CaseInsensitive) == 0){
        vlistaDropped[lposicao]->vnomeAlternativo.append(rlistaNomesAlternativos);
    }
    else if(llista.compare("Completed", Qt::CaseInsensitive) == 0){
        vlistaCompleted[lposicao]->vnomeAlternativo.append(rlistaNomesAlternativos);
    }
}

bool leitorlistaanimes::fdeletedaLista(QString rid)
{
    QString llista = fbuscaAnimePorIDERetornaLista(rid);
    int lposicao = fbuscaAnimePorIDERetornaPosicao(rid);
    if(lposicao == -1)
        return false;

    if(llista == "Watching"){
        vlistaWatching.remove(lposicao);
        return true;
    }
    else if(llista == "Completed"){
        vlistaCompleted.remove(lposicao);
        return true;
    }
    else if(llista == "On Hold"){
        vlistaOnHold.remove(lposicao);
        return true;
    }
    else if(llista == "Dropped"){
        vlistaDropped.remove(lposicao);
        return true;
    }
    else if(llista == "Plan to Watch"){
        vlistaPlanToWatch.remove(lposicao);
        return true;
    }
    else if(llista == "Reading(Manga)"){
        vlistaMangaReading.remove(lposicao);
        return true;
    }
    else if(llista == "Completed(Manga)"){
        vlistaMangaCompleted.remove(lposicao);
        return true;
    }
    else if(llista == "On Hold(Manga)"){
        vlistaMangaOnHold.remove(lposicao);
        return true;
    }
    else if(llista == "Dropped(Manga)"){
        vlistaMangaDropped.remove(lposicao);
        return true;
    }
    else if(llista == "Plan to Read(Manga)"){
        vlistaMangaPlanToRead.remove(lposicao);
        return true;
    }
    else if(llista == "Reading(Novel)"){
        vlistaNovelReading.remove(lposicao);
        return true;
    }
    else if(llista == "Completed(Novel)"){
        vlistaNovelCompleted.remove(lposicao);
        return true;
    }
    else if(llista == "On Hold(Novel)"){
        vlistaNovelOnHold.remove(lposicao);
        return true;
    }
    else if(llista == "Dropped(Novel)"){
        vlistaNovelDropped.remove(lposicao);
        return true;
    }
    else if(llista == "Plan to Read(Novel)"){
        vlistaNovelPlanToRead.remove(lposicao);
        return true;
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
    else if(rlista.contains("season", Qt::CaseInsensitive)){
        rlista = rlista.remove("season");
        if(rlista.contains("Winter", Qt::CaseInsensitive)){
            rlista = rlista.remove("Winter");
            llistaTemp = fleListaAnoSeason(rlista.toInt(), "winter");
        }
        else if(rlista.contains("Spring", Qt::CaseInsensitive)){
            rlista = rlista.remove("Spring");
            llistaTemp = fleListaAnoSeason(rlista.toInt(), "spring");
        }
        else if(rlista.contains("Summer", Qt::CaseInsensitive)){
            rlista = rlista.remove("Summer");
            llistaTemp = fleListaAnoSeason(rlista.toInt(), "summer");
        }
        else if(rlista.contains("Fall", Qt::CaseInsensitive)){
            rlista = rlista.remove("Fall");
            llistaTemp = fleListaAnoSeason(rlista.toInt(), "fall");
        }
        else
            llistaTemp = fleListaAno(rlista.toInt());
    }
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


///Testando nova função aqui
QVector<anime *> leitorlistaanimes::fbuscaLista(QString rnome, QString rtipoMidia){
    vlistaBusca.clear();
    if(rtipoMidia.compare("anime", Qt::CaseInsensitive) == 0){
        foreach(QString key, vHashNomeAnimesPorId.keys()){
            for(int i = 0; i < vHashNomeAnimesPorId[key].size(); i++){
                if(vHashNomeAnimesPorId[key].at(i).contains(rnome)){
                    QString lista = vHashListaAnimesPorId[key];
                    int pos = vHashPosicaoAnimesPorId[key];
                    if(pos == -1)
                        break;
                    else if(lista.compare("Watching", Qt::CaseInsensitive) == 0 && vlistaWatching.size() > pos)
                        vlistaBusca.append(vlistaWatching[pos]);
                    else if(lista.compare("Plan to Watch", Qt::CaseInsensitive) == 0 && vlistaPlanToWatch.size() > pos)
                        vlistaBusca.append(vlistaPlanToWatch[pos]);
                    else if(lista.compare("On Hold", Qt::CaseInsensitive) == 0 && vlistaOnHold.size() > pos)
                        vlistaBusca.append(vlistaOnHold[pos]);
                    else if(lista.compare("Completed", Qt::CaseInsensitive) == 0 && vlistaCompleted.size() > pos)
                        vlistaBusca.append(vlistaCompleted[pos]);
                    else if(lista.compare("Dropped", Qt::CaseInsensitive) == 0 && vlistaDropped.size() > pos)
                        vlistaBusca.append(vlistaDropped[pos]);
                }
            }
        }
//        for(int i = 0; i < vlistaWatching.size(); i++){
//            if(vlistaWatching[i]->vnome.contains(rnome, Qt::CaseInsensitive) ||
//                    vlistaWatching[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive)){
//                vlistaBusca.append(vlistaWatching[i]);
//            }
//            else if(vlistaWatching[i]->vnomeAlternativo.size() != 0){
//                for(int w = 0; w < vlistaWatching[i]->vnomeAlternativo.size(); w++){
//                    if(vlistaWatching[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive)){
//                        vlistaBusca.append(vlistaWatching[i]);
//                        break;
//                    }
//                }
//            }
//        }
//        for(int i = 0; i < vlistaCompleted.size(); i++){
//            if(vlistaCompleted[i]->vnome.contains(rnome, Qt::CaseInsensitive) ||
//                    vlistaCompleted[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive))
//                vlistaBusca.append(vlistaCompleted[i]);
//            else if(vlistaCompleted[i]->vnomeAlternativo.size() != 0){
//                for(int w = 0; w < vlistaCompleted[i]->vnomeAlternativo.size(); w++){
//                    if(vlistaCompleted[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive)){
//                        vlistaBusca.append(vlistaCompleted[i]);
//                        break;
//                    }
//                }
//            }
//        }
//        for(int i = 0; i < vlistaOnHold.size(); i++){
//            if(vlistaOnHold[i]->vnome.contains(rnome, Qt::CaseInsensitive) ||
//                    vlistaOnHold[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive))
//                vlistaBusca.append(vlistaOnHold[i]);
//            else if(vlistaOnHold[i]->vnomeAlternativo.size() != 0){
//                for(int w = 0; w < vlistaOnHold[i]->vnomeAlternativo.size(); w++){
//                    if(vlistaOnHold[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive)){
//                        vlistaBusca.append(vlistaOnHold[i]);
//                        break;
//                    }
//                }
//            }
//        }
//        for(int i = 0; i < vlistaDropped.size(); i++){
//            if(vlistaDropped[i]->vnome.contains(rnome, Qt::CaseInsensitive) ||
//                    vlistaDropped[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive))
//                vlistaBusca.append(vlistaDropped[i]);
//            else if(vlistaDropped[i]->vnomeAlternativo.size() != 0){
//                for(int w = 0; w < vlistaDropped[i]->vnomeAlternativo.size(); w++){
//                    if(vlistaDropped[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive)){
//                        vlistaBusca.append(vlistaDropped[i]);
//                        break;
//                    }
//                }
//            }
//        }
//        for(int i = 0; i < vlistaPlanToWatch.size(); i++){
//            if(vlistaPlanToWatch[i]->vnome.contains(rnome, Qt::CaseInsensitive) ||
//                    vlistaPlanToWatch[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive))
//                vlistaBusca.append(vlistaPlanToWatch[i]);
//            else if(vlistaPlanToWatch[i]->vnomeAlternativo.size() != 0){
//                for(int w = 0; w < vlistaPlanToWatch[i]->vnomeAlternativo.size(); w++){
//                    if(vlistaPlanToWatch[i]->vnomeAlternativo.at(w).contains(rnome, Qt::CaseInsensitive)){
//                        vlistaBusca.append(vlistaPlanToWatch[i]);
//                        break;
//                    }
//                }
//            }
//        }
    }
    else if(rtipoMidia.compare("manga", Qt::CaseInsensitive) == 0){
        for(int i = 0; i < vlistaMangaReading.size(); i++){
            if(vlistaMangaReading[i]->vnome.contains(rnome, Qt::CaseInsensitive) ||
                    vlistaMangaReading[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive)){
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
            if(vlistaMangaCompleted[i]->vnome.contains(rnome, Qt::CaseInsensitive) ||
                    vlistaMangaCompleted[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive))
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
            if(vlistaMangaOnHold[i]->vnome.contains(rnome, Qt::CaseInsensitive) ||
                    vlistaMangaOnHold[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive))
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
            if(vlistaMangaDropped[i]->vnome.contains(rnome, Qt::CaseInsensitive) ||
                    vlistaMangaDropped[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive))
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
            if(vlistaMangaPlanToRead[i]->vnome.contains(rnome, Qt::CaseInsensitive) ||
                    vlistaMangaPlanToRead[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive))
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
            if(vlistaNovelReading[i]->vnome.contains(rnome, Qt::CaseInsensitive) ||
                    vlistaNovelReading[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive)){
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
            if(vlistaNovelCompleted[i]->vnome.contains(rnome, Qt::CaseInsensitive) ||
                    vlistaNovelCompleted[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive))
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
            if(vlistaNovelOnHold[i]->vnome.contains(rnome, Qt::CaseInsensitive) ||
                    vlistaNovelOnHold[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive))
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
            if(vlistaNovelDropped[i]->vnome.contains(rnome, Qt::CaseInsensitive) ||
                    vlistaNovelDropped[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive))
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
            if(vlistaNovelPlanToRead[i]->vnome.contains(rnome, Qt::CaseInsensitive) ||
                    vlistaNovelPlanToRead[i]->vnomeIngles.contains(rnome, Qt::CaseInsensitive))
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

QString leitorlistaanimes::fbuscaAnimePorIDERetornaEpisodio(QString rid)
{   
    QString llista = fbuscaAnimePorIDERetornaLista(rid);
    int lposicao = fbuscaAnimePorIDERetornaPosicao(rid);
    if(lposicao == -1)
        return "0";

    if(llista.compare("Watching", Qt::CaseInsensitive) == 0){
        if(vlistaWatching.size() > lposicao)
            return vlistaWatching[lposicao]->vnumEpisodiosAssistidos;
    }
    else if(llista.compare("Plan to Watch", Qt::CaseInsensitive) == 0){
        if(vlistaWatching.size() > lposicao)
            return vlistaPlanToWatch[lposicao]->vnumEpisodiosAssistidos;
    }
    else if(llista.compare("On Hold", Qt::CaseInsensitive) == 0){
        if(vlistaWatching.size() > lposicao)
            return vlistaOnHold[lposicao]->vnumEpisodiosAssistidos;
    }
    else if(llista.compare("Completed", Qt::CaseInsensitive) == 0){
        if(vlistaWatching.size() > lposicao)
            return vlistaCompleted[lposicao]->vnumEpisodiosAssistidos;
    }
    else if(llista.compare("Dropped", Qt::CaseInsensitive) == 0){
        if(vlistaWatching.size() > lposicao)
            return vlistaDropped[lposicao]->vnumEpisodiosAssistidos;
    }
    return "0";
}

QString leitorlistaanimes::fbuscaAnimePorIDERetornaNota(QString rid)
{
    QString llista = fbuscaAnimePorIDERetornaLista(rid);
    int lposicao = fbuscaAnimePorIDERetornaPosicao(rid);
    if(lposicao == -1)
        return "0";
    if(llista.compare("Watching", Qt::CaseInsensitive) == 0)
        return vlistaWatching[lposicao]->vnotaMediaPessoal;
    else if(llista.compare("Plan to Watch", Qt::CaseInsensitive) == 0)
        return vlistaPlanToWatch[lposicao]->vnotaMediaPessoal;
    else if(llista.compare("On Hold", Qt::CaseInsensitive) == 0)
        return vlistaOnHold[lposicao]->vnotaMediaPessoal;
    else if(llista.compare("Completed", Qt::CaseInsensitive) == 0)
        return vlistaCompleted[lposicao]->vnotaMediaPessoal;
    else if(llista.compare("Dropped", Qt::CaseInsensitive) == 0)
        return vlistaDropped[lposicao]->vnotaMediaPessoal;
    return "0";
}

QString leitorlistaanimes::fbuscaAnimePorIDERetornaTitulo(QString rid)
{
    QString llista = fbuscaAnimePorIDERetornaLista(rid);
    int lposicao = fbuscaAnimePorIDERetornaPosicao(rid);
    if(lposicao == -1)
        return "0";
    if(llista.compare("Watching", Qt::CaseInsensitive) == 0)
        return vlistaWatching[lposicao]->vnome;
    else if(llista.compare("Plan to Watch", Qt::CaseInsensitive) == 0)
        return vlistaPlanToWatch[lposicao]->vnome;
    else if(llista.compare("On Hold", Qt::CaseInsensitive) == 0)
            return vlistaOnHold[lposicao]->vnome;
    else if(llista.compare("Completed", Qt::CaseInsensitive) == 0)
            return vlistaCompleted[lposicao]->vnome;
    else if(llista.compare("Dropped", Qt::CaseInsensitive) == 0)
            return vlistaDropped[lposicao]->vnome;
    return "";
}

QString leitorlistaanimes::fbuscaAnimePorIDERetornaLista(QString ridAnime)
{
    if(vHashListaAnimesPorId.contains(ridAnime))
        return vHashListaAnimesPorId[ridAnime];
    return "";
}

int leitorlistaanimes::fbuscaAnimePorIDERetornaPosicao(QString ridAnime)
{
    if(vHashPosicaoAnimesPorId.contains(ridAnime))
        return vHashPosicaoAnimesPorId[ridAnime];
    return -1;
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
    QString llista = fbuscaAnimePorIDERetornaLista(rid);
    int lposicao = fbuscaAnimePorIDERetornaPosicao(rid);
    if(lposicao == -1)
        return false;
    if(rtipo.compare("anime", Qt::CaseInsensitive) == 0){
        if(llista.compare("Watching", Qt::CaseInsensitive) == 0){
            QPointer<anime> lnovoAnime(new anime);
            lnovoAnime = vlistaWatching[lposicao];
            if(rlista.compare("Completed") == 0)
                vlistaCompleted.append(lnovoAnime);
            else if(rlista.compare("Dropped") == 0)
                vlistaDropped.append(lnovoAnime);
            else if(rlista.compare("On Hold") == 0)
                vlistaOnHold.append(lnovoAnime);
            else if(rlista.compare("Plan to Watch") == 0)
                vlistaPlanToWatch.append(lnovoAnime);
            vlistaWatching.remove(lposicao);
            return true;
        }
        else if(llista.compare("Completed", Qt::CaseInsensitive) == 0){
            QPointer<anime> lnovoAnime(new anime);
            lnovoAnime = vlistaCompleted[lposicao];
            if(rlista.compare("Watching") == 0)
                vlistaWatching.append(lnovoAnime);
            else if(rlista.compare("Dropped") == 0)
                vlistaDropped.append(lnovoAnime);
            else if(rlista.compare("On Hold") == 0)
                vlistaOnHold.append(lnovoAnime);
            else if(rlista.compare("Plan to Watch") == 0)
                vlistaPlanToWatch.append(lnovoAnime);
            vlistaWatching.remove(lposicao);
            return true;
        }
        else if(llista.compare("On Hold", Qt::CaseInsensitive) == 0){
            QPointer<anime> lnovoAnime(new anime);
            lnovoAnime = vlistaOnHold[lposicao];
            if(rlista.compare("Watching") == 0)
                vlistaWatching.append(lnovoAnime);
            else if(rlista.compare("Completed") == 0)
                vlistaCompleted.append(lnovoAnime);
            else if(rlista.compare("Dropped") == 0)
                vlistaDropped.append(lnovoAnime);
            else if(rlista.compare("Plan to Watch") == 0)
                vlistaPlanToWatch.append(lnovoAnime);
            vlistaOnHold.remove(lposicao);
            return true;
        }
        else if(llista.compare("Dropped", Qt::CaseInsensitive) == 0){
            QPointer<anime> lnovoAnime(new anime);
            lnovoAnime = vlistaDropped[lposicao];
            if(rlista.compare("Watching") == 0)
                vlistaWatching.append(lnovoAnime);
            else if(rlista.compare("Completed") == 0)
                vlistaCompleted.append(lnovoAnime);
            else if(rlista.compare("On Hold") == 0)
                vlistaOnHold.append(lnovoAnime);
            else if(rlista.compare("Plan to Watch") == 0)
                vlistaPlanToWatch.append(lnovoAnime);
            vlistaDropped.remove(lposicao);
            return true;
        }
        else if(llista.compare("Plan to Watch", Qt::CaseInsensitive) == 0){
            QPointer<anime> lnovoAnime(new anime);
            lnovoAnime = vlistaPlanToWatch[lposicao];
            if(rlista.compare("Watching") == 0)
                vlistaWatching.append(lnovoAnime);
            else if(rlista.compare("Completed") == 0)
                vlistaCompleted.append(lnovoAnime);
            else if(rlista.compare("On Hold") == 0)
                vlistaOnHold.append(lnovoAnime);
            else if(rlista.compare("Dropped") == 0)
                vlistaDropped.append(lnovoAnime);
            vlistaPlanToWatch.remove(lposicao);
            return true;
        }
    }
    else if(rtipo.compare("novel", Qt::CaseInsensitive) == 0){
        if(llista.compare("Reading(Novel)", Qt::CaseInsensitive) == 0){
            QPointer<anime> lnovoAnime(new anime);
            lnovoAnime = vlistaNovelReading[lposicao];
            if(rlista.compare("Completed(Novel)") == 0)
                vlistaNovelCompleted.append(lnovoAnime);
            else if(rlista.compare("Dropped(Novel)") == 0)
                vlistaNovelDropped.append(lnovoAnime);
            else if(rlista.compare("On Hold(Novel)") == 0)
                vlistaNovelOnHold.append(lnovoAnime);
            else if(rlista.compare("Plan to Read(Novel)") == 0)
                vlistaNovelPlanToRead.append(lnovoAnime);
            vlistaNovelReading.remove(lposicao);
            return true;
        }
        else if(llista.compare("Completed(Novel)", Qt::CaseInsensitive) == 0){
            QPointer<anime> lnovoAnime(new anime);
            lnovoAnime = vlistaNovelCompleted[lposicao];
            if(rlista.compare("Reading(Novel)") == 0)
                vlistaNovelReading.append(lnovoAnime);
            else if(rlista.compare("Dropped(Novel)") == 0)
                vlistaNovelDropped.append(lnovoAnime);
            else if(rlista.compare("On Hold(Novel)") == 0)
                vlistaNovelOnHold.append(lnovoAnime);
            else if(rlista.compare("Plan to Read(Novel)") == 0)
                vlistaNovelPlanToRead.append(lnovoAnime);
            vlistaNovelCompleted.remove(lposicao);
            return true;
        }
        else if(llista.compare("On Hold(Novel)", Qt::CaseInsensitive) == 0){
            QPointer<anime> lnovoAnime(new anime);
            lnovoAnime = vlistaNovelOnHold[lposicao];
            if(rlista.compare("Reading(Novel)") == 0)
                vlistaNovelReading.append(lnovoAnime);
            else if(rlista.compare("Completed(Novel)") == 0)
                vlistaNovelCompleted.append(lnovoAnime);
            else if(rlista.compare("Dropped(Novel)") == 0)
                vlistaNovelDropped.append(lnovoAnime);
            else if(rlista.compare("Plan to Read(Novel)") == 0)
                vlistaNovelPlanToRead.append(lnovoAnime);
            vlistaNovelOnHold.remove(lposicao);
            return true;
        }
        else if(llista.compare("Dropped(Novel)", Qt::CaseInsensitive) == 0){
            QPointer<anime> lnovoAnime(new anime);
            lnovoAnime = vlistaNovelDropped[lposicao];
            if(rlista.compare("Watching(Novel)") == 0)
                vlistaNovelReading.append(lnovoAnime);
            else if(rlista.compare("Completed(Novel)") == 0)
                vlistaNovelCompleted.append(lnovoAnime);
            else if(rlista.compare("On Hold(Novel)") == 0)
                vlistaNovelOnHold.append(lnovoAnime);
            else if(rlista.compare("Plan to Read(Novel)") == 0)
                vlistaNovelPlanToRead.append(lnovoAnime);
            vlistaNovelDropped.remove(lposicao);
            return true;
        }
        else if(llista.compare("Plan to Read(Novel)", Qt::CaseInsensitive) == 0){
            QPointer<anime> lnovoAnime(new anime);
            lnovoAnime = vlistaNovelPlanToRead[lposicao];
            if(rlista.compare("Reading(Novel)") == 0)
                vlistaNovelReading.append(lnovoAnime);
            else if(rlista.compare("Completed(Novel)") == 0)
                vlistaNovelCompleted.append(lnovoAnime);
            else if(rlista.compare("On Hold(Novel)") == 0)
                vlistaNovelOnHold.append(lnovoAnime);
            else if(rlista.compare("Dropped(Novel)") == 0)
                vlistaNovelDropped.append(lnovoAnime);
            vlistaNovelPlanToRead.remove(lposicao);
            return true;
        }
    }
    else if(rtipo.compare("manga", Qt::CaseInsensitive) == 0){
        if(llista.compare("Reading(Manga)", Qt::CaseInsensitive) == 0){
            QPointer<anime> lnovoAnime(new anime);
            lnovoAnime = vlistaMangaReading[lposicao];
            if(rlista.compare("Completed(Manga)") == 0)
                vlistaMangaCompleted.append(lnovoAnime);
            else if(rlista.compare("Dropped(Manga)") == 0)
                vlistaMangaDropped.append(lnovoAnime);
            else if(rlista.compare("On Hold(Manga)") == 0)
                vlistaMangaOnHold.append(lnovoAnime);
            else if(rlista.compare("Plan to Read(Manga)") == 0)
                vlistaMangaPlanToRead.append(lnovoAnime);
            vlistaMangaReading.remove(lposicao);
            return true;
        }
        else if(llista.compare("Completed(Manga)", Qt::CaseInsensitive) == 0){
            QPointer<anime> lnovoAnime(new anime);
            lnovoAnime = vlistaMangaCompleted[lposicao];
            if(rlista.compare("Reading(Manga)") == 0)
                vlistaMangaReading.append(lnovoAnime);
            else if(rlista.compare("Dropped(Manga)") == 0)
                vlistaMangaDropped.append(lnovoAnime);
            else if(rlista.compare("On Hold(Manga)") == 0)
                vlistaMangaOnHold.append(lnovoAnime);
            else if(rlista.compare("Plan to Read(Manga)") == 0)
                vlistaMangaPlanToRead.append(lnovoAnime);
            vlistaMangaCompleted.remove(lposicao);
            return true;
        }
        else if(llista.compare("On Hold(Manga)", Qt::CaseInsensitive) == 0){
            QPointer<anime> lnovoAnime(new anime);
            lnovoAnime = vlistaMangaOnHold[lposicao];
            if(rlista.compare("Reading(Manga)") == 0)
                vlistaMangaReading.append(lnovoAnime);
            else if(rlista.compare("Completed(Manga)") == 0)
                vlistaMangaCompleted.append(lnovoAnime);
            else if(rlista.compare("Dropped(Manga)") == 0)
                vlistaMangaDropped.append(lnovoAnime);
            else if(rlista.compare("Plan to Read(Manga)") == 0)
                vlistaMangaPlanToRead.append(lnovoAnime);
            vlistaMangaOnHold.remove(lposicao);
            return true;
        }
        else if(llista.compare("Dropped(Manga)", Qt::CaseInsensitive) == 0){
            QPointer<anime> lnovoAnime(new anime);
            lnovoAnime = vlistaMangaDropped[lposicao];
            if(rlista.compare("Reading(Manga)") == 0)
                vlistaMangaReading.append(lnovoAnime);
            else if(rlista.compare("Completed(Manga)") == 0)
                vlistaMangaCompleted.append(lnovoAnime);
            else if(rlista.compare("On Hold(Manga)") == 0)
                vlistaMangaOnHold.append(lnovoAnime);
            else if(rlista.compare("Plan to Read(Manga)") == 0)
                vlistaMangaPlanToRead.append(lnovoAnime);
            vlistaMangaDropped.remove(lposicao);
            return true;
        }
        else if(llista.compare("Plan to Read(Manga)", Qt::CaseInsensitive) == 0){
            QPointer<anime> lnovoAnime(new anime);
            lnovoAnime = vlistaMangaPlanToRead[lposicao];
            if(rlista.compare("Reading(Manga)") == 0)
                vlistaMangaReading.append(lnovoAnime);
            else if(rlista.compare("Completed(Manga)") == 0)
                vlistaMangaCompleted.append(lnovoAnime);
            else if(rlista.compare("On Hold(Manga)") == 0)
                vlistaMangaOnHold.append(lnovoAnime);
            else if(rlista.compare("Dropped(Manga)") == 0)
                vlistaMangaDropped.append(lnovoAnime);
            vlistaMangaPlanToRead.remove(lposicao);
            return true;
        }
    }
    return false;
}
