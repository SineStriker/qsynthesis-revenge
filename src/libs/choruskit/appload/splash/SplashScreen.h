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
        QPair<int, int> anchor;
        int fontSize;
        QColor fontColor;
        int maxWidth;
        QString text;
    };

public:
    void setTextAttribute(const QString &id, const Attribute &attr);
    void setText(const QString &id, const QString &text);
    void showTexts();

protected:
    void drawContents(QPainter *painter) override;

private:
    bool m_showTexts;
    QHash<QString, Attribute> m_texts;
};



#endif // SPLASHSCREEN_H
