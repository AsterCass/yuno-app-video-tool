
#ifndef FRPFILEEXPOSEWIDGET_H
#define FRPFILEEXPOSEWIDGET_H

#include "http_service.h"
#include "output_widget.h"
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QProcess>
#include <QPushButton>
#include <QSpinBox>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QWidget>
#include <QtConcurrent/QtConcurrent>

class FrpFileExposeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FrpFileExposeWidget(QWidget* parent = nullptr);
    ~FrpFileExposeWidget() override;

public:
    void lanuch();

private:
    QWidget* createMainWidget();

private:
    void selectExposePath();
    bool configCheck();
    void saveConfig();
    void readConfig();
    void updateModule();
    void stopProccess();
    void clearConsole();

private:
    void readStdout();
    void readStderr();
    void processError(QProcess::ProcessError error);

private:
    // base
    QVBoxLayout* m_frpFileExposeLayout;
    QWidget*     m_mainWidget;
    QVBoxLayout* m_mainWidgetLayout;
    QProcess*    m_process = nullptr;
    // server address
    QLabel*      m_serverAddressLabel;
    QLineEdit*   m_serverAddress;
    QHBoxLayout* m_serverAddressWidgetLayout;
    QWidget*     m_serverAddressWidget;
    // server port
    QLabel*      m_serverPortLabel;
    QSpinBox*    m_serverPort;
    QHBoxLayout* m_serverPortWidgetLayout;
    QWidget*     m_serverPortWidget;
    // remot port
    QLabel*      m_remotePortLabel;
    QSpinBox*    m_remotePort;
    QHBoxLayout* m_remotePortWidgetLayout;
    QWidget*     m_remotePortWidget;
    // expose path
    QPushButton* m_selectExposePath;
    QLabel*      m_exposePath;
    QHBoxLayout* m_exposePathWidgetLayout;
    QWidget*     m_exposePathWidget;
    // strip path
    QLabel*      m_stripPathLabel;
    QLineEdit*   m_stripPath;
    QHBoxLayout* m_stripPathWidgetLayout;
    QWidget*     m_stripPathWidget;
    // control
    QPushButton* m_frpFileExposeSave;
    QPushButton* m_frpFileExposeStop;
    QPushButton* m_frpFileExposeClear;
    QHBoxLayout* m_frpFileExposeBtnWidgetLayout;
    QWidget*     m_frpFileExposeBtnWidget;

    //    const QMap<QString, QWidget*> configWidgetMap{ { "server_addr", m_serverAddress } };
};

#endif  // FRPFILEEXPOSEWIDGET_H
