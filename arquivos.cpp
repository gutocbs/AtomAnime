#include "arquivos.h"

arquivos::arquivos(QObject *parent) : QObject(parent)
{
}

bool arquivos::fcomparaDadosAnime(QString rfileName, QString rnomeAnime, QString rnomeAnimeIngles, QStringList rnomesAlternativosAnime,
                                  int repisodioAnime){
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
    //Alguns animes, normalmente filmes e ovas, não tem número de episódio, sendo lido como episódio 0
    //Por esse motivo, é dado o número 1 como número de episódio, assim o programa consegue reconhecer como episódio não visto
    if(lepisodioAnime == 0)
        lepisodioAnime++;
    if(rfileName.compare(rnomeAnime, Qt::CaseInsensitive) == 0 && lepisodioAnime == repisodioAnime+1){
        return true;
    }
    else if(rfileName.compare(rnomeAnimeIngles, Qt::CaseInsensitive) == 0 && lepisodioAnime == repisodioAnime+1){
        return true;
    }
    else{
        for(int i = 0; i < rnomesAlternativosAnime.size(); i++){
            if(rfileName.compare(rnomesAlternativosAnime.at(i), Qt::CaseInsensitive) == 0 && lepisodioAnime == repisodioAnime+1){
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
            if(lchecaSeArquivoOuPasta.isFile() == true && (lfile.fileName().endsWith("mkv") || lfile.fileName().endsWith("mp4"))){
                //Compara o nome do anime e o número do episódio
                if(fcomparaDadosAnime(lit.fileName(), ranimeBuscado->vnome, ranimeBuscado->vnomeIngles, ranimeBuscado->vnomeAlternativo,
                                           ranimeBuscado->vnumEpisodiosAssistidos.toInt()))
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
                                               ranimeBuscado->vnumEpisodiosAssistidos.toInt()))
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
    rnome = rnome.simplified();
    return rnome;
}

bool arquivos::fabreEpisodio(QString rcaminhoArquivo){
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
