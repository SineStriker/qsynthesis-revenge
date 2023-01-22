#ifndef UTATRANSLATOR_H
#define UTATRANSLATOR_H

#include "Utau/QUstPoint.h"

#include "QsSVSGlobal.h"

namespace UtaTranslator {

    QSSVS_API QString LoadingBar(int n, int sum);

    QSSVS_API QString EnvelopeUstToBat(const QString &s, const double &overlap);

    QSSVS_API QStringList EnvelopeToStringList(const QList<QUstPoint> &tpoints,
                                               const double &overlap);

    QSSVS_API void getCorrectPBSY(int prevNoteNum, const QString &prevLyric, int curNoteNum,
                                  QUstPoint &curPoint);

    QSSVS_API QList<QUstPoint> getDefaultPitch(const int &prevNoteNum, const QString &prevLyric,
                                               const int &curNoteNum);

    QSSVS_API QString fixFlags(const QString &s);

    QSSVS_API QString fixFilename(const QString &filename);

}; // namespace UtaTranslator

#endif // UTATRANSLATOR_H
