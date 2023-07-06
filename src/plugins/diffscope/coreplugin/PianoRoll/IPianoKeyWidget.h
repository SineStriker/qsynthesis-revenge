#ifndef CHORUSKIT_IPIANOKEYWIDGET_H
#define CHORUSKIT_IPIANOKEYWIDGET_H

#include <QFrame>

#include "coreplugin/CoreGlobal.h"
#include "coreplugin/Interfaces/IPianoRollComponent.h"

namespace Core {

    class IPianoKeyWidget;

    class CORE_EXPORT IPianoKeyWidgetFactory {
    public:
        virtual ~IPianoKeyWidgetFactory();
        virtual IPianoKeyWidget *create(IProjectWindow *iWin, QWidget *parent) = 0;

        virtual QString name() const = 0;
    };

    class CORE_EXPORT IPianoKeyWidget : public QFrame, public IPianoRollComponent {
        Q_OBJECT
    public:
        explicit IPianoKeyWidget(IProjectWindow *iWin, QWidget *parent = nullptr);
        ~IPianoKeyWidget();

        virtual int currentHeight() const = 0;
        virtual void setCurrentHeight(int height) = 0;
    };

}


#endif // CHORUSKIT_IPIANOKEYWIDGET_H
