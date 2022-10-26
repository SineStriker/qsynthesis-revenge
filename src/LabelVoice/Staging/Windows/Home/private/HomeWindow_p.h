#ifndef HOMEWINDOWPRIVATE_H
#define HOMEWINDOWPRIVATE_H

#include "Controls/Windows/PlainWindow_p.h"

#include "../FileListWidget.h"
#include "../HomeWindow.h"

#include <QBoxLayout>
#include <QWidget>

#include <QLabel>
#include <QLineEdit>
#include <QSplitter>
#include <QListWidget>
#include <QPushButton>

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

    QLabel *titleLabel;
    QLabel *subtitleLabel;

    FileListWidget *templateList, *recentList;
    QLineEdit *searchBox;
};

#endif // HOMEWINDOWPRIVATE_H
