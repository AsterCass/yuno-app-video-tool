
#include "frp_file_expose_widget.h"

FrpFileExposeWidget::FrpFileExposeWidget(QWidget* parent) : QWidget{ parent }
{
    m_frpFileExposeLayout = new QVBoxLayout();
    m_frpFileExposeLayout->addWidget(createMainWidget());
    m_frpFileExposeLayout->setContentsMargins(20, 20, 20, 20);
    setLayout(m_frpFileExposeLayout);
    // update module
    updateModule();
    // read config
    readConfig();
}

FrpFileExposeWidget::~FrpFileExposeWidget()
{
    // server address
    delete m_serverAddressLabel;
    delete m_serverAddress;
    delete m_serverAddressWidgetLayout;
    delete m_serverAddressWidget;
    // server port
    delete m_serverPortLabel;
    delete m_serverPort;
    delete m_serverPortWidgetLayout;
    delete m_serverPortWidget;
    // remot port
    delete m_remotePortLabel;
    delete m_remotePort;
    delete m_remotePortWidgetLayout;
    delete m_remotePortWidget;
    // expose path
    delete m_selectExposePath;
    delete m_exposePath;
    delete m_exposePathWidgetLayout;
    delete m_exposePathWidget;
    // strip path
    delete m_stripPathLabel;
    delete m_stripPath;
    delete m_stripPathWidgetLayout;
    delete m_stripPathWidget;
    // control
    delete m_frpFileExposeSave;
    delete m_frpFileExposeStop;
    delete m_frpFileExposeClear;
    delete m_frpFileVersionUpdate;
    delete m_frpFileExposeBtnWidgetLayout;
    delete m_frpFileExposeBtnWidget;
    delete m_frpFielSystemBtnWidgetLayout;
    delete m_frpFielSystemBtnWidget;
    // base
    delete m_process;
    delete m_mainWidgetLayout;
    delete m_mainWidget;
    delete m_frpFileExposeLayout;
}

void FrpFileExposeWidget::lanuch()
{
    if (nullptr != m_process && QProcess::NotRunning != m_process->state())
    {
        OutputWidget::printMessageError("穿透执行中，请勿重复执行");
        return;
    }
    if (!QFile::exists("./config/frpc.ini"))
    {
        OutputWidget::printMessageError("配置未保存，请保存配置");
        return;
    }
    OutputWidget::printMessage("服务启动中...");
    m_process        = new QProcess();
    QString     exec = "./extend/frpc";
    QStringList params;
    params << "-c"
           << "./config/frpc.ini";
    connect(m_process, &QProcess::readyReadStandardOutput, this, &FrpFileExposeWidget::readStdout);
    connect(m_process, &QProcess::readyReadStandardError, this, &FrpFileExposeWidget::readStderr);
    connect(m_process, &QProcess::errorOccurred, this, &FrpFileExposeWidget::processError);
    m_process->start(exec, params);
}

void FrpFileExposeWidget::selectExposePath()
{
    QString selectedExposePath = QFileDialog::getExistingDirectory(this, ("选择对外开放文件夹"), QDir::currentPath());
    m_exposePath->setText(selectedExposePath);
}

void FrpFileExposeWidget::readStderr()
{
    while (m_process->canReadLine())
    {
        OutputWidget::printMessageError(m_process->readLine());
    }
}

void FrpFileExposeWidget::readStdout()
{
    while (m_process->canReadLine())
    {
        OutputWidget::printMessage(m_process->readLine());
    }
}

void FrpFileExposeWidget::processError(QProcess::ProcessError error)
{
    switch (error)
    {
    case QProcess::FailedToStart:
        OutputWidget::printMessageError("辅助程序无法启动，请尝试：\n"
                                        "【1】查看是否Windows防护或者其他杀毒软件拦截，解除拦截，还原隔离文件\n"
                                        "【2】删除目录下除了exe可执行文件外其他文件，以管理员权限重新执行\n"
                                        "【3】如果以上方法都不生效，请自行在GITHUB下载frp可执行文件frpc.exe到extend文件夹目录下");
        break;
    case QProcess::Crashed:
        OutputWidget::printMessageWarn("服务已终止");
        break;
    default:
        OutputWidget::printMessageError("辅助程序执行发生未知错误，请联系【astercass@qq.com】");
    }
}

bool FrpFileExposeWidget::configCheck()
{
    bool isCompleted = true;
    if (m_serverAddress->text().isEmpty())
    {
        isCompleted = false;
        OutputWidget::printMessageError("【公网域名/IP】不能为空");
    }
    if (0 == m_serverPort->value())
    {
        isCompleted = false;
        OutputWidget::printMessageError("【公网服务连接端口】不能为0");
    }
    if (0 == m_remotePort->value())
    {
        isCompleted = false;
        OutputWidget::printMessageError("【文件服务公网穿透端口】不能为0");
    }
    if (m_exposePath->text().isEmpty())
    {
        isCompleted = false;
        OutputWidget::printMessageError("请选择【对外开放文件夹】");
    }
    if (!m_stripPath->text().isEmpty() && m_stripPath->text().at(0) == '/')
    {
        m_stripPath->setText(m_stripPath->text().mid(1));
    }
    return isCompleted;
}

void downloadFinish(FrpResult result)
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

void FrpFileExposeWidget::updateModuleForce()
{
    m_frpFileVersionUpdate->setDisabled(true);
    OutputWidget::printMessage("组件下载中...");
    QtConcurrent::run([=]() {
        FrpResult result = HttpService::getLatestReleaseData(QString("fatedier"), QString("frp"));
        HttpService::downloadFile("./extend/frp.zip", result, downloadFinish);
    });
}

void FrpFileExposeWidget::updateModule()
{
    bool configBuild = QDir("config").exists() ? true : QDir().mkdir("config");
    bool extendBuild = QDir("extend").exists() ? true : QDir().mkdir("extend");
    if (!configBuild || !extendBuild)
    {
        OutputWidget::printMessageError("基础文件夹配置失败，请检查权限");
        return;
    }
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
                m_frpFileVersionUpdate->setDisabled(frpcTag == result.tag);
            }
            extendAdmin.close();
        }
    }
    else
    {
        OutputWidget::printMessage("基础组件FRPC不存在");
        updateModuleForce();
    }
}

void FrpFileExposeWidget::readConfig()
{
    QFile configFile("./config/frpc.ini");
    if (!configFile.open(QIODeviceBase::ReadOnly))
    {
        //        OutputWidget::printMessage(QString("未找到默认配置"));
        //        qDebug() << QString("未在【").append(QDir::currentPath()).append("/config").append("】下找到默认配置");
        return;
    }
    const QMap<QString, QWidget*> configWidgetMap{
        { "server_addr", m_serverAddress }, { "server_port", m_serverPort }, { "remote_port", m_remotePort }, { "plugin_local_path", m_exposePath }, { "plugin_strip_prefix", m_stripPath }
    };
    QTextStream configFileStream(&configFile);
    while (!configFileStream.atEnd())
    {
        QString line = configFileStream.readLine();
        for (auto config = configWidgetMap.constBegin(); config != configWidgetMap.constEnd(); ++config)
        {
            QRegularExpression              matchConfig(QString("^").append(config.key()).append("=([A-Za-z0-9.:\\/]+)"));
            QRegularExpressionMatchIterator matchConfigIterator = matchConfig.globalMatch(line);
            if (matchConfigIterator.hasNext() && QLineEdit::staticMetaObject.className() == config.value()->metaObject()->className())
            {
                auto configCastObj = dynamic_cast<QLineEdit*>(config.value());
                configCastObj->setText(matchConfigIterator.next().captured(1));
            }
            if (matchConfigIterator.hasNext() && QLabel::staticMetaObject.className() == config.value()->metaObject()->className())
            {
                auto configCastObj = dynamic_cast<QLabel*>(config.value());
                configCastObj->setText(matchConfigIterator.next().captured(1));
            }
            if (matchConfigIterator.hasNext() && QSpinBox::staticMetaObject.className() == config.value()->metaObject()->className())
            {
                auto configCastObj = dynamic_cast<QSpinBox*>(config.value());
                configCastObj->setValue(matchConfigIterator.next().captured(1).toInt());
            }
        }
    }
    configFile.close();
}

void FrpFileExposeWidget::saveConfig()
{
    if (configCheck())
    {
        QFile configFile("./config/frpc.ini");
        if (configFile.open(QIODeviceBase::ReadWrite))
        {
            configFile.resize(0);
            configFile.write("[common]\n");
            configFile.write(QString("server_addr=").append(m_serverAddress->text()).append("\n").toUtf8());
            configFile.write(QString("server_port=").append(m_serverPort->text()).append("\n").toUtf8());
            configFile.write("[test_static_file]\ntype=tcp\nplugin=static_file\n");
            configFile.write(QString("remote_port=").append(m_remotePort->text()).append("\n").toUtf8());
            configFile.write(QString("plugin_local_path=").append(m_exposePath->text()).append("\n").toUtf8());
            configFile.write(QString("plugin_strip_prefix=").append(m_stripPath->text()).append("\n\n\n").toUtf8());
            configFile.close();
            QString launchTemplate("保存成功，");
            if (m_stripPath->text().isEmpty())
            {
                launchTemplate.append("启动后可通过访问【http://%1:%2/%3】穿透访问【%4】");
            }
            else
            {
                launchTemplate.append("启动后可通过访问【http://%1:%2/%3/】穿透访问【%4】");
            }
            OutputWidget::printMessage(launchTemplate.arg(m_serverAddress->text(), m_remotePort->text(), m_stripPath->text(), m_exposePath->text()));
        }
        else
        {
            OutputWidget::printMessageError("配置文件创建失败，请检查权限");
        }
    }
}

void FrpFileExposeWidget::stopProccess()
{
    if (nullptr == m_process || QProcess::NotRunning == m_process->state())
    {
        OutputWidget::printMessageError("文件穿透未启动");
        return;
    }
    m_process->close();
}

void FrpFileExposeWidget::clearConsole()
{
    OutputWidget::clearConsole();
}

QWidget* FrpFileExposeWidget::createMainWidget()
{
    m_mainWidget = new QWidget();
    // style
    {
        m_mainWidget->setStyleSheet("QWidget {font-size: 15px;color: rgb(86, 86, 86);}");
    }
    // server address
    m_serverAddressWidget = new QWidget();
    {
        m_serverAddressLabel = new QLabel("公网域名/IP：");
        m_serverAddress      = new QLineEdit();
        m_serverAddress->setPlaceholderText("114.114.114.114");

        m_serverAddressWidgetLayout = new QHBoxLayout();
        m_serverAddressWidgetLayout->addWidget(m_serverAddressLabel);
        m_serverAddressWidgetLayout->addWidget(m_serverAddress);
        m_serverAddressWidget->setLayout(m_serverAddressWidgetLayout);
    }
    // server port
    m_serverPortWidget = new QWidget();
    {
        m_serverPortLabel = new QLabel("公网服务连接端口：");
        m_serverPort      = new QSpinBox();
        m_serverPort->setMaximum(65535);
        m_serverPort->setValue(8080);
        m_serverPortWidgetLayout = new QHBoxLayout();
        m_serverPortWidgetLayout->addWidget(m_serverPortLabel);
        m_serverPortWidgetLayout->addWidget(m_serverPort);
        m_serverPortWidget->setLayout(m_serverPortWidgetLayout);
    }
    // remot port
    m_remotePortWidget = new QWidget();
    {
        m_remotePortLabel = new QLabel("文件服务公网穿透端口：");
        m_remotePort      = new QSpinBox();
        m_remotePort->setMaximum(65535);
        m_remotePort->setValue(8081);
        m_remotePortWidgetLayout = new QHBoxLayout();
        m_remotePortWidgetLayout->addWidget(m_remotePortLabel);
        m_remotePortWidgetLayout->addWidget(m_remotePort);
        m_remotePortWidget->setLayout(m_remotePortWidgetLayout);
    }
    // expose path
    m_exposePathWidget = new QWidget();
    {
        m_selectExposePath = new QPushButton(" 选择对外开放文件夹 ");
        connect(m_selectExposePath, &QPushButton::clicked, this, &FrpFileExposeWidget::selectExposePath);
        m_exposePath = new QLabel();
        // 自动换行
        m_exposePath->setWordWrap(true);
        // 允许拷贝
        m_exposePath->setTextInteractionFlags(Qt::TextSelectableByMouse);
        // layout
        m_exposePathWidgetLayout = new QHBoxLayout();
        m_exposePathWidgetLayout->setSpacing(20);
        m_exposePathWidgetLayout->addWidget(m_selectExposePath);
        m_exposePathWidgetLayout->addWidget(m_exposePath);
        m_exposePathWidget->setLayout(m_exposePathWidgetLayout);
    }
    // strip path
    m_stripPathWidget = new QWidget();
    {
        m_stripPathLabel = new QLabel("链接前缀：");
        m_stripPath      = new QLineEdit();
        m_stripPath->setPlaceholderText("static/videos");
        m_stripPathWidgetLayout = new QHBoxLayout();
        m_stripPathWidgetLayout->addWidget(m_stripPathLabel);
        m_stripPathWidgetLayout->addWidget(m_stripPath);
        m_stripPathWidget->setLayout(m_stripPathWidgetLayout);
    }
    // control
    m_frpFileExposeBtnWidget = new QWidget();
    {
        m_frpFileExposeSave = new QPushButton(" 保存配置 ");
        connect(m_frpFileExposeSave, &QPushButton::clicked, this, &FrpFileExposeWidget::saveConfig);
        m_frpFileExposeStop = new QPushButton(" 停止穿透 ");
        connect(m_frpFileExposeStop, &QPushButton::clicked, this, &FrpFileExposeWidget::stopProccess);

        m_frpFileExposeBtnWidgetLayout = new QHBoxLayout();
        m_frpFileExposeBtnWidgetLayout->setSpacing(20);
        m_frpFileExposeBtnWidgetLayout->addWidget(m_frpFileExposeSave);
        m_frpFileExposeBtnWidgetLayout->addWidget(m_frpFileExposeStop);
        m_frpFileExposeBtnWidget->setLayout(m_frpFileExposeBtnWidgetLayout);
    }
    m_frpFielSystemBtnWidget = new QWidget();
    {
        m_frpFileExposeClear = new QPushButton(" 清空控制台 ");
        connect(m_frpFileExposeClear, &QPushButton::clicked, this, &FrpFileExposeWidget::clearConsole);
        m_frpFileVersionUpdate = new QPushButton(" 版本更新 ");
        connect(m_frpFileVersionUpdate, &QPushButton::clicked, this, &FrpFileExposeWidget::updateModuleForce);

        m_frpFielSystemBtnWidgetLayout = new QHBoxLayout();
        m_frpFielSystemBtnWidgetLayout->setSpacing(20);
        m_frpFielSystemBtnWidgetLayout->addWidget(m_frpFileExposeClear);
        m_frpFielSystemBtnWidgetLayout->addWidget(m_frpFileVersionUpdate);
        m_frpFielSystemBtnWidget->setLayout(m_frpFielSystemBtnWidgetLayout);
    }

    // layout
    m_mainWidgetLayout = new QVBoxLayout();
    m_mainWidgetLayout->addWidget(m_serverAddressWidget);
    m_mainWidgetLayout->addWidget(m_serverPortWidget);
    m_mainWidgetLayout->addWidget(m_remotePortWidget);
    m_mainWidgetLayout->addWidget(m_exposePathWidget);
    m_mainWidgetLayout->addWidget(m_stripPathWidget);
    m_mainWidgetLayout->addWidget(m_frpFileExposeBtnWidget);
    m_mainWidgetLayout->addWidget(m_frpFielSystemBtnWidget);
    m_mainWidget->setLayout(m_mainWidgetLayout);
    return m_mainWidget;
}
