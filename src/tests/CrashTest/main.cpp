#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>

#include <QBreakpadHandler.h>

#ifdef Q_OS_WINDOWS
#    include <Windows.h>
#endif

void showCrash() {
    QString text = "Unexpected exception happens, ready to crash!";
#ifdef Q_OS_WINDOWS
    ::MessageBoxW(nullptr, text.toStdWString().data(), qApp->applicationName().toStdWString().data(),
                  MB_OK | MB_TOPMOST | MB_ICONERROR);
#else
    fprintf(stderr, "%s\n", text.toStdString().data());
#endif
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QFont f("Microsoft YaHei UI");
    f.setPixelSize(15);
    a.setFont(f);

    // Set dump path
    QBreakpadInstance.setDumpPath(a.applicationDirPath() + "/crashes");

    // Set handler
    QBreakpadHandler::UniqueExtraHandler = []() {
        showCrash();
        QFile file("crashes/1.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.write(QDateTime::currentDateTime().toString().toUtf8());
        }

        file.close();
    };

    QMainWindow w;

    {
        auto btn = new QPushButton("Crash Now!");
        btn->setStyleSheet("margin: 20px");
        w.setCentralWidget(btn);
        QObject::connect(btn, &QPushButton::clicked, []() { qDebug() << reinterpret_cast<QString *>(1)->toInt(); });
    }

    // QMainWindow w;
    w.show();

    return a.exec();
}