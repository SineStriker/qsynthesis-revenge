#ifndef CNAVFRAME_H
#define CNAVFRAME_H

#include <QAbstractButton>
#include <QFrame>
#include <QSplitter>

#include "QMWidgetToolsGlobal.h"

class CNavFramePrivate;

class QMWTOOLS_EXPORT CNavFrame : public QFrame {
    Q_OBJECT
public:
    explicit CNavFrame(QWidget *parent = nullptr);
    ~CNavFrame();

    QWidget *topWidget() const;
    void setTopWidget(QWidget *w);
    QWidget *takeTopWidget();

    QWidget *bottomWidget() const;
    void setBottomWidget(QWidget *w);
    QWidget *takeBottomWidget();

    QSplitter *splitter() const;

    /**
     * @brief Button which will control the widget visibility, set text or icon after call
     *
     */
    QAbstractButton *addWidget(QWidget *w);
    QAbstractButton *insertWidget(int index, QWidget *w);
    void removeWidget(QWidget *w);
    void removeAt(int index);

    QWidget *widget(int index);
    QWidget *currentWidget() const;
    int currentIndex() const;
    int count() const;
    int indexOf(QWidget *w) const;

    void setCurrentIndex(int index);
    void setCurrentWidget(QWidget *w);

protected:
    QScopedPointer<CNavFramePrivate> d_ptr;

    friend class CNavFramePrivate;

private:
    void _q_currentChanged(int index);
    void _q_buttonClicked(int id);
};

#endif // CNAVFRAME_H
