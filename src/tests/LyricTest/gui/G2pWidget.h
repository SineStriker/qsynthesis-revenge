#ifndef CHORUSKIT_G2PWIDEGET_H
#define CHORUSKIT_G2PWIDEGET_H

#include <QButtonGroup>
#include <QCheckBox>
#include <QDebug>
#include <QPushButton>
#include <QRadioButton>
#include <QTableWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

class G2pWidget : public QWidget {
    Q_OBJECT
public:
    explicit G2pWidget(QWidget *parent = nullptr);
    ~G2pWidget();

protected:
    QTextEdit *m_textEditTop;
    QTextEdit *m_textEditBottom;
    QPushButton *m_buttonConvert;
    QPushButton *m_buttonReorder;
    QRadioButton *m_radioBtnOption1;
    QRadioButton *m_radioBtnOption2;
    QButtonGroup *m_buttonGroup;
    QCheckBox *m_checkBox;
private:
    void convertLyric();
    void reorderLyric();
};



#endif // CHORUSKIT_G2PWIDEGET_H
