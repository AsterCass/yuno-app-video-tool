
#include "frp_file_expose_widget.h"

FrpFileExposeWidget::FrpFileExposeWidget(QWidget* parent) : QWidget{ parent }
{
    m_frpFileExposeLayout = new QVBoxLayout();
    m_frpFileExposeLayout->addWidget(createMainWidget());
    m_frpFileExposeLayout->setContentsMargins(20, 20, 20, 20);
    setLayout(m_frpFileExposeLayout);
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
    delete m_frpFileExposeBtnWidgetLayout;
    delete m_frpFileExposeBtnWidget;
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
        OutputWidget::printMessageError("辅助程序丢失，请检查程序目录下是否包含【extend】文件夹，或者您可能没有足够的权限或资源来调用该程序");
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

void FrpFileExposeWidget::readConfig()
{
    QFile configFile("./config/frpc.ini");
    if (!configFile.open(QIODeviceBase::ReadOnly))
    {
        OutputWidget::printMessageWarn(QString("未在【").append(QDir::currentPath()).append("/config").append("】下找到默认配置"));
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
        m_frpFileExposeClear = new QPushButton(" 清空控制台 ");
        connect(m_frpFileExposeClear, &QPushButton::clicked, this, &FrpFileExposeWidget::clearConsole);
        m_frpFileExposeBtnWidgetLayout = new QHBoxLayout();
        m_frpFileExposeBtnWidgetLayout->addWidget(m_frpFileExposeSave);
        m_frpFileExposeBtnWidgetLayout->addWidget(m_frpFileExposeStop);
        m_frpFileExposeBtnWidgetLayout->addWidget(m_frpFileExposeClear);
        m_frpFileExposeBtnWidget->setLayout(m_frpFileExposeBtnWidgetLayout);
    }

    // layout
    m_mainWidgetLayout = new QVBoxLayout();
    m_mainWidgetLayout->addWidget(m_serverAddressWidget);
    m_mainWidgetLayout->addWidget(m_serverPortWidget);
    m_mainWidgetLayout->addWidget(m_remotePortWidget);
    m_mainWidgetLayout->addWidget(m_exposePathWidget);
    m_mainWidgetLayout->addWidget(m_stripPathWidget);
    m_mainWidgetLayout->addWidget(m_frpFileExposeBtnWidget);
    m_mainWidget->setLayout(m_mainWidgetLayout);
    return m_mainWidget;
}
