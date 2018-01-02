/*
 * Copyright (c) 2009
 * Petr Ovtchenkov
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

#ifndef _STLP_STDINT_H
#define _STLP_STDINT_H

#ifndef _STLP_OUTERMOST_HEADER_ID
#  define _STLP_OUTERMOST_HEADER_ID 0x263
#  include <stl/_cprolog.h>
#elif (_STLP_OUTERMOST_HEADER_ID == 0x263) && !defined(_STLP_DONT_POP_HEADER_ID)
#  define _STLP_DONT_POP_HEADER_ID
#endif

#ifdef _MSC_VER
#  include "stl/msc_stdint.h"
#else /* _MSC_VER */
#  ifdef _STLP_HAS_INCLUDE_NEXT
#   include_next <stdint.h>
#  else
#   include _STLP_NATIVE_C_HEADER(stdint.h)
#  endif
#endif /* _MSC_VER */

#if defined(__cplusplus) && (_STLP_OUTERMOST_HEADER_ID == 0x263) && !defined(_STLP_INTERNAL_CSTDINT)
#  include "stl/_cstdint.h"
#endif /* __cplusplus */

#if (_STLP_OUTERMOST_HEADER_ID == 0x263)
#  ifndef _STLP_DONT_POP_HEADER_ID
#    include <stl/_epilog.h>
#    undef _STLP_OUTERMOST_HEADER_ID
#  else
#    undef _STLP_DONT_POP_HEADER_ID
#  endif
#endif

#endif /* _STLP_STDINT_H */
