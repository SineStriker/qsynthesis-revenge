//
// Created by Crs_1 on 2023/5/13.
//
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QProcess>
#include <QDebug>

struct Callbacks {
    void (*setDirty)();
    void (*setError)(const char *error);
    void (*setStatus)(const char *status);
};

extern "C" bool Initializer ();
extern "C" void CallbacksBinder (const Callbacks &callbacks_);

int main(int argc, char **argv) {
    CallbacksBinder({
       [](){},
        [](const char *error){
            qDebug() << error;
        },
        [](const char *status){
            qDebug() << "Status: " << status;
        },
    });
    Initializer();
}
