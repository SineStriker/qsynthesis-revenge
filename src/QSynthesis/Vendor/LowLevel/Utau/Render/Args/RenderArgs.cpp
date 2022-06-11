#include "RenderArgs.h"

RenderArgs::RenderArgs() {
}

RenderArgs::RenderArgs(const ResamplerArgs &res, const WavtoolArgs &wav) {
    resamplerArgs = res;
    wavtoolArgs = wav;
}

void RenderArgs::clear() {
    wavtoolPath.clear();
    resamplerPath.clear();
    resamplerArgs.clear();
    wavtoolArgs.clear();
}

bool RenderArgs::operator==(const RenderArgs &oArgs) const {
    return (resamplerArgs == oArgs.resamplerArgs) && (resamplerPath == oArgs.resamplerPath);
}

bool RenderArgs::operator!=(const RenderArgs &oArgs) const {
    return !((*this) == oArgs);
}
