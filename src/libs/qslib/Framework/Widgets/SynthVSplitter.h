#ifndef SYNTHVSPLITTER_H
#define SYNTHVSPLITTER_H

#include <QAbstractScrollArea>
#include <QFrame>
#include <QSplitter>

#include "QsFrameworkGlobal.h"

namespace QsApi {

    class SynthVSplitterPrivate;

    class QSFRAMEWORK_API SynthVSplitter : public QFrame {
        Q_OBJECT
        Q_DECLARE_PRIVATE(SynthVSplitter)
    public:
        explicit SynthVSplitter(QWidget *parent = nullptr);
        ~SynthVSplitter();

        void addWidget(QWidget *w);
        void insertWidget(int index, QWidget *w);
        void removeWidget(QWidget *w);

        QWidget *widget(int index) const;
        int indexOf(QWidget *w) const;
        int count() const;
        bool isEmpty() const;

        QSize sizeHint() const override;

    public:
        QAbstractScrollArea *scrollArea() const;
        QAbstractScrollArea *takeScrollArea();
        void setScrollArea(QAbstractScrollArea *area);

    protected:
        void resizeEvent(QResizeEvent *event) override;

    protected:
        SynthVSplitter(SynthVSplitterPrivate &d, QWidget *parent = nullptr);

        QScopedPointer<SynthVSplitterPrivate> d_ptr;
    };

} // namespace QsApi

#endif // SYNTHVSPLITTER_H
