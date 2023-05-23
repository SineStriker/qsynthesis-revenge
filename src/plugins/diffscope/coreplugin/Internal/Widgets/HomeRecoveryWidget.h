#ifndef CHORUSKIT_HOMERECOVERYWIDGET_H
#define CHORUSKIT_HOMERECOVERYWIDGET_H

#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>

#include <QMWidgetsMacros.h>
#include <QPixelSize.h>

namespace Core::Internal {

    // StyleSheet prefix: Core--Internal--

    class HomeRecoveryWidget : public QFrame {
        Q_OBJECT
        Q_LAYOUT_PROPERTY_DELCARE
    public:
        explicit HomeRecoveryWidget(QWidget *parent = nullptr);
        ~HomeRecoveryWidget();

        void reloadStrings();

    private:
        QListWidget *listWidget;
        QVBoxLayout *layout;
        QLabel *emptyLabel;
    };


}



#endif // CHORUSKIT_HOMERECOVERYWIDGET_H
