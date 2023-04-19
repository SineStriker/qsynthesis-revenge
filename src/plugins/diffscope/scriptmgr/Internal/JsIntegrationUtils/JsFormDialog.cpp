//
// Created by Crs_1 on 2023/4/17.
//

#include "JsFormDialog.h"
#include "CTabButton.h"
#include "JsInternalObject.h"
#include "QMEqualBoxLayout.h"
#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QMetaMethod>
#include <QPlainTextEdit>
#include <QPushButton>

#define PARAM_OPTIONAL_IF(prop, type) if(JS_PROP_ASSERT(widgetParams, prop, type)) { auto prop##Prop = JS_PROP_AS(widgetParams, prop, type);
#define PARAM_OPTIONAL_ELSE } else {
#define PARAM_OPTIONAL_ENDIF }
#define PARAM_REQUIRED(prop, type) if(!JS_PROP_ASSERT(widgetParams, prop, type)) return false; auto prop##Prop = JS_PROP_AS(widgetParams, prop, type)

namespace ScriptMgr::Internal {

    JsFormDialog::JsFormDialog(QJSEngine *engine, QWidget *parent) : ConfigurableDialog(parent) {
        setApplyButtonVisible(false);
        ret = engine->newObject();
        ret.setProperty("form", engine->newArray());
        formLayout = new QFormLayout;
        auto dlgWidget = new QWidget;
        dlgWidget->setLayout(formLayout);
        setWidget(dlgWidget);
    }

    JsFormDialog::~JsFormDialog() noexcept {
    }

    bool JsFormDialog::addFormWidgets(const QVariantList &widgets) {
        for (const auto & widget : widgets) {
            if (!widget.canConvert(QVariant::Map)) {
                return false;
            }
            auto widgetParams = widget.toMap();
            PARAM_REQUIRED(type, String);
            bool successful;
            auto creatorMethodName = QString("create")+typeProp;
            qDebug() << creatorMethodName;
            if(!QMetaObject::invokeMethod(this, creatorMethodName.toLocal8Bit(), Q_RETURN_ARG(bool, successful), Q_ARG(QVariantMap, widgetParams))) {
                return false;
            }
            if(!successful) return false;
        }
        emit formWidgetsCreated();
        return true;
    }

    QJSValue JsFormDialog::jsExec() {
        auto res = exec();
        if (res == QDialog::Accepted) {
            ret.setProperty("result", "Ok");
        } else {
            ret.setProperty("result", "Cancel");
        }
        return ret;
    }
    bool JsFormDialog::createTextBox(const QVariantMap &widgetParams) {
        auto index = formWidgets.size();
        PARAM_REQUIRED(label, String);
        ret.property("form").setProperty(index, "");
        auto textBox = new QLineEdit;
        connect(textBox, &QLineEdit::textChanged, this, [=](const QString &text) {
            ret.property("form").setProperty(index, text);
        });

        PARAM_OPTIONAL_IF(defaultValue, String)
            textBox->setText(defaultValueProp);
        PARAM_OPTIONAL_ENDIF

        formLayout->addRow(labelProp, textBox);
        formWidgets.push_back(textBox);
        return true;
    }

    bool JsFormDialog::createTextArea(const QVariantMap &widgetParams) {
        auto index = formWidgets.size();
        PARAM_REQUIRED(label, String);
        ret.property("form").setProperty(index, "");
        auto textArea = new QPlainTextEdit;
        connect(textArea, &QPlainTextEdit::textChanged, this, [=]() {
            ret.property("form").setProperty(index, textArea->toPlainText());
        });

        PARAM_OPTIONAL_IF(defaultValue, String)
            textArea->setPlainText(defaultValueProp);
        PARAM_OPTIONAL_ENDIF

        formLayout->addRow(labelProp, textArea);
        formWidgets.push_back(textArea);
        return true;
    }

    bool JsFormDialog::createLabel(const QVariantMap &widgetParams) {
        auto index = formWidgets.size();
        PARAM_REQUIRED(label, String);
        ret.property("form").setProperty(index, QJSValue::UndefinedValue);
        auto label = new QLabel(labelProp);
        formLayout->addRow(label);
        formWidgets.push_back(label);
        return true;
    }

    bool JsFormDialog::createNumberBox(const QVariantMap &widgetParams) {
        auto index = formWidgets.size();
        PARAM_REQUIRED(label, String);
        ret.property("form").setProperty(index, 0);
        auto numberBox = new QDoubleSpinBox;
        connect(numberBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [=](double value){
            ret.property("form").setProperty(index, value);
        });

        PARAM_OPTIONAL_IF(precision, Int)
            numberBox->setDecimals(precisionProp);
        PARAM_OPTIONAL_ELSE
            numberBox->setDecimals(0);
        PARAM_OPTIONAL_ENDIF

        PARAM_OPTIONAL_IF(max, Double)
            numberBox->setMaximum(maxProp);
        PARAM_OPTIONAL_ELSE
            numberBox->setMaximum(std::numeric_limits<double>::max());
        PARAM_OPTIONAL_ENDIF

        PARAM_OPTIONAL_IF(min, Double)
            numberBox->setMinimum(minProp);
        PARAM_OPTIONAL_ELSE
            numberBox->setMinimum(std::numeric_limits<double>::lowest());
        PARAM_OPTIONAL_ENDIF

        PARAM_OPTIONAL_IF(step, Double)
            numberBox->setSingleStep(stepProp);
        PARAM_OPTIONAL_ENDIF

        PARAM_OPTIONAL_IF(prefix, String)
            numberBox->setPrefix(prefixProp);
        PARAM_OPTIONAL_ENDIF

        PARAM_OPTIONAL_IF(suffix, String)
            numberBox->setSuffix(suffixProp);
        PARAM_OPTIONAL_ENDIF

        PARAM_OPTIONAL_IF(defaultValue, Double)
            numberBox->setValue(defaultValueProp);
        PARAM_OPTIONAL_ENDIF

        formLayout->addRow(labelProp, numberBox);
        formWidgets.push_back(numberBox);
        return true;
    }

    bool JsFormDialog::createKeyNameBox(const QVariantMap &widgetParams) {
        return false;
    }

    bool JsFormDialog::createTimeBox(const QVariantMap &widgetParams) {
        return false;
    }

    bool JsFormDialog::createCheckBox(const QVariantMap &widgetParams) {
        auto index = formWidgets.size();
        PARAM_REQUIRED(label, String);
        ret.property("form").setProperty(index, false);
        auto checkBox = new QCheckBox;
        QList<int> triggerEnableIndices;
        QList<int> triggerDisableIndices;
        PARAM_OPTIONAL_IF(bindings, Map)
            if(JS_PROP_ASSERT(bindingsProp, enable, List)) {
                auto enableList = JS_PROP_AS(bindingsProp, enable, List);
                for(const auto& enableIndex: enableList) {
                    if(enableIndex.canConvert(QVariant::Int)) {
                        triggerEnableIndices.push_back(enableIndex.toInt());
                    }
                }
            }
            if(JS_PROP_ASSERT(bindingsProp, disable, List)) {
                auto disableList = JS_PROP_AS(bindingsProp, disable, List);
                for(const auto& disableIndex: disableList) {
                    if(disableIndex.canConvert(QVariant::Int)) {
                        triggerDisableIndices.push_back(disableIndex.toInt());
                    }
                }
            }
        PARAM_OPTIONAL_ENDIF
        connect(checkBox, &QCheckBox::clicked, this, [=](bool checked){
            ret.property("form").setProperty(index, checked);
            for(auto i: triggerEnableIndices) {
                if(i >= formWidgets.size()) continue;
                formWidgets[i]->setDisabled(!checked);
            }
            for(auto i: triggerDisableIndices) {
                if(i >= formWidgets.size()) continue;
                formWidgets[i]->setDisabled(checked);
            }
        });

        PARAM_OPTIONAL_IF(defaultValue, Bool)
            checkBox->setChecked(defaultValueProp);
            connect(this, &JsFormDialog::formWidgetsCreated, this, [=](){
                emit checkBox->clicked(defaultValueProp);
            });
        PARAM_OPTIONAL_ELSE
            connect(this, &JsFormDialog::formWidgetsCreated, this, [=](){
                emit checkBox->clicked();
            });
        PARAM_OPTIONAL_ENDIF

        checkBox->setText(labelProp);

        formLayout->addRow(checkBox);
        formWidgets.push_back(checkBox);
        return true;
    }

    bool JsFormDialog::createSelect(const QVariantMap &widgetParams) {
        auto index = formWidgets.size();
        PARAM_REQUIRED(label, String);
        ret.property("form").setProperty(index, 0);
        auto select = new QComboBox;
        PARAM_REQUIRED(options, StringList);
        select->addItems(optionsProp);
        QMap<int, QList<int>> triggerEnableIndices;
        QMap<int, QList<int>> triggerDisableIndices;
        PARAM_OPTIONAL_IF(bindings, List)
            for(auto bindingsEntry: bindingsProp) {
                if(!bindingsEntry.canConvert(QVariant::Map)) continue;
                auto bindingsEntryMap = bindingsEntry.toMap();
                if(!JS_PROP_ASSERT(bindingsEntryMap, index, Int)) continue;
                auto triggerBy = JS_PROP_AS(bindingsEntryMap, index, Int);
                triggerEnableIndices[triggerBy] = QList<int>();
                triggerDisableIndices[triggerBy] = QList<int>();
                if(JS_PROP_ASSERT(bindingsEntryMap, enable, List)) {
                    auto enableList = JS_PROP_AS(bindingsEntryMap, enable, List);
                    for(const auto& enableIndex: enableList) {
                        if(enableIndex.canConvert(QVariant::Int)) {
                            triggerEnableIndices[triggerBy].push_back(enableIndex.toInt());
                        }
                    }
                }
                if(JS_PROP_ASSERT(bindingsEntryMap, disable, List)) {
                    auto disableList = JS_PROP_AS(bindingsEntryMap, disable, List);
                    for(const auto& disableIndex: disableList) {
                        if(disableIndex.canConvert(QVariant::Int)) {
                            triggerDisableIndices[triggerBy].push_back(disableIndex.toInt());
                        }
                    }
                }
            }

        PARAM_OPTIONAL_ENDIF
        connect(select, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int currentIndex){
            ret.property("form").setProperty(index, currentIndex);
            if(triggerEnableIndices.contains(currentIndex)){
                for(auto i: triggerEnableIndices[currentIndex]) {
                    if(i >= formWidgets.size()) continue;
                    formWidgets[i]->setDisabled(false);
                }
            }
            if(triggerDisableIndices.contains(currentIndex)) {
                for (auto i : triggerDisableIndices[currentIndex]) {
                    if (i >= formWidgets.size())
                        continue;
                    formWidgets[i]->setDisabled(true);
                }
            }
        });

        PARAM_OPTIONAL_IF(defaultValue, Int)
            connect(this, &JsFormDialog::formWidgetsCreated, this, [=](){
                select->setCurrentIndex(defaultValueProp);
                if(defaultValueProp == 0) emit select->currentIndexChanged(0);
            });
        PARAM_OPTIONAL_ELSE
            connect(this, &JsFormDialog::formWidgetsCreated, this, [=](){
                emit select->currentIndexChanged(0);
            });
        PARAM_OPTIONAL_ENDIF

        formLayout->addRow(labelProp, select);
        formWidgets.push_back(select);
        return true;
    }

}