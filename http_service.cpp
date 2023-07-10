
#include "http_service.h"

HttpService::HttpService(QObject* parent) : QObject{ parent } {}

HttpService::~HttpService() {}

bool HttpService::downloadFile(QString localPath, FrpResult frpResult, void (*callback)(FrpResult res))
{
    QNetworkRequest       request(frpResult.latestUrl);
    QNetworkAccessManager manager;
    QNetworkReply*        reply = manager.get(request);
    QFile                 file(localPath, reply);
    if (!file.open(QFile::ReadWrite))
    {
        return false;
    }
    // sync
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    file.write(reply->readAll());
    file.close();
    reply->deleteLater();
    (*callback)(frpResult);
    return true;
}

FrpResult HttpService::getLatestReleaseData(QString owner, QString repo)
{
    FrpResult result;

    QNetworkAccessManager manager;
    QUrl                  url(QString("https://api.github.com/repos/%1/%2/releases/latest").arg(owner, repo));
    QNetworkRequest       request(url);
    // request.setRawHeader("Authorization", "Bearer github_1234567");
    QNetworkReply* reply = manager.get(request);

    //    //async
    //    connect(reply, &QNetworkReply::finished, [=]() {
    //        QByteArray contents = reply->readAll();
    //        if (!contents.isEmpty())
    //        {
    //            QJsonDocument jsonResponse = QJsonDocument::fromJson(contents);
    //            QJsonObject   jsonObject   = jsonResponse.object();
    //            qDebug() << jsonObject["tag_name"].toString();
    //        }
    //        delete manager;
    //    });

    // sync
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QByteArray contents = reply->readAll();
    if (!contents.isEmpty())
    {
        qDebug() << contents;
        QJsonDocument jsonResponse = QJsonDocument::fromJson(contents);
        QJsonObject   jsonObject   = jsonResponse.object();
        result.tag                 = jsonObject["tag_name"].toString();
        QJsonArray assets          = jsonObject["assets"].toArray();
        if (repo == "frp")
        {
            for (QJsonValue asset : assets)
            {
                static QRegularExpression windowsAmd64Re("^.*amd64[.]zip$");
                QString                   name = asset.toObject()["name"].toString();
                if (windowsAmd64Re.match(name).hasMatch())
                {
                    result.latestUrl = asset.toObject()["browser_download_url"].toString();
                    result.name      = name;
                    break;
                }
            }
        }
    }
    return result;
}
