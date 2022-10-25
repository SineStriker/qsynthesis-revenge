#ifndef RENDERARGS_H
#define RENDERARGS_H

#include "ResamplerArgs.h"
#include "WavtoolArgs.h"

class RenderArgs {
public:
    RenderArgs();
    RenderArgs(const ResamplerArgs &res, const WavtoolArgs &wav);

    QString wavtoolPath;
    QString resamplerPath;

    ResamplerArgs resamplerArgs;
    WavtoolArgs wavtoolArgs;

    void clear();

    bool operator==(const RenderArgs &oArgs) const;
    bool operator!=(const RenderArgs &oArgs) const;
};

#endif // RENDERARGS_H
