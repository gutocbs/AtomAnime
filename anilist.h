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
    bool fmudaLista(int, QString);
    bool fmudaNota(int, int);
    ///id, progresso
    bool fmudaProgresso(int, int);
    bool fexcluiAnime(int);

    QString fretornaAvatar();

    void fbaixaListaThread(QThread &cThread);
    void frecebeAutorizacao(QString, QString);

public slots:
    bool fgetList();

signals:
    void sterminouDownload(bool);
private:
    QNetworkReply *vreply = nullptr;
    QString vtoken;
    QString vusername;
    QString vavatar;
};

#endif // ANILIST_H
