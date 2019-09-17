#include "leitorarquivos.h"


leitorarquivos::leitorarquivos(){
}

leitorarquivos::~leitorarquivos(){
}

void leitorarquivos::leLinha(QString lista)
{
    QString watching = "Configurações/confTaiga.txt";
    if(lista == "completed"){
        watching = "Configurações/confCompleted.txt";
    }
    else if(lista == "onhold"){
        watching = "Configurações/confOnHold.txt";
    }
    else if(lista == "dropped"){
        watching = "Configurações/confDropped.txt";
    }
    else if(lista == "plantowatch"){
        watching = "Configurações/confPlanToWatch.txt";
    }
    NomeLista = 0;
    QFile inputFile(watching);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
            nomeAnime.append(in.readLine());
            nomeIngles.append(in.readLine());
            progresso.append(in.readLine());
            arrumaProgresso(NomeLista);
            numEpi.append(in.readLine());
            score.append(in.readLine());
            scoreGeral.append(in.readLine());
            date.append(in.readLine());
            season.append(in.readLine());
            status.append(in.readLine());
            produtores.append(in.readLine().split(','));
            link.append(in.readLine());
            genero.append(in.readLine().split(','));
            sinopse.append(in.readLine());
            arrumaSinopse(NomeLista);
            id.append(in.readLine());
            NomeLista++;
       }
       inputFile.close();
    }
//    NomeLista = 0;
}

void leitorarquivos::arrumaSinopse(int i){
    //Unicode … -> ...
    sinopse[i].replace(" \u00e2\u20ac\u00a6", "...");
    //Unicode “ -> "
    sinopse[i].replace("\u00e2\u20ac\u009d", "\"");
    //Unicode ” -> "
    sinopse[i].replace("\u00e2\u20ac\u0153", "\"");
    //Unicode — -> -
    sinopse[i].replace("\u00e2\u20ac\u201d", " - ");
    //Unicode ’ -> '
    sinopse[i].replace("\u00e2\u20ac\u2122", "'");
}

void leitorarquivos::arrumaProgresso(int i){
    if(progresso[i] == "1"){
        progresso[i] = "01";
    }
    else if(progresso[i] == "2"){
        progresso[i] = "02";
    }
    else if(progresso[i] == "3"){
        progresso[i] = "03";
    }
    else if(progresso[i] == "4"){
        progresso[i] = "04";
    }
    else if(progresso[i] == "5"){
        progresso[i] = "05";
    }
    else if(progresso[i] == "6"){
        progresso[i] = "06";
    }
    else if(progresso[i] == "7"){
        progresso[i] = "07";
    }
    else if(progresso[i] == "8"){
        progresso[i] = "08";
    }
    else if(progresso[i] == "9"){
        progresso[i] = "09";
    }
}


void leitorarquivos::OrdenaVetor(QString ordem){
    if(ordem.compare("Ordem alfabética crescente") == 0){
        OrdenaVetorNome();
    }
    else if(ordem.compare("Ordem alfabética decrescente") == 0){
        OrdenaVetorNomeInverso();
    }
    else if(ordem.compare("Progresso crescente") == 0){
        OrdenaVetorProgresso();
    }
    else if(ordem.compare("Progresso decrescente") == 0){
        OrdenaVetorProgressoInverso();
    }
    else if(ordem.compare("Season crescente") == 0){
        OrdenaVetorData();
    }
    else if(ordem.compare("Season decrescente") == 0){
        OrdenaVetorDataInverso();
    }
    else if(ordem.compare("Nota crescente") == 0){
        OrdenaVetorNota();
    }
    else if(ordem.compare("Nota decrescente") == 0){
        OrdenaVetorNotaInverso();
    }
}

void leitorarquivos::OrdenaVetorDataInverso(){
    QString AnimeTemp;
    for(int a = 0; a < NomeLista; a++){
        for(int i = a+1; i < NomeLista; i++){
            if(date[a] < date[i]){
                nomeAnime.move(a,i);
                nomeAnime.move(i-1,a);
                nomeIngles.move(a,i);
                nomeIngles.move(i-1,a);
                progresso.move(a,i);
                progresso.move(i-1,a);
                numEpi.move(a,i);
                numEpi.move(i-1,a);
                score.move(a,i);
                score.move(i-1,a);
                scoreGeral.move(a,i);
                scoreGeral.move(i-1,a);
                date.move(a,i);
                date.move(i-1,a);
                season.move(a,i);
                season.move(i-1,a);
                status.move(a,i);
                status.move(i-1,a);
                link.move(a,i);
                link.move(i-1,a);
                sinopse.move(a,i);
                sinopse.move(i-1,a);
                id.move(a,i);
                id.move(i-1,a);
            }
        }
    }
}

void leitorarquivos::OrdenaVetorData(){
    QString AnimeTemp;
    for(int a = 0; a < NomeLista; a++){
        for(int i = a+1; i < NomeLista; i++){
            if(date[a] > date[i]){
                nomeAnime.move(a,i);
                nomeAnime.move(i-1,a);
                nomeIngles.move(a,i);
                nomeIngles.move(i-1,a);
                progresso.move(a,i);
                progresso.move(i-1,a);
                numEpi.move(a,i);
                numEpi.move(i-1,a);
                score.move(a,i);
                score.move(i-1,a);
                scoreGeral.move(a,i);
                scoreGeral.move(i-1,a);
                date.move(a,i);
                date.move(i-1,a);
                season.move(a,i);
                season.move(i-1,a);
                status.move(a,i);
                status.move(i-1,a);
                link.move(a,i);
                link.move(i-1,a);
                sinopse.move(a,i);
                sinopse.move(i-1,a);
                id.move(a,i);
                id.move(i-1,a);
            }
        }
    }
}

void leitorarquivos::OrdenaVetorNotaInverso(){
    QString AnimeTemp;
    for(int a = 0; a < NomeLista; a++){
        for(int i = a+1; i < NomeLista; i++){
            if(score[a].toInt() < score[i].toInt()){
                nomeAnime.move(a,i);
                nomeAnime.move(i-1,a);
                nomeIngles.move(a,i);
                nomeIngles.move(i-1,a);
                progresso.move(a,i);
                progresso.move(i-1,a);
                numEpi.move(a,i);
                numEpi.move(i-1,a);
                score.move(a,i);
                score.move(i-1,a);
                scoreGeral.move(a,i);
                scoreGeral.move(i-1,a);
                date.move(a,i);
                date.move(i-1,a);
                season.move(a,i);
                season.move(i-1,a);
                status.move(a,i);
                status.move(i-1,a);
                link.move(a,i);
                link.move(i-1,a);
                sinopse.move(a,i);
                sinopse.move(i-1,a);
                id.move(a,i);
                id.move(i-1,a);
            }
        }
    }
}

void leitorarquivos::OrdenaVetorNota(){
    QString AnimeTemp;
    for(int a = 0; a < NomeLista; a++){
        for(int i = a+1; i < NomeLista; i++){
            if(score[a].toInt() > score[i].toInt()){
                nomeAnime.move(a,i);
                nomeAnime.move(i-1,a);
                nomeIngles.move(a,i);
                nomeIngles.move(i-1,a);
                progresso.move(a,i);
                progresso.move(i-1,a);
                numEpi.move(a,i);
                numEpi.move(i-1,a);
                score.move(a,i);
                score.move(i-1,a);
                scoreGeral.move(a,i);
                scoreGeral.move(i-1,a);
                date.move(a,i);
                date.move(i-1,a);
                season.move(a,i);
                season.move(i-1,a);
                status.move(a,i);
                status.move(i-1,a);
                link.move(a,i);
                link.move(i-1,a);
                sinopse.move(a,i);
                sinopse.move(i-1,a);
                id.move(a,i);
                id.move(i-1,a);
            }
        }
    }
}

void leitorarquivos::OrdenaVetorProgressoInverso(){
    QString AnimeTemp;
    for(int a = 0; a < NomeLista; a++){
        for(int i = a+1; i < NomeLista; i++){
            if(progresso[a].toInt() < progresso[i].toInt()){
                nomeAnime.move(a,i);
                nomeAnime.move(i-1,a);
                nomeIngles.move(a,i);
                nomeIngles.move(i-1,a);
                progresso.move(a,i);
                progresso.move(i-1,a);
                numEpi.move(a,i);
                numEpi.move(i-1,a);
                score.move(a,i);
                score.move(i-1,a);
                scoreGeral.move(a,i);
                scoreGeral.move(i-1,a);
                date.move(a,i);
                date.move(i-1,a);
                season.move(a,i);
                season.move(i-1,a);
                status.move(a,i);
                status.move(i-1,a);
                link.move(a,i);
                link.move(i-1,a);
                sinopse.move(a,i);
                sinopse.move(i-1,a);
                id.move(a,i);
                id.move(i-1,a);
            }
        }
    }
}

void leitorarquivos::OrdenaVetorProgresso(){
    QString AnimeTemp;
    for(int a = 0; a < NomeLista; a++){
        for(int i = a+1; i < NomeLista; i++){
            if(progresso[a].toInt() > progresso[i].toInt()){
                nomeAnime.move(a,i);
                nomeAnime.move(i-1,a);
                nomeIngles.move(a,i);
                nomeIngles.move(i-1,a);
                progresso.move(a,i);
                progresso.move(i-1,a);
                numEpi.move(a,i);
                numEpi.move(i-1,a);
                score.move(a,i);
                score.move(i-1,a);
                scoreGeral.move(a,i);
                scoreGeral.move(i-1,a);
                date.move(a,i);
                date.move(i-1,a);
                season.move(a,i);
                season.move(i-1,a);
                status.move(a,i);
                status.move(i-1,a);
                link.move(a,i);
                link.move(i-1,a);
                sinopse.move(a,i);
                sinopse.move(i-1,a);
                id.move(a,i);
                id.move(i-1,a);
            }
        }
    }
}

void leitorarquivos::OrdenaVetorNome(){
    QString AnimeTemp;
    for(int a = 0; a < NomeLista; a++){
        for(int i = a+1; i < NomeLista; i++){
            if(nomeAnime[a] > nomeAnime[i]){
                nomeAnime.move(a,i);
                nomeAnime.move(i-1,a);
                nomeIngles.move(a,i);
                nomeIngles.move(i-1,a);
                progresso.move(a,i);
                progresso.move(i-1,a);
                numEpi.move(a,i);
                numEpi.move(i-1,a);
                score.move(a,i);
                score.move(i-1,a);
                scoreGeral.move(a,i);
                scoreGeral.move(i-1,a);
                date.move(a,i);
                date.move(i-1,a);
                season.move(a,i);
                season.move(i-1,a);
                status.move(a,i);
                status.move(i-1,a);
                link.move(a,i);
                link.move(i-1,a);
                sinopse.move(a,i);
                sinopse.move(i-1,a);
                id.move(a,i);
                id.move(i-1,a);
            }
        }
    }
}

void leitorarquivos::OrdenaVetorNomeInverso(){
    QString AnimeTemp;
    for(int a = 0; a < NomeLista; a++){
        for(int i = a+1; i < NomeLista; i++){
            if(nomeAnime[a] < nomeAnime[i]){
                nomeAnime.move(a,i);
                nomeAnime.move(i-1,a);
                nomeIngles.move(a,i);
                nomeIngles.move(i-1,a);
                progresso.move(a,i);
                progresso.move(i-1,a);
                numEpi.move(a,i);
                numEpi.move(i-1,a);
                score.move(a,i);
                score.move(i-1,a);
                scoreGeral.move(a,i);
                scoreGeral.move(i-1,a);
                date.move(a,i);
                date.move(i-1,a);
                season.move(a,i);
                season.move(i-1,a);
                status.move(a,i);
                status.move(i-1,a);
                link.move(a,i);
                link.move(i-1,a);
                sinopse.move(a,i);
                sinopse.move(i-1,a);
                id.move(a,i);
                id.move(i-1,a);
            }
        }
    }
}


QString leitorarquivos::imagem(int i, QString pastaImagens){
    QString local = pastaImagens;
    local.append(id[i]);
//    local.append(".jpg");
    return local;
}

QString leitorarquivos::imagemBig(int i){
    std::string local = "C:/Programas Extras/TaigaTV/Imagens/";
    local.append(id[i].toUtf8().constData());
    local.append(".jpg");
    return QString::fromStdString(local);
}

int leitorarquivos::retornaTamanhoLista(){
    return NomeLista;
}

QString leitorarquivos::retornaNome(int i){
    return nomeAnime[i];
}
QString leitorarquivos::retornaNomeIngles(int i){
    return nomeIngles[i];
}

int leitorarquivos::retornaProgresso(int i){
    return progresso[i].toInt();
}

int leitorarquivos::retornaScore(int i){
    return score[i].toInt();
}

QString leitorarquivos::retornaNumEpi(int i){
    return numEpi[i];
}

QString leitorarquivos::retornaSinopse(int i){
    return sinopse[i];

}

QString leitorarquivos::retornaNumEpisodiosLancados(int i){
    QStringList data = date[i].split('-');
    qint64 episodios;
    QDate d1 = QDate::currentDate();
    QDate inicio(data.at(0).toInt(),data.at(1).toInt(),data.at(2).toInt());
//    qDebug() << inicio.dayOfWeek();
//    qDebug() << d1.dayOfWeek();
    episodios = d1.daysTo(inicio);
    if((((-1)*episodios)/7)+1 < numEpi[i].toInt())
        episodios = (((-1)*episodios)/7)+1;
    else
        episodios = numEpi[i].toInt();
//    qDebug() << nomeAnime[i] <<episodios;
    return QString::number(episodios);
}

QString leitorarquivos::retornaId(int i){
    return id[i];
}

QString leitorarquivos::retornaLink(int i){
    return link[i];
}

QString leitorarquivos::retornaDiretorio(int i){
    return diretorio[i];
}

QStringList leitorarquivos::retornaGeneros(int i){
    return genero[i];
}

QString leitorarquivos::retornaSeason(int i){
    return season[i];
}

QString leitorarquivos::retornaNotaGeral(int i){
    return scoreGeral[i];
}

QString leitorarquivos::retornaStatus(int i){
    return status[i];
}

QStringList leitorarquivos::retornaProdutores(int i){
    return produtores[i];
}

int leitorarquivos::setProgress(int i){
    int valor = ((float)progresso[i].toInt()/numEpi[i].toInt())*100;
    if(valor > 0 && valor <= 5){
        return 5;
    }
    else if(valor >5 && valor <= 10){
        return 10;
    }
    else if(valor > 10 && valor <= 15){
        return 15;
    }
    else if(valor > 15 && valor <= 20){
        return 20;
    }
    else if(valor > 20 && valor <=25){
        return 25;
    }
    else if(valor > 25 && valor <= 30){
        return 30;
    }
    else if(valor > 30 && valor <= 35){
        return 35;
    }
    else if(valor > 35 && valor <= 40){
            return 40;
    }
    else if(valor > 40 && valor <= 45){
        return 45;
    }
    else if(valor > 45 && valor <= 50){
        return 50;
    }
    else if(valor > 50 && valor <= 55){
        return 55;
    }
    else if(valor > 55 && valor <= 60){
        return 60;
    }
    else if(valor > 60 && valor <= 65){
        return 65;
    }
    else if(valor > 65 && valor <= 70){
        return 70;
    }
    else if(valor > 70 && valor <= 75){
        return 75;
    }
    else if(valor > 75 && valor <= 80){
        return 80;
    }
    else if(valor > 80 && valor <= 85){
        return 85;
    }
    else if(valor > 85 && valor <= 90){
        return 90;
    }
    else if(valor > 90 && valor <= 95){
        return 95;
    }
    else if(valor > 95 && valor <= 100){
        return 100;
    }
    return 0;
}

int leitorarquivos::busca(QString busca)
{
    QString lista[5];
    lista[0] = "Configurações/confTaiga.txt";
    lista[1] = "Configurações/confCompleted.txt";
    lista[2] = "Configurações/confOnHold.txt";
    lista[3] = "Configurações/confDropped.txt";
    lista[4] = "Configurações/confPlanToWatch.txt";
    NomeLista = 0;
    //Procura em todas as listas
    //Quando lista de not in the list estiver pronta, mudar pra 6
    for(int i = 0; i < 5; i++){
        QFile inputFile(lista[i]);
        if (inputFile.open(QIODevice::ReadOnly))
        {
           QTextStream in(&inputFile);
           while (!in.atEnd())
           {
                QString linha = in.readLine();
                QString linha2 = in.readLine();
                if(linha.toLower().contains(busca.toLower()) || linha2.toLower().contains(busca.toLower())){
                    nomeAnime.append(linha);
                    nomeIngles.append(linha2);
                    progresso.append(in.readLine());
                    arrumaProgresso(NomeLista);
                    numEpi.append(in.readLine());
                    score.append(in.readLine());
                    scoreGeral.append(in.readLine());
                    date.append(in.readLine());
                    season.append(in.readLine());
                    status.append(in.readLine());
                    produtores.append(in.readLine().split(','));
                    link.append(in.readLine());
                    genero.append(in.readLine().split(','));
                    sinopse.append(in.readLine());
                    arrumaSinopse(NomeLista);
                    id.append(in.readLine());
                    NomeLista++;
                }
                else{
                    in.readLine();
                    in.readLine();
                    in.readLine();
                    in.readLine();
                    in.readLine();
                    in.readLine();
                    in.readLine();
                    in.readLine();
                    in.readLine();
                    in.readLine();
                    in.readLine();
                    in.readLine();
                }
           }
           inputFile.close();
        }
    }
    return NomeLista;
}
