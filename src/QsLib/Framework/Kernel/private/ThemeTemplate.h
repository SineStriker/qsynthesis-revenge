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
    int left;
    int right;

    bool load(const QString &filename);

    QString parse(const QMap<QString, QString> &colors, const QMap<QString, int> &sizes) const;
};

#endif // __THEMETEMPLATE_H__
