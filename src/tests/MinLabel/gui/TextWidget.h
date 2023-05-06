#ifndef TEXTWIDGET_H
#define TEXTWIDGET_H

#include <QAction>
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

    QLineEdit *wordsText;
    QPlainTextEdit *contentText;

protected:
    QPushButton *replaceButton;
    QPushButton *appendButton;
    QPushButton *pasteButton;

    QAction *replaceAction;

    QHBoxLayout *lineLayout;
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

    void _q_pasteButtonClicked();
    void _q_replaceButtonClicked();
    void _q_appendButtonClicked();

signals:
};

#endif // TEXTWIDGET_H
