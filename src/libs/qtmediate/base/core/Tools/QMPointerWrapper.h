#ifndef CHORUSKIT_QMPOINTERWRAPPER_H
#define CHORUSKIT_QMPOINTERWRAPPER_H

#include <type_traits>

template <class T>
class QMPointerWrapper {
public:
    QMPointerWrapper() : ref(nullptr){};

    template <class T1>
    QMPointerWrapper(T1 **ref) : ref(reinterpret_cast<T **>(ref)) {
        static_assert(std::is_base_of<T, T1>::value, "T1 should inherit from T");
    }

    T *&get() const {
        return *ref;
    }

    T **pointer() const {
        return ref;
    }

private:
    T **ref;
};

#endif // CHORUSKIT_QMPOINTERWRAPPER_H
