#ifndef SYNTHVSPLITTERPRIVATE_H
#define SYNTHVSPLITTERPRIVATE_H

#include <QAbstractScrollArea>
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

        void normalizeDelta();

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

    class SynthVSplitterHandle : public QFrame {
        Q_OBJECT
        Q_PROPERTY(int handleHeight READ handleHeight WRITE setHandleHeight)
    public:
        explicit SynthVSplitterHandle(QWidget *w, SynthVLayoutItem *item, SynthVSplitterPrivate *d,
                                      SynthVSplitter *parent);
        ~SynthVSplitterHandle();

        QSize sizeHint() const override;

        int handleHeight();
        void setHandleHeight(int h);

    protected:
        void paintEvent(QPaintEvent *e) override;
        void mousePressEvent(QMouseEvent *e) override;
        void mouseMoveEvent(QMouseEvent *e) override;
        void mouseReleaseEvent(QMouseEvent *e) override;
        bool event(QEvent *event) override;

    private:
        QWidget *w;
        SynthVLayoutItem *item;

        SynthVSplitterPrivate *d;
        SynthVSplitter *s;

        int m_handleHeight;

        bool hover;
        bool pressed;

        int orgY;
        int orgDelta;
        int orgMinDelta;
        int orgMaxDelta;

        int mouseOffset;
    };

    class SynthVLayoutItem : public QWidgetItemV2 {
    public:
        explicit SynthVLayoutItem(QWidget *w) : QWidgetItemV2(w), delta(0) {
        }

        QSize sizeHint() const override {
            return QWidgetItemV2::sizeHint() + QSize(0, delta);
        }

        QSize realSizeHint() const {
            return QWidgetItemV2::sizeHint();
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
