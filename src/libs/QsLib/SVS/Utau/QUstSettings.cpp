#include "QUstSettings.h"

#include "Config/UtaConstants.h"

QUstSettings::QUstSettings() {
    QUstSettings::reset();
}

QUstSettings::~QUstSettings() {
}

void QUstSettings::reset() {
    globalTempo = Utau::DEFAULT_VALUE_TEMPO;
    globalFlags.clear();
    projectName.clear();
    outputFileName.clear();
    voiceDirectory.clear();
    cacheDirectory.clear();
    wavtoolPath.clear();
    resamplerPath.clear();
    isMode2 = true;
}

bool QUstSettings::operator==(const QUstSettings &another) const {
    return (globalTempo == another.globalTempo) && (globalFlags == another.globalFlags) &&
           (projectName == another.projectName) && (outputFileName == another.outputFileName) &&
           (voiceDirectory == another.voiceDirectory) &&
           (cacheDirectory == another.cacheDirectory) && (wavtoolPath == another.wavtoolPath) &&
           (resamplerPath == another.resamplerPath) && (isMode2 == another.isMode2);
}

bool QUstSettings::operator!=(const QUstSettings &another) const {
    return !((*this) == another);
}
