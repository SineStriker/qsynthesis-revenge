//
// Created by Crs_1 on 2023/4/16.
//

#include "JsInternalObject.h"
#include "CoreApi/IWindow.h"
#include "JsIntegrationUtils/JsFormDialog.h"
#include "QMCoreDecoratorV2.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>

namespace ScriptMgr::Internal {

    JsInternalObject::JsInternalObject(QJSEngine *engine)
        : engine(engine) {
    }

    QString JsInternalObject::jsTr(const QString &text) {
        return tr(text.toLocal8Bit());
    }

    QString JsInternalObject::getLang() {
        return qIDec->locale();
    }

    void JsInternalObject::infoMsgBox(const QString &title, const QString &message) {
        if(!addOn) {
            engine->throwError(QJSValue::ReferenceError, "AddOn not initialized.");
            return;
        }
        QMessageBox::information(addOn->windowHandle()->window(), title, message);
    }

    bool JsInternalObject::questionMsgBox(const QString &title, const QString &message, const QString &defaultButton) {
        if(!addOn) {
            engine->throwError(QJSValue::ReferenceError, "AddOn not initialized.");
            return false;
        }
        auto defaultButtonFlag = QMessageBox::Yes;
        if(defaultButton == "No") defaultButtonFlag = QMessageBox::No;
        return QMessageBox::question(addOn->windowHandle()->window(), title, message, QMessageBox::Yes|QMessageBox::No, defaultButtonFlag) == QMessageBox::Yes;
    }

    bool JsInternalObject::_createFormWidget(QFormLayout &formLayout, const QVariantMap& widgetParams, QJSValue& ret, int index) {
        if(!widgetParams.contains("type") || !widgetParams.value("type").canConvert(QVariant::String)) return false;
        if(!widgetParams.contains("label") || !widgetParams.value("label").canConvert(QVariant::String)) return false;
        auto widgetType = widgetParams.value("type").toString();
        if(widgetType == "TextBox") {
            auto textBox = new QLineEdit;
            connect(textBox, &QLineEdit::textChanged, this, [=, &ret](const QString &text) {
                ret.property("form").setProperty(index, text);
            });
            if(widgetParams.contains("defaultValue") && widgetParams.value("defaultValue").canConvert(QVariant::String)) {
                textBox->setText(widgetParams.value("defaultValue").toString());
            }
            formLayout.addRow(widgetParams.value("label").toString(), textBox);
        } else {
            return false;
        }
        return true;
    }

    QJSValue JsInternalObject::form(const QString &title, const QVariantList &widgets) {
        auto ret = engine->newObject();
        ret.setProperty("form", engine->newArray());
        auto formLayout = new QFormLayout;
        auto dlg = new JsFormDialog;
        dlg->setWindowTitle(title);
        for(int i = 0; i < widgets.size(); i++) {
            const auto& widget = widgets[i];
            if(!widget.canConvert(QVariant::Map)) {
                engine->throwError(QJSValue::TypeError, "Invalid widget argument type");
                return ret;
            }
            if(!_createFormWidget(*formLayout, widget.toMap(), ret, i)){
                engine->throwError(QJSValue::TypeError, "Invalid widget argument type");
                return ret;
            }
        }
        auto dlgWidget = new QWidget;
        dlgWidget->setLayout(formLayout);
        dlg->setWidget(dlgWidget);
        auto res = dlg->exec();
        if(res == QDialog::Accepted) {
            ret.setProperty("result", "Ok");
        } else {
            ret.setProperty("result", "Cancel");
        }
        return ret;
    }

    void JsInternalObject::registerScript(const QString &id, int flags) {
    }

    void JsInternalObject::registerScriptSet(const QString &id, const QStringList &childrenId, int flags) {
    }

    void JsInternalObject::setAddOn(ScriptMgrAddOn *addOn) {
        this->addOn = addOn;
    }

}