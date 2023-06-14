#ifndef CWORKFLOWPAGE_H
#define CWORKFLOWPAGE_H

#include <QFrame>
#include <QSplitter>

#include "QMWidgetToolsGlobal.h"

class CWorkflowPagePrivate;

class QMWTOOLS_EXPORT CWorkflowPage : public QFrame {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CWorkflowPage)
public:
    explicit CWorkflowPage(QWidget *parent = nullptr);
    ~CWorkflowPage();

public:
    enum Button {
        PreviousButton = 1,
        NextButton = 2,
        FinishButton = 4,
        OkButton = 8,
        CancelButton = 16,
        HelpButton = 32,

        SingleStepButtons = OkButton | CancelButton,
        StandardButtons = PreviousButton | NextButton | CancelButton,
        FinalButtons = PreviousButton | FinishButton | CancelButton,
    };
    Q_ENUM(Button)
    Q_DECLARE_FLAGS(Buttons, Button)

    QSplitter *splitter() const;

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
    CWorkflowPage(CWorkflowPagePrivate &d, QWidget *parent = nullptr);

    QScopedPointer<CWorkflowPagePrivate> d_ptr;
};

#endif // CWORKFLOWPAGE_H
