//
// Created by Functioner on 2023/5/31.
//

#ifndef CHORUSKIT_IPIANOKEYWIDGET_H
#define CHORUSKIT_IPIANOKEYWIDGET_H

#include <QFrame>

namespace Core {

    class IPianoKeyWidget;

    class IPianoKeyWidgetFactory {
    public:
        virtual ~IPianoKeyWidgetFactory() = default;
        virtual IPianoKeyWidget *create(QWidget *parent) = 0;

        virtual QString name() const = 0;
    };

    class IPianoKeyWidget : public QFrame {
        Q_OBJECT
    public:
        explicit IPianoKeyWidget(QWidget *parent = nullptr) : QFrame(parent) {
        }

        virtual int currentHeight() const = 0;
        virtual void setCurrentHeight(int height) = 0;
    };

}


#endif // CHORUSKIT_IPIANOKEYWIDGET_H
