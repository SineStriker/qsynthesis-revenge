#ifndef QSCROLLABLETABBARTAB_H
#define QSCROLLABLETABBARTAB_H

#include <QFrame>
#include <QPushButton>

class QScrollableTabBarTabPrivate;

class QScrollableTabBarTab : public QFrame {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QScrollableTabBarTab)

    Q_PROPERTY(QIcon icon READ icon WRITE setIcon NOTIFY styleChanged)
    Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize NOTIFY styleChanged)
    Q_PROPERTY(QMargins iconMargins READ iconMargins WRITE setIconMargins NOTIFY styleChanged)
    Q_PROPERTY(QMargins textMargins READ textMargins WRITE setTextMargins NOTIFY styleChanged)
public:
    explicit QScrollableTabBarTab(QWidget *parent = nullptr);
    ~QScrollableTabBarTab();

    friend class QScrollableTabWidget;

public:
    QIcon icon() const;
    void setIcon(const QIcon &icon);

    QSize iconSize() const;
    void setIconSize(const QSize &iconSize);

    QMargins iconMargins() const;
    void setIconMargins(const QMargins &iconMargins);

    QString text() const;
    void setText(const QString &text);

    QMargins textMargins() const;
    void setTextMargins(const QMargins &textPadding);

    QVariant data() const;
    void setData(const QVariant &data);

    bool selected() const;
    void setSelected(bool selected);

public:
    QAbstractButton *closeButton() const;
    void setCloseButton(QAbstractButton *button);

protected:
    QScrollableTabBarTab(QScrollableTabBarTabPrivate &d, QWidget *parent = nullptr);

    QScopedPointer<QScrollableTabBarTabPrivate> d_ptr;

    void paintEvent(QPaintEvent *event) override;

signals:
    void styleChanged();
};

#endif // QSCROLLABLETABBARTAB_H
