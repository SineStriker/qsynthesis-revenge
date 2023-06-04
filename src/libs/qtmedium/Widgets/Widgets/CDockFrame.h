#ifndef __CDOCKFRAME_H__
#define __CDOCKFRAME_H__

#include <QAbstractButton>
#include <QFrame>

#include "QMNamespace.h"

#include "CDockCard.h"

class CDockFramePrivate;

class QMWIDGETS_API CDockFrame : public QFrame {
    Q_OBJECT
public:
    explicit CDockFrame(QWidget *parent = nullptr);
    ~CDockFrame();

public:
    QWidget *widget() const;
    void setWidget(QWidget *w);
    QWidget *takeWidget();

    /**
     * @brief Button which will control the widget visibility, set text or icon after call
     *
     */
    CDockCard *addWidget(Qt::Edge edge, QM::Priority number, QWidget *w);
    void removeWidget(CDockCard *card);

    bool barVisible() const;
    void setBarVisible(bool visible);
    void toggleBarVisible();

signals:
    void barVisibleToggled(bool visible);
    void cardAdded(CDockCard *card);
    void cardAboutToRemove(CDockCard *card);
    void cardToggled(Qt::Edge edge, QM::Priority number, CDockCard *card);

protected:
    virtual void widgetAdded(Qt::Edge edge, QM::Priority number, QWidget *w, CDockCard *card);
    virtual void widgetAboutToRemove(CDockCard *card);

protected:
    CDockFramePrivate *d;

    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    friend class CDockTabDragProxy;
};


#endif // __CDOCKFRAME_H__
