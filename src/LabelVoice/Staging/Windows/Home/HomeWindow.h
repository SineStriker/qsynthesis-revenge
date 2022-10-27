#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include "Controls/Windows/PlainWindow.h"

#include "QTypeList.h"

class HomeWindowPrivate;

class HomeWindow : public PlainWindow {
    Q_OBJECT
    Q_DECLARE_PRIVATE(HomeWindow)

    Q_PROPERTY(QTypeList templateStyleData READ templateStyleData WRITE setTemplateStyleData NOTIFY
                   styleDataChanged)
public:
    HomeWindow(QWidget *parent = nullptr);
    ~HomeWindow();

public:
    void reloadStrings();

public:
    QTypeList templateStyleData() const;
    void setTemplateStyleData(const QTypeList &list);

protected:
    HomeWindow(HomeWindowPrivate &d, QWidget *parent = nullptr);

private:
    void _q_searchBoxChanged(const QString &text);

signals:
    void styleDataChanged();
};

#endif // HOMEWINDOW_H
