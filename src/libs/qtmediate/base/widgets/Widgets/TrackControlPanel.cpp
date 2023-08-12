//
// Created by fluty on 2023/8/7.
//

#include "TrackControlPanel.h"
#include "CLineEdit.h"
#include "LevelMeter.h"
#include "PanSlider.h"
#include "QMDecoratorV2.h"
#include <QLabel>
#include <QPushButton>
#include "QDebug"

class TrackControlPanelPrivate {
public:
    TrackControlPanel *q;
    QPushButton *m_btnColor;
    QLabel *m_lbTrackIndex;
    QPushButton *m_btnMute;
    QPushButton *m_btnSolo;
    CLineEdit *m_leTrackName;
    PanSlider *m_panSlider;
    CLineEdit *m_lePan;
    PanSlider *m_volumeSlider;
    CLineEdit *m_leVolume;
    QSpacerItem *m_panVolumeSpacer;
    LevelMeter *m_levelMeter;
    QHBoxLayout *m_mainLayout;
    QVBoxLayout *m_controlWidgetLayout;
    QHBoxLayout *m_muteSoloTrackNameLayout;
    QHBoxLayout *m_panVolumeLayout;

    explicit TrackControlPanelPrivate(TrackControlPanel *q) : q(q) {
        q->setAttribute(Qt::WA_StyledBackground);

        m_btnColor = new QPushButton;
        m_btnColor->setObjectName("btnColor");
//        m_btnColor->setMaximumWidth(12);
        m_btnColor->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

        m_lbTrackIndex = new QLabel("1");
        m_lbTrackIndex->setObjectName("lbTrackIndex");
        m_lbTrackIndex->setAlignment(Qt::AlignCenter);
        m_lbTrackIndex->setMinimumWidth(30);
        m_lbTrackIndex->setMaximumWidth(30);
        m_lbTrackIndex->setMinimumHeight(30);
        m_lbTrackIndex->setMaximumHeight(30);

        m_btnMute = new QPushButton("M");
        m_btnMute->setObjectName("btnMute");
        m_btnMute->setCheckable(true);
        m_btnMute->setChecked(true);
        m_btnMute->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        m_btnMute->setMinimumWidth(30);
        m_btnMute->setMaximumWidth(30);
        m_btnMute->setMinimumHeight(30);
        m_btnMute->setMaximumHeight(30);

        m_btnSolo = new QPushButton("S");
        m_btnSolo->setObjectName("btnSolo");
        m_btnSolo->setCheckable(true);
        m_btnSolo->setChecked(true);
        m_btnSolo->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        m_btnSolo->setMinimumWidth(30);
        m_btnSolo->setMaximumWidth(30);
        m_btnSolo->setMinimumHeight(30);
        m_btnSolo->setMaximumHeight(30);

        m_leTrackName = new CLineEdit("Track 1");
        m_leTrackName->setObjectName("leTrackName");
        m_leTrackName->setMinimumHeight(30);
        m_leTrackName->setMaximumHeight(30);

        m_muteSoloTrackNameLayout = new QHBoxLayout;
        m_muteSoloTrackNameLayout->setObjectName("muteSoloTrackNameLayout");
        m_muteSoloTrackNameLayout->addWidget(m_lbTrackIndex);
        m_muteSoloTrackNameLayout->addWidget(m_btnMute);
        m_muteSoloTrackNameLayout->addWidget(m_btnSolo);
        m_muteSoloTrackNameLayout->addWidget(m_leTrackName);
        m_muteSoloTrackNameLayout->setSpacing(4);
        m_muteSoloTrackNameLayout->setContentsMargins(4, 8, 4, 8);

        auto placeHolder = new QWidget;
        placeHolder->setMinimumWidth(30);
        placeHolder->setMaximumWidth(30);
        placeHolder->setMinimumHeight(30);
        placeHolder->setMaximumHeight(30);

        m_panSlider = new PanSlider;
        m_panSlider->setObjectName("m_panSlider");
//        m_panSlider->setValue(50);

        m_lePan = new CLineEdit("R50");
        m_lePan->setObjectName("lePan");
        m_lePan->setMinimumWidth(60);
        m_lePan->setMaximumWidth(60);
        m_lePan->setMinimumHeight(30);
        m_lePan->setMaximumHeight(30);
        m_lePan->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        m_lePan->setAlignment(Qt::AlignCenter);

        m_volumeSlider = new PanSlider;
        m_volumeSlider->setObjectName("m_volumeSlider");
        m_volumeSlider->setMax(120);
        m_volumeSlider->setMin(0);
        m_volumeSlider->setDefaultValue(100);
//        m_volumeSlider->setValue(100);

        m_leVolume = new CLineEdit("0.0");
        m_leVolume->setObjectName("leVolume");
        m_leVolume->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        m_leVolume->setAlignment(Qt::AlignCenter);
        m_leVolume->setMinimumWidth(60);
        m_leVolume->setMaximumWidth(60);
        m_leVolume->setMinimumHeight(30);
        m_leVolume->setMaximumHeight(30);

        m_panVolumeSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        m_panVolumeLayout = new QHBoxLayout;
        m_panVolumeLayout->addWidget(placeHolder);
        m_panVolumeLayout->addWidget(m_panSlider);
        m_panVolumeLayout->addWidget(m_lePan);
        m_panVolumeLayout->addWidget(m_volumeSlider);
        m_panVolumeLayout->addWidget(m_leVolume);
        m_panVolumeLayout->setSpacing(0);
        m_panVolumeLayout->setContentsMargins(4, 0, 4, 8);

        m_controlWidgetLayout = new QVBoxLayout;
        m_controlWidgetLayout->addLayout(m_muteSoloTrackNameLayout);
        m_controlWidgetLayout->addLayout(m_panVolumeLayout);
        m_controlWidgetLayout->addItem(m_panVolumeSpacer);

        m_levelMeter = new LevelMeter(Qt::Vertical);
        m_levelMeter->initBuffer(8);
        m_levelMeter->setMaximumWidth(12);

        m_mainLayout = new QHBoxLayout;
        m_mainLayout->setObjectName("TrackControlPanel");
        m_mainLayout->addWidget(m_btnColor);
        m_mainLayout->addLayout(m_controlWidgetLayout);
        m_mainLayout->addWidget(m_levelMeter);
        m_mainLayout->setSpacing(0);
        m_mainLayout->setMargin(0);

        q->setLayout(m_mainLayout);

        QObject::connect(m_panSlider, &PanSlider::valueChanged, m_lePan, [=](double value) {
//            q->setTrackPan(value);
//            if (value > 0)
//                m_lePan->setText("R" + QString::number(value));
//            else if (value < 0)
//                m_lePan->setText("L" + QString::number(-value));
//            else
//                m_lePan->setText("C");
        });
    }
};

TrackControlPanel::TrackControlPanel(QWidget *parent) : QWidget(parent), d(new TrackControlPanelPrivate(this)) {
    this->setStyleSheet(QMDecoratorV2::evaluateStyleSheet(R"(
TrackControlPanel {
    background-color: #f1f5ff;
    border-style: none;
    border-radius: 4px;
}

TrackControlPanel > QPushButton#btnColor {
    background-color: #709cff;
    border-style: none;
    border-radius: 0px;
    width: 10px;
}

TrackControlPanel > QLabel#lbTrackIndex{
    font-size: 11pt;
    color: #595959;
}

TrackControlPanel > QPushButton#btnMute{
    color: #595959;
    font-size: 9pt;
    background-color: #fff;
    border-style: none;
    border-radius: 4px;
    border: 1px solid #D9D9D9;
}

TrackControlPanel > QPushButton#btnMute:checked {
    color: #fff;
    background-color: #ff7c80;
    border: none
}

TrackControlPanel > QPushButton#btnSolo{
    color:#595959;
    font-size: 9pt;
    background-color: #fff;
    border-style: none;
    border-radius: 4px;
    border: 1px solid #D9D9D9;
}

TrackControlPanel > QPushButton#btnSolo:checked {
    color: #fff;
    background-color: #ffaf5f;
    border: none
}

TrackControlPanel > CLineEdit {
    font-size: 9pt;
    color: #595959;
    background-color: transparent;
    border: 1px solid transparent;
    border-radius: 4px;
}

TrackControlPanel > CLineEdit:hover {
    border: 1px solid #D9D9D9;
}

TrackControlPanel > CLineEdit:focus {
    color: #595959;
    background-color: #fff;
    border: 1px solid #709cff;
}

)"));
}

TrackControlPanel::~TrackControlPanel() {
    delete d;
}
void TrackControlPanel::resizeEvent(QResizeEvent *event) {
//    qDebug() << event->size();
    if (event->size().height() < 85) {
        for (int i = 0; i < d->m_panVolumeLayout->count(); ++i) {
            QWidget* w = d->m_panVolumeLayout->itemAt(i)->widget();
            if (w != nullptr)
                w->setVisible(false);
            d->m_panVolumeLayout->setContentsMargins(0, 0, 0, 0);
        }
    } else {
        for (int i = 0; i < d->m_panVolumeLayout->count(); ++i) {
            QWidget* w = d->m_panVolumeLayout->itemAt(i)->widget();
            if (w != nullptr)
                w->setVisible(true);
            d->m_panVolumeLayout->setContentsMargins(4, 0, 4, 8);
        }
    }
    QWidget::resizeEvent(event);
}

void TrackControlPanel::setTrackIndex(int index) {
    m_trackIndex = index;
    d->m_lbTrackIndex->setText(QString::number(index));
}
void TrackControlPanel::setTrackColor(QColor color) {
    m_trackColor = color;
    auto hexColor = qRgba(color.red(), color.green(), color.blue(), color.alpha());
    auto qss = QString("background-color: #%1;").arg(QString::number(hexColor, 16));
    qDebug() << qss;
    d->m_btnColor->setStyleSheet(qss);
}
void TrackControlPanel::setTrackName(QString name) {
    m_trackName = name;
    d->m_leTrackName->setText(name);
}

void TrackControlPanel::setTrackPan(double pan) {
    m_trackPan = pan;
    d->m_lePan->setText(QString::number(pan, 'f', 1));
    d->m_panSlider->setValue(pan);
}

void TrackControlPanel::setTrackVolume(double volume) {
    m_trackVolume = volume;
    d->m_leVolume->setText(QString::number(volume, 'f', 1));
    d->m_volumeSlider->setValue(volume);
}
