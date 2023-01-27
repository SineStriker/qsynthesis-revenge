#ifndef __CDOCKFRAME_H__
#define __CDOCKFRAME_H__

#include <QAbstractButton>
#include <QFrame>

#include "QsNamespace.h"

class CDockFramePrivate;

class QSFRAMEWORK_API CDockFrame : public QFrame {
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
    QAbstractButton *addWidget(Qt::Edge edge, Qs::Priority number, QWidget *w);
    void removeWidget(QAbstractButton *card);

    QSize dragAreaSize() const;
    void setDragAreaSize(const QSize &dragAreaSize);

    bool barVisible() const;
    void setBarVisible(bool visible);
    void toggleBarVisible();

protected:
    CDockFramePrivate *d;

    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    friend class CDockTabDragProxy;

signals:
    void dragAreaSizeChanged();
    void barVisibleToggled(bool visible);
    void cardToggled(Qt::Edge edge, Qs::Priority number, QAbstractButton *card);
};


#endif // __CDOCKFRAME_H__
