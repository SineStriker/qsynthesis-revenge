#ifndef QSVGURI_H
#define QSVGURI_H

#include <QDebug>
#include <QIcon>
#include <QStringList>

#include "QMGuiGlobal.h"

class QMGUI_EXPORT QSvgUri {
public:
    explicit QSvgUri();
    QSvgUri(const QString &filename);
    QSvgUri(const QString &filename, const QColor &color);
    ~QSvgUri();

    enum Mode {
        Normal = 1,
        Disabled = 2,
        Active = 4,
        Selected = 8,
    };

    Q_DECLARE_FLAGS(Modes, Mode)

public:
    QStringList toStringList() const;
    QIcon toIcon(int modes = Normal) const;

    QString filename() const;
    void setFilename(const QString &filename);

    QString currentColor() const;
    void setCurrentColor(const QString &currentColor);

    bool isNull() const;

    inline operator QIcon() const{
        return toIcon();
    }

private:
    QString m_filename;
    QString m_currentColor;

public:
    static QSvgUri fromStringList(const QStringList &stringList);

    static QLatin1String MetaFunctionName();

    QMGUI_EXPORT friend QDebug operator<<(QDebug debug, const QSvgUri &uri);
};

Q_DECLARE_METATYPE(QSvgUri)

#endif // QSVGURI_H
