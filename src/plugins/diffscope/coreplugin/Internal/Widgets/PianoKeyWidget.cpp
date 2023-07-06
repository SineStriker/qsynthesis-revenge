#include "PianoKeyWidget.h"

#include <QResizeEvent>
#include <QStyle>
#include <QtMath>

#include <Utau/Config/UtaConstants.h>
#include <Utau/Utils/QUtaUtils.h>

#include "PianoKeyContainer.h"

namespace Core::Internal {

    using namespace Utau;

    const char DefaultPianoKeyWidget[] = "uta.pianoKeys";

    PianoKey::PianoKey(PianoKey::KeyColor color, QWidget *parent) : CToolButton(parent) {
        setColor(color);
    }

    PianoKey::~PianoKey() {
    }

    PianoKey::KeyColor PianoKey::color() const {
        return m_color;
    }

    void PianoKey::setColor(PianoKey::KeyColor color) {
        m_color = color;

        setProperty("color", m_color == Black ? "black" : "white");
        style()->polish(this);
    }

    int PianoKey::posFactor() const {
        return m_posFactor;
    }

    void PianoKey::setPosFactor(int posFactor) {
        m_posFactor = posFactor;
    }

    int PianoKey::heightFactor() const {
        return m_heightFactor;
    }

    void PianoKey::setHeightFactor(int heightFactor) {
        m_heightFactor = heightFactor;
    }

    Core::IPianoKeyWidget *PianoKeyWidgetFactory::create(QWidget *parent) {
        return new PianoKeyWidget(parent);
    }

    QString PianoKeyWidgetFactory::name() const {
        return PianoKeyWidget::tr("UTAU/FL Studio");
    }

    PianoKeyWidget::PianoKeyWidget(QWidget *parent)
        : Core::IPianoKeyWidget(parent), m_currentHeight(30) {
        init();
    }

    PianoKeyWidget::PianoKeyWidget(int currentHeight, QWidget *parent)
        : Core::IPianoKeyWidget(parent), m_currentHeight(currentHeight) {
        init();
    }

    PianoKeyWidget::~PianoKeyWidget() {
    }

    QPixelSize PianoKeyWidget::preferredWidth() const {
        return width();
    }

    void PianoKeyWidget::setPreferredWidth(const QPixelSize &width) {
        resize(width, height());
        emit preferredWidthChanged(width);
    }

    void PianoKeyWidget::init() {
        setAttribute(Qt::WA_StyledBackground);

        int curHeight, totalPos;

        // White Keys
        totalPos = 0;
        for (int i = TONE_OCTAVE_MIN; i <= TONE_OCTAVE_MAX; ++i) {
            for (int j = 0; j < 7; ++j) {
                if (j == 1 || j == 2 || j == 5) {
                    curHeight = 4;
                } else {
                    curHeight = 3;
                }

                auto key = new PianoKey(PianoKey::White, this);
                key->setHeightFactor(curHeight);
                key->setPosFactor(totalPos);

                totalPos += curHeight;
                m_whites.append(key);
            }
        }

        // Black Keys
        totalPos = 2;
        curHeight = 2;
        for (int i = TONE_OCTAVE_MIN; i <= TONE_OCTAVE_MAX; ++i) {
            for (int j = 0; j < 5; ++j) {
                auto key = new PianoKey(PianoKey::Black, this);
                key->setHeightFactor(curHeight);
                key->setPosFactor(totalPos);

                if (j == 0 || j == 1 || j == 3) {
                    totalPos += 4;
                } else {
                    totalPos += 6;
                }
                m_blacks.append(key);
            }
        }

        for (int i = TONE_OCTAVE_MIN; i <= TONE_OCTAVE_MAX; ++i) {
            auto label = new QLabel("C" + QString::number(TONE_OCTAVE_MAX + 1 - i), this);
            label->setObjectName("tone-label");
            label->setProperty("type", "standard");
            label->setAttribute(Qt::WA_TransparentForMouseEvents);
            label->adjustSize();
            label->setAlignment(Qt::AlignCenter);
            m_labels.append(label);
        }

        m_toneLabel = new QLabel(this);
        m_toneLabel->setObjectName("tone-label");
        m_toneLabel->setProperty("type", "realtime");
        m_toneLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
        m_toneLabel->setAlignment(Qt::AlignCenter);

        m_currentNoteNum = -1;

        adjustHeight();

        startTimer(10);
        handleUpdateToneLabel();
    }

    int PianoKeyWidget::currentHeight() const {
        return m_currentHeight;
    }

    void PianoKeyWidget::setCurrentHeight(int currentHeight) {
        m_currentHeight = currentHeight;
        adjustHeight();
    }

    void PianoKeyWidget::adjustHeight() {
        for (auto key : qAsConst(m_whites)) {
            key->move(0, key->posFactor() * m_currentHeight / 2);
            key->resize(width(), key->heightFactor() * m_currentHeight / 2);
        }

        for (auto key : qAsConst(m_blacks)) {
            key->move(0, key->posFactor() * m_currentHeight / 2);
            key->resize(width() / 2, key->heightFactor() * m_currentHeight / 2);
        }

        for (int i = 0; i < m_labels.size(); ++i) {
            auto key = m_whites.at(i * 7 + 6);
            auto label = m_labels.at(i);

            label->move(key->x() + key->width() * 3 / 4 - label->width() / 2,
                        key->y() + key->height() * 2 / 3 - label->height() / 2);
        }

        resize(width(), (TONE_OCTAVE_MAX - TONE_OCTAVE_MIN + 1) * 12 * m_currentHeight);
        handleUpdateToneLabel();
    }

    void PianoKeyWidget::handleUpdateToneLabel() {
        QPoint thisMouse = mapFromGlobal(QCursor::pos());
        int index = qFloor(double(thisMouse.y()) / m_currentHeight);

        int min = TONE_NUMBER_BASE;
        int max = TONE_NUMBER_BASE + (TONE_OCTAVE_MAX - TONE_OCTAVE_MIN + 1) * 12;
        int number = max - 1 - index;

        if (number == m_currentNoteNum) {
            return;
        } else {
            m_currentNoteNum = number;
        }

        if (number < min || number >= max) {
            m_toneLabel->setVisible(false);
            return;
        } else {
            m_toneLabel->setVisible(true);
        }

        int toY = index * m_currentHeight;

        QString name = QUtaUtils::ToneNumToToneName(m_currentNoteNum);
        if (name.contains('#')) {
            m_toneLabel->setProperty("black", true);
            style()->polish(m_toneLabel);
        } else {
            m_toneLabel->setProperty("black", false);
            style()->polish(m_toneLabel);
        }

        m_toneLabel->setText(name);
        m_toneLabel->adjustSize();
        m_toneLabel->move(10, toY + (m_currentHeight - m_toneLabel->height()) / 2);
    }

    void PianoKeyWidget::timerEvent(QTimerEvent *event) {
        Q_UNUSED(event)
        handleUpdateToneLabel();
    }

    void PianoKeyWidget::resizeEvent(QResizeEvent *event) {
        int w = width();
        if (event->oldSize().width() != w) {
            for (auto key : qAsConst(m_whites)) {
                key->resize(w, key->height());
            }
            for (auto key : qAsConst(m_blacks)) {
                key->resize(w / 2, key->height());
            }
            for (int i = 0; i < m_labels.size(); ++i) {
                auto key = m_whites.at(i * 7 + 6);
                auto label = m_labels.at(i);
                label->move(key->x() + key->width() * 3 / 4 - label->width() / 2, label->y());
            }
            handleUpdateToneLabel();
        }
        return QWidget::resizeEvent(event);
    }

} // namespace Core::Internal
