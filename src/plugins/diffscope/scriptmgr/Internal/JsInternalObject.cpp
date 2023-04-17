//
// Created by Crs_1 on 2023/4/16.
//

#include "JsInternalObject.h"
#include "JsInternalObject_p.h"
#include "QMCoreDecoratorV2.h"
#include <QMessageBox>
#include "CoreApi/IWindow.h"

namespace ScriptMgr::Internal {

    JsInternalObject::JsInternalObject(QJSEngine *engine)
        : JsInternalObject(new JsInternalObjectPrivate(engine)) {
    }

    QString JsInternalObject::jsTr(const QString &text) {
        return tr(text.toLocal8Bit());
    }

    QString JsInternalObject::getLang() {
        return qIDec->locale();
    }

    void JsInternalObject::infoMsgBox(const QString &title, const QString &message) {
        Q_D(JsInternalObject);
        if(!d->addOn) {
            d->engine->throwError(QJSValue::ReferenceError, "AddOn not initialized.");
            return;
        }
        QMessageBox::information(d->addOn->windowHandle()->window(), title, message);
    }

    bool JsInternalObject::questionMsgBox(const QString &title, const QString &message, const QString &defaultButton) {
        Q_D(JsInternalObject);
        if(!d->addOn) {
            d->engine->throwError(QJSValue::ReferenceError, "AddOn not initialized.");
            return false;
        }
        auto defaultButtonFlag = QMessageBox::Yes;
        if(defaultButton == "No") defaultButtonFlag = QMessageBox::No;
        return QMessageBox::question(d->addOn->windowHandle()->window(), title, message, QMessageBox::Yes|QMessageBox::No, defaultButtonFlag) == QMessageBox::Yes;
    }

    QJSValue JsInternalObject::form(const QJSValue &val) {
        Q_D(JsInternalObject);
        return d->engine->newObject();
    }

    void JsInternalObject::registerScript(const QString &id, int flags) {
    }

    void JsInternalObject::registerScriptSet(const QString &id, const QStringList &childrenId, int flags) {
    }

    JsInternalObject::JsInternalObject(JsInternalObjectPrivate *d) : d_ptr(d) {
        d->q_ptr = this;
    }
    void JsInternalObject::setAddOn(ScriptMgrAddOn *addOn) {
        Q_D(JsInternalObject);
        d->addOn = addOn;
    }

    JsInternalObjectPrivate::JsInternalObjectPrivate(QJSEngine *engine)
        : engine(engine) {
    }

}