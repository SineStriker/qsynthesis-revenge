#ifndef TITLELISTITEMDELEGATEPRIVATE_H
#define TITLELISTITEMDELEGATEPRIVATE_H

#include "TitleListItemDelegate.h"

namespace QsApi {

    class TitleListItemDelegatePrivate {
        Q_DECLARE_PUBLIC(TitleListItemDelegate)
    public:
        TitleListItemDelegatePrivate();
        virtual ~TitleListItemDelegatePrivate();

        void init();

        TitleListItemDelegate *q_ptr;

        QTypeFace m_idleType;
        QTypeFace m_selectType;
        QTypeFace m_underline;

        QTypeFace m_fileType;
        QTypeFace m_locType;
        QTypeFace m_dateType;
        QTypeFace m_dateBackType;

        QMargins m_fileMargins;
        QMargins m_locMargins;
        QMargins m_dateMargins;
        QMargins m_margins;

        QMargins m_iconMargins;
    };

}

#endif // TITLELISTITEMDELEGATEPRIVATE_H