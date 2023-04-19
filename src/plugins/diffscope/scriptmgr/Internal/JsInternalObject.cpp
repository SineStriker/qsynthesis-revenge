//
// Created by Crs_1 on 2023/4/16.
//

#include "JsInternalObject.h"
#include "AddOn/ScriptMgrAddOn.h"
#include "CoreApi/IWindow.h"
#include "JsIntegrationUtils/JsFormDialog.h"
#include "QMCoreDecoratorV2.h"
#include <QLineEdit>
#include <QMessageBox>

namespace ScriptMgr::Internal {

    JsInternalObject::JsInternalObject(QJSEngine *engine, ScriptMgrAddOn *addOn) : engine(engine), addOn(addOn) {
    }

    JsInternalObject::~JsInternalObject() {
    }

    QString JsInternalObject::jsTr(const QString &text) {
        return tr(text.toLocal8Bit());
    }

    QString JsInternalObject::getLang() {
        return qIDec->locale();
    }

    void JsInternalObject::infoMsgBox(const QString &title, const QString &message) {
        QMessageBox::information(addOn->windowHandle()->window(), title, message);
    }

    bool JsInternalObject::questionMsgBox(const QString &title, const QString &message, const QString &defaultButton) {
        auto defaultButtonFlag = QMessageBox::Yes;
        if (defaultButton == "No")
            defaultButtonFlag = QMessageBox::No;
        return QMessageBox::question(addOn->windowHandle()->window(), title, message,
                                     QMessageBox::Yes | QMessageBox::No, defaultButtonFlag) == QMessageBox::Yes;
    }

    QJSValue JsInternalObject::form(const QString &title, const QVariantList &widgets) {
        auto dlg = new JsFormDialog(engine, addOn->windowHandle()->window());
        dlg->setWindowTitle(title);
        if(!dlg->addFormWidgets(widgets)) {
            engine->throwError(QJSValue::TypeError, "Invalid widget parameters.");
            return QJSValue::UndefinedValue;
        }
        return dlg->jsExec();
    }

    void JsInternalObject::registerScript(const QString &id, int flags) {
        addOn->registerScript(id, flags);
    }

    void JsInternalObject::registerScriptSet(const QString &id, const QStringList &childrenId, int flags) {
        addOn->registerScript(id, childrenId, flags);
    }

}