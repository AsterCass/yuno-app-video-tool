
#ifndef COREFUNCTION_H
#define COREFUNCTION_H

#include "modules/core_module.h"
#include "output_widget.h"
#include <QDebug>
#include <QDir>
#include <QLabel>
#include <QLineEdit>
#include <QProcess>
#include <QSpinBox>
#include <QTextBrowser>
#include <QWidget>

class CoreFunction : public QWidget
{
    Q_OBJECT
public:
    explicit CoreFunction(QWidget* parent = nullptr);
    virtual ~CoreFunction() override;

public:
    virtual void lanuch() = 0;
    inline void  setModule(CoreModule* module)
    {
        m_module = module;
    }
    inline void updateModule()
    {
        m_module->updateModule();
    }

protected:
    void readStdout();
    void readStderr();
    void processErrorGen(QProcess::ProcessError error, CoreModuleData moduleData);
    void readConfigGen(QString path, const QMap<QString, QWidget*> configWidgetMap);
    bool baseDirCheck();

protected:
    CoreModule* m_module  = nullptr;
    QProcess*   m_process = nullptr;
};

#endif  // COREFUNCTION_H
