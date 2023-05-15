#ifndef CHORUSKIT_CONFIGWIDGET_H
#define CHORUSKIT_CONFIGWIDGET_H

#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSlider>
#include <QWidget>

namespace TemplatePlg {
    class ConfigWidget : public QWidget {
        Q_OBJECT
    public:
        ConfigWidget(QWidget *parent = nullptr);

    private:
        QRadioButton *m_radio1;
        QRadioButton *m_radio2;
        QLabel *m_checkBox_label;
        QCheckBox *m_checkBox;
        QLabel *m_comboBox_label;
        QComboBox *m_comboBox;
        QLabel *m_slider_label;
        QSlider *m_slider;
        QLabel *m_lineEdit_label;
        QLineEdit *m_lineEdit;
    private slots:
        void save();
    };

}

#endif // CHORUSKIT_CONFIGWIDGET_H
