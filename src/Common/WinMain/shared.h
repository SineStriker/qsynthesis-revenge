#pragma once

#ifdef _WIN32
#  define DECL_EXPORT __declspec(dllexport)
#  define DECL_IMPORT __declspec(dllimport)
#else
#  define DECL_EXPORT
#  define DECL_IMPORT
#endif