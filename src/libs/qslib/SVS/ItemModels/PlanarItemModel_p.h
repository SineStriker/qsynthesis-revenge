#ifndef PLANARITEMMODELPRIVATE_H
#define PLANARITEMMODELPRIVATE_H

#include "PlanarItemModel.h"

namespace QsApi {

    class QSSVS_API PlanarItemModelPrivate {
        Q_DECLARE_PUBLIC(PlanarItemModel)
    public:
        PlanarItemModelPrivate();
        virtual ~PlanarItemModelPrivate();

        void init();

        PlanarItemModel *q_ptr;
    };

}

#endif // PLANARITEMMODELPRIVATE_H