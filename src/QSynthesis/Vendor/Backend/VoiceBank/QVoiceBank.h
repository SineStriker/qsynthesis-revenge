#ifndef QVOICEBANK_H
#define QVOICEBANK_H

#include "QOtoIni.h"
#include "QPrefixMap.h"
#include "QReadmeText.h"
#include "QVoiceInfo.h"

class QVoiceBank {
public:
    QVoiceBank();
    ~QVoiceBank();

    QVoiceInfo CharInfo;
    QReadmeText ReadmeTxt;
    QPrefixMap PrefixMap;
    QMap<QString, QOtoIni> OtoInis;

public:
    QString dirname() const;
    void setDirname(const QString &dirname);

    void reset();
    void clear();

    bool load(bool *overload = nullptr);
    bool save();

    const int SUBDIR_MAX = 100;

protected:
    QString m_dirname;

private:
    bool loadOtos(bool *overload);
    bool saveOtos();
};

#endif // QVOICEBANK_H
