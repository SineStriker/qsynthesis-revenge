#ifndef COMMANDPALETTEPRIVATE_H
#define COMMANDPALETTEPRIVATE_H

#include <QLineEdit>
#include <QVBoxLayout>

#include "CommandPalette.h"
#include "TitleListWidget.h"

namespace QsApi {

    class CommandPalettePrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(CommandPalette)
    public:
        CommandPalettePrivate();
        virtual ~CommandPalettePrivate();

        void init();

        CommandPalette *q_ptr;

        QVBoxLayout *m_layout;
        QLineEdit *m_lineEdit;
        TitleListWidget *m_listWidget;

        bool noClickOutsideEventToHandle;
        bool paletteActive;

        void showPalette();
        void hidePalette();

        void adjustPalette();
        void clearPalette();

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;

    private:
        void _q_textChanged(const QString &text);
        void _q_currentRowChanged(int row);
        void _q_currentItemChanged(QListWidgetItem *cur, QListWidgetItem *prev);
        void _q_delegateClicked(const QModelIndex &index, int button);
    };

}

#endif // COMMANDPALETTEPRIVATE_H