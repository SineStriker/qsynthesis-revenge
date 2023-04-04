//
// Created by Crs_1 on 2023/4/3.
//

#ifndef CHORUSKIT_DIALOGHELPER_H
#define CHORUSKIT_DIALOGHELPER_H

#include "IDialogPage.h"
#include "ObjectPool.h"
#include <QDialog>
#include <QMap>

namespace Core {
    class DialogHelperPrivate;

    class CKAPPCORE_API DialogHelper: public QObject {
        Q_OBJECT
    public:
        explicit DialogHelper(QObject *parent = nullptr);
        ~DialogHelper();

        enum Control {
            OkButton = 1,
            CloseButton = 2,
            CancelButton = 4,
            ApplyButton = 8,
        };

        enum Result {
            NotOpened = -1,
            Rejected = 0,
            Accepted = 1,
        };

        void addDialogPage(IDialogPage *dialogPage);
        void removeDialogPage(const QString &id);
        IDialogPage *getDialogPage(const QString &id);

        int showDialog(const QString &id, QWidget* parent, int controls = OkButton | CloseButton);

        void showModelessDialog(const QString &id, QWidget* parent, int controls = OkButton | CloseButton);
    protected:
        QScopedPointer<DialogHelperPrivate> d_ptr;
        Q_DECLARE_PRIVATE(DialogHelper)
        explicit DialogHelper(DialogHelperPrivate *d, QObject *parent = nullptr);
        QMap<QString, IDialogPage *> &dialogs();
        void setDialogOpenState(IDialogPage *dialogPage, bool state);
        bool getDialogOpenState(IDialogPage *dialogPage);

    private:
        QDialog *prepareDialog(IDialogPage *dialogPage, QWidget* parent, bool modal, int controls);
    };
}

#endif // CHORUSKIT_DIALOGHELPER_H