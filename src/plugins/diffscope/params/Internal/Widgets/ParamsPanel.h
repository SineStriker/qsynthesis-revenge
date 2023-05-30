#ifndef CHORUSKIT_PARAMSPANEL_H
#define CHORUSKIT_PARAMSPANEL_H

#include <QFrame>

namespace Params::Internal {

    class ParamsPanel : public QFrame {
        Q_OBJECT
    public:
        explicit ParamsPanel(QWidget *parent = nullptr);
        ~ParamsPanel();
    };

}

#endif // CHORUSKIT_PARAMSPANEL_H
