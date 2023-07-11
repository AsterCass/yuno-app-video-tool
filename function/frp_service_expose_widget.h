
#ifndef FRPSERVICEEXPOSEWIDGET_H
#define FRPSERVICEEXPOSEWIDGET_H

#include "function/core_function.h"
#include "modules/core_module.h"
#include "modules/frp_module.h"
#include <QLabel>
#include <QLineEdit>
#include <QProcess>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>

class FrpServiceExposeWidget : public CoreFunction
{
    Q_OBJECT
public:
    explicit FrpServiceExposeWidget(QWidget* parent = nullptr);
    ~FrpServiceExposeWidget() override;

public:
    void lanuch();

private:
    void readConfig();
    bool configCheck();
    void saveConfig();

    void stopProccess();
    void updateModule();
    void updateModuleForce();
    void clearConsole();

private:
    void processError(QProcess::ProcessError error);

private:
    QWidget* createMainWidget();

private:
    // base
    QVBoxLayout* m_frpServiceExposeLayout;
    QWidget*     m_mainWidget;
    QVBoxLayout* m_mainWidgetLayout;
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
    // local service port
    QLabel*      m_servicePortLabel;
    QSpinBox*    m_servicePort;
    QHBoxLayout* m_servicePortWidgetLayout;
    QWidget*     m_servicePortWidget;
    // control
    QPushButton* m_frpServiceExposeSave;
    QPushButton* m_frpServiceExposeStop;
    QPushButton* m_frpServiceExposeClear;
    QPushButton* m_frpServiceVersionUpdate;
    QHBoxLayout* m_frpServiceExposeBtnWidgetLayout;
    QWidget*     m_frpServiceExposeBtnWidget;
    QHBoxLayout* m_frpServiceSystemBtnWidgetLayout;
    QWidget*     m_frpServiceSystemBtnWidget;
};

#endif  // FRPSERVICEEXPOSEWIDGET_H
