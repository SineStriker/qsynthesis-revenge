#ifndef HOMEPROJCONFWIDGET_H
#define HOMEPROJCONFWIDGET_H

#include <QBoxLayout>
#include <QFrame>
#include <QLabel>

#include "LabelTextBlock.h"

#include "QTypeList.h"

class HomeProjConfWidget : public QFrame {
    Q_OBJECT
public:
    explicit HomeProjConfWidget(QWidget *parent = nullptr);
    ~HomeProjConfWidget();

    void reloadStrings();

    void clear();

    QTypeList styleData() const;
    void setStyleData(const QTypeList &list);

    QHBoxLayout *upperLayout;
    QVBoxLayout *entityLayout;

    QLabel *titleLabel;
    QLabel *subtitleLabel;

    LabelTextBlock *projectName;
    LabelTextBlock *location;
    LabelTextBlock *solution;

    CPushButton *backButton;
    CPushButton *createButton;

    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;

private:
    void _q_backButtonClicked();
    void _q_createButtonClicked();

signals:
    void confirmed();
    void canceled();
};

#endif // HOMEPROJCONFWIDGET_H
