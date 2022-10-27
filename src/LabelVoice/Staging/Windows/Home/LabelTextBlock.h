#ifndef LABELTEXTBLOCK_H
#define LABELTEXTBLOCK_H

#include <QWidget>

#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

#include "CPushButton.h"

class LabelTextBlock : public QFrame {
    Q_OBJECT
public:
    explicit LabelTextBlock(bool browse, QWidget *parent = nullptr);
    ~LabelTextBlock();

public:
    QLabel *label;
    QLineEdit *text;
    CPushButton *browseButton;

    void clear();

protected:
    QVBoxLayout *mainLayout;
    QHBoxLayout *textButtonLayout;

signals:

private:
    void _q_browseClicked();
};

#endif // LABELTEXTBLOCK_H
