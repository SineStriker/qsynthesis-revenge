#ifndef ANATIVETITLEBAR_H
#define ANATIVETITLEBAR_H

#include <QAbstractButton>
#include <QLabel>
#include <QMenuBar>
#include <QWidget>

class ANativeTitleBarPrivate;

class Q_DECL_EXPORT ANativeTitleBar : public QFrame {
    Q_OBJECT
    Q_DECLARE_PRIVATE(ANativeTitleBar)
public:
    explicit ANativeTitleBar(QWidget *parent = nullptr);
    virtual ~ANativeTitleBar();

public:
    QMenuBar *menuBar() const;
    QLabel *titleLabel() const;
    QAbstractButton *iconButton() const;
    QAbstractButton *minButton() const;
    QAbstractButton *maxButton() const;
    QAbstractButton *closeButton() const;

    void setMenuBar(QMenuBar *bar);
    void setTitleLabel(QLabel *label);
    void setIconButton(QAbstractButton *btn);
    void setMinButton(QAbstractButton *btn);
    void setMaxButton(QAbstractButton *btn);
    void setCloseButton(QAbstractButton *btn);

    QMenuBar *takeMenuBar();
    QLabel *takeTitleLabel();
    QAbstractButton *takeIconButton();
    QAbstractButton *takeMinButton();
    QAbstractButton *takeMaxButton();
    QAbstractButton *takeCloseButton();

    QWidget *widget() const;
    void setWidget(QWidget *w);

    bool titleFollowWindow() const;
    void setTitleFollowWindow(bool value);

    bool iconFollowWindow() const;
    void setIconFollowWindow(bool value);

protected:
    ANativeTitleBar(ANativeTitleBarPrivate &d, QWidget *parent);

protected:
    QScopedPointer<ANativeTitleBarPrivate> d_ptr;

    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void q_minButtonClicked();
    void q_maxButtonClicked();
    void q_closeButtonClicked();

signals:
    void eleInstalled(QWidget *widget);
    void eleRemoved(QWidget *widget);
};

#endif // ANATIVETITLEBAR_H
