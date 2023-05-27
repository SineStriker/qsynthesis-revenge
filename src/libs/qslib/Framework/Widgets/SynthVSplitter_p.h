#ifndef SYNTHVSPLITTERPRIVATE_H
#define SYNTHVSPLITTERPRIVATE_H

#include <QAbstractScrollArea>
#include <QDebug>
#include <QScrollBar>
#include <QSplitter>
#include <QVBoxLayout>

#include "SynthVSplitter.h"

namespace QsApi {

    class SynthVSplitterHandle;

    class SynthVLayoutItem;

    class SynthVTopSpacerItem;

    class SynthVSplitterPrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(SynthVSplitter)
    public:
        SynthVSplitterPrivate();
        virtual ~SynthVSplitterPrivate();

        void init();

        bool correctArea();
        void copyScrollBarProperties(QScrollBar *oldBar, QScrollBar *scrollBar);

        void updateScrollBar();
        void updateScrollBarGeometry_helper(bool *needH, bool *needV);

        void normalizeItems();
        SynthVLayoutItem *findResizableItem(SynthVSplitterHandle *handle);

        SynthVSplitter *q_ptr;

        QBoxLayout *layout;
        SynthVTopSpacerItem *spacer;

        QWidget *blockWidget;

        QAbstractScrollArea *area;
        QScrollBar *hbar;
        QScrollBar *vbar;

        Qt::ScrollBarPolicy vbarpolicy;
        Qt::ScrollBarPolicy hbarpolicy;

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;

    private:
        void _q_showOrHideScrollBars(int min, int max);
        void _q_spacerGeometryChanged(const QRect &rect);
    };

    class SynthVSplitterHandlePrivate {
    public:
        SynthVSplitterHandlePrivate(SynthVSplitter *s, SynthVSplitterHandle *q);

        void init();

        void correctEntity();

        void mousePressEvent(QMouseEvent *e);
        void mouseMoveEvent(QMouseEvent *e);
        void mouseReleaseEvent(QMouseEvent *e);

        SynthVSplitterHandle *q;

        SynthVSplitter *s;

        int m_handleHeight;
        QWidget *entity;

        bool hover;
        bool pressed;

        int orgY;
        int orgDelta;
        int orgMinDelta;
        int orgMaxDelta;

        SynthVLayoutItem *currentItem;

        int mouseOffset;
    };

    class SynthVLayoutItem : public QWidgetItem {
    public:
        explicit SynthVLayoutItem(QWidget *w) : QWidgetItem(w), delta(0) {
        }

        QSize sizeHint() const override {
            // isVisible() is false during the showing process
            return (!wid->testAttribute(Qt::WA_WState_Hidden)) ? fixedSizeHint() : realSizeHint();
        }

        QSize fixedSizeHint() const {
            return QWidgetItem::sizeHint() + QSize(0, delta);
        }

        QSize realSizeHint() const {
            return QWidgetItem::sizeHint();
        }

        int delta;
    };

    class SynthVTopSpacerItem : public QObject, public QSpacerItem {
        Q_OBJECT
    public:
        SynthVTopSpacerItem(int w, int h, QSizePolicy::Policy hData = QSizePolicy::Minimum,
                            QSizePolicy::Policy vData = QSizePolicy::Minimum)
            : QSpacerItem(w, h, hData, vData) {
        }

        void setGeometry(const QRect &rect) override {
            QSpacerItem::setGeometry(rect);
            emit geometryChanged(rect);
        }

    signals:
        void geometryChanged(const QRect &rect);
    };

} // namespace QsApi

#endif // SYNTHVSPLITTERPRIVATE_H
