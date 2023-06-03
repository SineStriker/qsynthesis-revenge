#ifndef __CDOCKFRAME_H__
#define __CDOCKFRAME_H__

#include <QAbstractButton>
#include <QFrame>

#include "QMNamespace.h"
#include "QMWidgetsGlobal.h"

class CDockFramePrivate;

class QMWIDGETS_API CDockFrame : public QFrame {
    Q_OBJECT
    Q_PROPERTY(QSize dragAreaSize //
                   READ dragAreaSize WRITE setDragAreaSize NOTIFY dragAreaSizeChanged)
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
    QAbstractButton *addWidget(Qt::Edge edge, QM::Priority number, QWidget *w);
    void removeWidget(QAbstractButton *card);

    QSize dragAreaSize() const;
    void setDragAreaSize(const QSize &dragAreaSize);

    bool barVisible() const;
    void setBarVisible(bool visible);
    void toggleBarVisible();

signals:
    void dragAreaSizeChanged();
    void barVisibleToggled(bool visible);
    void cardAdded(QAbstractButton *card);
    void cardAboutToRemove(QAbstractButton *card);
    void cardToggled(Qt::Edge edge, QM::Priority number, QAbstractButton *card);

protected:
    virtual void widgetAdded(Qt::Edge edge, QM::Priority number, QWidget *w, QAbstractButton *card);
    virtual void widgetAboutToRemove(QAbstractButton *card);

protected:
    CDockFramePrivate *d;

    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    friend class CDockTabDragProxy;
};


#endif // __CDOCKFRAME_H__
