#ifndef ISETTINGPAGEPRIVATE_H
#define ISETTINGPAGEPRIVATE_H

#include "ISettingPage.h"

namespace Core {

    class ISettingPagePrivate {
        Q_DECLARE_PUBLIC(ISettingPage)
    public:
        ISettingPagePrivate();
        virtual ~ISettingPagePrivate();

        void init();

        ISettingPage *q_ptr;

        QString id;
        QString title;
    };

}

#endif // ISETTINGPAGEPRIVATE_H