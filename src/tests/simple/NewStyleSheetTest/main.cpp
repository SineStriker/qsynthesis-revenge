#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMetaEnum>

#include <CTabButton.h>
#include <QMAppExtension.h>
#include <QMDecoratorV2.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QMAppExtension appExt;

    auto button = new QPushButton("Test");
    button->setObjectName("test-button");

    QMainWindow w;
    w.setCentralWidget(button);
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

)"));

    return a.exec();
}
