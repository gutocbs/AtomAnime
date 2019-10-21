#include "confusuario.h"

confUsuario::confUsuario(QObject *parent) : QObject(parent)
{
    vdiretorioAnimes.append("E:/Animes/");
    vdiretorioAnimes.append("E:/Downloads/");
}


QString confUsuario::fretornaDiretorioEspecifico(int id){
    return vdiretorioEspecificoAnime[QString::number(id)];
}

QVector<QString> confUsuario::fretornaDiretoriosAnimes(){
    return vdiretorioAnimes;
}

QString confUsuario::fsimplificaNomeArquivo(QString rnomeArquivo){
    anitomy::Anitomy lanitomy;
    lanitomy.Parse(rnomeArquivo.toStdWString());
    const auto& lelements = lanitomy.elements();
    rnomeArquivo = QString::fromStdWString(lelements.get(anitomy::kElementAnimeTitle));
    rnomeArquivo = rnomeArquivo.toLower();
    rnomeArquivo.remove(".");
    rnomeArquivo.remove("?");
    rnomeArquivo.remove(":");
    rnomeArquivo.remove("-");

    rnomeArquivo.remove("s1");
    rnomeArquivo.remove("s2");
    rnomeArquivo.remove("s3");
    rnomeArquivo.remove("s4");
    rnomeArquivo.remove("s01");
    rnomeArquivo.remove("s02");
    rnomeArquivo.remove("s03");
    rnomeArquivo.remove("s04");
    rnomeArquivo.remove("season 1");
    rnomeArquivo.remove("season 2");
    rnomeArquivo.remove("season 3");
    rnomeArquivo.remove("season 4");
    rnomeArquivo = rnomeArquivo.simplified();
    return rnomeArquivo;
}

void confUsuario::fbuscaDiretoriosAnimes(){
    qDebug() << "Searching for anime folders";
    //Busca cada diretorio existente nas configurações
    for(int i = 0; i < vdiretorioAnimes.size(); i++){
        //Procura apenas por diretorios e subdiretorios
        QDirIterator lit(vdiretorioAnimes[i], QDir::Dirs| QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        while(lit.hasNext()){
            QFile lfile(lit.next());
            //Pega o nome de cada diretorio e divide, pegando apenas o nome do anime
            QString lnomeAnime = fsimplificaNomeArquivo(lit.fileName());
            //Compara o nome do anime com os animes da lista
            for(int w = 0; w < vlistaAnimes.size(); w++){
                //Anitomy, aqui, simplifica os nomes dos anime para garantir que a função vai comparar os nomes certos
                QString lnomeAnimeLista = fsimplificaNomeArquivo(vlistaAnimes[w]->vnome);
                QString lnomeInglesAnimeLista = fsimplificaNomeArquivo(vlistaAnimes[w]->vnomeIngles);
                //Compara os nomes, sempre ignorando letras maiusculas e minusculas para garantir que o anime será encontrado.
                if(lnomeAnime.compare(lnomeAnimeLista,Qt::CaseInsensitive) == 0){
                    vdiretorioEspecificoAnime.insert(vlistaAnimes[w]->vid, lfile.fileName());
                    vlistaAnimes.remove(w);
                    break;
                }
                else if(lnomeAnime.compare(lnomeInglesAnimeLista, Qt::CaseInsensitive) == 0){
                    vdiretorioEspecificoAnime.insert(vlistaAnimes[w]->vid, lfile.fileName());
                    vlistaAnimes.remove(w);
                    break;
                }
                else{
                    //Compara os nomes alternativos dos animes, pro caso de serem usados nos arquivos
                    //Ex: Okaa-san Online em vez de Tsuujou Kougeki ga Zentai Kougeki de Ni-kai Kougeki no Okaasan wa Suki Desu ka?
                    for(int z = 0; z < vlistaAnimes[w]->vnomeAlternativo.size(); z++){
                        QString lnomeAlternativoAnimeLista = fsimplificaNomeArquivo(vlistaAnimes[w]->vnomeAlternativo.at(z));
                        if(lnomeAnime.compare(lnomeAlternativoAnimeLista, Qt::CaseInsensitive) == 0){
                            vdiretorioEspecificoAnime.insert(vlistaAnimes[w]->vid, lfile.fileName());
                            vlistaAnimes.remove(w);
                            break;
                        }
                    }
                }
            }
        }
    }
}

void confUsuario::frecebeListaAnime(QVector<anime *> rlistaAnime)
{
    vlistaAnimes = rlistaAnime;
}
