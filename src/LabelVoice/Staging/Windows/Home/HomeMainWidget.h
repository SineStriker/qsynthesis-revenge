#ifndef HOMEMAINWIDGET_H
#define HOMEMAINWIDGET_H

#include <QFrame>

#include <QBoxLayout>
#include <QLabel>
#include <QSplitter>
#include <QLineEdit>

#include "FileListWidget.h"
#include "CTabButton.h"

class HomeMainWidget : public QFrame {
    Q_OBJECT
public:
    explicit HomeMainWidget(QWidget *parent = nullptr);
    ~HomeMainWidget();

    void reloadStrings();
    void reloadTemplates();

    QTypeList styleData() const;
    void setStyleData(const QTypeList &list);

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

private:
    void _q_searchBoxChanged(const QString &text);
    void _q_templateItemClicked(const QModelIndex &index, int button);

signals:
    void newRequested(int type);
};

#endif // HOMEMAINWIDGET_H
