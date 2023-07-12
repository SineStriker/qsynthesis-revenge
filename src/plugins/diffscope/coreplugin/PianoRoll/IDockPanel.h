#ifndef IDOCKPANEL_H
#define IDOCKPANEL_H

#include <QLayout>
#include <QToolBar>

#include <CDockToolWindow.h>
#include <QPixelSize.h>

#include "QsFrameworkGlobal.h"

#include "coreplugin/CoreGlobal.h"
#include "coreplugin/Interfaces/IPianoRollComponent.h"

namespace Core {

    class IDockPanelPrivate;

    class CORE_EXPORT IDockPanel : public CDockToolWindow, public IPianoRollComponent {
        Q_OBJECT
        Q_LAYOUT_PROPERTY_DELCARE
        Q_DECLARE_PRIVATE(IDockPanel)
        Q_D_LAYOUT_PROPERTY(caption, Caption)
    public:
        explicit IDockPanel(IProjectWindow *iWin, QWidget *parent = nullptr);
        ~IDockPanel();

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
        IDockPanel(IDockPanelPrivate &d, IProjectWindow *iWin, QWidget *parent = nullptr);

        QScopedPointer<IDockPanelPrivate> d_ptr;
    };

}

#endif // IDOCKPANEL_H