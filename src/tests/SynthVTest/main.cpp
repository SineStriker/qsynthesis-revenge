#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

#include <QMWidgetsHost.h>
#include <Widgets/CDockFrame.h>
#include <Widgets/SynthVSplitter.h>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMWidgetsHost host;

    auto frame = new CDockFrame();
    frame->addWidget(Qt::LeftEdge, QM::Primary, new QLabel("A1"))->setText("A1");
    frame->addWidget(Qt::LeftEdge, QM::Primary, new QLabel("A2"))->setText("A2");
    frame->addWidget(Qt::RightEdge, QM::Primary, new QLabel("A3"))->setText("A3");
    frame->addWidget(Qt::LeftEdge, QM::Secondary, new QLabel("A4"))->setText("A4");
    frame->setWidget(new QLabel("Central"));

    QMainWindow w;
    w.setCentralWidget(frame);
    w.show();

    w.setStyleSheet(R"(

QLabel {
    border: 2px solid red;
    margin: 2px;
}

QWidget#dock_card_container {
    border: 1px solid blue;
}

)");

    return app.exec();
}
