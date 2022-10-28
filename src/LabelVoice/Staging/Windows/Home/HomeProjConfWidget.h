#ifndef HOMEPROJCONFWIDGET_H
#define HOMEPROJCONFWIDGET_H

#include <QBoxLayout>
#include <QFrame>
#include <QLabel>

#include "LabelTextBlock.h"

#include "QTypeList.h"

#include "Windows/Basic/ProjectCommonBlock.h"

class HomeProjConfWidget : public QFrame {
    Q_OBJECT
public:
    explicit HomeProjConfWidget(ProjectCommonBlock *block, QWidget *parent = nullptr);
    ~HomeProjConfWidget();

    void reloadStrings();

    void clear();

    QTypeList styleData() const;
    void setStyleData(const QTypeList &list);

    QLabel *titleLabel;
    QLabel *subtitleLabel;

    LabelTextBlock *projectName;
    LabelTextBlock *location;
    LabelTextBlock *solution;

    CPushButton *backButton;
    CPushButton *createButton;

protected:
    ProjectCommonBlock *block;

    QHBoxLayout *upperLayout;
    QVBoxLayout *entityLayout;

    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;

private:
    void _q_backButtonClicked();
    void _q_createButtonClicked();

signals:
    void confirmed(const QString &filename);
    void canceled();
};

#endif // HOMEPROJCONFWIDGET_H
