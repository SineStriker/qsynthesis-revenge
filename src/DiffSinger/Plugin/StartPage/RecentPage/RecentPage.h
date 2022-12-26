#ifndef RECENTPAGE_H
#define RECENTPAGE_H

#include "Api/IStartPage.h"

class Q_DECL_EXPORT RecentPage : public IStartPage {
    Q_OBJECT
    Q_INTERFACES(IStartPage)
    Q_PLUGIN_METADATA(IID IStartPage_IID FILE "plugin.json")
public:
    explicit RecentPage(QObject *parent = nullptr);
    ~RecentPage();
};

#endif // RECENTPAGE_H
