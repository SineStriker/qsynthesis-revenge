#pragma once

#ifdef _WIN32
#  define MY_DECL_EXPORT __declspec(dllexport)
#  define MY_DECL_IMPORT __declspec(dllimport)
#else
#  define MY_DECL_EXPORT
#  define MY_DECL_IMPORT
#endif

#define MY_EXTERN_C extern "C"
#define MY_EXTERN_C_EXPORT MY_EXTERN_C MY_DECL_EXPORT
#define MY_EXTERN_C_IMPORT MY_EXTERN_C MY_DECL_IMPORT
#define MY_EXTERN_C_BEGIN MY_EXTERN_C {
#define MY_EXTERN_C_END }

#define _TO_UNICODE(y) L##y
#define TO_UNICODE(x) _TO_UNICODE(x)
