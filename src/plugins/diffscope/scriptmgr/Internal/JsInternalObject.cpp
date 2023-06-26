#include "JsInternalObject.h"

#include <QLineEdit>
#include <QMessageBox>

#include "AddOn/ScriptMgrAddOn.h"
#include "QMCoreDecoratorV2.h"
#include "CoreApi/IWindow.h"

#include "JsIntegrationUtils/JsFormDialog.h"
#include "JsUtils.h"

namespace ScriptMgr::Internal {

    JsInternalObject::JsInternalObject(ScriptMgrAddOn *addOn): addOn(addOn), QObject(addOn) {
    }

    JsInternalObject::~JsInternalObject() {
    }

    QString JsInternalObject::msgBox(const QString &title, const QString &message, const QString &icon,
                                 const QStringList &buttons, const QString &defaultButton) const {
        return jsMsgBox(addOn->windowHandle()->window(), title, message, icon, buttons, defaultButton);
    }

    QJSValue JsInternalObject::form(const QString &title, const QVariantList &widgets, QJSValue listener) const {
        JsFormDialog dlg(ScriptLoader::instance()->engine(), listener, addOn->windowHandle()->window());
        dlg.setWindowTitle(title);
        if(!dlg.addFormWidgets(widgets)) {
            ScriptLoader::instance()->engine()->throwError(QJSValue::TypeError, "Invalid widget parameters.");
            return QJSValue::UndefinedValue;
        }
        return dlg.jsExec();
    }

}