#ifndef CHORUSKIT_SettingsDialog_H
#define CHORUSKIT_SettingsDialog_H

#include <QDialog>

namespace Core {

    namespace Internal {

        class SettingsDialog : public QDialog {
            Q_OBJECT
        public:
            explicit SettingsDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
            ~SettingsDialog();

        public:
            void showPage(const QString &id);

        };

    }

}



#endif // CHORUSKIT_SettingsDialog_H
