
#include "frp_service_expose_widget.h"

FrpServiceExposeWidget::FrpServiceExposeWidget(QWidget* parent) : CoreFunction{ parent }
{
    // module load
    m_module = new FrpModule();
    setModule(m_module);
    // layout
    m_frpServiceExposeLayout = new QVBoxLayout();
    m_frpServiceExposeLayout->addWidget(createMainWidget());
    m_frpServiceExposeLayout->setContentsMargins(20, 20, 20, 20);
    setLayout(m_frpServiceExposeLayout);
    // update module
    updateModule();
    // read config
    readConfig();
}

FrpServiceExposeWidget::~FrpServiceExposeWidget()
{
    // local service port
    delete m_servicePortLabel;
    delete m_servicePort;
    delete m_servicePortWidgetLayout;
    delete m_servicePortWidget;
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
    // control
    delete m_frpServiceExposeSave;
    delete m_frpServiceExposeStop;
    delete m_frpServiceExposeClear;
    delete m_frpServiceVersionUpdate;
    delete m_frpServiceExposeBtnWidgetLayout;
    delete m_frpServiceSystemBtnWidgetLayout;
    delete m_frpServiceExposeBtnWidget;
    delete m_frpServiceSystemBtnWidget;
    // base
    delete m_process;
    delete m_module;
    delete m_mainWidgetLayout;
    delete m_mainWidget;
    delete m_frpServiceExposeLayout;
}

void FrpServiceExposeWidget::lanuch()
{
    if (nullptr != m_process && QProcess::NotRunning != m_process->state())
    {
        OutputWidget::printMessageError("穿透执行中，请勿重复执行");
        return;
    }
    if (!QFile::exists("./config/frpc-service-expose.ini"))
    {
        OutputWidget::printMessageError("配置未保存，请保存配置");
        return;
    }
    OutputWidget::printMessage("服务启动中...");
    m_process        = new QProcess();
    QString     exec = "./extend/frpc";
    QStringList params;
    params << "-c"
           << "./config/frpc-service-expose.ini";
    connect(m_process, &QProcess::readyReadStandardOutput, this, &FrpServiceExposeWidget::readStdout);
    connect(m_process, &QProcess::readyReadStandardError, this, &FrpServiceExposeWidget::readStderr);
    connect(m_process, &QProcess::errorOccurred, this, &FrpServiceExposeWidget::processError);
    m_process->start(exec, params);
}

void FrpServiceExposeWidget::stopProccess()
{
    if (nullptr == m_process || QProcess::NotRunning == m_process->state())
    {
        OutputWidget::printMessageError("服务穿透未启动");
        if (nullptr != m_process)
        {
            m_process->deleteLater();
        }
        return;
    }
    m_process->close();
    delete m_process;
    m_process = nullptr;
}

void FrpServiceExposeWidget::processError(QProcess::ProcessError error)
{
    processErrorGen(error, m_module->getModuleData());
}

void FrpServiceExposeWidget::readConfig()
{
    const QMap<QString, QWidget*> configWidgetMap{
        { "server_addr", m_serverAddress }, { "server_port", m_serverPort }, { "yuno_remote_port", m_remotePort }, { "local_port", m_servicePort }, { "custom_domains", m_serverAddress }
    };
    readConfigGen("./config/frpc-service-expose.ini", configWidgetMap);
}

bool FrpServiceExposeWidget::configCheck()
{
    bool isCompleted = true;
    if (m_serverAddress->text().isEmpty())
    {
        isCompleted = false;
        OutputWidget::printMessageError("【公网域名/IP】不能为空");
    }
    return isCompleted;
}

void FrpServiceExposeWidget::saveConfig()
{
    if (configCheck())
    {
        QFile configFile("./config/frpc-service-expose.ini");
        if (configFile.open(QIODeviceBase::ReadWrite))
        {
            configFile.resize(0);
            configFile.write("[common]\n");
            configFile.write(QString("server_addr=").append(m_serverAddress->text()).append("\n").toUtf8());
            configFile.write(QString("server_port=").append(m_serverPort->text()).append("\n").toUtf8());
            configFile.write(QString("yuno_remote_port=").append(m_remotePort->text()).append("\n").toUtf8());
            configFile.write("[web]\ntype=tcp\ntype=http\n");
            configFile.write(QString("local_port=").append(m_servicePort->text()).append("\n").toUtf8());
            configFile.write(QString("custom_domains=").append(m_serverAddress->text()).append("\n\n\n").toUtf8());
            configFile.close();
            QString launchTemplate("保存成功，启动后可通过访问【http://%1:%2】穿透访问【http://localhost:%3】");
            OutputWidget::printMessage(launchTemplate.arg(m_serverAddress->text(), m_remotePort->text(), m_servicePort->text()));
        }
        else
        {
            OutputWidget::printMessageError("配置文件创建失败，请检查权限");
        }
    }
}

void FrpServiceExposeWidget::updateModuleForce()
{
    m_module->updateModule();
    m_frpServiceVersionUpdate->setDisabled(true);
}

void FrpServiceExposeWidget::updateModule()
{
    if (!baseDirCheck())
    {
        return;
    }
    bool needUpdate = m_module->updateModuleSelective();
    m_frpServiceVersionUpdate->setDisabled(!needUpdate);
}

void FrpServiceExposeWidget::clearConsole()
{
    OutputWidget::clearConsole();
}

QWidget* FrpServiceExposeWidget::createMainWidget()
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
        m_serverPort->setMinimum(1);
        m_serverPort->setValue(8080);
        m_serverPortWidgetLayout = new QHBoxLayout();
        m_serverPortWidgetLayout->addWidget(m_serverPortLabel);
        m_serverPortWidgetLayout->addWidget(m_serverPort);
        m_serverPortWidget->setLayout(m_serverPortWidgetLayout);
    }
    // remot port
    m_remotePortWidget = new QWidget();
    {
        m_remotePortLabel = new QLabel("公网服务访问端口：");
        m_remotePort      = new QSpinBox();
        m_remotePort->setMaximum(65535);
        m_remotePort->setMinimum(1);
        m_remotePort->setValue(8081);
        m_remotePortWidgetLayout = new QHBoxLayout();
        m_remotePortWidgetLayout->addWidget(m_remotePortLabel);
        m_remotePortWidgetLayout->addWidget(m_remotePort);
        m_remotePortWidget->setLayout(m_remotePortWidgetLayout);
    }
    // local service port
    m_servicePortWidget = new QWidget();
    {
        m_servicePortLabel = new QLabel("本地服务端口：");
        m_servicePort      = new QSpinBox();
        m_servicePort->setMaximum(65535);
        m_servicePort->setMinimum(1);
        m_servicePort->setValue(8082);
        m_servicePortWidgetLayout = new QHBoxLayout();
        m_servicePortWidgetLayout->addWidget(m_servicePortLabel);
        m_servicePortWidgetLayout->addWidget(m_servicePort);
        m_servicePortWidget->setLayout(m_servicePortWidgetLayout);
    }
    // control
    m_frpServiceExposeBtnWidget = new QWidget();
    {
        m_frpServiceExposeSave = new QPushButton(" 保存配置 ");
        connect(m_frpServiceExposeSave, &QPushButton::clicked, this, &FrpServiceExposeWidget::saveConfig);
        m_frpServiceExposeStop = new QPushButton(" 停止穿透 ");
        connect(m_frpServiceExposeStop, &QPushButton::clicked, this, &FrpServiceExposeWidget::stopProccess);

        m_frpServiceExposeBtnWidgetLayout = new QHBoxLayout();
        m_frpServiceExposeBtnWidgetLayout->setSpacing(20);
        m_frpServiceExposeBtnWidgetLayout->addWidget(m_frpServiceExposeSave);
        m_frpServiceExposeBtnWidgetLayout->addWidget(m_frpServiceExposeStop);
        m_frpServiceExposeBtnWidget->setLayout(m_frpServiceExposeBtnWidgetLayout);
    }
    m_frpServiceSystemBtnWidget = new QWidget();
    {
        m_frpServiceExposeClear = new QPushButton(" 清空控制台 ");
        connect(m_frpServiceExposeClear, &QPushButton::clicked, this, &FrpServiceExposeWidget::clearConsole);
        m_frpServiceVersionUpdate = new QPushButton(" 版本更新 ");
        connect(m_frpServiceVersionUpdate, &QPushButton::clicked, this, &FrpServiceExposeWidget::updateModuleForce);

        m_frpServiceSystemBtnWidgetLayout = new QHBoxLayout();
        m_frpServiceSystemBtnWidgetLayout->setSpacing(20);
        m_frpServiceSystemBtnWidgetLayout->addWidget(m_frpServiceExposeClear);
        m_frpServiceSystemBtnWidgetLayout->addWidget(m_frpServiceVersionUpdate);
        m_frpServiceSystemBtnWidget->setLayout(m_frpServiceSystemBtnWidgetLayout);
    }

    // layout
    m_mainWidgetLayout = new QVBoxLayout();
    m_mainWidgetLayout->addWidget(m_serverAddressWidget);
    m_mainWidgetLayout->addWidget(m_serverPortWidget);
    m_mainWidgetLayout->addWidget(m_remotePortWidget);
    m_mainWidgetLayout->addWidget(m_servicePortWidget);
    m_mainWidgetLayout->addWidget(m_frpServiceExposeBtnWidget);
    m_mainWidgetLayout->addWidget(m_frpServiceSystemBtnWidget);
    m_mainWidget->setLayout(m_mainWidgetLayout);
    return m_mainWidget;
}
