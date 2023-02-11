#ifndef HOMERECENTWIDGET_H
#define HOMERECENTWIDGET_H

#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QSplitter>
#include <QVBoxLayout>

#include "CAutoResizer.h"
#include "CEqualBoxLayout.h"
#include "CTabButton.h"
#include "Display/FileListWidget.h"
#include "Frames/LinearScrollArea.h"
#include "QPixelSize.h"
#include "QsFrameworkMacros.h"

/**
 * @brief Recent widget top frame
 */
class HomeRecentTopFrame : public QFrame {
    Q_OBJECT
    Q_LAYOUT_PROPERTY_DELCARE
public:
    explicit HomeRecentTopFrame(QWidget *parent = nullptr);
    ~HomeRecentTopFrame();

    void reloadStrings();

    CTabButton *newButton;
    CTabButton *openButton;
    CTabButton *importButton;

    QLineEdit *searchBox;

    CEqualBoxLayout *topLayout;

signals:

private:
    void _q_newButtonClicked();
    void _q_openButtonClicked();
    void _q_importButtonClicked();
};

/**
 * @brief Recent widget
 */
class HomeRecentWidget : public QSplitter {
    Q_OBJECT
    Q_PROPERTY(QTypeList styleData READ styleData WRITE setStyleData NOTIFY styleDataChanged)
public:
    explicit HomeRecentWidget(QWidget *parent = nullptr);
    ~HomeRecentWidget();

    void reloadStrings();

    HomeRecentTopFrame *topWidget;
    LinearScrollArea *bottomWidget;

    QLabel *recentEmptyLabel;
    FileListWidget *recentListWidget;

    QFrame *recentContainer;
    QVBoxLayout *recentContainerLayout;
    CAutoResizer *recentListWidgetResizer;

    QIcon icon;
    QSize iconSize;

    QTypeList styleData() const;
    void setStyleData(const QTypeList &value);

    void reloadRecentList();

signals:
    void styleDataChanged();

private:
    void _q_searchTextChanged(const QString &text);
    void _q_recentItemClicked(const QModelIndex &index, int button);
    void _q_recentCommited();
};

#endif // HOMERECENTWIDGET_H
