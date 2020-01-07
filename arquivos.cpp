#include "arquivos.h"

arquivos::arquivos(QObject *parent) : QObject(parent)
{
}

bool arquivos::fcomparaDadosAnime(QString rfileName, QString rnomeAnime, QString rnomeAnimeIngles, QStringList rnomesAlternativosAnime,
                                  int repisodioAnime, int rtemporada){
    //Anitomy é uma classe linda que separa os elementos de uma string
    anitomy::Anitomy lanitomy;
    lanitomy.Parse(rfileName.toStdWString());
    const auto& lelements = lanitomy.elements();
    //Usamos isso para pegar o nome do anime a partir do nome do arquivo
    rfileName = QString::fromStdWString(lelements.get(anitomy::kElementAnimeTitle));
    //E pegar o número do episódio do arquivo
    int lepisodioAnime = QString::fromStdWString(lelements.get(anitomy::kElementEpisodeNumber)).toInt();
    lanitomy.Parse(rnomeAnime.toStdWString());
    //Alguns animes tem títulos complicados, usando Season 2 ou 2 no título
    //Anitomy, nesse caso, remove tudo o que não é essencial ao nome
    rnomeAnime = QString::fromStdWString(lelements.get(anitomy::kElementAnimeTitle));
    //Após isso, simplificamos o nome de tudo, removendo tudo o que pode causar falhas na comparação e não é importante para o título
    rfileName = fremoveCaracteresDiferentes(rfileName);
    rnomeAnime = fremoveCaracteresDiferentes(rnomeAnime);
    rnomeAnimeIngles = fremoveCaracteresDiferentes(rnomeAnimeIngles);
    int repisodiosTotais = 0;
    if(rtemporada != 1)
        repisodiosTotais = fcomparaSeasons(rnomeAnime,lepisodioAnime, rtemporada);
    //Alguns animes, normalmente filmes e ovas, não tem número de episódio, sendo lido como episódio 0
    //Por esse motivo, é dado o número 1 como número de episódio, assim o programa consegue reconhecer como episódio não visto
    if(lepisodioAnime == 0)
        lepisodioAnime++;
    //Episódios totais é a variável que conta todos os episódios do anime, em todas as seasons. Caso algum sub coloque, por exemplo
    //One Piece episódio 201, ele ainda vai ser lido e saberemos qual o episódio/temporada certa.
    if(rfileName.compare(rnomeAnime, Qt::CaseInsensitive) == 0 && (lepisodioAnime == repisodioAnime+1 ||
                                                                   lepisodioAnime - repisodiosTotais == repisodioAnime+1)){
        return true;
    }
    else if(rfileName.compare(rnomeAnimeIngles, Qt::CaseInsensitive) == 0 && (lepisodioAnime == repisodioAnime+1 ||
            lepisodioAnime - repisodiosTotais == repisodioAnime+1)){
        return true;
    }
    else{
        for(int i = 0; i < rnomesAlternativosAnime.size(); i++){
            if(rfileName.compare(rnomesAlternativosAnime.at(i), Qt::CaseInsensitive) == 0 && (lepisodioAnime == repisodioAnime+1 ||
                                                                                lepisodioAnime - repisodiosTotais == repisodioAnime+1)){
                return true;
            }
        }
    }
    return false;
}

QString arquivos::fprocuraEpisodio(anime *ranimeBuscado){
    //Verifica se a função retorna um valor que não está vazio, ou seja
    //Se existe uma pasta com o nome do anime
    if(!cconfUsuario->fretornaDiretorioEspecifico(ranimeBuscado->vid.toInt()).isEmpty()){
        //Começa a iterar a pasta em busca das pastas de animes
        QDirIterator lit(cconfUsuario->fretornaDiretorioEspecifico(ranimeBuscado->vid.toInt()), QDirIterator::Subdirectories);
        while(lit.hasNext()){
            QFile lfile(lit.next());
            QFileInfo lchecaSeArquivoOuPasta(lfile.fileName());
            //Checa se o que foi encontrado é um arquivo ou uma pasta e, no caso de ser um arquivo, se é um arquivo de vídeo
            if(lchecaSeArquivoOuPasta.isFile() == true && (lfile.fileName().endsWith("mkv", Qt::CaseInsensitive) ||
                                                           lfile.fileName().endsWith("mp4", Qt::CaseInsensitive))){
                //Compara o nome do anime e o número do episódio
                if(fcomparaDadosAnime(lit.fileName(), ranimeBuscado->vnome, ranimeBuscado->vnomeIngles, ranimeBuscado->vnomeAlternativo,
                                           ranimeBuscado->vnumEpisodiosAssistidos.toInt(), ranimeBuscado->vtemporada))
                    return lfile.fileName();
            }
        }
    }
    else{
        //Começa a iterar a pasta em busca das pastas de animes
        for(int i = 0; i < cconfUsuario->fretornaDiretoriosAnimes().size(); i++){
            QDirIterator lit(cconfUsuario->fretornaDiretoriosAnimes().at(i), QDir::Files);
            while(lit.hasNext()){
                QFile lfile(lit.next());
                QFileInfo lchecaSeArquivoOuPasta(lfile.fileName());
                //Checa se o que foi encontrado é um arquivo ou uma pasta e, no caso de ser um arquivo, se é um arquivo de vídeo
                if(lchecaSeArquivoOuPasta.isFile() == true && (lfile.fileName().endsWith("mkv", Qt::CaseInsensitive) ||
                                                               lfile.fileName().endsWith("mp4", Qt::CaseInsensitive))){
                    //Compara o nome do anime e o número do episódio
                    if(fcomparaDadosAnime(lit.fileName(), ranimeBuscado->vnome, ranimeBuscado->vnomeIngles, ranimeBuscado->vnomeAlternativo,
                                               ranimeBuscado->vnumEpisodiosAssistidos.toInt(), ranimeBuscado->vtemporada))
                        return lfile.fileName();
                }
            }
        }
    }
    return "";
}

QString arquivos::fprocuraUltimoEpisodio(anime *ranimeBuscado, QString repisodio){
    //Verifica se a função retorna um valor que não está vazio, ou seja
    //Se existe uma pasta com o nome do anime
        if(!cconfUsuario->fretornaDiretorioEspecifico(ranimeBuscado->vid.toInt()).isEmpty()){
        //Começa a iterar a pasta em busca das pastas de animes
        QDirIterator lit(cconfUsuario->fretornaDiretorioEspecifico(ranimeBuscado->vid.toInt()), QDirIterator::Subdirectories);
        while(lit.hasNext()){
            QFile lfile(lit.next());
            QFileInfo lchecaSeArquivoOuPasta(lfile.fileName());
            //Checa se o que foi encontrado é um arquivo ou uma pasta e, no caso de ser um arquivo, se é um arquivo de vídeo
            if(lchecaSeArquivoOuPasta.isFile() == true && (lfile.fileName().endsWith("mkv") || lfile.fileName().endsWith("mp4"))){
                //Compara o nome do anime e o número do episódio
                if(fcomparaDadosAnime(lit.fileName(), ranimeBuscado->vnome, ranimeBuscado->vnomeIngles, ranimeBuscado->vnomeAlternativo,
                                           repisodio.toInt()-1, ranimeBuscado->vtemporada))
                    return lfile.fileName();
            }
        }
    }
    else{
        //Começa a iterar a pasta em busca das pastas de animes
        for(int i = 0; i < cconfUsuario->fretornaDiretoriosAnimes().size(); i++){
            QDirIterator lit(cconfUsuario->fretornaDiretoriosAnimes().at(i), QDir::Files);
            while(lit.hasNext()){
                QFile lfile(lit.next());
                QFileInfo lchecaSeArquivoOuPasta(lfile.fileName());
                //Checa se o que foi encontrado é um arquivo ou uma pasta e, no caso de ser um arquivo, se é um arquivo de vídeo
                if(lchecaSeArquivoOuPasta.isFile() == true && (lfile.fileName().endsWith("mkv") || lfile.fileName().endsWith("mp4"))){
                    //Compara o nome do anime e o número do episódio
                    if(fcomparaDadosAnime(lit.fileName(), ranimeBuscado->vnome, ranimeBuscado->vnomeIngles, ranimeBuscado->vnomeAlternativo,
                                               repisodio.toInt()-1, ranimeBuscado->vtemporada))
                        return lfile.fileName();
                }
            }
        }
    }
    return "";
}

QString arquivos::fremoveCaracteresDiferentes(QString rnome)
{
    rnome = rnome.toLower();
    rnome.remove(".");
    rnome.remove("?");
    rnome.remove("!");
    rnome.remove(":");
    rnome.remove("-");
    rnome.remove("s1");
    rnome.remove("s2");
    rnome.remove("s3");
    rnome.remove("s4");
    rnome.remove("s01");
    rnome.remove("s02");
    rnome.remove("s03");
    rnome.remove("s04");
    rnome.remove("season 1");
    rnome.remove("season 2");
    rnome.remove("season 3");
    rnome.remove("season 4");
    rnome.remove("1");
    rnome.remove("2");
    rnome.remove("3");
    rnome.remove("4");
    rnome = rnome.simplified();
    return rnome;
}

bool arquivos::fabreEpisodio(QString rcaminhoArquivo){
    qDebug() << rcaminhoArquivo;
    if(!rcaminhoArquivo.isEmpty()){
        QDesktopServices::openUrl(QUrl("file:///"+rcaminhoArquivo,QUrl::TolerantMode));
        return true;
    }
    return false;
}

void arquivos::frecebeDiretorios(confUsuario *rconfUsuario)
{
    cconfUsuario = rconfUsuario;
}

void arquivos::frecebeAnimes(QVector<anime *> vlistaWatching, QVector<anime *> vlistaCompleted, QVector<anime *> vlistaOnHold,
                             QVector<anime *> vlistaDropped, QVector<anime *> vlistaPlanToWatch)
{

    this->vlistaWatching = vlistaWatching;
    this->vlistaCompleted = vlistaCompleted;
    this->vlistaOnHold = vlistaOnHold;
    this->vlistaDropped = vlistaDropped;
    this->vlistaPlanToWatch = vlistaPlanToWatch;
}

int arquivos::fcomparaSeasons(QString rnome, int repisodio, int rtemporada)
{
    QString lnomeSimplificado;
    int lepisodiosTotais = 0;
    //Mano, confuso. Basicamente, eu comparo o nome do anime com o nome dos outros animes na lista.
    //Se forem o mesmo anime, checo se são animes ou ovas/filmes.
    //Se forem animes, checo se a temporada é diferente da que eu busco, e caso for menor, eu só somo todos os episódios
    //Pra obter o número total de episódios
    for(int i = 0; i < vlistaWatching.size(); i++){
        lnomeSimplificado = fremoveCaracteresDiferentes(vlistaWatching[i]->vnome);
        if(vlistaWatching[i]->vnome.contains(rnome, Qt::CaseInsensitive) && vlistaWatching[i]->vtemporada < rtemporada
                && vlistaWatching[i]->vformato == "TV"){ //DUVIDA NISSO DA TV. SERÁ QUE OVAS CONTAM PRO INDEX DO HORRIBLE?
            if(vlistaWatching[i]->vnumEpisodiosTotais.toInt() != 0){
                if(lepisodiosTotais+vlistaWatching[i]->vnumEpisodiosTotais.toInt() < repisodio){
                    lepisodiosTotais += vlistaWatching[i]->vnumEpisodiosTotais.toInt();
                }
                else
                    return lepisodiosTotais;
            }
        }
    }
    for(int i = 0; i < vlistaCompleted.size(); i++){
        lnomeSimplificado = fremoveCaracteresDiferentes(vlistaCompleted[i]->vnome);
        if(vlistaCompleted[i]->vnome.contains(rnome, Qt::CaseInsensitive) && vlistaCompleted[i]->vtemporada < rtemporada
                && vlistaCompleted[i]->vformato == "TV"){ //DUVIDA NISSO DA TV. SERÁ QUE OVAS CONTAM PRO INDEX DO HORRIBLE?
            if(vlistaCompleted[i]->vnumEpisodiosTotais.toInt() != 0){
                if(lepisodiosTotais+vlistaCompleted[i]->vnumEpisodiosTotais.toInt() < repisodio){
                    lepisodiosTotais += vlistaCompleted[i]->vnumEpisodiosTotais.toInt();
                }
                else
                    return lepisodiosTotais;
            }
        }
    }
    for(int i = 0; i < vlistaOnHold.size(); i++){
        lnomeSimplificado = fremoveCaracteresDiferentes(vlistaOnHold[i]->vnome);
        if(vlistaOnHold[i]->vnome.contains(rnome, Qt::CaseInsensitive) && vlistaOnHold[i]->vtemporada < rtemporada
                && vlistaOnHold[i]->vformato == "TV"){ //DUVIDA NISSO DA TV. SERÁ QUE OVAS CONTAM PRO INDEX DO HORRIBLE?
            if(vlistaOnHold[i]->vnumEpisodiosTotais.toInt() != 0){
                if(lepisodiosTotais+vlistaOnHold[i]->vnumEpisodiosTotais.toInt() < repisodio){
                    lepisodiosTotais += vlistaOnHold[i]->vnumEpisodiosTotais.toInt();
                }
                else
                    return lepisodiosTotais;
            }
        }
    }
    for(int i = 0; i < vlistaDropped.size(); i++){
        lnomeSimplificado = fremoveCaracteresDiferentes(vlistaDropped[i]->vnome);
        if(vlistaDropped[i]->vnome.contains(rnome, Qt::CaseInsensitive) && vlistaDropped[i]->vtemporada < rtemporada
                && vlistaDropped[i]->vformato == "TV"){ //DUVIDA NISSO DA TV. SERÁ QUE OVAS CONTAM PRO INDEX DO HORRIBLE?
            if(vlistaDropped[i]->vnumEpisodiosTotais.toInt() != 0){
                if(lepisodiosTotais+vlistaDropped[i]->vnumEpisodiosTotais.toInt() < repisodio){
                    lepisodiosTotais += vlistaDropped[i]->vnumEpisodiosTotais.toInt();
                }
                else
                    return lepisodiosTotais;
            }
        }
    }
    for(int i = 0; i < vlistaPlanToWatch.size(); i++){
        lnomeSimplificado = fremoveCaracteresDiferentes(vlistaPlanToWatch[i]->vnome);
        if(vlistaPlanToWatch[i]->vnome.contains(rnome, Qt::CaseInsensitive) && vlistaPlanToWatch[i]->vtemporada < rtemporada
                && vlistaPlanToWatch[i]->vformato == "TV"){ //DUVIDA NISSO DA TV. SERÁ QUE OVAS CONTAM PRO INDEX DO HORRIBLE?
            if(vlistaPlanToWatch[i]->vnumEpisodiosTotais.toInt() != 0){
                if(lepisodiosTotais+vlistaPlanToWatch[i]->vnumEpisodiosTotais.toInt() < repisodio){
                    lepisodiosTotais += vlistaPlanToWatch[i]->vnumEpisodiosTotais.toInt();
                }
                else
                    return lepisodiosTotais;
            }
        }
    }

    return lepisodiosTotais;
}
