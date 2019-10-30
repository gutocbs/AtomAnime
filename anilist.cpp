#include "anilist.h"

const QUrl graphqlUrl("https://graphql.anilist.co");

anilist::anilist(QObject *parent) : QObject(parent)
{
    vusername = "\"gutocbs\"";
    vtoken = "x";
    fgetList();
}

anilist::~anilist(){
    vreply->deleteLater();
}

bool anilist::fgetList(){
    //Salvar o usuário em um arquivo

    QNetworkRequest lrequest(graphqlUrl);
    lrequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    lrequest.setRawHeader(QByteArray("Accept"), "application/json; charset=utf-8");
    QJsonObject json;
    QNetworkAccessManager lacessManager;

    QString totalPages = "query ($id: Int) {Page (page: 1 perPage: 50) {pageInfo {total currentPage lastPage hasNextPage perPage}mediaList(userName: " + vusername + ", id: $id, sort: MEDIA_ID) {user{id name}}}}";
    json.insert("query", totalPages);
    vreply = lacessManager.post(lrequest, QJsonDocument(json).toJson());
    while (!vreply->isFinished())
    {
        qApp->processEvents();
    }
    QByteArray response_data = vreply->readAll();
    QJsonDocument jsond = QJsonDocument::fromJson(response_data);
    QString lastpage = jsond.toJson();
    if(lastpage.contains("error"))
        return false;
    lastpage = lastpage.toLatin1();
    QString llastpage = lastpage.mid(lastpage.lastIndexOf("lastPage")+11);
    lastpage = llastpage.left(llastpage.indexOf(",\n"));
    QFile t("Configurações/Temp/animeList.txt");
    if(t.open(QIODevice::WriteOnly)){
        for(int i = 1; i < lastpage.toInt()+1; i++){
            QString query = "query ($id: Int, $perPage: Int) {  	Page (page:" + QString::number(i) + ", perPage: $perPage) { 		mediaList(userName: " + vusername + ", id: $id, sort: MEDIA_ID) { 			status  			score 			progress 			media{           		format 				averageScore 				id 				title{ 					romaji  					english 				} 				synonyms 				description           		status           		coverImage{ 					large 				} 				season 				startDate {             		year             		month           		} 				episodes 				nextAiringEpisode{ 					airingAt 					episode 				} 			} 		} 	} }";
            json.insert("query", query.trimmed());
            vreply = lacessManager.post(lrequest, QJsonDocument(json).toJson());
            while (!vreply->isFinished())
            {
                qApp->processEvents();
            }
            QByteArray response_data = vreply->readAll();
            jsond = QJsonDocument::fromJson(response_data);
            t.write(jsond.toJson());
        }
        t.close();
    }
    response_data = vreply->readAll();
    jsond = QJsonDocument::fromJson(response_data);
    QString lreplyString = jsond.toJson();
    if(lreplyString.contains("error"))
        return false;
    else
        return true;
}

///Fazer isso em thread?
bool anilist::fmudaLista(int rid, QString rNovaLista){
    ///Preciso por o token em um arquvio de configuração criptografado
    QByteArray auth = "Bearer ";
    auth.append(vtoken);

    //Como pegar o nome ou id de usuário pelo token?
    //Posso pegar igual fazia antes,/ sem o token?
    QNetworkRequest lrequest(graphqlUrl);
    lrequest.setRawHeader(QByteArray("Authorization"), auth);
    lrequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    lrequest.setRawHeader(QByteArray("Accept"), "application/json; charset=utf-8");
    QJsonObject json;
    QNetworkAccessManager lacessManager;

    //Pra deletar
    //QString lnovaLista = "mutation{     DeleteMediaListEntry (mediaId: " + lnovoId + ", status: " + rnovaLista + ") {         id         status     } }";
    //Cria string com a lista antiga
    QString llistaAtual = "mutation{     SaveMediaListEntry (mediaId: " + QString::number(rid) + ", status: " + rNovaLista + ") {         id         status     } }";
    //Insere item no json
    json.insert("query", llistaAtual);
    //Manda a solicitação de mudança
    vreply = lacessManager.post(lrequest, QJsonDocument(json).toJson());

    //Espera solicitação voltar do servidor
    while (!vreply->isFinished())
    {
        qApp->processEvents();
    }
    QByteArray response_data = vreply->readAll();
    QJsonDocument jsond = QJsonDocument::fromJson(response_data);
    QString lreplyString = jsond.toJson();
    if(lreplyString.contains("error"))
        return false;
    else
        return true;
}

///fmudaNota(id, nova nota)
bool anilist::fmudaNota(int rid, int rnovaNota){
    ///Preciso por o token em um arquvio de configuração criptografado
    QByteArray auth = "Bearer ";
    auth.append(vtoken);

    //Como pegar o nome ou id de usuário pelo token?
    //Posso pegar igual fazia antes,/ sem o token?
    QNetworkRequest lrequest(graphqlUrl);
    lrequest.setRawHeader(QByteArray("Authorization"), auth);
    lrequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    lrequest.setRawHeader(QByteArray("Accept"), "application/json; charset=utf-8");
    QJsonObject json;
    QNetworkAccessManager lacessManager;

    //Pra deletar
    //QString lnovaLista = "mutation{     DeleteMediaListEntry (mediaId: " + lnovoId + ", status: " + rnovaLista + ") {         id         status     } }";
    //Cria string com a lista antiga
    QString llistaAtual = "mutation{     SaveMediaListEntry (mediaId: " + QString::number(rid) + ", score: " + QString::number(rnovaNota) + ") {         id         status     } }";
    //Insere item no json
    json.insert("query", llistaAtual);
    //Manda a solicitação de mudança
    vreply = lacessManager.post(lrequest, QJsonDocument(json).toJson());

    //Espera solicitação voltar do servidor
    while (!vreply->isFinished())
    {
        qApp->processEvents();
    }
    QByteArray response_data = vreply->readAll();
    QJsonDocument jsond = QJsonDocument::fromJson(response_data);
    QString lreplyString = jsond.toJson();
    if(lreplyString.contains("error"))
        return false;
    else
        return true;
}

bool anilist::fmudaProgresso(int rid, int rnovoProgresso){
    ///Preciso por o token em um arquvio de configuração criptografado
    QByteArray auth = "Bearer ";
    auth.append(vtoken);

    //Como pegar o nome ou id de usuário pelo token?
    //Posso pegar igual fazia antes,/ sem o token?
    QNetworkRequest lrequest(graphqlUrl);
    lrequest.setRawHeader(QByteArray("Authorization"), auth);
    lrequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    lrequest.setRawHeader(QByteArray("Accept"), "application/json; charset=utf-8");
    QJsonObject json;
    QNetworkAccessManager lacessManager;

    //Pra deletar
    //QString lnovaLista = "mutation{     DeleteMediaListEntry (mediaId: " + lnovoId + ", status: " + rnovaLista + ") {         id         status     } }";
    //Cria string com a lista antiga
    QString llistaAtual = "mutation{     SaveMediaListEntry (mediaId: " + QString::number(rid) + ", progress: " + QString::number(rnovoProgresso) + ") {         id         status     } }";
    //Insere item no json
    json.insert("query", llistaAtual);
    //Manda a solicitação de mudança
    vreply = lacessManager.post(lrequest, QJsonDocument(json).toJson());

    //Espera solicitação voltar do servidor
    while (!vreply->isFinished())
    {
        qApp->processEvents();
    }
    QByteArray response_data = vreply->readAll();
    QJsonDocument jsond = QJsonDocument::fromJson(response_data);
    QString lreplyString = jsond.toJson();
    if(lreplyString.contains("error"))
        return false;
    else
        return true;
}
