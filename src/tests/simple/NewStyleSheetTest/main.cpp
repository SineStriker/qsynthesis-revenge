#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QMetaEnum>

#include <CTabButton.h>
#include <QFontInfoEx.h>
#include <QMAppExtension.h>
#include <QRectInfo.h>

#include "private/QMetaTypeUtils.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QMAppExtension appExt;

    auto fi = QRectInfo::fromStringList({"qrect", "(white, up2=lightgrey), 0, 2px"});
    qDebug() << fi;

    QStringList colors;
    for (int i = 0; i < 8; ++i) {
        auto key = static_cast<QM::ClickState>(i);
        colors << fi.color(key).name();
    }
    qDebug() << colors;

    auto button = new CTabButton("Information");
    button->setCheckable(true);
    button->setStyleSheet(R"(

CPushButton {
    qproperty-svgIcon: svg((
"C:/Users/truef/Documents/GitHub/qsynthesis-revenge/src/plugins/diffscope/coreplugin/Res/svg/home/info.svg",
up2="C:/Users/truef/Documents/GitHub/qsynthesis-revenge/src/plugins/diffscope/coreplugin/Res/svg/home/new-file.svg")
, auto);
    color: blue;
    qproperty-spaceRatio: 0.5;
}

CPushButton:hover {
    color: red;
}

CPushButton:pressed {
    color: green;
}

)");

    QMainWindow w;
    w.setCentralWidget(button);
    w.show();

    return a.exec();
}
