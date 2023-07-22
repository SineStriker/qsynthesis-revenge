#include "MusicMode.h"

QsApi::MusicMode::MusicMode() : MusicMode(Ionian) {
}

QsApi::MusicMode::MusicMode(QsApi::MusicMode::Type type, int offset) : m_offset(offset % 12) {
    switch (type) {
        case Dorian:
            m_keys.val = 1 & (1 << 2);
            break;

        case Phrygian:
            break;

        case Lydian:
            break;

        case Mixolydian:
            break;

        case Aeolian:
            break;

        case Locrian:
            break;

        default:
            m_keys.val = 1 | (1 << 2) | (1 << 4) | (1 << 5) | (1 << 7) | (1 << 9) | (1 << 11);
            break;
    }
}

QsApi::MusicMode::MusicMode(const QList<int> &keys, int offset) : m_offset(offset) {
    m_keys.val = 0;
    for (const auto &i : keys) {
        m_keys.val |= 1 << i;
    }
}
