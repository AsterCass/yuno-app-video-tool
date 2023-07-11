
#include "frp_module.h"

bool FrpModule::isUpdated = false;

FrpModule::FrpModule() {}

FrpModule::~FrpModule() {}

CoreModuleData FrpModule::getModuleData()
{
    return { "GITHUB", "Frp", "frpc.exe" };
}

void FrpModule::downloadFinish(FrpResult result)
{
    QFile extendAdmin("./extend/extend-admin.json");
    if (!extendAdmin.open(QIODeviceBase::ReadWrite))
    {
        OutputWidget::printMessageError("基础文件夹配置失败，请检查权限");
    }
    extendAdmin.resize(0);
    QJsonObject frpcObj;
    frpcObj["fileName"] = "frpc.exe";
    // todo get version
    frpcObj["tag"] = result.tag;
    QJsonObject obj;
    obj["frpc"] = frpcObj;
    QJsonDocument doc(obj);
    extendAdmin.write(doc.toJson());
    extendAdmin.close();
    // get version
    if (0 == QFile("./extend/frp.zip").size())
    {
        OutputWidget::printMessageError("下载超时，请开启梯子或选择自带基础组件的版本");
        return;
    }
    OutputWidget::printMessage(QString("FRPC下载完成，当前版本为【%1】，正在解压...").arg(result.tag));
    // unzip
    QProcess*   unzipProcess = new QProcess();
    QString     exec         = "tar";
    QStringList params;
    params << "-xf"
           << "./extend/frp.zip"
           << "-C"
           << "./extend/";
    unzipProcess->start(exec, params);
    unzipProcess->waitForFinished();
    delete unzipProcess;
    QString dirName = "./extend/" + result.name.left(result.name.size() - QString(".zip").size());
    if (!QDir(dirName).exists())
    {
        OutputWidget::printMessageError("解压失败，请联系【astercass@qq.com】");
        return;
    }
    OutputWidget::printMessage("解压完成，请配置FRP文件穿透相关参数后启动");
    QFile frpcFile(QString("%1/frpc.exe").arg(dirName));
    QFile frpsFile(QString("%1/frps.exe").arg(dirName));
    if (!frpcFile.exists() || !frpsFile.exists())
    {
        OutputWidget::printMessageError("下载链接不报包括指定资源，请联系【astercass@qq.com】修复");
    }
    // move
    frpcFile.rename(QString("./extend/frpc.exe"));
    frpsFile.rename(QString("./extend/frps.exe"));
    frpcFile.close();
    frpsFile.close();
    // delete
    QFile::remove("./extend/frp.zip");
    QDir(QDir(dirName).absolutePath()).removeRecursively();
}

void FrpModule::updateModule()
{
    OutputWidget::printMessage("组件下载中...");
    QtConcurrent::run([=]() {
        FrpResult result = HttpService::getLatestReleaseData(QString("fatedier"), QString("frp"));
        HttpService::downloadFile("./extend/frp.zip", result, NULL, std::bind(&FrpModule::downloadFinish, this, result));
    });
}

bool FrpModule::updateModuleSelective()
{
    if (isUpdated)
    {
        return false;
    }
    isUpdated       = true;
    bool needUpdate = true;
    if (QFile("./extend/frpc.exe").exists())
    {
        QFile extendAdmin("./extend/extend-admin.json");
        if (extendAdmin.open(QIODeviceBase::ReadWrite))
        {
            // read
            QJsonDocument extendAdminJsonDoc = QJsonDocument::fromJson(extendAdmin.readAll());
            QJsonObject   jsonObject         = extendAdminJsonDoc.object();
            QString       frpcTag            = jsonObject["frpc"].toObject()["tag"].toString();
            // read fail
            if (frpcTag.isEmpty())
            {
                OutputWidget::printMessageWarn("检测到管理文件被修改，正在修复...");
                extendAdmin.resize(0);
                QJsonObject frpcObj;
                frpcObj["fileName"] = "frpc.exe";
                frpcObj["tag"]      = "YUNO_UNKNOWN_TAG";
                QJsonObject obj;
                obj["frpc"] = frpcObj;
                QJsonDocument doc(obj);
                extendAdmin.write(doc.toJson());
                OutputWidget::printMessage("文件修复完成，当前FRPC版本号未知");
                frpcTag = "YUNO_UNKNOWN_TAG";
            }
            else
            {
                FrpResult result = HttpService::getLatestReleaseData(QString("fatedier"), QString("frp"));
                OutputWidget::printMessage(QString("FRPC初始化完成，当前版本为【%1】，最新版本号为【%2】").arg(frpcTag, result.tag));
                needUpdate = frpcTag != result.tag;
            }
            extendAdmin.close();
        }
    }
    else
    {
        OutputWidget::printMessage("基础组件FRPC不存在");
        updateModule();
    }
    return needUpdate;
}
