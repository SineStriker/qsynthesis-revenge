#ifndef HOMEWINDOWPRIVATE_H
#define HOMEWINDOWPRIVATE_H

#include "Controls/Windows/PlainWindow_p.h"

#include "../FileListWidget.h"
#include "../HomeWindow.h"

#include "CTabButton.h"

#include <QBoxLayout>
#include <QWidget>

#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QSplitter>

class HomeWindowPrivate : public PlainWindowPrivate {
    Q_DECLARE_PUBLIC(HomeWindow)
public:
    HomeWindowPrivate();
    virtual ~HomeWindowPrivate();

    void init();

    void reloadStrings_helper();

    QWidget *w;

    QWidget *leftWidget;
    QWidget *rightWidget;

    QHBoxLayout *mainLayout;
    QVBoxLayout *leftLayout, *rightLayout;

    QSplitter *splitter;

    CTabButton *titleLabel;
    QLabel *subtitleLabel;

    FileListWidget *templateList, *recentList;
    QLineEdit *searchBox;

    // Style
    struct TemplateConfig {
        QIcon icon;
        QSize iconSize;
        QString title;
        QString subtitle;
        QString cont;
        int id;
    };
    TemplateConfig emptyItemConfig;
    TemplateConfig opencpopItemConfig;
    TemplateConfig diffItemConfig;
    TemplateConfig openvpiItemConfig;

    void reloadTemplates();
};

#endif // HOMEWINDOWPRIVATE_H
