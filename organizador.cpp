#include "organizador.h"

Organizador::Organizador(configPC *conf)
{
    this->conf = conf;
    anitomy = new anitomy::Anitomy;
}

//Checa se a pasta já existe
void Organizador::criaPastas(QString caminho){
    //Caso exista, não faz nada
    if(QDir(caminho).exists()){}
    //Se a pasta não existir, ela vai ser criada
    else {
        QDir().mkdir(caminho);
    }
}

void Organizador::mudaArquivo(QString caminho, QString nomeArquivo){
    //Cria e recebe uma string com o caminho do arquivo antes de mudar
    QString caminhoVelho = caminho;
    caminhoVelho.append(nomeArquivo);

    //Escolhe o nome da pasta
    caminho.append(retornaNomeAnime(nomeArquivo));

    //Cria a pasta onde o novo arquivo vai ficar
    criaPastas(caminho);
    caminho.append("/");

    //Entra na pasta criada e prepara a mudança do anime
    caminho.append(nomeArquivo);

    //Muda o arquivo de lugar
    QDir diretorioNovo;
    diretorioNovo.rename(caminhoVelho, caminho);
}

//Diz qual o número do episódio do arquivo que está sendo lido
QString Organizador::retornaEpisodeNumber(QString nome){
    anitomy::Anitomy anitomy;
    anitomy.Parse(nome.toStdWString());
    const auto& elements = anitomy.elements();
    std::wstring a = elements.get(anitomy::kElementEpisodeNumber);
    std::string b(a.begin(), a.end());
    return QString::fromStdString(b);
}

//Pega o nome de um arquivo e retorna apenas o nome do anime
QString Organizador::retornaNomeAnime(QString nome){
    anitomy::Anitomy anitomy;
    anitomy.Parse(nome.toStdWString());
    const auto& elements = anitomy.elements();
    std::wstring a = elements.get(anitomy::kElementAnimeTitle);
    std::string b(a.begin(), a.end());
    return QString::fromStdString(b);
}

//Checa quantos episódios tem na pasta
int Organizador::retornaNumEpiNaPasta(int numEpisodios, int animeAtual){//Vou usar o anime atual depois, para buscar pastas de animes especificos
    QDirIterator it(conf->RetornaDiretorioAnimeEspecifico(animeAtual));
    while(it.hasNext()){
        QFile f(it.next());
        if(retornaEpisodeNumber(f.fileName()) != "" && (f.fileName().right(3) == "mkv" || f.fileName().right(3) == "mp4")){
            numEpisodios++;
        }
    }
    return numEpisodios;
}

void Organizador::MoveArquivoDeDownloadPraPasta(QString pastaDownloads, QString nome){
    //Abre a pasta onde o anime foi baixado
    QDirIterator it(pastaDownloads);
    while(it.hasNext()){
        QFile f(it.next());
        if(retornaNomeAnime(f.fileName().mid(f.fileName().lastIndexOf(QChar('/'))+1)).compare(nome) == 0 && (f.fileName().right(3) == "mkv" || f.fileName().right(3) == "mp4")){
            mudaArquivo(pastaDownloads, f.fileName().mid(f.fileName().lastIndexOf(QChar('/'))+1));
            break;
        }
    }
}

void Organizador::abre(QString caminho){
    QDesktopServices::openUrl(QUrl("file:///"+caminho,QUrl::TolerantMode));
}

int Organizador::AbreArquivo(int progresso, int id, QString nome, QString nome2)
{
    QString nomeAlternativo;
    nomeAlternativo = nome;
    //Primeiro verifica se o anime tem uma pasta específica definida
    QString direct = conf->RetornaDiretorioAnimeEspecifico(id);
    if(direct != "0"){
        QDirIterator it(direct, QDirIterator::Subdirectories);
        //Lê o nome de todos os episódisos da pasta
        while(it.hasNext()){
            QFile f(it.next());
            if(retornaNomeAnime(f.fileName().mid(f.fileName().lastIndexOf(QChar('/'))+1)).toLower().contains(nome.toLower()) || nome.toLower().contains(retornaNomeAnime(f.fileName().mid(f.fileName().lastIndexOf(QChar('/'))+1)).toLower()) || retornaNomeAnime(f.fileName().mid(f.fileName().lastIndexOf(QChar('/'))+1)).toLower().contains(nome2.toLower()) || nome2.toLower().contains(retornaNomeAnime(f.fileName().mid(f.fileName().lastIndexOf(QChar('/'))+1)).toLower())){
                nomeAlternativo = retornaNomeAnime(f.fileName().mid(f.fileName().lastIndexOf(QChar('/'))+1));
            }
            if(nome == retornaNomeAnime(f.fileName().mid(f.fileName().lastIndexOf(QChar('/'))+1)) || nomeAlternativo == retornaNomeAnime(f.fileName().mid(f.fileName().lastIndexOf(QChar('/'))+1))){
                if(retornaEpisodeNumber(f.fileName()).toInt() == progresso+1  && (f.fileName().right(3) == "mkv" || f.fileName().right(3) == "mp4")){
                    QDesktopServices::openUrl(QUrl("file:///"+f.fileName(),QUrl::TolerantMode));
                    return 0;
                }
            }
        }
    }
    //Caso não tenha pasta específica, começa a procurar por todas as pastas, vendo qual é a pasta do anime certo
    for(int i = 0; i < conf->diretorioAnimes.length(); i++){
//        qDebug() << "veio pra cá?2";
        QDirIterator iteradorPasta(conf->diretorioAnimes[i], QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        while(iteradorPasta.hasNext()){
            //Iterador
            iteradorPasta.next();

            //Parse no nome da pasta
            anitomy->Parse(iteradorPasta.fileName().toStdWString());
            const auto& elements = anitomy->elements();
            std::wstring a = elements.get(anitomy::kElementAnimeTitle);

            //Compara o nome da pasta com o nome do anime
            if(QString::fromStdWString(a).toLower().contains(nome.toLower()) || nome.toLower().contains(QString::fromStdWString(a).toLower())) {
                QDirIterator it(iteradorPasta.filePath(), QDirIterator::Subdirectories);
                //Lê o nome de toods os episódisos da pasta
                while(it.hasNext()){
                    QFile f(it.next());
                    //Caso ache o episódio que eu quero, o próximo após o progresso
                    if(retornaNomeAnime(f.fileName().mid(f.fileName().lastIndexOf(QChar('/'))+1)).toLower().contains(nome.toLower()) || nome.toLower().contains(retornaNomeAnime(f.fileName().mid(f.fileName().lastIndexOf(QChar('/'))+1)).toLower())){
                        nomeAlternativo = retornaNomeAnime(f.fileName().mid(f.fileName().lastIndexOf(QChar('/'))+1));
                    }
                    if(nome == retornaNomeAnime(f.fileName().mid(f.fileName().lastIndexOf(QChar('/'))+1)) || nomeAlternativo == retornaNomeAnime(f.fileName().mid(f.fileName().lastIndexOf(QChar('/'))+1))){
                        if(retornaEpisodeNumber(f.fileName()).toInt() == progresso+1  && (f.fileName().right(3) == "mkv" || f.fileName().right(3) == "mp4")){
                            QDesktopServices::openUrl(QUrl("file:///"+f.fileName(),QUrl::TolerantMode));
                            return 0;
                        }
                    }
                }
            }
        }
        //Começa a procurar por arquivos soltos dentro da pasta
        QDirIterator it(iteradorPasta.filePath(), QDir::Files);
        //Lê o nome de toods os episódisos da pasta
//        qDebug() << "ops";
        while(it.hasNext()){
            QFile f(it.next());
            //Caso ache o episódio que eu quero, o próximo após o progresso
            if(retornaNomeAnime(f.fileName().mid(f.fileName().lastIndexOf(QChar('/'))+1)).toLower().contains(nome.toLower()) || nome.toLower().contains(retornaNomeAnime(f.fileName().mid(f.fileName().lastIndexOf(QChar('/'))+1)).toLower())){
                nomeAlternativo = retornaNomeAnime(f.fileName().mid(f.fileName().lastIndexOf(QChar('/'))+1));
            }
            if(nome == retornaNomeAnime(f.fileName().mid(f.fileName().lastIndexOf(QChar('/'))+1)) || nomeAlternativo == retornaNomeAnime(f.fileName().mid(f.fileName().lastIndexOf(QChar('/'))+1))){
                if(retornaEpisodeNumber(f.fileName()).toInt() == progresso+1  && (f.fileName().right(3) == "mkv" || f.fileName().right(3) == "mp4")){
                    QDesktopServices::openUrl(QUrl("file:///"+f.fileName(),QUrl::TolerantMode));
                    return 0;
                }
            }
        }
    }
    return 1;
}
