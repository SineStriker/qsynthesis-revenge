#ifdef _WIN32

#include <windows.h>
#include "asiolist.h"

static char tmp[64];

void *my_memcpy(char *&dst, void *src, size_t siz) {
    if(siz == 28) {
        auto len = strlen((char*)src);
        if(len > 63) len = 63;
        auto ret = memcpy(dst, src, len);
        ((char*)dst)[len] = 0;
        dst = tmp;
        return ret;
    } else {
        return memcpy(dst, src, siz);
    }
}

void *my_memcpy(CLSID *dst, void *src, size_t siz) {
    return memcpy(dst, src, siz);
}

#define memcpy my_memcpy

#include "asiolist.cpp"

#endif