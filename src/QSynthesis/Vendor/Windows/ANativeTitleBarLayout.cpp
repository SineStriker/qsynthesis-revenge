#include "ANativeTitleBarLayout.h"

ANativeTitleBarLayout::ANativeTitleBarLayout(QWidget *parent)
    : ANativeBoxLayout(LeftToRight, parent) {
    A_LAYOUT_INIT(0, 6);
}

ANativeTitleBarLayout::~ANativeTitleBarLayout() {
}
