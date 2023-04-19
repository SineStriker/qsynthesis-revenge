//
// Created by Crs_1 on 2023/4/17.
//

#ifndef CHORUSKIT_JSFORMDIALOG_H
#define CHORUSKIT_JSFORMDIALOG_H

#include "Dialogs/ConfigurableDialog.h"
#include <QFormLayout>
#include <QJSValue>

namespace ScriptMgr::Internal {

    class JsFormDialog: public QsApi::ConfigurableDialog {
        Q_OBJECT
    public:
        explicit JsFormDialog(QJSEngine *engine, QWidget *parent = nullptr);
        ~JsFormDialog();
        bool addFormWidgets(const QVariantList &widgets);
        QJSValue jsExec();
    signals:
        void formWidgetsCreated();
    protected:
        QFormLayout *formLayout = nullptr;
        QList<QWidget *> formWidgets;
        QJSValue ret;
        Q_INVOKABLE bool createTextBox(const QVariantMap &widgetParams);
        Q_INVOKABLE bool createTextArea(const QVariantMap &widgetParams);
        Q_INVOKABLE bool createLabel(const QVariantMap &widgetParams);
        Q_INVOKABLE bool createNumberBox(const QVariantMap &widgetParams);
        Q_INVOKABLE bool createKeyNameBox(const QVariantMap &widgetParams);
        Q_INVOKABLE bool createTimeBox(const QVariantMap &widgetParams);
        Q_INVOKABLE bool createCheckBox(const QVariantMap &widgetParams);
        Q_INVOKABLE bool createSelect(const QVariantMap &currentIndex);
    };

} // Internal

#endif // CHORUSKIT_JSFORMDIALOG_H
