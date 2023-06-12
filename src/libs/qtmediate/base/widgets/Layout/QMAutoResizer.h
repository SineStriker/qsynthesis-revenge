#ifndef QMAUTORESIZER_H
#define QMAUTORESIZER_H

#include <QWidget>

#include "QMWidgetsGlobal.h"

class QMWIDGETS_EXPORT QMAutoResizer : public QObject {
    Q_OBJECT
public:
    enum SizeOption {
        SizeHint,
        MinimumSizeHint,
    };

    enum MeasureOption {
        Width = 1,
        Height = 2,
        WidthAndHeight = Width | Height,
    };

    explicit QMAutoResizer(QWidget *parent = nullptr);
    QMAutoResizer(SizeOption so, QWidget *parent = nullptr);
    QMAutoResizer(SizeOption so, bool fixed, QWidget *parent = nullptr);
    QMAutoResizer(SizeOption so, bool fixed, MeasureOption mo, QWidget *parent = nullptr);
    ~QMAutoResizer();

    inline QWidget *widget() const {
        return w;
    }

    inline SizeOption sizeOption() const {
        return so;
    }

    inline bool fixed() const {
        return fix;
    }

    inline MeasureOption measureOption() const {
        return mo;
    }

protected:
    QWidget *w;

    SizeOption so;    // Default: sizeHint
    bool fix;         // Default: false
    MeasureOption mo; // Default: WidthAndHeight

    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // QMAUTORESIZER_H
