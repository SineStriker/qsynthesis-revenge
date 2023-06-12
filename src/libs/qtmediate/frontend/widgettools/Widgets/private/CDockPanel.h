#ifndef CDOCKPANEL_H
#define CDOCKPANEL_H

#include <QSplitter>
#include <QStackedWidget>

#include "QMNamespace.h"
#include "QMWidgetToolsGlobal.h"

class QMWTOOLS_EXPORT CDockPanel : public QSplitter {
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

    int addWidget(QM::Priority num, QWidget *w, bool visible = true);
    int insertWidget(QM::Priority num, int index, QWidget *w, bool visible = true);
    void removeWidget(QM::Priority num, QWidget *w);

    QWidget *currentWidget(QM::Priority num) const;
    int currentIndex(QM::Priority num) const;

    int indexOf(QM::Priority num, QWidget *w) const;
    QWidget *widget(QM::Priority num, int index) const;
    int count(QM::Priority num) const;

    void setCurrentIndex(QM::Priority num, int index);
    void setCurrentWidget(QM::Priority num, QWidget *w);

    // Addition
    void setContainerVisible(QM::Priority num, bool visible);

private:
    void updateVisibility();

protected:
    QStackedWidget *m_firstWidget;
    QStackedWidget *m_secondWidget;
};

#endif // CDOCKPANEL_H
