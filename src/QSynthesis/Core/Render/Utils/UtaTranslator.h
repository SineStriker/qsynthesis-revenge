#ifndef UTATRANSLATOR_H
#define UTATRANSLATOR_H

#include "Utau/QUstPoint.h"

namespace UtaTranslator {

    QString LoadingBar(int n, int sum);

    QString EnvelopeUstToBat(const QString &s, const double &overlap);

    QStringList EnvelopeToStringList(const QList<QUstPoint> &tpoints, const double &overlap);

    void getCorrectPBSY(int prevNoteNum, const QString &prevLyric, int curNoteNum,
                        QUstPoint &curPoint);

    QList<QUstPoint> getDefaultPitch(const int &prevNoteNum, const QString &prevLyric,
                                         const int &curNoteNum);

    QString fixFlags(const QString &s);

    QString fixFilename(const QString &filename);

}; // namespace UtaTranslator

#endif // UTATRANSLATOR_H
