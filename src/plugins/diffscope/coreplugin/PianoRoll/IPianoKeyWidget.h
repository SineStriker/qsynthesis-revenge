#ifndef CHORUSKIT_IPIANOKEYWIDGET_H
#define CHORUSKIT_IPIANOKEYWIDGET_H

#include <QFrame>

#include "coreplugin/CoreGlobal.h"

namespace Core {

    class IPianoKeyWidget;

    class CORE_EXPORT IPianoKeyWidgetFactory {
    public:
        virtual ~IPianoKeyWidgetFactory();
        virtual IPianoKeyWidget *create(QWidget *parent) = 0;

        virtual QString name() const = 0;
    };

    class CORE_EXPORT IPianoKeyWidget : public QFrame {
        Q_OBJECT
    public:
        explicit IPianoKeyWidget(QWidget *parent = nullptr);
        ~IPianoKeyWidget();

        virtual int currentHeight() const = 0;
        virtual void setCurrentHeight(int height) = 0;
    };

}


#endif // CHORUSKIT_IPIANOKEYWIDGET_H
