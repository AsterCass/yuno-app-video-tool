
#include "core_function.h"

CoreFunction::CoreFunction(QWidget* parent) : QWidget{ parent } {}

CoreFunction::~CoreFunction() {}

bool CoreFunction::baseDirCheck()
{
    bool configBuild = QDir("config").exists() ? true : QDir().mkdir("config");
    bool extendBuild = QDir("extend").exists() ? true : QDir().mkdir("extend");
    bool result      = configBuild && extendBuild;
    if (!result)
    {
        OutputWidget::printMessageError("基础文件夹配置失败，请检查权限");
    }
    return result;
}

void CoreFunction::readStderr()
{
    while (m_process->canReadLine())
    {
        OutputWidget::printMessageError(m_process->readLine());
    }
}

void CoreFunction::readStdout()
{
    while (m_process->canReadLine())
    {
        OutputWidget::printMessage(m_process->readLine());
    }
}

void CoreFunction::processErrorGen(QProcess::ProcessError error, CoreModuleData moduleData)
{
    switch (error)
    {
    case QProcess::FailedToStart:
        OutputWidget::printMessageError(QString("辅助程序无法启动，请尝试：\n"
                                                "【1】查看是否Windows防护或者其他杀毒软件拦截，解除拦截，还原隔离文件\n"
                                                "【2】删除目录下除了exe可执行文件外其他文件，以管理员权限重新执行\n"
                                                "【3】如果以上方法都不生效，请自行在%1下载%2可执行文件%3到extend文件夹目录下")
                                            .arg(moduleData.from, moduleData.name, moduleData.executeFileName));
        break;
    case QProcess::Crashed:
        OutputWidget::printMessageWarn("服务已终止");
        break;
    default:
        OutputWidget::printMessageError("辅助程序执行发生未知错误，请联系【astercass@qq.com】");
    }
}

void CoreFunction::readConfigGen(QString path, const QMap<QString, QWidget*> configWidgetMap)
{
    QFile configFile(path);
    if (!configFile.open(QIODeviceBase::ReadOnly))
    {
        //        OutputWidget::printMessage(QString("未找到默认配置"));
        //        qDebug() << QString("未在【").append(QDir::currentPath()).append("/config").append("】下找到默认配置");
        return;
    }
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
