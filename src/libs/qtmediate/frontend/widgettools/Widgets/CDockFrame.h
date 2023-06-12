#ifndef CDOCKFRAME_H
#define CDOCKFRAME_H

#include <QAbstractButton>
#include <QFrame>

#include "CDockCard.h"
#include "QMNamespace.h"

class CDockFramePrivate;

class QMWTOOLS_EXPORT CDockFrame : public QFrame {
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
    void moveWidget(CDockCard *card, Qt::Edge edge, QM::Priority number);
    QList<CDockCard *> widgets(Qt::Edge edge, QM::Priority number);

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


#endif // CDOCKFRAME_H
