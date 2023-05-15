#include "ConfigWidget.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>
namespace TemplatePlg {
    ConfigWidget::ConfigWidget(QWidget *parent) : QWidget(parent) {
        // 创建单选按钮组
        QGroupBox *groupBox = new QGroupBox(tr("Radio Buttons"));
        groupBox->setMaximumHeight(100);
        m_radio1 = new QRadioButton(tr("Option 1"));
        m_radio2 = new QRadioButton(tr("Option 2"));
        m_radio1->setChecked(true);

        QHBoxLayout *radioBox = new QHBoxLayout;
        radioBox->addWidget(m_radio1);
        radioBox->addWidget(m_radio2);
        groupBox->setLayout(radioBox);

        QHBoxLayout *checkHBox = new QHBoxLayout;
        m_checkBox_label = new QLabel(tr("QCheckbox"));
        m_checkBox = new QCheckBox(tr("Checkbox"));
        checkHBox->addWidget(m_checkBox_label);
        checkHBox->addWidget(m_checkBox);

        QHBoxLayout *comboHBox = new QHBoxLayout;
        m_comboBox_label = new QLabel(tr("QComboBox"));
        m_comboBox = new QComboBox;
        m_comboBox->addItem(tr("Option 1"));
        m_comboBox->addItem(tr("Option 2"));
        comboHBox->addWidget(m_comboBox_label);
        comboHBox->addWidget(m_comboBox);

        QHBoxLayout *sliderHBox = new QHBoxLayout;
        m_slider_label = new QLabel(tr("QSlider"));
        m_slider = new QSlider(Qt::Horizontal);
        m_slider->setRange(0, 100);
        m_slider->setValue(50);
        sliderHBox->addWidget(m_slider_label);
        sliderHBox->addWidget(m_slider);

        QHBoxLayout *lineHBox = new QHBoxLayout;
        m_lineEdit_label = new QLabel(tr("QLineEdit"));
        m_lineEdit = new QLineEdit(tr("blue"));
        lineHBox->addWidget(m_lineEdit_label);
        lineHBox->addWidget(m_lineEdit);

        QHBoxLayout *saveHBox = new QHBoxLayout;
        QPushButton *saveButton = new QPushButton(tr("Save"));
        saveButton->setMaximumHeight(40);
        saveButton->setMaximumWidth(100);

        saveHBox->addStretch();
        saveHBox->addWidget(saveButton);

        connect(saveButton, &QPushButton::clicked, this, &ConfigWidget::save);

        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(groupBox);
        layout->addLayout(checkHBox);
        layout->addLayout(comboHBox);
        layout->addLayout(sliderHBox);
        layout->addLayout(lineHBox);
        layout->addLayout(saveHBox);
        setLayout(layout);
    }

    void ConfigWidget::save() {
        // 获取控件状态并输出到qdebug()
        qDebug() << "Radio button 1 checked? " << m_radio1->isChecked();
        qDebug() << "Checkbox checked? " << m_checkBox->isChecked();
        qDebug() << "ComboBox current index: " << m_comboBox->currentIndex();
        qDebug() << "Slider value: " << m_slider->value();
        qDebug() << "LineEdit text: " << m_lineEdit->text();
    }
}