#pragma once

#ifdef _WIN32
#  define MY_DECL_EXPORT __declspec(dllexport)
#  define MY_DECL_IMPORT __declspec(dllimport)
#else
#  define MY_DECL_EXPORT
#  define MY_DECL_IMPORT
#endif

#define _TO_UNICODE(y) L##y
#define TO_UNICODE(x) _TO_UNICODE(x)
