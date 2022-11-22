#ifndef __CDOCKPANEL_H__
#define __CDOCKPANEL_H__

#include <QSplitter>
#include <QStackedWidget>

#include "Kernel/QsNamespace.h"

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

    int addWidget(Qs::Priority num, QWidget *w, bool visible = true);
    int insertWidget(Qs::Priority num, int index, QWidget *w, bool visible = true);
    void removeWidget(Qs::Priority num, QWidget *w);

    QWidget *currentWidget(Qs::Priority num) const;
    int currentIndex(Qs::Priority num) const;

    int indexOf(Qs::Priority num, QWidget *w) const;
    QWidget *widget(Qs::Priority num, int index) const;
    int count(Qs::Priority num) const;

    void setCurrentIndex(Qs::Priority num, int index);
    void setCurrentWidget(Qs::Priority num, QWidget *w);

    // Addition
    void setContainerVisible(Qs::Priority num, bool visible);

private:
    void updateVisible();

protected:
    QStackedWidget *m_firstWidget;
    QStackedWidget *m_secondWidget;
};

#endif // __CDOCKPANEL_H__
