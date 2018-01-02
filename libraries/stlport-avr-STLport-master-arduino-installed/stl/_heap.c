/*
 *
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Copyright (c) 1996,1997
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1997
 * Moscow Center for SPARC Technology
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
#ifndef _STLP_HEAP_C
#define _STLP_HEAP_C

#ifndef _STLP_INTERNAL_HEAP_H
# include <stl/_heap.h>
#endif

#ifndef _STLP_INTERNAL_ITERATOR_BASE_H
# include <stl/_iterator_base.h>
#endif

_STLP_BEGIN_NAMESPACE

template <class _RandomAccessIterator>
inline void __push_heap(_RandomAccessIterator __first,
                        typename iterator_traits<_RandomAccessIterator>::difference_type __holeIndex,
                        typename iterator_traits<_RandomAccessIterator>::difference_type __topIndex,
                        typename iterator_traits<_RandomAccessIterator>::value_type __val)
{
  typename iterator_traits<_RandomAccessIterator>::difference_type __parent = (__holeIndex - 1) / 2;
  while (__holeIndex > __topIndex && *(__first + __parent) < __val) {
    *(__first + __holeIndex) = *(__first + __parent);
    __holeIndex = __parent;
    __parent = (__holeIndex - 1) / 2;
  }
  *(__first + __holeIndex) = __val;
}

//template <class _RandomAccessIterator>
//inline void __push_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last)
//{
//  typedef typename iterator_traits<_RandomAccessIterator>::difference_type _Distance;

//  __push_heap(__first, _Distance((__last - __first) - 1), _Distance(0), *(__last - 1));
//}

template <class _RandomAccessIterator>
inline void push_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
  typedef typename iterator_traits<_RandomAccessIterator>::difference_type _Distance;

  __push_heap(__first, _Distance((__last - __first) - 1), _Distance(0), *(__last - 1));
}


template <class _RandomAccessIterator, class _Compare>
inline void __push_heap(_RandomAccessIterator __first,
                        typename iterator_traits<_RandomAccessIterator>::difference_type __holeIndex,
                        typename iterator_traits<_RandomAccessIterator>::difference_type __topIndex,
                        typename iterator_traits<_RandomAccessIterator>::value_type __val,
                        _Compare __comp)
{
  typename iterator_traits<_RandomAccessIterator>::difference_type __parent = (__holeIndex - 1) / 2;
  while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
    _STLP_VERBOSE_ASSERT(!__comp(__val, *(__first + __parent)), _StlMsg_INVALID_STRICT_WEAK_PREDICATE)
    *(__first + __holeIndex) = *(__first + __parent);
    __holeIndex = __parent;
    __parent = (__holeIndex - 1) / 2;
  }
  *(__first + __holeIndex) = __val;
}

//template <class _RandomAccessIterator, class _Compare>
//inline void __push_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
//{
//  typedef typename iterator_traits<_RandomAccessIterator>::difference_type _Distance;

//  __push_heap(__first, _Distance((__last - __first) - 1), _Distance(0), *(__last - 1), __comp);
//}

template <class _RandomAccessIterator, class _Compare>
inline void push_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{ 
  // __push_heap_aux(__first, __last, __comp);
  typedef typename iterator_traits<_RandomAccessIterator>::difference_type _Distance;

  __push_heap(__first, _Distance((__last - __first) - 1), _Distance(0), *(__last - 1), __comp);
}

template <class _RandomAccessIterator>
void __adjust_heap(_RandomAccessIterator __first,
                   typename iterator_traits<_RandomAccessIterator>::difference_type __holeIndex,
                   typename iterator_traits<_RandomAccessIterator>::difference_type __len,
                   typename iterator_traits<_RandomAccessIterator>::value_type __val)
{
  typename iterator_traits<_RandomAccessIterator>::difference_type __topIndex = __holeIndex;
  typename iterator_traits<_RandomAccessIterator>::difference_type __secondChild = 2 * __holeIndex + 2;
  while (__secondChild < __len) {
    if (*(__first + __secondChild) < *(__first + (__secondChild - 1)))
      __secondChild--;
    *(__first + __holeIndex) = *(__first + __secondChild);
    __holeIndex = __secondChild;
    __secondChild = 2 * (__secondChild + 1);
  }
  if (__secondChild == __len) {
    *(__first + __holeIndex) = *(__first + (__secondChild - 1));
    __holeIndex = __secondChild - 1;
  }
  __push_heap(__first, __holeIndex, __topIndex, __val);
}

//template <class _RandomAccessIterator>
//inline void __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last)
//{ __pop_heap(__first, __last - 1, __last - 1, *(__last - 1)); }

template <class _RandomAccessIterator>
inline void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
  // __pop_heap_aux(__first, __last);
  __pop_heap(__first, __last - 1, __last - 1, *(__last - 1));
}

template <class _RandomAccessIterator, class _Compare>
void __adjust_heap(_RandomAccessIterator __first,
                   typename iterator_traits<_RandomAccessIterator>::difference_type __holeIndex,
                   typename iterator_traits<_RandomAccessIterator>::difference_type __len,
                   typename iterator_traits<_RandomAccessIterator>::value_type __val,
                   _Compare __comp)
{
  typename iterator_traits<_RandomAccessIterator>::difference_type __topIndex = __holeIndex;
  typename iterator_traits<_RandomAccessIterator>::difference_type __secondChild = 2 * __holeIndex + 2;
  while (__secondChild < __len) {
    if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1)))) {
      _STLP_VERBOSE_ASSERT(!__comp(*(__first + (__secondChild - 1)), *(__first + __secondChild)),
                           _StlMsg_INVALID_STRICT_WEAK_PREDICATE)
      __secondChild--;
    }
    *(__first + __holeIndex) = *(__first + __secondChild);
    __holeIndex = __secondChild;
    __secondChild = 2 * (__secondChild + 1);
  }
  if (__secondChild == __len) {
    *(__first + __holeIndex) = *(__first + (__secondChild - 1));
    __holeIndex = __secondChild - 1;
  }
  __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}

//template <class _RandomAccessIterator, class _Compare>
//inline void __pop_heap_aux(_RandomAccessIterator __first,
//                           _RandomAccessIterator __last, _Compare __comp)
//{ __pop_heap(__first, __last - 1, __last - 1, *(__last - 1), __comp); }


template <class _RandomAccessIterator, class _Compare>
inline void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
  // __pop_heap_aux(__first, __last, __comp);
  __pop_heap(__first, __last - 1, __last - 1, *(__last - 1), __comp);
}

template <class _RandomAccessIterator>
inline void __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
{
  typedef typename iterator_traits<_RandomAccessIterator>::difference_type _Distance;

  if (__last - __first < 2) return;
  _Distance __len = __last - __first;
  _Distance __parent = (__len - 2)/2;

  for (;;) {
    __adjust_heap(__first, __parent, __len, *(__first + __parent));
    if (__parent == 0) return;
    __parent--;
  }
}

template <class _RandomAccessIterator>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last)
{ __make_heap(__first, __last); }

template <class _RandomAccessIterator, class _Compare>
inline void __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
                        _Compare __comp)
{
  typedef typename iterator_traits<_RandomAccessIterator>::difference_type _Distance;

  if (__last - __first < 2) return;
  _Distance __len = __last - __first;
  _Distance __parent = (__len - 2)/2;

  for (;;) {
    __adjust_heap(__first, __parent, __len, *(__first + __parent), __comp);
    if (__parent == 0) return;
    __parent--;
  }
}

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{ __make_heap(__first, __last, __comp); }

_STLP_END_NAMESPACE

#endif /*  _STLP_HEAP_C */

// Local Variables:
// mode:C++
// End:
