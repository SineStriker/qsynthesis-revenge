#ifndef QVOICEINFO_H
#define QVOICEINFO_H

#include "Macros.h"
#include "SpecialConfig.h"

// Brief Information of Voice Database
class QVoiceInfo : public SpecialConfig {
    Q_CHARSET
public:
    QVoiceInfo();
    QVoiceInfo(const QString &dirname);
    ~QVoiceInfo();

public:
    QString name() const;
    QString title() const;
    QString author() const;
    QString avatar() const;
    QString sprite() const;

    void setName(const QString &name);
    void setAuthor(const QString &author);
    void setAvatar(const QString &avatar);
    void setSprite(const QString &sprite);

protected:
    bool loadCore(const QString &filename, bool *valid) override;
    bool saveCore(const QString &filename) override;

    void resetCore() override;

protected:
    QString m_name;
    QString m_author;
    QString m_avatar;
    QString m_sprite;

    QStringList m_custom; // Other information which can't be edited

private:
    QString configName() const override;
};

#endif // QVOICEINFO_H
