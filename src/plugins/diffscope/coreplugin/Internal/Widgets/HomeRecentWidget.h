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
#include "Widgets/LinearScrollArea.h"
#include "Widgets/TitleListWidget.h"

#include "Interfaces/IButtonBar.h"

namespace Core::Internal {

    // StyleSheet prefix: Core--Internal--

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
    public:
        explicit HomeRecentBottomFrame(QWidget *parent = nullptr);
        ~HomeRecentBottomFrame();

    public:
        void reloadStrings();
        void reloadRecentFiles();

        void setFilterKeyword(const QString &keyword);

    signals:
        void openFileRequested(const QString &fileName);

    private:
        QVBoxLayout *bottomLayout;
        QsApi::TitleListWidget *fileWidget;
        QLabel *emptyLabel;

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
