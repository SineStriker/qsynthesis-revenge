#pragma once

#ifdef _WINDOWS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API
#endif

extern "C" DLL_API int main_entry(int argc, char *argv[]);
