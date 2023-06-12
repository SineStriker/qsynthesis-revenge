#ifndef QMWORKFLOWPAGE_P_H
#define QMWORKFLOWPAGE_P_H

#include <QBoxLayout>
#include <QLabel>
#include <QSplitter>
#include <QStackedWidget>

#include <QMap>

#include "QMWorkflowPage.h"

class QMWorkflowPagePrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(QMWorkflowPage)
public:
    QMWorkflowPagePrivate();
    ~QMWorkflowPagePrivate();

    void init();

    void reloadStrings();

    QString buttonText(QMWorkflowPage::Button button) const;
    void setButtonText(QMWorkflowPage::Button button, const QString &text);

    QMWorkflowPage *q_ptr;

    QLabel *titleLabel;
    QLabel *descriptionLabel;
    QStackedWidget *stackedWidget;

    QVBoxLayout *rightLayout;
    QWidget *rightWidget;

    QWidget *emptyWidget;

    QSplitter *splitter;
    QVBoxLayout *mainLayout;

    QMWorkflowPage::Buttons buttons;

    struct ButtonData {
        QString text;
        QString overrideText;
        bool enabled;
        ButtonData() : enabled(true){};
    };

    QMap<QMWorkflowPage::Button, ButtonData> buttonsData;
};

#endif // QMWORKFLOWPAGE_P_H
