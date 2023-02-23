#ifndef THEMETEMPLATE_H
#define THEMETEMPLATE_H

#include <QColor>
#include <QSharedPointer>
#include <QString>

#include "ThemeConfig.h"

// Parse theme qss.in file

class ThemeTemplate {
public:
    ThemeTemplate();
    ~ThemeTemplate();

public:
    QString content;
    QString pattern;
    QString separator;

    bool load(const QString &filename);

    QString parse(const QMap<QString, ThemeConfig::Value> &vals, double dpi) const;
};

#endif // THEMETEMPLATE_H
