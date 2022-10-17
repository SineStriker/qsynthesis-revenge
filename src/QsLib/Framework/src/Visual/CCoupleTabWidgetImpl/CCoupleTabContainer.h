#ifndef CCOUPLETABCONTAINER_H
#define CCOUPLETABCONTAINER_H

#include <QSplitter>
#include <QStackedWidget>

#include "CCoupleTabTypes.h"
#include "qsframework_global.h"

class QSFRAMEWORK_API CCoupleTabContainer : public QSplitter {
    Q_OBJECT
public:
    explicit CCoupleTabContainer(QWidget *parent = nullptr);
    explicit CCoupleTabContainer(Qt::Orientation orient, QWidget *parent = nullptr);
    ~CCoupleTabContainer();

private:
    void init();

public:
    // Encapsulate
    void addWidget(QWidget *widget) = delete;
    void insertWidget(int index, QWidget *widget) = delete;
    QWidget *replaceWidget(int index, QWidget *widget) = delete;

    int addWidget(CCoupleTabTypes::Number num, QWidget *w, bool visible = true);
    int insertWidget(CCoupleTabTypes::Number num, int index, QWidget *w, bool visible = true);
    void removeWidget(CCoupleTabTypes::Number num, QWidget *w);

    QWidget *currentWidget(CCoupleTabTypes::Number num) const;
    int currentIndex(CCoupleTabTypes::Number num) const;

    int indexOf(CCoupleTabTypes::Number num, QWidget *w) const;
    QWidget *widget(CCoupleTabTypes::Number num, int index) const;
    int count(CCoupleTabTypes::Number num) const;

    void setCurrentIndex(CCoupleTabTypes::Number num, int index);
    void setCurrentWidget(CCoupleTabTypes::Number num, QWidget *w);

    // Addition
    void setContainerVisible(CCoupleTabTypes::Number num, bool visible);

private:
    void updateVisible();

protected:
    QStackedWidget *m_firstWidget;
    QStackedWidget *m_secondWidget;
};
#endif // CCOUPLETABCONTAINER_H
