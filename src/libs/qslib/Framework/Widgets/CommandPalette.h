#ifndef COMMANDPALETTE_H
#define COMMANDPALETTE_H

#include <QListWidget>
#include <QVBoxLayout>

#include <QPixelSize.h>

#include "QsFrameworkGlobal.h"
#include "Widgets/BaseContainer.h"

namespace QsApi {

    class CommandPalettePrivate;

    class QSFRAMEWORK_API CommandPalette : public BaseContainer {
        Q_OBJECT
        Q_DECLARE_PRIVATE(CommandPalette)
        Q_LAYOUT_PROPERTY_DELCARE
    public:
        explicit CommandPalette(QWidget *parent = nullptr);
        ~CommandPalette();

        inline void addItem(QListWidgetItem *item);
        void insertItem(int index, QListWidgetItem *item);
        int count() const;
        void clear();
        QListWidgetItem *itemAt(int index) const;

        QListWidgetItem *currentItem() const;
        void setCurrentItem(QListWidgetItem *item);

        int currentRow() const;
        void setCurrentRow(int row);

        QString filterHint() const;
        void setFilterHint(const QString &hint);

        QString filterKeyword() const;
        void setFilterKeyword(const QString &keyword);

    public slots:
        void start();
        void activate(int index);
        void abandon();

    signals:
        void activated(int index);
        void finished(QListWidgetItem *item);
        void tabPressed();

        void currentRowChanged(int role);
        void currentItemChanged(QListWidgetItem *item);

        void filterKeywordChanged(const QString &text);

        void styleDataChanged();

    protected:
        bool event(QEvent *event) override;

    protected:
        CommandPalette(CommandPalettePrivate &d, QWidget *parent = nullptr);

        QScopedPointer<CommandPalettePrivate> d_ptr;
    };

    inline void CommandPalette::addItem(QListWidgetItem *item) {
        insertItem(count(), item);
    }

}

#endif // COMMANDPALETTE_H