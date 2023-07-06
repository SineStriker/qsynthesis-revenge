#ifndef PIANOROLL_H
#define PIANOROLL_H

#include <QAbstractButton>
#include <QFrame>

#include <CoreApi/IWindow.h>

#include "IPianoKeyWidget.h"
#include "SectionBar.h"

namespace Core {

    class IProjectWindow;

    class PianoRollPrivate;

    class CORE_EXPORT PianoRoll : public QFrame, public IPianoRollComponent {
        Q_OBJECT
        Q_DECLARE_PRIVATE(PianoRoll)
    public:
        explicit PianoRoll(IProjectWindow *iWin, QWidget *parent = nullptr);
        ~PianoRoll();

        void initialize() override;
        void extensionInitialized() override;

        enum FloatingPanelState {
            Hidden,
            Normal,
            Folded,
        };
        Q_ENUM(FloatingPanelState);

    public:
        void addPianoKeyWidgetFactory(const QString &key, IPianoKeyWidgetFactory *factory);
        void removePianoKeyWidgetFactory(const QString &key);
        QStringList pianoKeyWidgetKeys() const;
        QString currentPianoKeyWidgetKey() const;
        void setCurrentPianoKeyWidget(const QString &key);
        QWidget *currentPianoKeyWidget() const;
        IPianoKeyWidgetFactory *pianoKeyWidgetFactory(const QString &key) const;

        QAbstractButton *addFloatingPanel(const QString &key, QWidget *panel, QWidget *titleBar);
        void removeFloatingPanel(const QString &key);
        QStringList floatingPanelKeys() const;
        FloatingPanelState floatingPanelState(const QString &key);
        void setFloatingPanelState(const QString &key, FloatingPanelState state);
        QWidget *floatingPanel(const QString &key);

        SectionBar *sectionBar() const;

    signals:
        void pianoKeyWidgetChanged(const QString &key);
        void floatingPanelStateChanged(const QString &key, FloatingPanelState state);

    protected:
        PianoRoll(PianoRollPrivate &d, IProjectWindow *iWin, QWidget *parent = nullptr);

        QScopedPointer<PianoRollPrivate> d_ptr;

        friend class IProjectWindow;
    };

}

#endif // PIANOROLL_H