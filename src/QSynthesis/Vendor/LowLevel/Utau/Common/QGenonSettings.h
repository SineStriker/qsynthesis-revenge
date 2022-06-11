#ifndef QGenonSettings_H
#define QGenonSettings_H

#include "QCorrectGenon.h"
#include "QUtauStd.h"

// Normal Oto Sample
class Utau::Genon {
public:
    QString mFileName;
    QString mAlias;
    double mOffset;
    double mConstant; // Also signed as Consonant
    double mBlank;
    double mPreUtterance;
    double mVoiceOverlap;

    Genon();
    Genon(const QString &filename);

    void clear();
    bool isEmpty() const;

    bool operator<(const Genon &oGenon) const;
    bool operator==(const Genon &oGenon) const;
    bool operator!=(const Genon &oGenon) const;

    friend QDebug operator<<(QDebug debug, const Genon &oGenon);
};

typedef Utau::Genon QGenonSettings;

Q_DECLARE_METATYPE(QGenonSettings)

#endif // QGenonSettings_H
