#ifndef QUPLUGININFO_H
#define QUPLUGININFO_H

#include "Macros.h"
#include "SpecialConfig.h"

class QUPluginInfo : public SpecialConfig {
    Q_CHARSET
public:
    QUPluginInfo();
    QUPluginInfo(const QString &dirname);
    ~QUPluginInfo();

protected:
    bool loadCore(const QString &filename, bool *valid) override;
    bool saveCore(const QString &filename) override;

    void resetCore() override;

public:
    bool useShell() const;
    QString name() const;
    QString ustVersion() const;
    bool allNote() const;
    bool valid() const;
    QString charset() const;
    QStringList custom() const;

    QString exePath() const;
    void setExePath(const QString &path);

    void setUseShell(bool useShell);
    void setAllNote(bool allNote);
    void setCharset(const QString &charset);
    void setName(const QString &name);
    void setUstVersion(const QString &ustVersion);
    void setCustom(const QStringList &custom);

protected:
    bool m_useShell;
    bool m_allNote;

    QString m_charset;
    QString m_name;
    QString m_ustVersion;
    QString m_execute;

    QStringList m_custom; // Other information which can't be edited

private:
    QString configName() const override;
};

#endif // QUPLUGININFO_H
