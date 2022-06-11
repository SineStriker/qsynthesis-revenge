#ifndef QCOMMANDPALETTEPRIVATE_H
#define QCOMMANDPALETTEPRIVATE_H

#include <QLineEdit>
#include <QListWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QtGlobal>

class QCommandPalette;

class QCommandPalettePrivate {
    Q_DECLARE_PUBLIC(QCommandPalette)
public:
    QCommandPalettePrivate();
    ~QCommandPalettePrivate();

    void init();

    QCommandPalette *q_ptr;

    QVBoxLayout *layout;
    QLineEdit *lineEdit;
    QListWidget *listWidget;
};

#endif // QCOMMANDPALETTEPRIVATE_H
