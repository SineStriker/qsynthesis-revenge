#include "LevelMeter.h"
#include "QProgressBar"
#include "QRadioButton"
#include "SeekBar.h"
#include "TrackControlPanel.h"
#include <QAction>
#include <QApplication>
#include <QComboBox>
#include <QDebug>
#include <QListView>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMetaEnum>
#include <QStyledItemDelegate>
#include <QVBoxLayout>

#include <CCheckBox.h>
#include <CTabButton.h>
#include <QGraphicsDropShadowEffect>
#include <QGroupBox>
#include <QMAppExtension.h>
#include <QMDecoratorV2.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QApplication::setEffectEnabled(Qt::UI_AnimateCombo, false);

    QMAppExtension appExt;

    QMainWindow w;

    auto applyShadow = [](QWidget *widget, QWidget *parent) {
        widget->setWindowFlags(widget->windowFlags()
                               | Qt::FramelessWindowHint
                               | Qt::NoDropShadowWindowHint);
        widget->setAttribute(Qt::WA_TranslucentBackground, true);
        auto *shadow = new QGraphicsDropShadowEffect(parent);
        shadow->setOffset(0, 4);
        shadow->setColor(QColor(0, 0, 0, 80));
        shadow->setBlurRadius(12);
        widget->setGraphicsEffect(shadow);
    };

    auto button = new QPushButton("Test");
    button->setObjectName("test-button");

    auto comboBox1 = new QComboBox;
    comboBox1->setObjectName("test-comboBox");
    QStringList list1;
    list1 << "test1"
          << "test2"
          << "test3"
          << "test4"
          << "test5"
          << "test6"
          << "test7"
          << "test8"
          << "test9"
          << "test10"
          << "test11"
          << "test12";
    comboBox1->addItems(list1);
    comboBox1->setItemDelegate(new QStyledItemDelegate());
    applyShadow(comboBox1->view()->window(), &w);

    auto comboBox2 = new QComboBox;
    comboBox2->setObjectName("test-comboBox");
    QStringList list2;
    list2 << "test1"
          << "test2"
          << "test3";
    comboBox2->addItems(list2);
    comboBox2->setItemDelegate(new QStyledItemDelegate());
    applyShadow(comboBox2->view()->window(), &w);

    auto checkBox1 = new CCheckBox();
    checkBox1->setText("123");

    auto groupBox = new QGroupBox;

    auto verticalLayout = new QVBoxLayout(groupBox);

    auto radioButton1 = new QRadioButton("RadioButton1", groupBox);
    radioButton1->setChecked(true);
    auto radioButton2 = new QRadioButton("RadioButton2", groupBox);
    auto radioButton3 = new QRadioButton("RadioButton3", groupBox);

    verticalLayout->addWidget(radioButton1);
    verticalLayout->addWidget(radioButton2);
    verticalLayout->addWidget(radioButton3);

    auto progressBar = new QProgressBar();
    progressBar->setMaximum(100);
    progressBar->setMinimum(0);
    progressBar->setValue(50);

    auto panSlider = new SeekBar();
    panSlider->setValue(80);

    auto volumeSlider = new SeekBar();
    volumeSlider->setMax(120);
    volumeSlider->setMin(0);
    volumeSlider->setValue(100);
    volumeSlider->setDefaultValue(100);
    volumeSlider->setTrackActiveStartValue(0);

    auto reverseVolumeSlider = new SeekBar();
    reverseVolumeSlider->setMax(120);
    reverseVolumeSlider->setMin(0);
    reverseVolumeSlider->setValue(50);
    reverseVolumeSlider->setDefaultValue(100);
    reverseVolumeSlider->setTrackActiveStartValue(120);

    auto track1 = new TrackControlPanel;
    track1->setTrackIndex(1);
    track1->setTrackName("轨道1");
    track1->setTrackColor(QColor(112, 156, 255));
    track1->setTrackPan(-100);
    track1->setTrackVolume(0);

    auto track2 = new TrackControlPanel;
    track2->setTrackIndex(2);
    track2->setTrackName("轨道2");
    track2->setTrackColor(QColor(113, 218, 255));
    track2->setTrackPan(-50);
    track2->setTrackVolume(30);

    auto track3 = new TrackControlPanel;
    track3->setTrackIndex(3);
    track3->setTrackName("轨道3");
    track3->setTrackColor(QColor(255, 204, 153));
    track3->setTrackPan(0);
    track3->setTrackVolume(60);

    auto track4 = new TrackControlPanel;
    track4->setTrackIndex(4);
    track4->setTrackName("轨道4");
    track4->setTrackColor(QColor(92, 236, 130));
    track4->setTrackPan(50);
    track4->setTrackVolume(90);

    auto track5 = new TrackControlPanel;
    track5->setTrackIndex(5);
    track5->setTrackName("轨道5");
    track5->setTrackColor(QColor(255, 124, 128));
    track5->setTrackPan(100);
    track5->setTrackVolume(120);

    auto verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    auto mainLayout = new QVBoxLayout;
//    mainLayout->addWidget(button);
//    mainLayout->addWidget(comboBox1);
//    mainLayout->addWidget(comboBox2);
//    mainLayout->addWidget(checkBox1);
//    mainLayout->addWidget(groupBox);
//    mainLayout->addWidget(progressBar);
//    mainLayout->addWidget(panSlider);
//    mainLayout->addWidget(volumeSlider);
//    mainLayout->addWidget(reverseVolumeSlider);
    mainLayout->addWidget(track1);
    mainLayout->addWidget(track2);
    mainLayout->addWidget(track3);
    mainLayout->addWidget(track4);
    mainLayout->addWidget(track5);
//    mainLayout->addItem(verticalSpacer);

    auto mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);

    w.setCentralWidget(mainWidget);
    w.resize(500, 700);
    w.show();

    auto menu = new QMenu("File", &w);
    auto action1 = menu->addAction("Action1");
    action1->setCheckable(true);

    auto action2 = menu->addAction("Action2");
    action2->setIcon(QIcon(
        "C:/Users/truef/Documents/GitHub/qsynthesis-revenge/src/plugins/diffscope/coreplugin/Res/svg/home/info.svg"));

    auto action3 = menu->addAction("Action3");
    //    action3->setCheckable(true);

    applyShadow(menu, &w);
    w.menuBar()->addMenu(menu);

    w.setStyleSheet(QMDecoratorV2::evaluateStyleSheet(R"(

QPushButton#test-button {
    qproperty-icon: svg(C:/Users/truef/Documents/GitHub/qsynthesis-revenge/src/plugins/diffscope/coreplugin/Res/svg/home/info.svg, green);
}

QMenu::indicator::checked {
    image: svg(C:/Users/truef/Documents/GitHub/qsynthesis-revenge/src/plugins/diffscope/coreplugin/Res/svg/home/info.svg, green);
    height: 20px;
    width: 20px;
    left: 10px;
}

QMenu {
    background-color: white;
    border-radius: 8px;
    border: 1px solid #f0f0f0;
    margin: 4px 8px 12px 8px;
}

QMenu::icon {
    left: 10px;
}

QMenu::item {
    padding: 10px 20px;
    background: transparent;
}

QComboBox
{
    background-color:#1e2228;
    color:#adbac7;
    border-style: none;
    border-radius: 4px;
    border: 1px solid #6b6b6b;
    padding: 4px;
}
QComboBox:disabled
{
    color:rgb(160,160,160);
}
QComboBox:hover
{
    background-color:#4c5052;
}

QComboBox:on
{
    background-color:#4c5052;
}

QComboBox QAbstractItemView
{
    outline: 0px solid gray;
    background-color: #1e2228;
    color: #adbac7;
    selection-background-color: #2d333b;
    /* border-style: none; */
    border-radius: 8px;
    border: 1px solid #444c56;
    padding: 4px 0px 4px 0px;
    margin: 4px 8px 12px 8px;
}

QComboBox QAbstractItemView::item
{
    min-height: 24px;
    border-style: none;
    border-radius: 4px;
    margin: 0px 4px 0px 4px;
    padding: 4px;
}

QComboBox QAbstractItemView::item:hover
{
    background-color: #4c5052;
}

QComboBox QAbstractItemView::item:selected
{
    background-color: #4c5052;
}

QComboBox::drop-down
{
    border:none;
}

QComboBox::down-arrow
{
    right: 10px;
    width: 20px;
    height: 20px;
    image: svg(":/icons/chevron_down_16_filled.svg", #adbac7);
}

QComboBox::down-arrow:on
{
    image: svg(":/icons/chevron_down_16_filled.svg", #adbac7);
}

QComboBox QScrollBar::vertical{
    width:10px;
    background-color: #282c31;
    /* border-style: none;
    border-radius: 4px; */
}

QComboBox QScrollBar::add-page:vertical,
QComboBox QScrollBar::sub-page:vertical {
    background: none;
}

QComboBox QScrollBar::handle::vertical{
    border-radius:4px;
    width: 10px;
    background:#474b50;
}
QComboBox QScrollBar::handle::vertical::hover{
    background:#57595d;
}
QComboBox QScrollBar::add-line::vertical{
    border:none;
}
QComboBox QScrollBar::sub-line::vertical{
    border:none;
}




CCheckBox {
    --layoutMargins: 0;
    --layoutSpacing: 5px;
}

CCheckBox>QLabel#label {
    color: #adbac7;
}

CCheckBox>CToolButton#box {
    padding: 0;
    border-radius: 4px;
    background-color: #1e2228;
    border: 1px solid #6b6b6b;
    --icon: svg(":/icons/check-line.svg", (up=transparent, up2=#FFFFFF));
    --iconSize: 16px 16px;
}

CCheckBox>CToolButton#box:focus {
    border-color: #167FD2;
}

QRadioButton {
    border: none;
    color: #333;
}

QRadioButton::indicator:checked {
    border: 4px solid #0060c0;
    background-color: white;
    border-radius: 8px;
}

QRadioButton::indicator:checked:hover {
    border: 3px solid #0075EA;
}

QRadioButton::indicator:checked:pressed {
    border: 4px solid #004F9E;
}

QRadioButton::indicator:checked:disabled {
    border: 4px solid #e0e0e0;
}

QRadioButton::indicator:unchecked {
    border: 1px solid #d4d4d4;
    background-color: white;
    border-radius: 8px;
}

QRadioButton::indicator:unchecked:hover {
    background-color: #f6f6f6;
}

QRadioButton::indicator:unchecked:pressed {
    border: 4px solid #d4d4d4;
    background-color: #fff;
}

QRadioButton::indicator:unchecked:disabled {
    border: 1px solid #f0f0f0;
}

QProgressBar {
    background-color:#FFFFFF;
    color:#808080;
    border-style: none;
    border-radius: 4px;
    border: 1px solid #d4d4d4;
    text-align:center;
}

QProgressBar::chunk {
    background-color: #0060C0;
    border-radius: 4px;
}

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

    return a.exec();
}
