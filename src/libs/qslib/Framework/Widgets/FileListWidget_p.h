#ifndef CHORUSKIT_FILELISTWIDGET_P_H
#define CHORUSKIT_FILELISTWIDGET_P_H

#include "FileListWidget.h"
#include "ItemModel/TitleListItemDelegate.h"

namespace QsApi {

    class FileListWidgetPrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(FileListWidget)
    public:
        FileListWidgetPrivate();
        ~FileListWidgetPrivate();

        void init();

        QTypeList styleData_helper() const;
        void setStyleData_helper(const QTypeList &list);

        FileListWidget *q_ptr;

        TitleListItemDelegate *m_delegate;
        QSize oldContentsSize;

        template <class T>
        inline void decodeStyle(const QVariant &var, void (TitleListItemDelegate::*func)(const T &)) {
            if (var.canConvert<T>()) {
                (m_delegate->*func)(var.value<T>());
            }
        }

    private:
        void _q_delegateClicked(const QModelIndex &index, int button);
    };

}

#endif // CHORUSKIT_FILELISTWIDGET_P_H
