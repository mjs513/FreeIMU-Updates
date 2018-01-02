/* STLport configuration file
 * It is internal STLport header - DO NOT include it directly
 * Microsoft Visual C++ 6.0, 7.0, 7.1, 8.0, ICL
 */

#if !defined (_STLP_COMPILER)
#  define _STLP_COMPILER "Microsoft Visual Studio C++"
#endif

#if !defined (__ICL) && !defined (_STLP_MSVC)
#  define _STLP_MSVC _MSC_VER
#endif

#if !defined (_STLP_MSVC_LIB)
#  define _STLP_MSVC_LIB _MSC_VER
#endif

#if defined (__BUILDING_STLPORT) && defined (_MANAGED)
/* Building a managed version of STLport is not supported because we haven't
 * found a good reason to support it. However, building a managed translation
 * unit using STLport _is_ supported.
 */
#  error Sorry but building a managed version of STLport is not supported.
#endif

#if defined (_STLP_USING_PLATFORM_SDK_COMPILER)
/* This is a specific section for compilers coming with platform SDKs. Native
 * library coming with it is different from the one coming with commercial
 * MSVC compilers so there is some specific settings.
 */
#  define _STLP_NATIVE_INCLUDE_PATH ../crt
#  define _STLP_VENDOR_GLOBAL_CSTD
#  define _STLP_VENDOR_TERMINATE_STD
#  define _STLP_GLOBAL_NEW_HANDLER
#  if (_STLP_MSVC_LIB <= 1400)
/* We hope this bug will be fixed in future versions. */
#    define _STLP_NEW_DONT_THROW_BAD_ALLOC 1
#  endif
#endif

#define _STLP_CALL __cdecl

#ifndef _STLP_LONG_LONG
#  define _STLP_LONG_LONG __int64
#endif

#define _STLP_PRAGMA_ONCE

/* These switches depend on compiler flags. We are hoping here that compilers
 * simulating MSVC behavior use identical macros to report compilation context.
 * Otherwise those macros will have to be undef in specific compiler configuration
 * files.
 */
#ifndef _CPPUNWIND
#  define _STLP_DONT_USE_EXCEPTIONS 1
#endif

#ifndef _CPPRTTI
#  define _STLP_NO_RTTI 1
#endif

#if defined (_MT) && !defined (_STLP_NO_THREADS)
#  define _STLP_THREADS 1
#endif

#if !defined (_NATIVE_WCHAR_T_DEFINED)
#  define _STLP_WCHAR_T_IS_USHORT 1
#endif

#define _STLP_NO_VENDOR_STDLIB_L 1

#if defined (_STLP_MSVC)

#  if (_STLP_MSVC <= 1300)
#    error Microsoft Visual C++ compilers before version 8 are not supported.
#  endif

#  define _STLP_NORETURN_FUNCTION __declspec(noreturn)

/* Full compiler version comes from boost library intrinsics.hpp header. */
#  if defined (_MSC_FULL_VER) && (_MSC_FULL_VER >= 140050215)
#    define _STLP_HAS_TRIVIAL_CONSTRUCTOR(T) __has_trivial_constructor(T)
#    define _STLP_HAS_TRIVIAL_COPY(T) __has_trivial_copy(T)
#    define _STLP_HAS_TRIVIAL_ASSIGN(T) __has_trivial_assign(T)
#    define _STLP_HAS_TRIVIAL_DESTRUCTOR(T) __has_trivial_destructor(T)
#    define _STLP_IS_POD(T) __is_pod(T)
#    define _STLP_HAS_TYPE_TRAITS_INTRINSICS
#  endif

#  define _STLP_DLLEXPORT_NEEDS_PREDECLARATION 1
#  define _STLP_HAS_SPECIFIC_PROLOG_EPILOG 1
#  define _STLP_NO_STATIC_CONST_DEFINITION 1

/* # ifndef __BUILDING_STLPORT
 * #  define _STLP_USE_TEMPLATE_EXPORT 1
 * # endif
 */

/** Note: the macro _STLP_NO_UNCAUGHT_EXCEPT_SUPPORT is defined
unconditionally and undef'ed here when applicable. */
#  if defined (UNDER_CE)
/* eVCx:
uncaught_exception is declared in the SDKs delivered with eVC4 (eVC3 is
unknown) and they all reside in namespace 'std' there. However, they are not
part of any lib so linking fails. When using VC8 to crosscompile for CE 5 on
an ARMV4I, the uncaught_exception test fails, the function returns the wrong
value. */
/* All eVCs up to at least VC8/CE5 have a broken new operator that
   returns null instead of throwing bad_alloc. */
#    define _STLP_NEW_DONT_THROW_BAD_ALLOC 1
#  else
/* VCx:
These are present at least since VC6, but the uncaught_exception() of VC6 is
broken, it returns the wrong value in the unittests. 7.1 and later seem to
work, 7.0 is still unknown (we assume it works until negative report). */
#    if (_STLP_MSVC >= 1300)// VC7 and later
#      undef _STLP_NO_UNCAUGHT_EXCEPT_SUPPORT
#    endif
#  endif

#  define _STLP_HAS_NATIVE_FLOAT_ABS 1

/* Boris : not defining this macro for SP5 causes other problems */
/*#    if !defined (_MSC_FULL_VER) || (_MSC_FULL_VER < 12008804 ) */
#    define _STLP_NO_USING_FOR_GLOBAL_FUNCTIONS 1
/*#    endif */
#    define _STLP_DONT_USE_BOOL_TYPEDEF 1
#    define _STLP_DONT_RETURN_VOID 1
#  endif

#endif /* _STLP_MSVC */

/* The desktop variants starting with VC8 have a set of more secure replacements
 * for the error-prone string handling functions of the C standard lib. */
/* When user do not consider 'unsafe' string functions as deprecated using _CRT_SECURE_NO_DEPRECATE
 * macro we use 'unsafe' functions for performance reasons. */
#if (_STLP_MSVC_LIB >= 1400) && !defined (_STLP_USING_PLATFORM_SDK_COMPILER) && !defined (UNDER_CE) && \
    !defined (_CRT_SECURE_NO_DEPRECATE)
#  define _STLP_USE_SAFE_STRING_FUNCTIONS 1
#endif

#if (_STLP_MSVC_LIB <= 1310)
#  define _STLP_VENDOR_GLOBAL_CSTD
#endif

#if (_STLP_MSVC_LIB >= 1300) && !defined(UNDER_CE)
/* Starting with MSVC 7.0 and compilers simulating it,
 * we assume that the new SDK is granted:
 */
#  define _STLP_NEW_PLATFORM_SDK 1
#endif

#define _STLP_EXPORT_DECLSPEC __declspec(dllexport)
#define _STLP_IMPORT_DECLSPEC __declspec(dllimport)
#define _STLP_CLASS_EXPORT_DECLSPEC __declspec(dllexport)
#define _STLP_CLASS_IMPORT_DECLSPEC __declspec(dllimport)

#if defined (__DLL) || defined (_DLL) || defined (_RTLDLL) || defined (_AFXDLL)
#  define _STLP_RUNTIME_DLL
#endif
#if defined (__BUILDING_STLPORT) && \
   (defined (_STLP_USE_DYNAMIC_LIB) || \
    defined (_STLP_RUNTIME_DLL) && !defined (_STLP_USE_STATIC_LIB))
#  define _STLP_DLL
#endif
#include <stl/config/_detect_dll_or_lib.h>
#undef _STLP_RUNTIME_DLL
#undef _STLP_DLL

#if defined (_STLP_USE_DYNAMIC_LIB)
#  undef  _STLP_USE_DECLSPEC
#  define _STLP_USE_DECLSPEC 1
#endif

#if !defined (_STLP_IMPORT_TEMPLATE_KEYWORD)
#  if !defined (_MSC_EXTENSIONS) || defined (_STLP_MSVC) && (_STLP_MSVC >= 1300)
#    define _STLP_IMPORT_TEMPLATE_KEYWORD
#  else
#    define _STLP_IMPORT_TEMPLATE_KEYWORD extern
#  endif
#endif
#define _STLP_EXPORT_TEMPLATE_KEYWORD

#include <stl/config/_auto_link.h>

#if defined (_STLP_USING_PLATFORM_SDK_COMPILER)
/* The Windows 64 bits SDK required for the moment link to bufferoverflowU.lib for
 * additional buffer overrun checks. Rather than require the STLport build system and
 * users to explicitely link with it we use the MSVC auto link feature.
 */
#  if !defined (_STLP_DONT_USE_AUTO_LINK) || defined (__BUILDING_STLPORT)
#    pragma comment (lib, "bufferoverflowU.lib")
#    if defined (_STLP_VERBOSE)
#      pragma message ("STLport: Auto linking to bufferoverflowU.lib")
#    endif
#  endif
#endif

#if defined (_STLP_MSVC)
#  include <stl/config/_feedback.h>
#endif
