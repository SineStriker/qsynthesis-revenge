#ifndef __THEMETEMPLATE_H__
#define __THEMETEMPLATE_H__

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

#endif // __THEMETEMPLATE_H__
