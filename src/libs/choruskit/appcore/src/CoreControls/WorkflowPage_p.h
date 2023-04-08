#ifndef WORKFLOWPAGE_P_H
#define WORKFLOWPAGE_P_H

#include <QBoxLayout>
#include <QLabel>
#include <QSplitter>
#include <QStackedWidget>

#include <QMap>

#include "WorkflowPage.h"

namespace Core {

    class WorkflowPagePrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(WorkflowPage)
    public:
        WorkflowPagePrivate();
        ~WorkflowPagePrivate();

        void init();

        void reloadStrings();

        QString buttonText(WorkflowPage::Button button) const;
        void setButtonText(WorkflowPage::Button button, const QString &text);

        WorkflowPage *q_ptr;

        QLabel *titleLabel;
        QLabel *descriptionLabel;
        QStackedWidget *stackedWidget;

        QVBoxLayout *rightLayout;
        QWidget *rightWidget;

        QWidget *emptyWidget;

        WorkflowPage::Buttons buttons;

        struct ButtonData {
            QString text;
            QString overrideText;
            bool enabled;
            ButtonData() : enabled(true){};
        };

        QMap<WorkflowPage::Button, ButtonData> buttonsData;
    };


}

#endif // WORKFLOWPAGE_P_H
