#ifndef QPREFIXMAP_H
#define QPREFIXMAP_H

#include "SpecialConfig.h"
#include "Macros.h"

#include <QMap>

class QPrefixMap : public SpecialConfig {
    Q_CHARSET
public:
    QPrefixMap();
    QPrefixMap(const QString &filename);
    ~QPrefixMap();

    QMap<int, QString> PrefixMap;
    QMap<int, QString> SuffixMap;

public:
    QString prefixedLyric(int oNoteNum, const QString &oLyric) const;
    bool isEmpty() const;

protected:
    bool loadCore(const QString &filename, bool *valid) override;
    bool saveCore(const QString &filename) override;

    void resetCore() override;

private:
    QString configName() const override;
};

#endif // QPREFIXMAP_H
