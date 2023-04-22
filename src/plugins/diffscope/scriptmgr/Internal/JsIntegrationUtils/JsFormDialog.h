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
        JsFormDialog(QJSEngine *engine, QJSValue listener, QWidget *parent = nullptr);
        ~JsFormDialog();
        bool addFormWidgets(const QVariantList &widgets);
        QJSValue jsExec();
    signals:
        void formWidgetsCreated();
        void formValueChanged();
    protected:
        friend class JsFormHandle;
        QJSEngine *engine;
        QFormLayout *formLayout = nullptr;
        QList<QWidget *> formWidgets;
        QJSValue ret;
        QJSValue listener;
        Q_INVOKABLE bool createTextBox(const QVariantMap &widgetParams);
        Q_INVOKABLE bool createTextArea(const QVariantMap &widgetParams);
        Q_INVOKABLE bool createLabel(const QVariantMap &widgetParams);
        Q_INVOKABLE bool createNumberBox(const QVariantMap &widgetParams);
        Q_INVOKABLE bool createKeyNameBox(const QVariantMap &widgetParams);
        Q_INVOKABLE bool createTimeBox(const QVariantMap &widgetParams);
        Q_INVOKABLE bool createCheckBox(const QVariantMap &widgetParams);
        Q_INVOKABLE bool createSelect(const QVariantMap &currentIndex);
    };

    class JsFormHandle: public QObject {
        Q_OBJECT
    public:
        explicit JsFormHandle(JsFormDialog *dlg);
    public slots:
        QJSValue value(int index);
        bool disabled(int index);
        void setDisabled(int index, bool disabled);
        bool visible(int index);
        void setVisible(int index, bool visible);
        QString label(int index);
        void setLabel(int index, const QString &label);
    protected:
        JsFormDialog *dlg;
    };

} // Internal

#endif // CHORUSKIT_JSFORMDIALOG_H
