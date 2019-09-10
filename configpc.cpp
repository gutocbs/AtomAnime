#include "configpc.h"

configPC::configPC()
{
    anitomy = new anitomy::Anitomy;
    diretorioImagensMedio = "Configurações/Imagens/Medio/";
    diretorioImagensGrandes = "Configurações/Imagens/Grande/";
    diretorioAnimes.append("E:/Animes");
    diretorioAnimes.append(QDir::homePath() + "/Downloads/Animes");
    diretorioTaiga = QDir::homePath() + "/AppData/Roaming/Taiga/data";
    CriaPastasBase();
}

void configPC::CriaPastasBase(){
    if(!QDir("Configurações").exists()){
        QDir().mkdir("Configurações");
    }
    if(!QDir("Configurações/Imagens").exists()){
        QDir().mkdir("Configurações/Imagens");
    }
    if(!QDir("Configurações/Imagens/Medio").exists()){
        QDir().mkdir("Configurações/Imagens/Medio");
    }
    if(!QDir("Configurações/Imagens/Grande").exists()){
        QDir().mkdir("Configurações/Imagens/Grande");
    }
}


void configPC::EscreveArquivo(){
    QString filename = "Configurações/conf.txt";
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        for(int i = 0; i < diretorioAnimes.length(); i++){
            stream << "D01>diretorioAnime[" << i << "]>" << diretorioAnimes[i] << endl;
        }
        for(int i = 0; i < diretorioAnimeEspecifico.length(); i++){
            stream << "D02>" << idAnime[i] << ">" << diretorioAnimeEspecifico[i] << endl;
        }
    }
}


std::string configPC::RetornaDiretorioTaiga(){
    return diretorioTaiga.toStdString();
}

int configPC::BuscaPastasAnimesEspecificos(QString nome, QString nomeAlternativo, QString id){
    //Busca todos os diretórios posíveis onde tem animes
    //pro caso de ter mais de um diretório
    //Ex: downloads/animes/ + e:/animes/
    QString nomeAnime = nome.toLower();
    QString nome2Anime = nomeAlternativo.toLower();
    QString nomeArquivoPelado;
//    nomeAnime.remove("-");
    nomeAnime.remove(".");
    nomeAnime.remove("?");
    nome2Anime.remove(".");
    nome2Anime.remove("?");
    QString pelado1, pelado2;
    for(int i = 0; i < diretorioAnimes.length(); i++){
        //Busca diretórios apenas
        QDirIterator it(diretorioAnimes[i], QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        while(it.hasNext()){
            //Iterador
            it.next();
            //Parse no nome da pasta
            anitomy->Parse(it.fileName().toStdWString());
            const auto& elements = anitomy->elements();
            std::wstring a = elements.get(anitomy::kElementAnimeTitle);
            nomeArquivoPelado = QString::fromStdWString(a).toLower();
            nomeArquivoPelado.remove(".");
            nomeArquivoPelado.remove("?");
            nomeArquivoPelado.remove("S1");
            nomeArquivoPelado.remove("S2");
            nomeArquivoPelado.remove("s1");
            nomeArquivoPelado.remove("s2");
            nomeArquivoPelado.remove("s01");
            nomeArquivoPelado.remove("s02");
            nomeArquivoPelado.remove("S01");
            nomeArquivoPelado.remove("S02");
            nomeArquivoPelado = nomeArquivoPelado.simplified();
            //Compara o nome da pasta com o nome do anime
            //Compara os dois nomes em letras minusculas, pra evitar erros
            if(nomeAnime.contains(nomeArquivoPelado) == true || nome2Anime.contains(nomeArquivoPelado) == true){// || pelado2.compare(nomeAnime) == 0 || pelado2.compare(nome2Anime) == 0){
                idAnime.append(id);
                diretorioAnimeEspecifico.append(it.filePath());
                return 0;
            }
        }
    }
    return 0;
}

//USAR ESSA FUNÇÃO NA HORA DE ABRIR O EPISÓDIO
//SEMPRE CHECAR SE EXISTE DIRETORIO ESPECIFICO ANTES DE PROCURAR
QString configPC::RetornaDiretorioAnimeEspecifico(int id){
    QFile inputFile("Configurações/conf.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       QStringList linha;
       while (!in.atEnd())
       {
           linha = in.readLine().split('>');
           if(linha.at(0).compare("D02") == 0)
               if(linha.at(1).toInt() == id){
                   return linha.at(2);
               }
       }
       inputFile.close();
    }
    return "0";
}

int configPC::RetornaTamanhoVector(){
    return diretorioAnimeEspecifico.length();
}
