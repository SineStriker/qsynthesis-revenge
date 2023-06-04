#ifndef PIANOROLL_H
#define PIANOROLL_H

#include <QAbstractButton>
#include <QFrame>

#include <CoreApi/IWindow.h>

#include "IPianoKeyWidget.h"

namespace Core {

    class IProjectWindow;

    class PianoRollPrivate;

    class CORE_EXPORT PianoRoll : public QFrame {
        Q_OBJECT
        Q_DECLARE_PRIVATE(PianoRoll)
    public:
        explicit PianoRoll(QWidget *parent = nullptr);
        ~PianoRoll();

        enum FloatingPanelState {
            Hidden,
            Normal,
            Folded,
        };

        Q_ENUM(FloatingPanelState);

    public:
        void addPianoKeyWidget(const QString &id, IPianoKeyWidgetFactory *factory);
        QAbstractButton *addFloatingPanel(const QString &id, QWidget *panel, QWidget *titleBar);

        QStringList pianoKeyWidgets() const;
        QString currentPianoKeyWidgetId() const;
        void setCurrentPianoKeyWidget(const QString &id);
        QWidget *currentPianoKeyWidget() const;
        IPianoKeyWidgetFactory *pianoKeyWidgetFactory(const QString &id) const;

        QStringList floatingPanels() const;
        FloatingPanelState floatingPanelState(const QString &id);
        void setFloatingPanelState(const QString &id, FloatingPanelState state);
        QWidget *floatingPanel(const QString &id);

    signals:
        void pianoKeyWidgetChanged(const QString &id);
        void floatingPanelStateChanged(const QString &id, FloatingPanelState state);

    protected:
        PianoRoll(PianoRollPrivate &d, QWidget *parent = nullptr);

        QScopedPointer<PianoRollPrivate> d_ptr;

        friend class IProjectWindow;
    };

}

#endif // PIANOROLL_H