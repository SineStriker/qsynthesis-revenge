#ifndef CHORUSKIT_PROJECTWIDGET_H
#define CHORUSKIT_PROJECTWIDGET_H

#include <QFrame>
#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>

#include <CDockFrame.h>

namespace Core::Internal {

    class ProjectWidget : public QFrame {
        Q_OBJECT
    public:
        explicit ProjectWidget(QWidget *parent = nullptr);
        ~ProjectWidget();

        QToolBar *mainToolbar() const {
            return m_toolbar;
        }

        CDockFrame *mainDock() const {
            return m_frame;
        }

    private:
        QToolBar *m_toolbar;
        CDockFrame *m_frame;

        QVBoxLayout *m_layout;
    };

}



#endif // CHORUSKIT_PROJECTWIDGET_H
