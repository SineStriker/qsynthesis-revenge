#ifndef CCOUPLETABFRAME_H
#define CCOUPLETABFRAME_H

#include <QGridLayout>
#include <QWidget>

#include "qsframework_global.h"

#include "CCoupleTabWidgetImpl/CCoupleTabDoubleBar.h"
#include "CCoupleTabWidgetImpl/CCoupleTabDragProxy.h"
#include "CCoupleTabWidgetImpl/CCoupleTabSplitter.h"
#include "CCoupleTabWidgetImpl/CCoupleTabContainer.h"

class QSFRAMEWORK_API CCoupleTabFrame : public QWidget {
    Q_OBJECT
    Q_PROPERTY(
        QSize dragAreaSize READ dragAreaSize WRITE setDragAreaSize NOTIFY dragAreaSizeChanged)
public:
    explicit CCoupleTabFrame(QWidget *parent = nullptr);
    ~CCoupleTabFrame();

private:
    void init();

public:
    QWidget *widget() const;
    void setWidget(QWidget *w);
    QWidget *takeWidget();

    CCoupleTabDoubleBar *leftBar() const;
    CCoupleTabDoubleBar *topBar() const;
    CCoupleTabDoubleBar *rightBar() const;
    CCoupleTabDoubleBar *bottomBar() const;

    QSize dragAreaSize() const;
    void setDragAreaSize(const QSize &dragAreaSize);

    bool barVisible() const;
    void setBarVisible(bool visible);
    void toggleBarVisible();

protected:
    QGridLayout *m_layout;

    CCoupleTabDoubleBar *m_leftBar;
    CCoupleTabDoubleBar *m_topBar;
    CCoupleTabDoubleBar *m_rightBar;
    CCoupleTabDoubleBar *m_bottomBar;

    CCoupleTabContainer *m_leftPanel;
    CCoupleTabContainer *m_topPanel;
    CCoupleTabContainer *m_rightPanel;
    CCoupleTabContainer *m_bottomPanel;

    CCoupleTabSplitter *m_verticalSplitter;
    CCoupleTabSplitter *m_horizontalSplitter;

    QStackedWidget *m_centralContainer;

    CCoupleTabDragProxy *m_dragProxy;

    QSize m_dragAreaSize;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void handleCardAdded(CCoupleTabTypes::Number number, CCoupleTabBarCard *card);
    void handleCardRemoved(CCoupleTabTypes::Number number, CCoupleTabBarCard *card);
    void handleCardToggled(CCoupleTabTypes::Number number, CCoupleTabBarCard *card);

signals:
    void dragAreaSizeChanged();
    void cardToggled(Qt::Edge edge, CCoupleTabTypes::Number number, CCoupleTabBarCard *card);
    void barVisibleToggled(bool visible);
};

#endif // CCOUPLETABFRAME_H
