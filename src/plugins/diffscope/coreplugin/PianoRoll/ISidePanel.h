#ifndef ISIDEPANEL_H
#define ISIDEPANEL_H

#include <QLayout>
#include <QToolBar>

#include <CDockToolWindow.h>
#include <QPixelSize.h>

#include "QsFrameworkGlobal.h"

#include "coreplugin/CoreGlobal.h"
#include "coreplugin/Interfaces/IPianoRollComponent.h"

namespace Core {

    class ISidePanelPrivate;

    class CORE_EXPORT ISidePanel : public CDockToolWindow, public IPianoRollComponent {
        Q_OBJECT
        Q_LAYOUT_PROPERTY_DELCARE
        Q_DECLARE_PRIVATE(ISidePanel)
        Q_D_LAYOUT_PROPERTY(caption, Caption)
    public:
        explicit ISidePanel(IProjectWindow *iWin, QWidget *parent = nullptr);
        ~ISidePanel();

    public:
        void initialize() override;
        void extensionInitialized() override;

    public:
        QToolBar *toolBar() const;

        QWidget *captionWidget() const;
        QWidget *takeCaptionWidget();
        void setCaptionWidget(QWidget *w);

        QWidget *centralWidget() const;
        QWidget *takeCentralWidget();
        void setCentralWidget(QWidget *w);

    protected:
        bool event(QEvent *event) override;

        void viewModeChanged(CDockCard::ViewMode viewMode) override;

    protected:
        ISidePanel(ISidePanelPrivate &d, IProjectWindow *iWin, QWidget *parent = nullptr);

        QScopedPointer<ISidePanelPrivate> d_ptr;
    };

}

#endif // ISIDEPANEL_H