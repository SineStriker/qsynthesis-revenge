#ifndef BASETAB_H
#define BASETAB_H

#include <QIcon>
#include <QWidget>

class BaseTabPrivate;

class BaseTab : public QWidget {
    Q_OBJECT
    Q_DECLARE_PRIVATE(BaseTab)
    Q_PROPERTY(QIcon icon READ icon WRITE setIcon NOTIFY iconChanged)
public:
    explicit BaseTab(QWidget *parent = nullptr);
    ~BaseTab();

    virtual void enter();
    virtual void leave();

    QString title() const; // Get the title of tab

    QIcon icon() const;
    void setIcon(const QIcon &icon);

    bool active() const;

protected:
    virtual void setTabName(const QString &value); // Set the title of tab
    virtual void updateTabName();

protected:
    BaseTab(BaseTabPrivate &d, QWidget *parent = nullptr);

    QScopedPointer<BaseTabPrivate> d_ptr;

signals:
    void titleChanged(const QString &name);
    void iconChanged(const QIcon &icon);
};

#endif // BASETAB_H
