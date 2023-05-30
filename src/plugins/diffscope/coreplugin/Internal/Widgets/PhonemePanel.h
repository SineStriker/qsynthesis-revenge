#ifndef CHORUSKIT_PHONEMEPANEL_H
#define CHORUSKIT_PHONEMEPANEL_H

#include <QFrame>

namespace Core::Internal {

    class PhonemePanel : public QFrame {
        Q_OBJECT
    public:
        explicit PhonemePanel(QWidget *parent = nullptr);
        ~PhonemePanel();
    };

}



#endif // CHORUSKIT_PHONEMEPANEL_H
