//
// Created by Crs_1 on 2023/4/5.
//

#ifndef CHORUSKIT_IPREFERENCECATEGORY_H
#define CHORUSKIT_IPREFERENCECATEGORY_H
#include "IPreferenceItem.h"
#include <QObject>
namespace Core {
    class IPreferenceCategory: public IPreferenceItem {
    protected:
        NodeType nodeType() override { return Category; }
    };
}
#endif // CHORUSKIT_IPREFERENCECATEGORY_H
