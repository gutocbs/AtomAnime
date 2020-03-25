#include "checainfoplayer.h"

ChecaInfoPlayer::ChecaInfoPlayer(QObject *parent) : QObject(parent)
{
}

void ChecaInfoPlayer::fgetStreamLanguages()
{
    janeladeconfig j;
    vStreamLanguages.append(j.fretornaStreamLanguages());
}

///fchecaStream(QString player, QString janela)
void ChecaInfoPlayer::fchecaStream(QString player, QString janela)
{
    fgetStreamLanguages();
    if(!janela.isEmpty()){
        vnomeDaJanela = janela;
        if(player.compare("Crunchyroll", Qt::CaseInsensitive) == 0)
            fchecaCrunchyroll();
        else if(player.compare("AnimeLab", Qt::CaseInsensitive) == 0)
            fchecaAnimeLab();
        else if(player.compare("Funimation", Qt::CaseInsensitive) == 0)
            fchecaFunimation();
        else if(player.compare("KissAnime", Qt::CaseInsensitive) == 0)
            fchecaKissAnime();
        else
            fchecaLocalPlayer();
    }
    qDebug() << vanime << vepisodio;
}

QString ChecaInfoPlayer::fretornaAnime()
{
    return vanime;
}

QString ChecaInfoPlayer::fretornaEpisodio()
{
    return vepisodio;
}

void ChecaInfoPlayer::fchecaCrunchyroll()
{
    if(!vStreamLanguages.isEmpty()){
        vnomeDaJanela.remove(vStreamLanguages.at(0));
    }
    vnomeDaJanela = vnomeDaJanela.simplified();
    QStringList nomeJanela = vnomeDaJanela.split(QRegExp("\\b"));
    int indexOfEpisode = 0;
    if(!vStreamLanguages.isEmpty()){
        for(int i = 1; i < vStreamLanguages.size(); i++){
            if(nomeJanela.contains(vStreamLanguages.at(i)) && vStreamLanguages.at(i).compare("الحلقة") != 0){
                indexOfEpisode = nomeJanela.indexOf(vStreamLanguages.at(i));
                vepisodio = nomeJanela.at(indexOfEpisode+2);
                break;
            }
            else if(nomeJanela.contains(vStreamLanguages.at(i)) && vStreamLanguages.at(i).compare("الحلقة") == 0){
                indexOfEpisode = nomeJanela.indexOf(vStreamLanguages.at(i));
                vepisodio = nomeJanela.at(indexOfEpisode-2);
                break;
            }
        }
    }
    int tamanho = nomeJanela.size();
    for(int i = indexOfEpisode; i < tamanho; i++){
        nomeJanela.removeAt(indexOfEpisode);
    }
    vanime = nomeJanela.join("");
    vanime = vanime.simplified();
}

void ChecaInfoPlayer::fchecaAnimeLab()
{
    vnomeDaJanela = vnomeDaJanela.simplified();
    QStringList nomeJanela = vnomeDaJanela.split("-");
    QStringList nomeAnime = nomeJanela.at(0).split(":");

    vanime = nomeAnime.at(0);
    vanime = vanime.simplified();
    vepisodio= nomeAnime.at(1);
    vepisodio.remove("E");
    vepisodio = vepisodio.simplified();
}

void ChecaInfoPlayer::fchecaFunimation()
{
    vnomeDaJanela = vnomeDaJanela.simplified();
    QStringList nomeJanela = vnomeDaJanela.split(QRegExp("\\b"));
    int indexOfEpisode = 0;
    if(nomeJanela.contains("Episode")){
        indexOfEpisode = nomeJanela.indexOf("Episode");
        vepisodio = nomeJanela.at(indexOfEpisode+1);
    }
    int tamanho = nomeJanela.size();
    for(int i = indexOfEpisode; i < tamanho; i++){
        nomeJanela.removeAt(indexOfEpisode);
    }
    vanime = nomeJanela.join(" ");
    vanime = vanime.simplified();
}

void ChecaInfoPlayer::fchecaKissAnime()
{
    vnomeDaJanela.remove("online in high quality", Qt::CaseInsensitive);
    vnomeDaJanela.remove("- Watch", Qt::CaseInsensitive);
    vnomeDaJanela = vnomeDaJanela.simplified();
    QStringList nomeJanela = vnomeDaJanela.split(QRegExp("\\b"));
    nomeJanela.removeDuplicates();
    int indexOfEpisode = 0;
    if(nomeJanela.contains("Episode")){
        indexOfEpisode = nomeJanela.indexOf("Episode");
        vepisodio = nomeJanela.at(indexOfEpisode+1);
    }
    int tamanho = nomeJanela.size();
    for(int i = indexOfEpisode; i < tamanho; i++){
        nomeJanela.removeAt(indexOfEpisode);
    }
    //as vezes o episódio é 000001. Assim, ele converte o nome pra ficar certo.
    int episodio = vepisodio.toInt();
    vepisodio = QString::number(episodio);
    vanime = nomeJanela.join(" ");
    vanime.remove("( Sub )");
    vanime = vanime.simplified();
}

void ChecaInfoPlayer::fchecaLocalPlayer()
{
    anitomy::Anitomy lanitomy;
    lanitomy.Parse(vnomeDaJanela.toStdWString());
    const auto& lelements = lanitomy.elements();
    vanime = QString::fromStdWString(lelements.get(anitomy::kElementAnimeTitle));
    vepisodio = QString::fromStdWString(lelements.get(anitomy::kElementEpisodeNumber));
}
