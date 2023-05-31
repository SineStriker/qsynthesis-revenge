#ifndef TPIANOKEY_H
#define TPIANOKEY_H

#include "CToolButton.h"

class TPianoKey : public CToolButton {
    Q_OBJECT
public:
    enum KeyColor {
        Black,
        White,
    };

    explicit TPianoKey(KeyColor color, QWidget *parent = nullptr);
    ~TPianoKey();

    KeyColor color() const;
    void setColor(KeyColor color);

    int posFactor() const;
    void setPosFactor(int posFactor);

    int heightFactor() const;
    void setHeightFactor(int heightFactor);

private:
    KeyColor m_color;

    int m_posFactor;
    int m_heightFactor;
};


#endif // TPIANOKEY_H
