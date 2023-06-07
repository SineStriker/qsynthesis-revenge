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

    signals:
        void pianoKeyWidgetChanged(const QString &key);
        void floatingPanelStateChanged(const QString &key, FloatingPanelState state);

    protected:
        PianoRoll(PianoRollPrivate &d, QWidget *parent = nullptr);

        QScopedPointer<PianoRollPrivate> d_ptr;

        friend class IProjectWindow;
    };

}

#endif // PIANOROLL_H