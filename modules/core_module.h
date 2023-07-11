
#ifndef COREMODULE_H
#define COREMODULE_H

#include "output_widget.h"
#include <QDebug>
#include <QFile>

struct CoreModuleData
{
    QString from;
    QString name;
    QString executeFileName;
};

class CoreModule
{
public:
    CoreModule();
    virtual ~CoreModule();

public:
    virtual bool           updateModuleSelective() = 0;
    virtual void           updateModule()          = 0;
    virtual CoreModuleData getModuleData()         = 0;
};

#endif  // COREMODULE_H
