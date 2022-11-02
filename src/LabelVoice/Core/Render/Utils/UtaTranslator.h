#ifndef UTATRANSLATOR_H
#define UTATRANSLATOR_H

#include "Utau/QUstPoint.h"

#include "lvcore_global.h"

namespace UtaTranslator {

    LVCORE_API QString LoadingBar(int n, int sum);

    LVCORE_API QString EnvelopeUstToBat(const QString &s, const double &overlap);

    LVCORE_API QStringList EnvelopeToStringList(const QList<QUstPoint> &tpoints,
                                                   const double &overlap);

    LVCORE_API void getCorrectPBSY(int prevNoteNum, const QString &prevLyric, int curNoteNum,
                                      QUstPoint &curPoint);

    LVCORE_API QList<QUstPoint> getDefaultPitch(const int &prevNoteNum, const QString &prevLyric,
                                                   const int &curNoteNum);

    LVCORE_API QString fixFlags(const QString &s);

    LVCORE_API QString fixFilename(const QString &filename);

}; // namespace UtaTranslator

#endif // UTATRANSLATOR_H
