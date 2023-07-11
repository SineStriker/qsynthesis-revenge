#ifndef CDOCKFRAME_H
#define CDOCKFRAME_H

#include <QAbstractButton>
#include <QFrame>

#include "CDockCard.h"

#include "QMNamespace.h"

class CDockToolWindow;

class CDockFramePrivate;

class QMWTOOLS_EXPORT CDockFrame : public QFrame {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CDockFrame)
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
    QList<CDockCard *> widgets(Qt::Edge edge, QM::Priority number) const;

    bool barVisible() const;
    void setBarVisible(bool visible);
    void toggleBarVisible();
    void toggleMaximize(Qt::Edge edge);

    QList<int> orientationSizes(Qt::Orientation orientation) const;
    void setOrientationSizes(Qt::Orientation orientation, const QList<int> &sizes);

    int edgeSize(Qt::Edge edge) const;
    void setEdgeSize(Qt::Edge edge, int size);

signals:
    void barVisibleToggled(bool visible);
    void cardAdded(CDockCard *card);
    void cardAboutToRemove(CDockCard *card);
    void cardToggled(Qt::Edge edge, QM::Priority number, CDockCard *card);

protected:
    virtual CDockCard *createCard(QWidget *w);
    virtual void widgetAdded(Qt::Edge edge, QM::Priority number, QWidget *w, CDockCard *card);
    virtual void widgetAboutToRemove(CDockCard *card);

    bool eventFilter(QObject *obj, QEvent *event) override;

protected:
    QScopedPointer<CDockFramePrivate> d_ptr;

    CDockFrame(CDockFramePrivate &d, QWidget *parent = nullptr);

    friend class CDockTabDragProxy;
    friend class CDockCard;
    friend class CDockToolWindow;
};


#endif // CDOCKFRAME_H
