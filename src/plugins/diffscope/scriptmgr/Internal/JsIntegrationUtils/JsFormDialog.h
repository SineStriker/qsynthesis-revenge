#ifndef JSFORMDIALOG_H
#define JSFORMDIALOG_H

#include <QFormLayout>
#include <QJSValue>

#include "CConfigurableDialog.h"

namespace ScriptMgr::Internal {

    class JsFormDialog: public CConfigurableDialog {
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

#endif // JSFORMDIALOG_H
