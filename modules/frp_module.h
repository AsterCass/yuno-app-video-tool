
#ifndef FRPMODULE_H
#define FRPMODULE_H

#include "core_module.h"
#include "utils/http_service.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtConcurrent/QtConcurrent>

class FrpModule : public CoreModule
{
public:
    FrpModule();
    ~FrpModule() override;

public:
    bool           updateModuleSelective();
    void           updateModule();
    CoreModuleData getModuleData();

private:
    void downloadFinish(FrpResult result);

private:
    static bool isUpdated;
};

#endif  // FRPMODULE_H
