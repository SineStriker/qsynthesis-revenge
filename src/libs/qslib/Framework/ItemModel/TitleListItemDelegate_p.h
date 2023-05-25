#ifndef TITLELISTITEMDELEGATEPRIVATE_H
#define TITLELISTITEMDELEGATEPRIVATE_H

#include <QTypeList.h>

#include "TitleListItemDelegate.h"

namespace QsApi {

    class TitleListItemDelegatePrivate {
        Q_DECLARE_PUBLIC(TitleListItemDelegate)
    public:
        TitleListItemDelegatePrivate();
        virtual ~TitleListItemDelegatePrivate();

        void init();

        TitleListItemDelegate *q_ptr;

        QTypeFace m_backgroundType;
        QLineStyle m_underline;

        QTypeFace m_fileType;
        QTypeFace m_locType;
        QTypeFace m_dateType;
        QTypeFace m_dateHighlightType;
        QRectStyle m_dateBackType;

        QMargins m_fileMargins;
        QMargins m_locMargins;
        QMargins m_dateMargins;
        QMargins m_margins;

        QMargins m_iconMargins;

        [[nodiscard]] QTypeList styleData_helper() const;
        void setStyleData_helper(const QTypeList &list);

        template <class T>
        inline void decodeStyle(const QVariant &var, void (TitleListItemDelegate::*func)(const T &)) {
            Q_Q(TitleListItemDelegate);
            if (var.canConvert<T>()) {
                (q->*func)(var.value<T>());
            }
        }
    };

}

#endif // TITLELISTITEMDELEGATEPRIVATE_H