/*
 * Copyright (c) 1999
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1999
 * Boris Fomitchev
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

#ifndef _STLP_C_LOCALE_H
#define _STLP_C_LOCALE_H

/*
 * Implementation dependent definitions.
 * Beware: This header is not a purely internal header, it is also included
 * from the outside world when building the STLport library. So this header
 * should not reference internal headers (stlport/stl/_*.h) directly.
 */
#if defined (_STLP_USE_GLIBC)
#  include <ctype.h>
#endif

/*
 * GENERAL FRAMEWORK
 */

/*
 * Opaque types, implementation (if there is one) depends
 * on platform localisation API.
 */
struct _Locale_ctype;
struct _Locale_codecvt;
struct _Locale_numeric;
struct _Locale_time;
struct _Locale_collate;
struct _Locale_monetary;
struct _Locale_messages;

/*
  Bitmask macros.
*/

/*
 * For narrow characters, we expose the lookup table interface.
 */

#ifdef _STLP_USE_GLIBC
#  ifdef __ANDROID__
#    define _Locale_CNTRL _C
#    define _Locale_UPPER _U
#    define _Locale_LOWER _L
#    define _Locale_DIGIT _N
#    define _Locale_XDIGIT (_N|_X)
#    define _Locale_PUNCT _P
#    define _Locale_SPACE _S
#    define _Locale_PRINT (_P|_U|_L|_N|_B)
#    define _Locale_ALPHA (_U|_L)
#  else
/* This section uses macros defined in the gnu libc ctype.h header */
#    define _Locale_CNTRL  _IScntrl
#    define _Locale_UPPER  _ISupper
#    define _Locale_LOWER  _ISlower
#    define _Locale_DIGIT  _ISdigit
#    define _Locale_XDIGIT _ISxdigit
#    define _Locale_PUNCT  _ISpunct
#    define _Locale_SPACE  _ISspace
#    define _Locale_PRINT  _ISprint
#    define _Locale_ALPHA  _ISalpha
#  endif
#else
/* Default values based on C++ Standard 22.2.1.
 * Under Windows the localisation implementation take care of mapping its
 * mask values to those internal values. For other platforms without real
 * localization support we are free to use the most suitable values.*/
#  define _Locale_SPACE  0x0001
#  define _Locale_PRINT  0x0002
#  define _Locale_CNTRL  0x0004
#  define _Locale_UPPER  0x0008
#  define _Locale_LOWER  0x0010
#  define _Locale_ALPHA  0x0020
#  define _Locale_DIGIT  0x0040
#  define _Locale_PUNCT  0x0080
#  define _Locale_XDIGIT 0x0100
#endif

#endif /* _STLP_C_LOCALE_H */
