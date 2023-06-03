#ifndef CHORUSKIT_PIANOKEYCONTAINER_H
#define CHORUSKIT_PIANOKEYCONTAINER_H

#include <Widgets/NoneScrollArea.h>

#include "PianoRoll/IPianoKeyWidget.h"

namespace Core::Internal {

    class PianoKeyContainer : public QsApi::NoneScrollArea {
        Q_OBJECT
    public:
        explicit PianoKeyContainer(QWidget *parent = nullptr);
        ~PianoKeyContainer();

    public:
        Core::IPianoKeyWidget *area() const;
        void setArea(Core::IPianoKeyWidget *area);

        QSize sizeHint() const override;
        QSize minimumSizeHint() const override;

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;

        Core::IPianoKeyWidget *m_area;

    signals:
    };

}



#endif // CHORUSKIT_PIANOKEYCONTAINER_H
