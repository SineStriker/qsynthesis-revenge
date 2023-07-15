#ifndef CMENUIMPL_H
#define CMENUIMPL_H

#include <QMenu>

#include <private/qmenu_p.h>

/*
 *
 * This file contains the implementation of some members of QMenuPrivate
 * which does not export symbols in the shared library (considered as a bug),
 * the overrided CMenu will call these members.
 *
 */

namespace CMenuImpl {

    bool appUseFullScreenForPopup();

}

#endif // CMENUIMPL_H
