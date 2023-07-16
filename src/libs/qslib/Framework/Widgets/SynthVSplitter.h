#ifndef SYNTHVSPLITTER_H
#define SYNTHVSPLITTER_H

#include <QAbstractScrollArea>
#include <QFrame>
#include <QLayout>
#include <QSplitter>

#include <QPixelSize.h>

#include "QsFrameworkGlobal.h"

namespace QsApi {

    class SynthVSplitterPrivate;

    class SynthVSplitterHandle;

    class QSFRAMEWORK_API SynthVSplitter : public QFrame {
        Q_OBJECT
        Q_LAYOUT_PROPERTY_DELCARE
        Q_DECLARE_PRIVATE(SynthVSplitter)
    public:
        explicit SynthVSplitter(QWidget *parent = nullptr);
        ~SynthVSplitter();

        void addWidget(QWidget *w, QWidget *titleBar = nullptr);
        void insertWidget(int index, QWidget *w, QWidget *titleBar = nullptr);
        void removeWidget(QWidget *w);
        void moveWidget(int from, int to);

        QWidget *widget(int index) const;
        int indexOf(QWidget *w) const;
        int count() const;
        bool isEmpty() const;

        QList<int> saveState() const;
        void restoreState(const QList<int> &state);

        SynthVSplitterHandle *handle(int index) const;
        int indexOfHandle(SynthVSplitterHandle *handle) const;

        int indexAt(const QPoint &pos) const;

        QSize sizeHint() const override;

    public:
        QAbstractScrollArea *scrollArea() const;
        QAbstractScrollArea *takeScrollArea();
        void setScrollArea(QAbstractScrollArea *area);

    signals:
        void widgetInserted(QWidget *w);
        void aboutToRemoveWidget(QWidget *w);

    protected:
        virtual SynthVSplitterHandle *createHandle(QWidget *w, QWidget *titleBar);
        virtual void handleDestroyed(SynthVSplitterHandle *handle);

        void resizeEvent(QResizeEvent *event) override;

    protected:
        SynthVSplitter(SynthVSplitterPrivate &d, QWidget *parent = nullptr);

        QScopedPointer<SynthVSplitterPrivate> d_ptr;

        friend class SynthVSplitterHandle;
        friend class SynthVSplitterHandlePrivate;
    };

    class SynthVSplitterHandlePrivate;

    class QSFRAMEWORK_API SynthVSplitterHandle : public QFrame {
        Q_OBJECT
        Q_LAYOUT_PROPERTY_DELCARE
        Q_PROPERTY(QPixelSize handleHeight READ handleHeight WRITE setHandleHeight)
    public:
        explicit SynthVSplitterHandle(SynthVSplitter *parent);
        ~SynthVSplitterHandle();

        SynthVSplitter *splitter() const;

        QPixelSize handleHeight();
        void setHandleHeight(const QPixelSize &h);

        QSize sizeHint() const override;

    protected:
        void resizeEvent(QResizeEvent *event) override;
        bool event(QEvent *event) override;
        bool eventFilter(QObject *obj, QEvent *event) override;

    private:
        SynthVSplitterHandlePrivate *d;

        friend class SynthVSplitter;
        friend class SynthVSplitterPrivate;
    };

} // namespace QsApi

#endif // SYNTHVSPLITTER_H
