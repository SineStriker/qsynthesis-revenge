#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include "Controls/Windows/PlainWindow.h"
#include "Windows/Basic/ProjectCommonBlock.h"

#include "QTypeList.h"

class HomeWindowPrivate;

class HomeWindow : public PlainWindow, public ProjectCommonBlock {
    Q_OBJECT
    Q_DECLARE_PRIVATE(HomeWindow)

    Q_PROPERTY(QTypeList templateStyleData READ templateStyleData //
                   WRITE setTemplateStyleData NOTIFY styleDataChanged)
public:
    HomeWindow(QWidget *parent = nullptr);
    ~HomeWindow();

public:
    void reloadStrings();

public:
    QTypeList templateStyleData() const;
    void setTemplateStyleData(const QTypeList &list);

    QTypeList recentStyleData() const;
    void setRecentStyleData(const QTypeList &list);

protected:
    HomeWindow(HomeWindowPrivate &d, QWidget *parent = nullptr);

private:
    void _q_openButtonClicked();
    void _q_searchBoxChanged(const QString &text);
    void _q_templateItemClicked(const QModelIndex &index, int button);

    void _q_confirmCreate();
    void _q_cancelCreate();

signals:
    void styleDataChanged();
};

#endif // HOMEWINDOW_H
