#ifndef WORKFLOWDIALOG_H
#define WORKFLOWDIALOG_H

#include <QDialog>
#include <QSplitter>

#include "coreplugin/CoreGlobal.h"

namespace Core {

    class WorkflowPagePrivate;

    class WorkflowDialogPrivate;

    class CORE_EXPORT WorkflowPage : public QSplitter {
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
        void setButton(Button button, bool enabled);

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

    class CORE_EXPORT WorkflowDialog : public QDialog {
        Q_OBJECT
        Q_DECLARE_PRIVATE(WorkflowDialog)
    public:
        explicit WorkflowDialog(QWidget *parent = nullptr);
        ~WorkflowDialog();

    public:
        WorkflowPage *page() const;
        WorkflowPage *takePage();
        void setPage(WorkflowPage *w);

    protected:
        virtual void prev();
        virtual void next();
        virtual void finish();

    signals:
        void aboutToPrev();
        void aboutToNext();
        void finished();

        void helpRequested();

    protected:
        WorkflowDialog(WorkflowDialogPrivate &d, QWidget *parent = nullptr);

        QScopedPointer<WorkflowDialogPrivate> d_ptr;
    };

}

#endif // WORKFLOWDIALOG_H