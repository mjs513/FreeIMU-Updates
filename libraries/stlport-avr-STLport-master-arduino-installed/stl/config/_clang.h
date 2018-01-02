/* STLport configuration file
 * It is internal STLport header - DO NOT include it directly
 */

#define _STLP_COMPILER "CLang"

#define _STLP_HAS_INCLUDE_NEXT 1

/* Systems having GLIBC installed have different traits */
#if defined (__linux__)
#  ifndef _STLP_USE_GLIBC
#    define _STLP_USE_GLIBC 1
#  endif
#  if defined (__UCLIBC__) && !defined (_STLP_USE_UCLIBC)
#    define _STLP_USE_UCLIBC 1
#  endif
#endif

#if defined (__CYGWIN__) && \
     (__GNUC__ >= 3) && (__GNUC_MINOR__ >= 3) && !defined (_GLIBCPP_USE_C99)
#  define _STLP_NO_VENDOR_MATH_L
#  define _STLP_NO_VENDOR_STDLIB_L
#endif

#if (__GNUC__ < 3)
#  define _STLP_NO_VENDOR_STDLIB_L
#endif

#if (__GNUC__ < 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ < 4))
/* define for gcc versions before 3.4.0. */
#  define _STLP_NO_MEMBER_TEMPLATE_KEYWORD /* support removed */
#endif

#if !defined (_REENTRANT) && (defined (_THREAD_SAFE) || \
                             (defined (__OpenBSD__) && defined (_POSIX_THREADS)) || \
                             (defined (__MINGW32__) && defined (_MT)))
#  define _REENTRANT
#endif

#if defined (__DJGPP)
#  define _STLP_RAND48    1
#  define _NOTHREADS    1
#  undef  _PTHREADS
#  define _STLP_LITTLE_ENDIAN
#endif

#if defined (__MINGW32__)
/* Mingw32, egcs compiler using the Microsoft C runtime */
#  if (__GNUC__ >= 3)
/* For gcc before version 3 this macro is defined below */
#    define _STLP_VENDOR_GLOBAL_CSTD
#  endif
#  define _STLP_CALL
#endif /* __MINGW32__ */

#if defined (__CYGWIN__) || defined (__MINGW32__)
#  if !defined (_STLP_USE_STATIC_LIB)
#    define _STLP_USE_DECLSPEC 1
#    if !defined (_STLP_USE_DYNAMIC_LIB)
#      define _STLP_USE_DYNAMIC_LIB
#    endif
#    define _STLP_EXPORT_DECLSPEC __declspec(dllexport)
#    define _STLP_CLASS_EXPORT_DECLSPEC __declspec(dllexport)
#    define _STLP_CLASS_IMPORT_DECLSPEC __declspec(dllimport)
#  endif
/* The following is defined independently of _STLP_USE_STATIC_LIB because it is also
 * used to import symbols from PSDK under MinGW
 */
#  define _STLP_IMPORT_DECLSPEC __declspec(dllimport)
#else
#  if (__GNUC__ >= 4)
#    if !defined (_STLP_USE_STATIC_LIB)
#      if !defined (_STLP_USE_DYNAMIC_LIB)
#        define _STLP_USE_DYNAMIC_LIB
#      endif
#      define _STLP_USE_DECLSPEC 1
#      define _STLP_EXPORT_DECLSPEC __attribute__((visibility("default")))
#      define _STLP_IMPORT_DECLSPEC __attribute__((visibility("default")))
#      define _STLP_CLASS_EXPORT_DECLSPEC __attribute__((visibility("default")))
#      define _STLP_CLASS_IMPORT_DECLSPEC __attribute__((visibility("default")))
#    endif
#  endif
#endif

#if defined (__CYGWIN__) || defined (__MINGW32__) || !(defined (_STLP_USE_GLIBC) || defined (__sun) || defined(__APPLE__))
#  if !defined (__MINGW32__) && !defined (__CYGWIN__)
#    define _STLP_NO_NATIVE_MBSTATE_T    1
#  endif
#  if !defined (__MINGW32__) || (__GNUC__ < 3) || (__GNUC__ == 3) && (__GNUC_MINOR__ < 4)
#    define _STLP_NO_NATIVE_WIDE_FUNCTIONS 1
#  endif
#  define _STLP_NO_NATIVE_WIDE_STREAMS   1
#endif

#define _STLP_NORETURN_FUNCTION __attribute__((noreturn))

/* Mac OS X is a little different with namespaces and cannot instantiate
 * static data members in template classes */
#if defined (__APPLE__)
#  if ((__GNUC__ < 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ < 3)))
/* Mac OS X is missing a required typedef and standard macro */
typedef unsigned int wint_t;
#  endif

#  define __unix

#  define _STLP_NO_LONG_DOUBLE

/* Mac OS X needs all "::" scope references to be "std::" */
#  define _STLP_USE_NEW_C_HEADERS

#  define _STLP_NO_VENDOR_STDLIB_L

#endif /* __APPLE__ */

#define _STLP_LONG_LONG long long

#ifdef _STLP_USE_UCLIBC
  /* No *f math fuctions variants (i.e. sqrtf, fabsf, etc.) */
#  define _STLP_NO_VENDOR_MATH_F
  /* No *l math fuctions variants (i.e. sqrtl, fabsl, etc.) */
#  define _STLP_NO_VENDOR_MATH_L
#  define _STLP_NO_LONG_DOUBLE
#endif

#if defined (__OpenBSD__) || defined (__FreeBSD__)
#  define _STLP_NO_VENDOR_MATH_L
#  define _STLP_NO_VENDOR_STDLIB_L /* no llabs */
#  ifndef __unix
#    define __unix
#  endif
#endif

#if defined (__alpha__)
#  define _STLP_NO_VENDOR_MATH_L
#endif

#if defined (__hpux)
#  define _STLP_VENDOR_GLOBAL_CSTD 1
#  define _STLP_NO_VENDOR_STDLIB_L /* no llabs */
  /* No *f math fuctions variants (i.e. sqrtf, fabsf, etc.) */
#  define _STLP_NO_VENDOR_MATH_F
#endif

#ifdef __ANDROID__
#  define _STLP_HAS_NO_NEW_C_HEADERS 1
#endif

#ifndef _STLP_HAS_NO_NEW_C_HEADERS
#  define _STLP_HAS_NO_NEW_C_HEADERS /* experiment */
#endif
#ifndef _STLP_HAS_NO_NEW_C_HEADERS
/*
#  ifndef _STLP_USE_UCLIBC
*/
#    define _STLP_HAS_NATIVE_FLOAT_ABS
/*
#  endif
*/
#else
#  ifdef _STLP_USE_GLIBC
#    define _STLP_VENDOR_LONG_DOUBLE_MATH  1
#  endif
#endif

#ifndef __ANDROID__
#  undef _STLP_NO_UNCAUGHT_EXCEPT_SUPPORT
#  undef _STLP_NO_UNEXPECTED_EXCEPT_SUPPORT
#endif

/* strict ANSI prohibits "long long" ( gcc) */
#if defined ( __STRICT_ANSI__ )
#  undef _STLP_LONG_LONG 
#endif

#ifndef __EXCEPTIONS
#  undef  _STLP_DONT_USE_EXCEPTIONS
#  define _STLP_DONT_USE_EXCEPTIONS 1
#endif

/* #define _STLP_NO_EXCEPTION_HEADER */

#define _STLP_NO_FORCE_INSTANTIATE

/* #if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 5)) */
#  define _STLP_IS_POD(T)             __is_pod(T)
#  define _STLP_IS_STANDARD_LAYOUT(T) __is_standard_layout(T)
#  define _STLP_IS_ABSTRACT(T)        __is_abstract(T)
#  define _STLP_IS_POLYMORPHIC(T)     __is_polymorphic(T)
#  define _STLP_IS_TRIVIAL(T)         __is_trivial(T)
#  define _STLP_IS_EMPTY(T)           __is_empty(T)
#  define _STLP_IS_CLASS(T)           __is_class(T)
#  define _STLP_IS_UNION(T)           __is_union(T)
#  define _STLP_IS_ENUM(T)            __is_enum(T)
#  define _STLP_IS_BASE_OF(B,D)       __is_base_of(B,D)
/* #  define _STLP_IS_LITERAL_TYPE(T)    __is_literal_type(T) */
/* in 4.5.1 known, but not implemented: */
/* #  define _STLP_IS_CONVERTIBLE(F,T)  __is_convertible_to(F,T) */
#  define _STLP_HAS_VIRTUAL_DESTRUCTOR(T) __has_virtual_destructor(T)
#  define _STLP_HAS_TRIVIAL_DESTRUCTOR(T) __has_trivial_destructor(T)
#  define _STLP_HAS_TRIVIAL_COPY(T)   __has_trivial_copy(T)
#  define _STLP_HAS_NOTHROW_COPY(T)   __has_nothrow_copy(T)
#  define _STLP_HAS_TRIVIAL_CONSTRUCTOR(T) __has_trivial_constructor(T)
#  define _STLP_HAS_NOTHROW_CONSTRUCTOR(T) __has_nothrow_constructor(T)
#  define _STLP_HAS_TRIVIAL_ASSIGN(T) __has_trivial_assign(T)
#  define _STLP_HAS_NOTHROW_ASSIGN(T) __has_nothrow_assign(T)
/* #  define _STLP_UNDERLYING_TYPE(T) __underlying_type(T) */
/* #endif */

/* #ifdef __GXX_EXPERIMENTAL_CXX0X__ */
#  define _STLP_RVR /* we have rvalue reference, T&& */
/* #  if (__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3)) */
#if __has_feature(cxx_variadic_templates)
#  define _STLP_VARIADIC_TEMPLATES
#endif

#define _STLP_CPP_0X
/* #  endif */
/* #  if ((__GNUC__ == 4) && (__GNUC_MINOR__ < 6)) */
  /* at least for __clang_major__ >= 3 */
#if !__has_feature(cxx_nullptr)
#  define _STLP_NO_NULLPTR_T
#endif

#define _STLP_NOEXCEPT noexcept

/* #endif */

#define _STLP_OPERATORS_NEW_DELETE /* use own implemenation of new and delete */
#define _STLP_VENDOR_BAD_ALLOC /* use vendor's std::bad_alloc, but don't include vendor's header */
#if (__clang_major__ < 3) || ((__clang_major__ == 3) && (__clang_minor__ < 1))
#  define _STLP_LAMBDA_PAR_BUG /* can't parse lambda expression with parameters */
#endif /* CLang before 3.1 */
#define _STLP_VENDOR_EXCEPTION /* use vendor's std::exception, but don't include vendor's header */
