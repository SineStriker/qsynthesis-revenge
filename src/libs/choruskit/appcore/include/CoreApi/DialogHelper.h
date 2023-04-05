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

        void closeDialog(const QString &id, int result);

        bool getDialogOpenState(const QString &id);
    protected:
        QScopedPointer<DialogHelperPrivate> d_ptr;
        Q_DECLARE_PRIVATE(DialogHelper)
        explicit DialogHelper(DialogHelperPrivate *d, QObject *parent = nullptr);
        QMap<QString, IDialogPage *> &dialogPages();
        void setDialogOpenState(const QString &id, QDialog* dialog, bool state);
    private:
        QDialog *prepareDialog(IDialogPage *dialogPage, QWidget* parent, bool modal, int controls);
        void finalizeDialog(QDialog *dialog);
    };
}

#endif // CHORUSKIT_DIALOGHELPER_H