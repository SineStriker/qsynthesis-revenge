#ifndef CHORUSKIT_PROJECTWIDGET_H
#define CHORUSKIT_PROJECTWIDGET_H

#include <QWidget>

namespace Core::Internal {

    class ProjectWidget : public QWidget {
        Q_OBJECT
    public:
        explicit ProjectWidget(QWidget *parent = nullptr);
        ~ProjectWidget();
    };

}



#endif // CHORUSKIT_PROJECTWIDGET_H
