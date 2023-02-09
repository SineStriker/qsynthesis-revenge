#ifndef CAUTORESIZER_H
#define CAUTORESIZER_H

#include <QWidget>

#include "QsFrameworkGlobal.h"

class QSFRAMEWORK_API CAutoResizer : public QObject {
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

    explicit CAutoResizer(QWidget *parent = nullptr);
    CAutoResizer(SizeOption so, QWidget *parent = nullptr);
    CAutoResizer(SizeOption so, bool fixed, QWidget *parent = nullptr);
    CAutoResizer(SizeOption so, bool fixed, MeasureOption mo, QWidget *parent = nullptr);
    ~CAutoResizer();

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

#endif // CAUTORESIZER_H
