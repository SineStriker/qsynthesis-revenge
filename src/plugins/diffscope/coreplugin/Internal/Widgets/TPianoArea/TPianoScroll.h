#ifndef TPIANOSCROLL_H
#define TPIANOSCROLL_H

#include <Widgets/NoneScrollArea.h>

#include <Interfaces/IPianoKeyWidget.h>

class TPianoScroll : public QsApi::NoneScrollArea {
    Q_OBJECT
public:
    explicit TPianoScroll(QWidget *parent = nullptr);
    ~TPianoScroll();

    friend class TuningScope;

public:
    Core::IPianoKeyWidget *area() const;
    void setArea(Core::IPianoKeyWidget *area);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

    Core::IPianoKeyWidget *m_area;

signals:
};

#endif // TPIANOSCROLL_H
