#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H

#include <QLineEdit>
#include <QPlainTextEdit>
#include <QProcess>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class TextWidget : public QWidget {
    Q_OBJECT
public:
    explicit TextWidget(QWidget *parent = nullptr);
    ~TextWidget();

    void setText(const QString &text);
    QString text() const;

protected:
    QLineEdit *wordsText;
    QPlainTextEdit *contentText;

    QPushButton *replaceButton;
    QPushButton *appendButton;

    QHBoxLayout *buttonsLayout;
    QVBoxLayout *mainLayout;

    QProcess *pinyin;
    bool finished;

    enum ConvertAction {
        Replace,
        Append,
    };

    ConvertAction lastConvertAction;

private:
    void startTool();
    void terminateTool();

    void enterSentence();

    void handleProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void handleProcessError(QProcess::ProcessError error);
    void handleReadOutput();
    void handleReadError();

    void _q_replaceButtonClicked();
    void _q_appendButtonClicked();

signals:
};

#endif // TEXTWIDGET_H
