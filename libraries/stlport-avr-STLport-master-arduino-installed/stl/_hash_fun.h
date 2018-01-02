/*
 * Copyright (c) 1996-1998
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */

/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

#ifndef _STLP_HASH_FUN_H
#define _STLP_HASH_FUN_H

#ifndef _STLP_CSTDDEF
#  include <cstddef>
#endif

#include <stdint.h>

_STLP_BEGIN_NAMESPACE

template <class _Key> struct hash { };

_STLP_MOVE_TO_PRIV_NAMESPACE

inline size_t __stl_hash_string(const char* __s)
{
  // _STLP_FIX_LITERAL_BUG(__s)
  uint32_t __h = 0;
  while ( *__s ) {
    __h += (__h << 5) + *(__s++); // __h = 33 * __h + __s[i]
  }

  return size_t(__h);
}

_STLP_MOVE_TO_STD_NAMESPACE

_STLP_TEMPLATE_NULL
struct hash<char*> {
  size_t operator()(const char* __s) const {
    _STLP_FIX_LITERAL_BUG(__s)
    return _STLP_PRIV __stl_hash_string(__s);
  }
};

_STLP_TEMPLATE_NULL
struct hash<const char*> {
  size_t operator()(const char* __s) const {
    _STLP_FIX_LITERAL_BUG(__s)
    return _STLP_PRIV __stl_hash_string(__s);
  }
};

_STLP_TEMPLATE_NULL struct hash<char> {
  size_t operator()(char __x) const { return __x; }
};
_STLP_TEMPLATE_NULL struct hash<unsigned char> {
  size_t operator()(unsigned char __x) const { return __x; }
};
#if !defined (_STLP_NO_SIGNED_BUILTINS)
_STLP_TEMPLATE_NULL struct hash<signed char> {
  size_t operator()(unsigned char __x) const { return __x; }
};
#endif
_STLP_TEMPLATE_NULL struct hash<short> {
  size_t operator()(short __x) const { return __x; }
};
_STLP_TEMPLATE_NULL struct hash<unsigned short> {
  size_t operator()(unsigned short __x) const { return __x; }
};
_STLP_TEMPLATE_NULL struct hash<int> {
  size_t operator()(int __x) const { return __x; }
};

#if !defined (_STLP_MSVC) || (_STLP_MSVC < 1300) || defined (_WIN64)
_STLP_TEMPLATE_NULL struct hash<unsigned int> {
  size_t operator()(unsigned int __x) const { return __x; }
};
#else
/* MSVC .Net since 2002 has a 64 bits portability warning feature. typedef
 * like size_t are tagged as potential 64 bits variables making them different from
 * unsigned int. To avoid the warning when a hash container is instanciated with
 * the size_t key we prefer to grant the size_t specialization rather than the
 * unsigned int one.
 */
_STLP_TEMPLATE_NULL struct hash<size_t> {
  size_t operator()(size_t __x) const { return __x; }
};
#endif

_STLP_TEMPLATE_NULL struct hash<long> {
  size_t operator()(long __x) const { return __x; }
};
_STLP_TEMPLATE_NULL struct hash<unsigned long> {
  size_t operator()(unsigned long __x) const { return __x; }
};

#if defined (_STLP_LONG_LONG)
_STLP_TEMPLATE_NULL struct hash<_STLP_LONG_LONG> {
  size_t operator()(_STLP_LONG_LONG x) const { return (size_t)x; }
};
_STLP_TEMPLATE_NULL struct hash<unsigned _STLP_LONG_LONG> {
  size_t operator()(unsigned _STLP_LONG_LONG x) const { return (size_t)x; }
};
#endif

template <int L>
inline size_t __stl_pointer_hash(const void* __s)
{
  size_t __h = 0;
  size_t __len = L;
  const unsigned char* __data = reinterpret_cast<const unsigned char*>(&__s);
  while ( __len-- > 0 ) {
    __h += (__h << 5) + *(__data++); // __h = 33 * __h + __s[i]
  }

  return __h;
}

template <>
inline size_t __stl_pointer_hash<4>(const void* __s)
{
  const unsigned char* __data = reinterpret_cast<const unsigned char*>(&__s);
  size_t __h = *(__data++); // __h = 33 * __h + __s[i]
  __h += (__h << 5) + *(__data++); // __h = 33 * __h + __s[i]
  __h += (__h << 5) + *(__data++); // __h = 33 * __h + __s[i]
  __h += (__h << 5) + *__data; // __h = 33 * __h + __s[i]

  return __h;
}

template <>
inline size_t __stl_pointer_hash<8>(const void* __s)
{
  const unsigned char* __data = reinterpret_cast<const unsigned char*>(&__s);
  size_t __h = *(__data++); // __h = 33 * __h + __s[i]
  __h += (__h << 5) + *(__data++); // __h = 33 * __h + __s[i]
  __h += (__h << 5) + *(__data++); // __h = 33 * __h + __s[i]
  __h += (__h << 5) + *(__data++); // __h = 33 * __h + __s[i]
  __h += (__h << 5) + *(__data++); // __h = 33 * __h + __s[i]
  __h += (__h << 5) + *(__data++); // __h = 33 * __h + __s[i]
  __h += (__h << 5) + *(__data++); // __h = 33 * __h + __s[i]
  __h += (__h << 5) + *__data; // __h = 33 * __h + __s[i]

  return __h;
}

_STLP_TEMPLATE_NULL
struct hash<void *>
{
    size_t operator()(const void* __x) const
      { return __stl_pointer_hash<sizeof(const void*)>(__x); }
};

_STLP_END_NAMESPACE

#endif /* _STLP_HASH_FUN_H */

// Local Variables:
// mode:C++
// End:
