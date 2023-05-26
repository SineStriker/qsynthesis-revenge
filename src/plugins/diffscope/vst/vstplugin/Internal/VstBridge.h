//
// Created by Crs_1 on 2023/5/26.
//

#ifndef CHORUSKIT_VSTBRIDGE_H
#define CHORUSKIT_VSTBRIDGE_H

#include "rep_VstBridge_source.h"

namespace Vst {

    class VstBridge: public VstBridgeSource {
        Q_OBJECT
    public:
        explicit VstBridge(QObject *parent = nullptr);

    private:
        bool testProcess(int64_t pos, int32_t size) override;
    };

} // Vst

#endif // CHORUSKIT_VSTBRIDGE_H
