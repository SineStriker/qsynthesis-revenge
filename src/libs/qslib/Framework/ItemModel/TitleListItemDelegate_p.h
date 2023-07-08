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

        QRectStyle m_backgroundType;
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
        QMargins m_padding;

        QMargins m_iconMargins;
        QSize m_defaultIconSize;

        QTypeMap styleData_helper() const;
        void setStyleData_helper(const QTypeMap &map);
    };

}

#endif // TITLELISTITEMDELEGATEPRIVATE_H