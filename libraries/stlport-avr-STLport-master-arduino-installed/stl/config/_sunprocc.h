// STLport configuration file
// It is internal STLport header - DO NOT include it directly

#define _STLP_COMPILER "Sunpro CC"

#define _STLP_LONG_LONG  long long

#if ! defined(_BOOL)
#  define _STLP_NO_BOOL 1
#endif

// compatibility mode stuff
#if (__SUNPRO_CC >= 0x510) && (!defined (__SUNPRO_CC_COMPAT) || (__SUNPRO_CC_COMPAT == 5 ))
#  define _STLP_NATIVE_INCLUDE_PATH ../CC/Cstd
#  define _STLP_NATIVE_CPP_RUNTIME_INCLUDE_PATH ../CC
#elif (__SUNPRO_CC >= 0x500) && (!defined (__SUNPRO_CC_COMPAT) || (__SUNPRO_CC_COMPAT == 5 ))
#  define _STLP_NATIVE_INCLUDE_PATH ../CC
#elif (defined (__SUNPRO_CC_COMPAT) && __SUNPRO_CC_COMPAT == 4)
#  define _STLP_NATIVE_INCLUDE_PATH ../CC4
#else
#  define _STLP_NATIVE_INCLUDE_PATH ../CC
#endif

#define _STLP_STATIC_CONST_INIT_BUG 1

#if (__SUNPRO_CC < 0x530)
// those are tested and proved not to work...
#  define _STLP_NO_CLASS_PARTIAL_SPECIALIZATION 1
#  define _STLP_NO_MEMBER_TEMPLATE_CLASSES 1 /* support removed */
#endif

#ifdef _STLP_USE_NO_IOSTREAMS
#  define _STLP_HAS_NO_NEW_C_HEADERS 1
#endif

// those do not depend on compatibility
#if (__SUNPRO_CC < 0x510)
#  define _STLP_NO_TYPENAME_ON_RETURN_TYPE 1
#  define _STLP_NONTEMPL_BASE_MATCH_BUG 1
#endif

// Haven't tested anything before Sun C++ 5.9 or the compatibility mode.
#if (__SUNPRO_CC >= 0x590) && ! (defined (__SUNPRO_CC_COMPAT) && (__SUNPRO_CC_COMPAT < 5))
#  undef _STLP_NO_UNCAUGHT_EXCEPT_SUPPORT
#  undef _STLP_NO_UNEXPECTED_EXCEPT_SUPPORT
#endif

#if (__SUNPRO_CC < 0x510) || (defined (__SUNPRO_CC_COMPAT) && (__SUNPRO_CC_COMPAT < 5))

#  define _STLP_NO_QUALIFIED_FRIENDS 1

// no partial , just for explicit one
#  define _STLP_PARTIAL_SPEC_NEEDS_TEMPLATE_ARGS
#  define _STLP_NON_TYPE_TMPL_PARAM_BUG 1

#  define _STLP_NO_MEMBER_TEMPLATES 1 /* support removed */

#  define _STLP_NO_FUNCTION_TMPL_PARTIAL_ORDER 1
#  define _STLP_NO_EXPLICIT_FUNCTION_TMPL_ARGS
#  define _STLP_NO_MEMBER_TEMPLATE_KEYWORD 1
#endif

// Features that depend on compatibility switch
#if ( __SUNPRO_CC < 0x500 ) || (defined (__SUNPRO_CC_COMPAT) && (__SUNPRO_CC_COMPAT < 5))

#  ifndef _STLP_USE_NO_IOSTREAMS
#    define _STLP_USE_NO_IOSTREAMS 1
#  endif
#  define _STLP_NO_NEW_NEW_HEADER 1 /* unsupported more */
// #  define _STLP_NO_RELOPS_NAMESPACE
#  define _STLP_HAS_NO_NAMESPACES 1
#  define _STLP_NEED_MUTABLE  1
#  define _STLP_NO_BAD_ALLOC 1
#  define _STLP_NO_EXCEPTION_HEADER 1
#  define _STLP_NATIVE_C_INCLUDE_PATH ../include
#elif (__SUNPRO_CC < 0x510)
// #  define _STLP_NATIVE_C_HEADER(header) <../CC/##header##.SUNWCCh>
#  define _STLP_NATIVE_CPP_C_HEADER(header) <../CC/##header##.SUNWCCh>
#  define _STLP_NATIVE_C_INCLUDE_PATH /usr/include
#elif defined( __SunOS_5_5_1 ) || defined( __SunOS_5_6 ) || defined( __SunOS_5_7 )
#  define _STLP_NATIVE_C_INCLUDE_PATH ../CC/std
#  define _STLP_NATIVE_CPP_C_INCLUDE_PATH ../CC/std
#else
#  define _STLP_NATIVE_C_INCLUDE_PATH /usr/include
#  define _STLP_NATIVE_CPP_C_INCLUDE_PATH ../CC/std
#endif

#if ( __SUNPRO_CC < 0x500 )

#error "Sorry. Use better compiler"

#endif /* <  5.0 */

#ifndef _MBSTATET_H
#  define _MBSTATET_H
#  undef _MBSTATE_T
#  define _MBSTATE_T
typedef struct __mbstate_t {
#  if defined(_LP64)
  long    __filler[4];
#  else
  int     __filler[6];
#  endif
} __mbstate_t;
#  ifndef _STLP_HAS_NO_NAMESPACES
namespace std {
  typedef __mbstate_t mbstate_t;
}
using std::mbstate_t;
#  else
typedef __mbstate_t mbstate_t;
#  endif
#endif  /* __MBSTATET_H */

#if !defined(_STLP_HAS_NO_NEW_C_HEADERS) && !defined(_STLP_NO_NEW_C_HEADERS)
/* Since Sun Studio uses <iso/stdlib_iso.h>, we are at its
 * mercy.  GCC is built on top of <stdlib.h>. */
#define _STLP_NO_VENDOR_STDLIB_L
#endif
