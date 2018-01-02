/*
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Copyright (c) 1996-1998
 * Silicon Graphics Computer Systems, Inc.
 *
 * Copyright (c) 1997
 * Moscow Center for SPARC Technology
 *
 * Copyright (c) 1999
 * Boris Fomitchev
 *
 * Copyright (c) 2012
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

/* NOTE: This is an internal header file, included by other STL headers.
 *   You should not attempt to use it directly.
 */

#ifndef _STLP_INTERNAL_ITERATOR_BASE_H
#define _STLP_INTERNAL_ITERATOR_BASE_H

#ifndef _STLP_CSTDDEF
#  include <cstddef>
#endif

_STLP_BEGIN_NAMESPACE

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template <class _Category, class _Tp, _STLP_DFL_TMPL_PARAM(_Distance,ptrdiff_t),
          _STLP_DFL_TMPL_PARAM(_Pointer,_Tp*), _STLP_DFL_TMPL_PARAM(_Reference,_Tp&) >
struct iterator {
  typedef _Category  iterator_category;
  typedef _Tp        value_type;
  typedef _Distance  difference_type;
  typedef _Pointer   pointer;
  typedef _Reference reference;
};
_STLP_TEMPLATE_NULL
struct iterator<output_iterator_tag, void, void, void, void> {
  typedef output_iterator_tag  iterator_category;
#ifdef _STLP_CLASS_PARTIAL_SPECIALIZATION
  typedef void                value_type;
  typedef void                difference_type;
  typedef void                pointer;
  typedef void                reference;
#endif
};

template <class _Iterator>
struct iterator_traits
{
    typedef typename _Iterator::difference_type   difference_type;
    typedef typename _Iterator::value_type        value_type;
    typedef typename _Iterator::pointer           pointer;
    typedef typename _Iterator::reference         reference;
    typedef typename _Iterator::iterator_category iterator_category;
};

template <class _Tp>
struct iterator_traits<_Tp*>
{
    typedef ptrdiff_t                  difference_type;
    typedef _Tp                        value_type;
    typedef _Tp*                       pointer;
    typedef _Tp&                       reference;
    typedef random_access_iterator_tag iterator_category;
};

template <class _Tp>
struct iterator_traits<const _Tp*>
{
    typedef ptrdiff_t                  difference_type;
    typedef _Tp                        value_type;
    typedef const _Tp*                 pointer;
    typedef const _Tp&                 reference;
    typedef random_access_iterator_tag iterator_category;
};

// specialization for _Tp __far* ? What is platforms for?

_STLP_END_NAMESPACE
#include <stl/_ptrs_specialize.h>
_STLP_BEGIN_NAMESPACE

_STLP_MOVE_TO_PRIV_NAMESPACE

template <class _IteratorCategory>
struct __distance_aux
{
    template <class _Iterator>
    static typename iterator_traits<_Iterator>::difference_type _STLP_CALL distance(const _Iterator& __first, const _Iterator& __last)
      {
        typename iterator_traits<_Iterator>::difference_type __n = 0;
        for (_Iterator __it(__first); __it != __last; ++__it) {
          ++__n;
        }
        return __n;
      }
};

template <>
struct __distance_aux<typename _STLP_STD::random_access_iterator_tag>
{
    template <class _Iterator>
    static typename iterator_traits<_Iterator>::difference_type _STLP_CALL distance(const _Iterator& __first, const _Iterator& __last)
      { return __last - __first; }
};

_STLP_MOVE_TO_STD_NAMESPACE

template <class _Iterator>
inline
typename iterator_traits<_Iterator>::difference_type _STLP_CALL distance(_Iterator __first, _Iterator __last)
{ return _STLP_PRIV __distance_aux<typename iterator_traits<_Iterator>::iterator_category>::distance(__first, __last); }

_STLP_MOVE_TO_PRIV_NAMESPACE

template <class _InputIter>
inline void _STLP_CALL
__advance(_InputIter& __i, typename iterator_traits<_InputIter>::difference_type __n, const input_iterator_tag&)
{ while (__n--) ++__i; }

// fbp : added output iterator tag variant
template <class _Iterator>
inline void _STLP_CALL
__advance(_Iterator& __i, typename iterator_traits<_Iterator>::difference_type __n, const output_iterator_tag&)
{ while (__n--) ++__i; }

template <class _BidirectionalIterator>
inline void _STLP_CALL
__advance(_BidirectionalIterator& __i, typename iterator_traits<_BidirectionalIterator>::difference_type __n,
          const bidirectional_iterator_tag&)
{
  if (__n > 0)
    while (__n--) ++__i;
  else
    while (__n++) --__i;
}

template <class _RandomAccessIterator>
inline void _STLP_CALL
__advance(_RandomAccessIterator& __i, typename iterator_traits<_RandomAccessIterator>::difference_type __n, const random_access_iterator_tag&)
{ __i += __n; }

_STLP_MOVE_TO_STD_NAMESPACE

template <class _InputIterator>
inline void _STLP_CALL advance(_InputIterator& __i, typename iterator_traits<_InputIterator>::difference_type __n)
{ _STLP_PRIV __advance(__i, __n, typename iterator_traits<_InputIterator>::iterator_category()); }

_STLP_END_NAMESPACE

#endif /* _STLP_INTERNAL_ITERATOR_BASE_H */


// Local Variables:
// mode:C++
// End:
