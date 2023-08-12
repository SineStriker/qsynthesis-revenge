//
// Created by fluty on 2023/8/6.
//

#include "LevelMeter.h"
#include "QMDecoratorV2.h"
#include "QDebug"

class LevelMeterPrivate {
public:
    LevelMeter *q;

//    QProgressBar *m_progressbar;

    explicit LevelMeterPrivate(LevelMeter *q) : q(q) {
        q->setAttribute(Qt::WA_StyledBackground);

//        m_progressbar = new QProgressBar;
//        m_progressbar->setObjectName("bar");
//        m_progressbar->setFocusPolicy(Qt::StrongFocus);
//        m_progressbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//        m_progressbar->setTextVisible(false);
//        m_progressbar->setMaximum(10000);
//        m_progressbar->setMinimum(0);


    }
};

LevelMeter::LevelMeter(Qt::Orientation orientation, ChannelType channelType, QWidget *parent) : QWidget(parent), d(new LevelMeterPrivate(this)) {
    m_orientation = orientation;
    m_channelType = channelType;
    m_chunk = new LevelMeterChunk;

    m_button = new QPushButton;
    m_button->setObjectName("button");
    m_button->setCheckable(true);
    m_button->setChecked(false);

    if (m_orientation == Qt::Horizontal) {
        m_chunk->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_chunk->setOrientation(Qt::Horizontal);
        m_button->setMaximumWidth(10);
        m_button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

        m_hLayout = new QHBoxLayout;
        m_hLayout->setSpacing(1);
        m_hLayout->setMargin(0);
        m_hLayout->setObjectName("hLayout");
        m_hLayout->addWidget(m_chunk);
        m_hLayout->addWidget(m_button);

        this->setMinimumHeight(24);
        this->setMaximumHeight(24);
        this->setLayout(m_hLayout);
    } else {
        m_chunk->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_chunk->setOrientation(Qt::Vertical);
        m_button->setMaximumHeight(10);
        m_button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

        m_vLayout = new QVBoxLayout;
        m_vLayout->setSpacing(1);
        m_vLayout->setMargin(0);
        m_vLayout->setObjectName("vLayout");
        m_vLayout->addWidget(m_button);
        m_vLayout->addWidget(m_chunk);
        this->setMinimumWidth(6);
        this->setMaximumWidth(24);
        this->setLayout(m_vLayout);
    }

    QObject::connect(m_button, &QPushButton::clicked, this, [=]() {
        this->m_button->setChecked(false);
    });

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

void LevelMeter::readSample(double sample) {
    m_chunk->readSample(sample);
    if (sample > 1)
        m_button->setChecked(true);
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

void LevelMeter::initBuffer(int bufferSize) {
    m_chunk->initBuffer(bufferSize);
}

void LevelMeter::dismissIndicator() {
    m_button->setChecked(false);
}
