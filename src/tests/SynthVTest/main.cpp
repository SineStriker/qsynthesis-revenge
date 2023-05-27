#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

#include <Widgets/SynthVSplitter.h>

class MySplitter : public QsApi::SynthVSplitter {
public:
    explicit MySplitter(QWidget *parent = nullptr) : QsApi::SynthVSplitter(parent) {
    }

protected:
    QsApi::SynthVSplitterHandle *createHandle(QWidget *w) override {
        auto handle = QsApi::SynthVSplitter::createHandle(w);
        auto titleBar =
            new QPushButton("TitleBar " + QString(w->metaObject()->className()) + " " + w->property("text").toString());
        titleBar->setObjectName("titleBar");
        titleBar->setStyleSheet("text-align: left;");
        titleBar->setCheckable(true);
        titleBar->setChecked(true);

        connect(titleBar, &QPushButton::toggled, w, &QWidget::setVisible);

        auto layout = new QVBoxLayout();
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(titleBar);
        handle->setLayout(layout);

        return handle;
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMainWindow w;

    auto s = new MySplitter();
    w.setCentralWidget(s);

    auto label1 = new QLabel("1234");
    label1->setObjectName("panel");
    label1->setMinimumHeight(50);

    auto label2 = new QLabel("5678");
    label2->setObjectName("panel");
    label2->setMinimumHeight(50);

    auto label3 = new QLabel("abcd");
    label3->setObjectName("panel");
    label3->setMinimumHeight(50);

    s->addWidget(label1);
    s->addWidget(label2);
    s->addWidget(label3);

    auto area = new QScrollArea();
    area->setFrameStyle(QFrame::NoFrame);

    auto bigWidget = new QPushButton("AAA\n\nBBB", area);
    bigWidget->setObjectName("screen");
    bigWidget->setFixedSize(1000, 1000);
    area->setWidget(bigWidget);
    s->setScrollArea(area);

    w.resize(800, 600);
    w.show();

    w.setStyleSheet(R"(

QWidget {
    font-family: "Microsoft YaHei";
}

QLabel#panel {
    background-color: rgba(0, 0, 0, 30%);
}

QWidget#screen {
    font-size: 50px;
    font-weight: bold;
}

QsApi--SynthVSplitterHandle {
    qproperty-handleHeight: 10;
}

QsApi--SynthVSplitterHandle>QPushButton#titleBar{
    color: white;
    background-color: rgba(0, 0, 0, 50%);
    border: none;
}

QsApi--SynthVSplitterHandle>QPushButton#titleBar:hover{
    background-color: rgba(0, 0, 0, 10%);
}

QPushButton {
    color: black;
    background-color: green;
}

QPushButton:hover {
    background-color: red;
}

QPushButton:pressed {
    background-color: yellow;
}






QScrollBar {
    background: transparent;
    margin: 0;
}

QScrollBar:vertical {
    width: 10px;
}

QScrollBar:horizontal {
    height: 10px;
}

QScrollBar::handle:vertical {
    min-height: 40px;
}

QScrollBar::handle:horizontal {
    min-width: 40px;
}

QScrollBar::handle {
    background: rgba(255, 255, 255, 0.3);
    border: 0px;
    /* margin: 4px 4px 4px 4px; */
}

QScrollBar::handle:hover {
    background: rgba(255, 255, 255, 0.5);
}

QScrollBar::handle:pressed {
    background: rgba(255, 255, 255, 0.4);
}

QScrollBar::sub-page {
    background: transparent;
}

QScrollBar::add-page {
    background: transparent;
}

QScrollBar::up-arrow {
    background: transparent;
}

QScrollBar::down-arrow {
    background: transparent;
}

QScrollBar::left-arrow {
    background: transparent;
}

QScrollBar::right-arrow {
    background: transparent;
}

QScrollBar::sub-line {
    background: transparent;
    width: 0px;
}

QScrollBar::add-line {
    background: transparent;
    width: 0px;
}

)");

    return app.exec();
}
