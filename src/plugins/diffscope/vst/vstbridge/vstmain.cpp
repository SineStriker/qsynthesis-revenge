#include "DiffScopeVstBridge.h"

using namespace Vst;

extern "C" {

Q_DECL_EXPORT DiffScopeVstBridge *CreateInstance() {
    return new DiffScopeVstBridge;
}

Q_DECL_EXPORT void DeleteInstance(DiffScopeVstBridge *instance) {
    delete instance;
}

}