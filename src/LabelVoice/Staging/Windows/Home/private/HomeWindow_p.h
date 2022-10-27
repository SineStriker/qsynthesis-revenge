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

#include "../LabelTextBlock.h"

class HomeWindowPrivate : public PlainWindowPrivate {
    Q_DECLARE_PUBLIC(HomeWindow)
public:
    HomeWindowPrivate();
    virtual ~HomeWindowPrivate();

    void init();

    // Init
    void initHome();

    void reloadStrings_helper();

    // Home
    QWidget *w;

    QWidget *leftWidget;
    QWidget *rightWidget;

    QHBoxLayout *mainLayout;
    QVBoxLayout *leftLayout, *rightLayout;

    QSplitter *splitter;

    CTabButton *titleLabel;

    QLabel *subtitleLabel;
    CPushButton *openButton;
    QHBoxLayout *subtitleButtonLayout;

    FileListWidget *templateList, *recentList;
    QLineEdit *searchBox;

    enum TemplateTypes {
        Empty,
        Opencpop,
        DiffSinger,
        OpenVPI,
    };
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

    // Empty
    struct CreateBlock {
        QWidget *w;

        QHBoxLayout *upperLayout;
        QVBoxLayout *entityLayout;

        QLabel *titleLabel;
        QLabel *subtitleLabel;

        LabelTextBlock *projectName;
        LabelTextBlock *location;
        LabelTextBlock *solution;

        CPushButton *backButton;
        CPushButton *createButton;

        QVBoxLayout *mainLayout;
        QHBoxLayout *buttonLayout;
    };

    CreateBlock cb;

    void cb_init();
    void cb_switchIn();
    void cb_switchOut();

    void cb_clear();
    void cb_reloadStrings();
};

#endif // HOMEWINDOWPRIVATE_H
