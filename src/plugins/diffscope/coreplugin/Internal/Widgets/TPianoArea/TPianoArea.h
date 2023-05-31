#ifndef TPIANOAREA_H
#define TPIANOAREA_H

#include <QLabel>
#include <QWidget>

#include "Interfaces/IPianoKeyWidget.h"
#include "QPixelSize.h"
#include "TPianoKey.h"

class TPianoAreaFactory : public Core::IPianoKeyWidgetFactory {
public:
    Core::IPianoKeyWidget *create(QWidget *parent) override;
    QString name() const override;
};

class TPianoArea : public Core::IPianoKeyWidget {
    Q_OBJECT
    Q_PROPERTY(QPixelSize preferredWidth READ preferredWidth WRITE setPreferredWidth)
public:
    explicit TPianoArea(QWidget *parent = nullptr);
    explicit TPianoArea(int currentHeight, QWidget *parent = nullptr);
    ~TPianoArea();

    int preferredWidth() const {
        return width();
    }

    void setPreferredWidth(const QPixelSize &width) {
        resize(width, height());
    }

private:
    void init();

public:
    int currentHeight() const override;
    void setCurrentHeight(int currentHeight) override;

protected:
    QList<TPianoKey *> m_whites;
    QList<TPianoKey *> m_blacks;
    QList<QLabel *> m_labels;

    QLabel *m_toneLabel;
    int m_currentNoteNum;
    int m_currentHeight;

    void adjustHeight();
    void handleUpdateToneLabel();

private:
    void timerEvent(QTimerEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

signals:
};

#endif // TPIANOAREA_H
