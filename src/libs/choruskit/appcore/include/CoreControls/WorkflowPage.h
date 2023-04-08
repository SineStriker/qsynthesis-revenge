#ifndef WORKFLOWPAGE_H
#define WORKFLOWPAGE_H

#include <QSplitter>

#include "CoreApi/CkAppCoreGlobal.h"

namespace Core {

    class WorkflowPagePrivate;

    class CKAPPCORE_API WorkflowPage : public QSplitter {
        Q_OBJECT
        Q_DECLARE_PRIVATE(WorkflowPage)
    public:
        explicit WorkflowPage(QWidget *parent = nullptr);
        ~WorkflowPage();

    public:
        enum Button {
            PreviousButton = 1,
            NextButton = 2,
            FinishButton = 4,
            CancelButton = 8,
            HelpButton = 16,

            StandardButtons = PreviousButton | NextButton | CancelButton,
            FinalButtons = PreviousButton | FinishButton | CancelButton,
        };
        Q_ENUM(Button)
        Q_DECLARE_FLAGS(Buttons, Button)

        QString title() const;
        void setTitle(const QString &title);

        QString description() const;
        void setDescription(const QString &description);

        QWidget *takeWidget();
        QWidget *widget() const;
        void setWidget(QWidget *w);

        QWidget *takeSideWidget();
        QWidget *sideWidget() const;
        void setSideWidget(QWidget *w);

        Buttons buttons() const;
        void setButtons(Buttons buttons);
        void setButton(Button button, bool visible);

        QString buttonText(Button which) const;
        void setButtonText(Button which, const QString &text);

        bool buttonEnabled(Button which) const;
        void setButtonEnabled(Button which, bool enabled);

        virtual void clearPage();
        virtual bool validatePage();

    signals:
        void buttonsChanged(Buttons buttons);
        void buttonTextChanged(Button which, const QString &text);
        void buttonEnabledChanged(Button which, bool enabled);
        void helpRequested();

    protected:
        WorkflowPage(WorkflowPagePrivate &d, QWidget *parent = nullptr);

        QScopedPointer<WorkflowPagePrivate> d_ptr;
    };

}

#endif // WORKFLOWPAGE_H