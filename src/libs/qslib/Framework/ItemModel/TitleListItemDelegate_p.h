#ifndef TITLELISTITEMDELEGATEPRIVATE_H
#define TITLELISTITEMDELEGATEPRIVATE_H

#include <QFontInfoEx.h>
#include <QPenInfo.h>
#include <QRectInfo.h>

#include "TitleListItemDelegate.h"

namespace QsApi {

    class TitleListItemDelegatePrivate {
        Q_DECLARE_PUBLIC(TitleListItemDelegate)
    public:
        TitleListItemDelegatePrivate();
        virtual ~TitleListItemDelegatePrivate();

        void init();

        TitleListItemDelegate *q_ptr;

        QRectInfo m_backgroundType;
        QPenInfo m_underline;

        QFontInfoEx m_fileType;
        QFontInfoEx m_locType;
        QFontInfoEx m_dateType;
        QFontInfoEx m_dateHighlightType;
        QRectInfo m_dateBackType;

        QMargins m_fileMargins;
        QMargins m_locMargins;
        QMargins m_dateMargins;
        QMargins m_margins;
        QMargins m_padding;

        QMargins m_iconMargins;
        QSize m_defaultIconSize;

        QCssValueMap styleData_helper() const;
        void setStyleData_helper(const QCssValueMap &map);
    };

}

#endif // TITLELISTITEMDELEGATEPRIVATE_H