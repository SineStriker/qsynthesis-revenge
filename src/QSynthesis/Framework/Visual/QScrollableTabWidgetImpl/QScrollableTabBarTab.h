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
public:
    explicit QScrollableTabBarTab(QWidget *parent = nullptr);
    ~QScrollableTabBarTab();

public:
    QIcon icon() const;
    void setIcon(const QIcon &icon);

    QSize iconSize() const;
    void setIconSize(const QSize &iconSize);

    QString text() const;
    void setText(const QString &text);

    QVariant data() const;
    void setData(const QVariant &data);

    bool selected() const;
    void setSelected(bool selected);

public:
    QAbstractButton *closeButton() const;

protected:
    QScrollableTabBarTab(QScrollableTabBarTabPrivate &d, QWidget *parent = nullptr);

    QScopedPointer<QScrollableTabBarTabPrivate> d_ptr;

signals:
    void styleChanged();
};

#endif // QSCROLLABLETABBARTAB_H
