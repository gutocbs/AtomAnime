#include "apiget.h"

apiget::apiget(QObject *parent) : QObject(parent)
{
    mPanelStories = nullptr;
    mNetMan = new QNetworkAccessManager(this);
    mNetReply = nullptr;
    mDataBuffer = new QByteArray;

}

void apiget::postRequest(){
//    query += "{ {id: \"105333\", page: 1, perPage: 10};}";

        //auto r = cpr::Post(cpr::Url{"https://graphql.anilist.co"},
          //             cpr::Body{"oi"},
            //           cpr::Header{{"Content-Type", "text/plain"}});
//    auto r = cpr::Post(cpr::Url{"http://www.httpbin.org/post"},
//               cpr::Body{"This is raw POST data"},
//               cpr::Header{{"Content-Type", "text/plain"}});
//    qDebug() << QString::fromStdString(r.text);
}

