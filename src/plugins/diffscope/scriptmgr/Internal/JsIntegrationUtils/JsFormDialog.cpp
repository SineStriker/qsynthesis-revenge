#include "JsFormDialog.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QMetaMethod>
#include <QPlainTextEdit>
#include <QPushButton>

#include "QMEqualBoxLayout.h"

#include "JsInternalObject.h"
#include "JsMacros.h"

namespace ScriptMgr::Internal {

    JsFormDialog::JsFormDialog(QJSEngine *engine, QJSValue listener, QWidget *parent)
        : CConfigurableDialog(parent), engine(engine), listener(listener) {
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
        for (const auto &widget : widgets) {
            if (!widget.canConvert(QVariant::Map)) {
                return false;
            }
            auto widgetParams = widget.toMap();
            JS_PROP_REQUIRED(widgetParams, type, String);
            bool successful;
            auto creatorMethodName = QString("create") + typeProp;
            if (!QMetaObject::invokeMethod(this, creatorMethodName.toLocal8Bit(), Q_RETURN_ARG(bool, successful),
                                           Q_ARG(QVariantMap, widgetParams))) {
                return false;
            }
            if (!successful)
                return false;
        }
        emit formWidgetsCreated();
        return true;
    }

    JsFormHandle::JsFormHandle(JsFormDialog *dlg) : dlg(dlg) {
    }
    QJSValue JsFormHandle::value(int index) {
        return dlg->ret.property("form").property(index);
    }
    bool JsFormHandle::disabled(int index) {
        return !dlg->formWidgets[index]->isEnabled();
    }
    void JsFormHandle::setDisabled(int index, bool disabled) {
        dlg->formWidgets[index]->setDisabled(disabled);
    }
    bool JsFormHandle::visible(int index) {
        return dlg->formLayout->itemAt(index)->widget()->isVisible();
    }
    void JsFormHandle::setVisible(int index, bool visible) {
        QLayoutItem *item;
        item = dlg->formLayout->itemAt(index, QFormLayout::LabelRole);
        if (item)
            item->widget()->setVisible(visible);
        item = dlg->formLayout->itemAt(index, QFormLayout::SpanningRole);
        if (item)
            item->widget()->setVisible(visible);
        item = dlg->formLayout->itemAt(index, QFormLayout::FieldRole);
        if (item)
            item->widget()->setVisible(visible);
    }
    QString JsFormHandle::label(int index) {
        QLabel *widget;
        widget = qobject_cast<QLabel *>(dlg->formLayout->itemAt(index)->widget());
        if (widget) {
            return widget->text();
        } else {
            return qobject_cast<QCheckBox *>(dlg->formLayout->itemAt(index, QFormLayout::FieldRole)->widget())->text();
        }
    }
    void JsFormHandle::setLabel(int index, const QString &label) {
        QLabel *widget;
        widget = qobject_cast<QLabel *>(dlg->formLayout->itemAt(index)->widget());
        if (widget) {
            widget->setText(label);
        } else {
            qobject_cast<QCheckBox *>(dlg->formLayout->itemAt(index, QFormLayout::FieldRole)->widget())->setText(label);
        }
    }

    QJSValue JsFormDialog::jsExec() {
        if (listener.isCallable()) {
            connect(this, &JsFormDialog::formValueChanged, this,
                    [=]() { listener.call({engine->newQObject(new JsFormHandle(this))}); });
            emit formValueChanged();
        }
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
        JS_PROP_REQUIRED(widgetParams, label, String);
        ret.property("form").setProperty(index, "");
        auto textBox = new QLineEdit;
        connect(textBox, &QLineEdit::textChanged, this, [=](const QString &text) {
            ret.property("form").setProperty(index, text);
            emit formValueChanged();
        });

        JS_PROP_OPTIONAL_IF(widgetParams, defaultValue, String)
        textBox->setText(defaultValueProp);
        JS_PROP_OPTIONAL_ENDIF

        formLayout->addRow(labelProp, textBox);
        formWidgets.push_back(textBox);
        return true;
    }

    bool JsFormDialog::createTextArea(const QVariantMap &widgetParams) {
        auto index = formWidgets.size();
        JS_PROP_REQUIRED(widgetParams, label, String);
        ret.property("form").setProperty(index, "");
        auto textArea = new QPlainTextEdit;
        connect(textArea, &QPlainTextEdit::textChanged, this, [=]() {
            ret.property("form").setProperty(index, textArea->toPlainText());
            emit formValueChanged();
        });

        JS_PROP_OPTIONAL_IF(widgetParams, defaultValue, String)
        textArea->setPlainText(defaultValueProp);
        JS_PROP_OPTIONAL_ENDIF

        formLayout->addRow(labelProp, textArea);
        formWidgets.push_back(textArea);
        return true;
    }

    bool JsFormDialog::createLabel(const QVariantMap &widgetParams) {
        auto index = formWidgets.size();
        JS_PROP_REQUIRED(widgetParams, label, String);
        ret.property("form").setProperty(index, QJSValue::UndefinedValue);
        auto label = new QLabel(labelProp);
        formLayout->addRow(label);
        formWidgets.push_back(label);
        return true;
    }

    bool JsFormDialog::createNumberBox(const QVariantMap &widgetParams) {
        auto index = formWidgets.size();
        JS_PROP_REQUIRED(widgetParams, label, String);
        ret.property("form").setProperty(index, 0);
        auto numberBox = new QDoubleSpinBox;
        connect(numberBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [=](double value) {
            ret.property("form").setProperty(index, value);
            emit formValueChanged();
        });

        JS_PROP_OPTIONAL_IF(widgetParams, precision, Int)
        numberBox->setDecimals(precisionProp);
        JS_PROP_OPTIONAL_ELSE
        numberBox->setDecimals(0);
        JS_PROP_OPTIONAL_ENDIF

        JS_PROP_OPTIONAL_IF(widgetParams, max, Double)
        numberBox->setMaximum(maxProp);
        JS_PROP_OPTIONAL_ELSE
        numberBox->setMaximum(std::numeric_limits<double>::max());
        JS_PROP_OPTIONAL_ENDIF

        JS_PROP_OPTIONAL_IF(widgetParams, min, Double)
        numberBox->setMinimum(minProp);
        JS_PROP_OPTIONAL_ELSE
        numberBox->setMinimum(std::numeric_limits<double>::lowest());
        JS_PROP_OPTIONAL_ENDIF

        JS_PROP_OPTIONAL_IF(widgetParams, step, Double)
        numberBox->setSingleStep(stepProp);
        JS_PROP_OPTIONAL_ENDIF

        JS_PROP_OPTIONAL_IF(widgetParams, prefix, String)
        numberBox->setPrefix(prefixProp);
        JS_PROP_OPTIONAL_ENDIF

        JS_PROP_OPTIONAL_IF(widgetParams, suffix, String)
        numberBox->setSuffix(suffixProp);
        JS_PROP_OPTIONAL_ENDIF

        JS_PROP_OPTIONAL_IF(widgetParams, defaultValue, Double)
        numberBox->setValue(defaultValueProp);
        JS_PROP_OPTIONAL_ENDIF

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
        JS_PROP_REQUIRED(widgetParams, label, String);
        ret.property("form").setProperty(index, false);
        auto checkBox = new QCheckBox;
        connect(checkBox, &QCheckBox::clicked, this, [=](bool checked) {
            ret.property("form").setProperty(index, checked);
            emit formValueChanged();
        });

        JS_PROP_OPTIONAL_IF(widgetParams, defaultValue, Bool)
        checkBox->setChecked(defaultValueProp);
        connect(this, &JsFormDialog::formWidgetsCreated, this, [=]() { emit checkBox->clicked(defaultValueProp); });
        JS_PROP_OPTIONAL_ELSE
        connect(this, &JsFormDialog::formWidgetsCreated, this, [=]() { emit checkBox->clicked(); });
        JS_PROP_OPTIONAL_ENDIF

        checkBox->setText(labelProp);

        formLayout->addRow(checkBox);
        formWidgets.push_back(checkBox);
        return true;
    }

    bool JsFormDialog::createSelect(const QVariantMap &widgetParams) {
        auto index = formWidgets.size();
        JS_PROP_REQUIRED(widgetParams, label, String);
        ret.property("form").setProperty(index, 0);
        auto select = new QComboBox;
        JS_PROP_REQUIRED(widgetParams, options, StringList);
        select->addItems(optionsProp);
        connect(select, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int currentIndex) {
            ret.property("form").setProperty(index, currentIndex);
            emit formValueChanged();
        });

        JS_PROP_OPTIONAL_IF(widgetParams, defaultValue, Int)
        connect(this, &JsFormDialog::formWidgetsCreated, this, [=]() {
            select->setCurrentIndex(defaultValueProp);
            if (defaultValueProp == 0)
                emit select->currentIndexChanged(0);
        });
        JS_PROP_OPTIONAL_ELSE
        connect(this, &JsFormDialog::formWidgetsCreated, this, [=]() { emit select->currentIndexChanged(0); });
        JS_PROP_OPTIONAL_ENDIF

        formLayout->addRow(labelProp, select);
        formWidgets.push_back(select);
        return true;
    }

}