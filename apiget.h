#ifndef APIGET_H
#define APIGET_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QUrlQuery>
#include <QEventLoop>

class apiget : public QObject
{
    Q_OBJECT
public:
    apiget(QObject *parent = nullptr);
    void postRequest();
    void onResult(QNetworkReply* reply);

private:
    QWidget * mPanelStories;
    QNetworkAccessManager * mNetMan;
    QNetworkReply * mNetReply;
    QByteArray * mDataBuffer;
};

#endif // APIGET_H
