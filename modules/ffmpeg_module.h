
#ifndef FFMPEGMODULE_H
#define FFMPEGMODULE_H

#include "core_module.h"

class FfmpegModule : public CoreModule
{
public:
    FfmpegModule();
    ~FfmpegModule() override;

public:
    bool           updateModuleSelective();
    void           updateModule();
    CoreModuleData getModuleData();
};

#endif  // FFMPEGMODULE_H
