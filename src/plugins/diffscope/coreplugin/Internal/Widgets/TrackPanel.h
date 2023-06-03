#ifndef CHORUSKIT_TRACKPANEL_H
#define CHORUSKIT_TRACKPANEL_H

#include <QFrame>

namespace Core::Internal {

    class TrackPanel : public QFrame {
        Q_OBJECT
    public:
        explicit TrackPanel(QWidget *parent = nullptr);
        ~TrackPanel();
    };

}



#endif // CHORUSKIT_TRACKPANEL_H
