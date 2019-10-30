#ifndef ANILIST_H
#define ANILIST_H

#include <QObject>
#include <QtNetwork>

#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>

#include <QDesktopServices>
#include <QSettings>

#include <QJsonObject>
#include <QJsonDocument>

class anilist : public QObject
{
    Q_OBJECT
public:
    explicit anilist(QObject *parent = nullptr);
    ~anilist();
    bool fgetList();
    bool fmudaLista(int, QString);
    bool fmudaNota(int, int);
    bool fmudaProgresso(int, int);

private:
    QNetworkReply *vreply = nullptr;
    QString vtoken;
    QString vusername;
//    QOAuth2AuthorizationCodeFlow oauth2;
//    QOAuthHttpServerReplyHandler *replyHandler{nullptr};
};

#endif // ANILIST_H
