#ifndef HXC_PLATFORM_MATRIX_LIBRARY_H_INCLUDED
#define HXC_PLATFORM_MATRIX_LIBRARY_H_INCLUDED

#if defined(_WIN32) && defined(HXC_PLATFORM_MATRIX_BUILD_DLL)
#define HXC_PLATFORM_MATRIX_API __declspec(dllexport)
#elif defined(_WIN32) && defined(HXC_PLATFORM_MATRIX_USE_DLL)
#define HXC_PLATFORM_MATRIX_API __declspec(dllimport)
#else
#define HXC_PLATFORM_MATRIX_API
#endif

#if defined(__cplusplus)
extern "C" {
#endif

HXC_PLATFORM_MATRIX_API int hxc_platform_matrix_value(void);

#if defined(__cplusplus)
}
#endif

#endif
