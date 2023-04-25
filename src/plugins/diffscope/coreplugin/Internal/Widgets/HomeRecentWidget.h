#ifndef HOMERECENTWIDGET_H
#define HOMERECENTWIDGET_H

#include <QButtonGroup>
#include <QFrame>
#include <QHash>
#include <QLabel>
#include <QLineEdit>
#include <QSplitter>
#include <QVBoxLayout>

#include "CTabButton.h"
#include "Layout/QMEqualBoxLayout.h"
#include "QPixelSize.h"
#include "Widgets/FileListWidget.h"
#include "Widgets/LinearScrollArea.h"

#include "Interfaces/IButtonBar.h"

namespace Core {

    class HomeRecentTopButtonBar;

    /**
     * @brief Recent widget top frame
     */
    class HomeRecentTopFrame : public QFrame {
        Q_OBJECT
        Q_LAYOUT_PROPERTY_DELCARE
    public:
        explicit HomeRecentTopFrame(QWidget *parent = nullptr);
        ~HomeRecentTopFrame();

    public:
        void reloadStrings();

        Q_INVOKABLE QAbstractButton *addButton(const QString &id);
        Q_INVOKABLE void removeButton(const QString &id);

        IButtonBar *buttonBar() const;

    signals:
        void newRequested();
        void openRequested();
        void textChanged(const QString &text);

    private:
        CTabButton *newButton;
        CTabButton *openButton;
        QLineEdit *searchBox;

        QMEqualBoxLayout *topLayout;
        QHash<QString, QAbstractButton *> externButtons;

        // Interfaces
        HomeRecentTopButtonBar *m_buttonBar;
    };

    /**
     * @brief Recent widget bottom frame
     */

    class HomeRecentBottomFrame : public QFrame {
        Q_OBJECT
        Q_LAYOUT_PROPERTY_DELCARE
        Q_PROPERTY(QIcon fileIcon READ fileIcon WRITE setFileIcon NOTIFY iconChanged)
        Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize NOTIFY iconChanged)
    public:
        explicit HomeRecentBottomFrame(QWidget *parent = nullptr);
        ~HomeRecentBottomFrame();

    public:
        void reloadStrings();
        void reloadRecentFiles();

        void setFilterKeyword(const QString &keyword);

        // Style data
    public:
        QIcon fileIcon() const;
        void setFileIcon(const QIcon &icon);

        QSize iconSize() const;
        void setIconSize(const QSize &iconSize);

    signals:
        void iconChanged();
        void openFileRequested(const QString &fileName);

    private:
        QVBoxLayout *bottomLayout;
        QsApi::FileListWidget *fileWidget;
        QLabel *emptyLabel;

        QIcon m_fileIcon;
        QSize m_iconSize;

        QString m_keyword;

    private:
        void updateListFilter();
        void updateEmptyLabel();

    private:
        void _q_recentFilesChanged();
        void _q_itemClickedEx(const QModelIndex &index, int button);
    };

    /**
     * @brief Recent widget
     */
    class HomeRecentWidget : public QSplitter {
        Q_OBJECT
    public:
        explicit HomeRecentWidget(QWidget *parent = nullptr);
        ~HomeRecentWidget();

        void reloadStrings();

        HomeRecentTopFrame *topWidget;
        HomeRecentBottomFrame *bottomWidget;

    private:
        void _q_searchTextChanged(const QString &text);
    };

} // Core

#endif // HOMERECENTWIDGET_H
