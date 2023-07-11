
#ifndef HTTPSERVICE_H
#define HTTPSERVICE_H

#include <QByteArray>
#include <QDebug>
#include <QEventLoop>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QRegularExpression>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

struct FrpResult
{
    QString tag;
    QString latestUrl;
    QString name;
};

class HttpService : public QObject
{
    Q_OBJECT
public:
    explicit HttpService(QObject* parent = nullptr);
    ~HttpService() override;

public:
    static bool      downloadFile(QString localPath, FrpResult frpResult, void (*callback)(FrpResult res), std::function<void(FrpResult)> callb = NULL);
    static FrpResult getLatestReleaseData(QString owner, QString repo);

signals:
};

#endif  // HTTPSERVICE_H
