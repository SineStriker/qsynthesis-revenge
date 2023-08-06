//
// Created by fluty on 2023/8/6.
//

#include "LevelMeter.h"
#include "QMDecoratorV2.h"

class LevelMeterPrivate {
public:
    LevelMeter *q;

//    QProgressBar *m_progressbar;
    LevelMeterChunk *m_chunk;
    QPushButton *m_button;
    QHBoxLayout *m_layout;

    explicit LevelMeterPrivate(LevelMeter *q) : q(q) {
        q->setAttribute(Qt::WA_StyledBackground);

//        m_progressbar = new QProgressBar;
//        m_progressbar->setObjectName("bar");
//        m_progressbar->setFocusPolicy(Qt::StrongFocus);
//        m_progressbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//        m_progressbar->setTextVisible(false);
//        m_progressbar->setMaximum(10000);
//        m_progressbar->setMinimum(0);

        m_chunk = new LevelMeterChunk;
        m_chunk->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        m_button = new QPushButton;
        m_button->setObjectName("button");
        m_button->setMaximumWidth(10);
        m_button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
        m_button->setCheckable(true);
        m_button->setChecked(false);

        m_layout = new QHBoxLayout;
        m_layout->setSpacing(0);
        m_layout->setMargin(0);
        m_layout->setObjectName("layout");
//        m_layout->addWidget(m_progressbar);
        m_layout->addWidget(m_chunk);
        m_layout->addWidget(m_button);

        q->setMinimumHeight(24);
        q->setMaximumHeight(24);
        q->setLayout(m_layout);

        QObject::connect(m_button, &QPushButton::clicked, q, [=]() {
            this->m_button->setChecked(false);
        });
    }
};

LevelMeter::LevelMeter(QWidget *parent) : QWidget(parent), d(new LevelMeterPrivate(this)) {
    this->setStyleSheet(QMDecoratorV2::evaluateStyleSheet(R"(
LevelMeter {
    border-style: none;
    border-radius: 4px;
}

LevelMeter > QProgressBar#bar {
    background-color:#d9d9d9;
    color:#808080;
    border-style: none;
    border-radius: 0px;
    text-align:center;
}

LevelMeter > QProgressBar#bar::chunk {
    background-color: #0060C0;
    border-radius: 0px;
}

LevelMeter > QPushButton#button {
    background-color: #d9d9d9;
    border-radius: 0px;
    border-left: 1px solid #d4d4d4;
}

LevelMeter > QPushButton#button:checked {
    background-color: #ff7c80;
}
)"));
}

LevelMeter::~LevelMeter() {
    delete d;
}

void LevelMeter::paintEvent(QPaintEvent *event) {
    return QWidget::paintEvent(event);
}

void LevelMeter::setLevel(double level) {
    this->d->m_chunk->setLevel(level);
    if (level > 1)
        this->d->m_button->setChecked(true);
    /*auto styleSheet = QMDecoratorV2::evaluateStyleSheet(R"(
QProgressBar#bar::chunk {
    background-color: qlineargradient(x1: 0, x2: 1, stop: 0 #709cff, stop: %1 #709cff, stop: %2 #ffcc99, stop: %3 #ffcc99, stop: %4 #ff7c80, stop: 1 #ff7c80);
    border-radius: 0px;
}
    )");
    if (level < 0.707946) { //-3dB
        this->d->m_progressbar->setValue(round(level * 10000));
//        this->d->m_progressbar->setStyleSheet(styleSheet.arg("0.9996").arg("0.9997").arg("0.9998").arg("0.9999"));
        this->d->m_progressbar->setStyleSheet(QMDecoratorV2::evaluateStyleSheet(R"(
QProgressBar#bar::chunk {
    background-color: #709cff;
    border-radius: 0px;
}
    )"));
    } else if (level < 0.891251) { //-1dB
        this->d->m_progressbar->setValue(round(level * 10000));
        auto value1 = 0.707946;
        auto pos1 = value1 / level;
//        this->d->m_progressbar->setStyleSheet(styleSheet.arg(pos1 - 0.001)
//                                                  .arg(pos1)
//                                                  .arg("0.9998")
//                                                  .arg("0.9999"));
        this->d->m_progressbar->setStyleSheet(QMDecoratorV2::evaluateStyleSheet(R"(
QProgressBar#bar::chunk {
    background-color: qlineargradient(x1: 0, x2: 1, stop: 0 #709cff, stop: %1 #709cff, stop: %2 #ffcc99, stop: 1 #ffcc99);
}
    )").arg(pos1 - 0.000001).arg(pos1));
    } else if (level < 1) {
        this->d->m_progressbar->setValue(round(level * 10000));
        auto value1 = 0.707946;
        auto value2 = 0.891251;
        auto pos1 = value1 / level;
        auto pos2 = 1 - (level - value2) / level;
//        this->d->m_progressbar->setStyleSheet(styleSheet.arg(pos1 - 0.001)
//                                                  .arg(pos1)
//                                                  .arg(pos2 - 0.001)
//                                                  .arg(pos2));
        this->d->m_progressbar->setStyleSheet(QMDecoratorV2::evaluateStyleSheet(R"(
QProgressBar#bar::chunk {
    background-color: qlineargradient(x1: 0, x2: 1, stop: 0 #709cff, stop: %1 #709cff, stop: %2 #ffcc99, stop: %3 #ffcc99, stop: %4 #ff7c80, stop: 1 #ff7c80);
}
    )").arg(pos1 - 0.000001).arg(pos1).arg(pos2 - 0.000001).arg(pos2));
    } else {
        this->d->m_button->setChecked(true);
        this->d->m_progressbar->setValue(10000);
        this->d->m_progressbar->setStyleSheet(QMDecoratorV2::evaluateStyleSheet(R"(
QProgressBar#bar::chunk {
    background-color: qlineargradient(x1: 0, x2: 1, stop: 0 #709cff, stop: 0.707945 #709cff, stop: 0.707946 #ffcc99, stop: 0.891250 #ffcc99, stop: 0.891251 #ff7c80, stop: 1 #ff7c80);
}
    )"));
    }*/
}
