#ifndef UTATRANSLATOR_H
#define UTATRANSLATOR_H

#include "../../Common/QControlPoint.h"

namespace UtaTranslator {

    QString LoadingBar(int n, int sum);

    QString EnvelopeUstToBat(const QString &s, const double &overlap);

    QStringList EnvelopeToStringList(const QList<QControlPoint> &tpoints, const double &overlap);

    void getCorrectPBSY(int prevNoteNum, const QString &prevLyric, int curNoteNum,
                        QControlPoint &curPoint);

    QList<QControlPoint> getDefaultPitch(const int &prevNoteNum, const QString &prevLyric,
                                         const int &curNoteNum);

    QString fixFlags(const QString &s);

    QString fixFilename(const QString &filename);

}; // namespace UtaTranslator

#endif // UTATRANSLATOR_H
