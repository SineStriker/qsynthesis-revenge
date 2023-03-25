#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QSplashScreen>

class SplashScreen : public QSplashScreen {
    Q_OBJECT
public:
    explicit SplashScreen(QScreen *screen = nullptr);
    ~SplashScreen();

    struct Attribute {
        QPoint pos;
        QPair<int, int> anchor{1, 1};
        int fontSize;
        QColor fontColor;
        int maxWidth;
        QString text;

        Attribute() : fontSize(15), maxWidth(0) {
        }
    };

public:
    void setTextAttribute(const QString &id, const Attribute &attr);
    void setText(const QString &id, const QString &text);
    void showTexts();

signals:
    void closed();

protected:
    void drawContents(QPainter *painter) override;

    void mousePressEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    bool m_showTexts;
    QHash<QString, Attribute> m_texts;

    void _q_messageChanged(const QString &message);
};



#endif // SPLASHSCREEN_H
