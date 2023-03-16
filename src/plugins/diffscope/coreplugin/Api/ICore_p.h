#ifndef ICORE_P_H
#define ICORE_P_H

namespace Core {

    class ICorePrivate {
    public:
        ICorePrivate();
        void init();

        ICore *q_ptr;

        ActionSystem *actionSystem;
    };

}

#endif // ICORE_P_H
