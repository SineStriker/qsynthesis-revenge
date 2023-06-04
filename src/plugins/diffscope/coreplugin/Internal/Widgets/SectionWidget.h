#ifndef CHORUSKIT_SECTIONWIDGET_H
#define CHORUSKIT_SECTIONWIDGET_H

#include <QFrame>

namespace Core::Internal {

    class SectionWidget : public QFrame {
        Q_OBJECT
    public:
        explicit SectionWidget(QWidget *parent = nullptr);
        ~SectionWidget();
    };

}


#endif // CHORUSKIT_SECTIONWIDGET_H
