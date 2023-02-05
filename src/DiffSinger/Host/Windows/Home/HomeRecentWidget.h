#ifndef HOMERECENTWIDGET_H
#define HOMERECENTWIDGET_H

#include <QFrame>
#include <QLineEdit>
#include <QSplitter>
#include <QVBoxLayout>

#include "CTabButton.h"
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
    CTabButton *importButton;
    QLineEdit *searchBox;

    QHBoxLayout *topLayout;
signals:
};

/**
 * @brief Recent widget
 */
class HomeRecentWidget : public QSplitter {
    Q_OBJECT
public:
    explicit HomeRecentWidget(QWidget *parent = nullptr);
    ~HomeRecentWidget();

    void reloadStrings();

    HomeRecentTopFrame *topWidget;
    LinearScrollArea *bottomWidget;
signals:
};

#endif // HOMERECENTWIDGET_H
