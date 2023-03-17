#ifndef ICORE_P_H
#define ICORE_P_H

namespace Core {

    class ICorePrivate {
    public:
        ICorePrivate();
        void init();

        ICore *q_ptr;

        ActionSystem *actionSystem;
        WindowSystem *windowSystem;
    };

}

#endif // ICORE_P_H
