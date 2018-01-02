/*
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
 * Copyright (c) 1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */

/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

#ifndef _STLP_INTERNAL_HEAP_H
#define _STLP_INTERNAL_HEAP_H

_STLP_BEGIN_NAMESPACE

// Heap-manipulation functions: push_heap, pop_heap, make_heap, sort_heap.

template <class _RandomAccessIterator>
void push_heap(_RandomAccessIterator __first, _RandomAccessIterator __last);


template <class _RandomAccessIterator, class _Compare>
void push_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);

template <class _RandomAccessIterator>
void __adjust_heap(_RandomAccessIterator __first,
              typename iterator_traits<_RandomAccessIterator>::difference_type __holeIndex,
              typename iterator_traits<_RandomAccessIterator>::difference_type __len,
              typename iterator_traits<_RandomAccessIterator>::value_type __val);

template <class _RandomAccessIterator>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
                       _RandomAccessIterator __result,
                       typename iterator_traits<_RandomAccessIterator>::value_type __val)
{
  typedef typename iterator_traits<_RandomAccessIterator>::difference_type _Distance;

  *__result = *__first;
  __adjust_heap(__first, _Distance(0), (__last - __first), __val);
}

template <class _RandomAccessIterator>
void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last);

template <class _RandomAccessIterator, class _Compare>
void __adjust_heap(_RandomAccessIterator __first,
                   typename iterator_traits<_RandomAccessIterator>::difference_type __holeIndex,
                   typename iterator_traits<_RandomAccessIterator>::difference_type __len,
                   typename iterator_traits<_RandomAccessIterator>::value_type __val,
                   _Compare __comp);

template <class _RandomAccessIterator, class _Compare>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
                       _RandomAccessIterator __result,
                       typename iterator_traits<_RandomAccessIterator>::value_type __val,
                       _Compare __comp)
{
  typedef typename iterator_traits<_RandomAccessIterator>::difference_type _Distance;

  *__result = *__first;
  __adjust_heap(__first, _Distance(0), (__last - __first), __val, __comp);
}

template <class _RandomAccessIterator, class _Compare>
void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);

template <class _RandomAccessIterator>
void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last);

template <class _RandomAccessIterator, class _Compare>
void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);

template <class _RandomAccessIterator>
inline void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
  while (__last - __first > 1)
    pop_heap(__first, __last--);
}

template <class _RandomAccessIterator, class _Compare>
inline void sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
  while (__last - __first > 1)
    pop_heap(__first, __last--, __comp);
}

_STLP_END_NAMESPACE

#include <stl/_heap.c>

#endif /* _STLP_INTERNAL_HEAP_H */

// Local Variables:
// mode:C++
// End:
