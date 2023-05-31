#include "TPianoKey.h"

#include <QStyle>

TPianoKey::TPianoKey(TPianoKey::KeyColor color, QWidget *parent) : CToolButton(parent) {
    setColor(color);
}

TPianoKey::~TPianoKey() {
}

TPianoKey::KeyColor TPianoKey::color() const {
    return m_color;
}

void TPianoKey::setColor(TPianoKey::KeyColor color) {
    m_color = color;

    setProperty("color", m_color == Black ? "black" : "white");
    style()->polish(this);
}

int TPianoKey::posFactor() const {
    return m_posFactor;
}

void TPianoKey::setPosFactor(int posFactor) {
    m_posFactor = posFactor;
}

int TPianoKey::heightFactor() const {
    return m_heightFactor;
}

void TPianoKey::setHeightFactor(int heightFactor) {
    m_heightFactor = heightFactor;
}
