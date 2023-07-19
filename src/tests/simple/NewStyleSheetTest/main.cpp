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
#include <QMAppExtension.h>
#include <QMDecoratorV2.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QApplication::setEffectEnabled(Qt::UI_AnimateCombo, false);

    QMAppExtension appExt;

    QMainWindow w;

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

    auto comboBox2 = new QComboBox;
    comboBox2->setObjectName("test-comboBox");
    QStringList list2;
    list2 << "test1"
          << "test2"
          << "test3";
    comboBox2->addItems(list2);
    comboBox2->setItemDelegate(new QStyledItemDelegate());

    auto checkBox1 = new CCheckBox();
    checkBox1->setText("123");

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(button);
    mainLayout->addWidget(comboBox1);
    mainLayout->addWidget(comboBox2);
    mainLayout->addWidget(checkBox1);

    auto mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);

    w.setCentralWidget(mainWidget);
    w.show();

    auto menu = new QMenu("File", &w);
    auto action1 = menu->addAction("Action1");
    action1->setCheckable(true);

    auto action2 = menu->addAction("Action2");
    action2->setIcon(QIcon(
        "C:/Users/truef/Documents/GitHub/qsynthesis-revenge/src/plugins/diffscope/coreplugin/Res/svg/home/info.svg"));

    auto action3 = menu->addAction("Action3");
    //    action3->setCheckable(true);

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
    border-style: none;
    /* border-radius: 8px; */
    border: 1px solid #444c56;
    padding: 4px 0px 4px 0px
}

QComboBox QAbstractItemView::item
{
    min-height: 24px;
    border-style: none;
    border-radius: 4px;
    margin: 0px 4px 0px 4px
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
    border-style: none;
    border-radius: 4px;
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

)"));

    return a.exec();
}
