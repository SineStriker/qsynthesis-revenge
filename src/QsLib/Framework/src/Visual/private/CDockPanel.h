#ifndef __CDOCKPANEL_H__
#define __CDOCKPANEL_H__

#include <QSplitter>
#include <QStackedWidget>

#include "../CVisualTypes.h"

#include "qsframework_global.h"

class QSFRAMEWORK_API CDockPanel : public QSplitter {
    Q_OBJECT
public:
    explicit CDockPanel(QWidget *parent = nullptr);
    explicit CDockPanel(Qt::Orientation orient, QWidget *parent = nullptr);
    ~CDockPanel();

private:
    void init();

public:
    // Encapsulate
    void addWidget(QWidget *widget) = delete;
    void insertWidget(int index, QWidget *widget) = delete;
    QWidget *replaceWidget(int index, QWidget *widget) = delete;

    int addWidget(CV::Priority num, QWidget *w, bool visible = true);
    int insertWidget(CV::Priority num, int index, QWidget *w, bool visible = true);
    void removeWidget(CV::Priority num, QWidget *w);

    QWidget *currentWidget(CV::Priority num) const;
    int currentIndex(CV::Priority num) const;

    int indexOf(CV::Priority num, QWidget *w) const;
    QWidget *widget(CV::Priority num, int index) const;
    int count(CV::Priority num) const;

    void setCurrentIndex(CV::Priority num, int index);
    void setCurrentWidget(CV::Priority num, QWidget *w);

    // Addition
    void setContainerVisible(CV::Priority num, bool visible);

private:
    void updateVisible();

protected:
    QStackedWidget *m_firstWidget;
    QStackedWidget *m_secondWidget;
};

#endif // __CDOCKPANEL_H__