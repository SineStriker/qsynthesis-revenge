#ifndef HOMEMAINWIDGET_H
#define HOMEMAINWIDGET_H

#include <QFrame>

#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSplitter>

#include "CTabButton.h"
#include "Editor/Display/FileListWidget.h"

#include "Windows/Basic/ProjectCommonBlock.h"

class HomeMainWidget : public QFrame {
    Q_OBJECT
public:
    explicit HomeMainWidget(ProjectCommonBlock *block, QWidget *parent = nullptr);
    ~HomeMainWidget();

    void reloadStrings();
    void reloadTemplates();

    void reloadRecentList();

    QTypeList templateStyleData() const;
    void setTemplateStyleData(const QTypeList &list);

    QTypeList recentStyleData() const;
    void setRecentStyleData(const QTypeList &list);

    QWidget *leftWidget;
    QWidget *rightWidget;

    QSplitter *splitter;

    CTabButton *titleLabel;

    QLabel *subtitleLabel;
    CPushButton *openButton;

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

    struct RecentConfig {
        QIcon icon;
        QSize iconSize;
    };

    TemplateConfig emptyItemConfig;
    TemplateConfig opencpopItemConfig;
    TemplateConfig diffItemConfig;
    TemplateConfig openvpiItemConfig;

    RecentConfig recentFileConfig;
    RecentConfig recentDirConfig;

protected:
    ProjectCommonBlock *block;

    QHBoxLayout *mainLayout;
    QVBoxLayout *leftLayout, *rightLayout;
    QHBoxLayout *subtitleButtonLayout;

private:
    void _q_searchBoxChanged(const QString &text);
    void _q_templateItemClicked(const QModelIndex &index, int button);
    void _q_recentItemClicked(const QModelIndex &index, int button);

    void _q_recentCommited();

signals:
    void newRequested(int type);
    void openRequested(const QString &filename);
};

#endif // HOMEMAINWIDGET_H
