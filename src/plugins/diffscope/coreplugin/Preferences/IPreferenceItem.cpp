//
// Created by Crs_1 on 2023/4/6.
//
#include "IPreferenceItem.h"
#include "IOptionsPage.h"

using namespace Core;

IOptionsPage *IPreferenceItem::asPage() {
    return dynamic_cast<IOptionsPage *>(this);
}
