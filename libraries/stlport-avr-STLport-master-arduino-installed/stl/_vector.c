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
#ifndef _STLP_VECTOR_C
#define _STLP_VECTOR_C

#include <stl/_range_errors.h>

_STLP_BEGIN_NAMESPACE

_STLP_MOVE_TO_PRIV_NAMESPACE

template <class _Tp, class _Alloc>
void _Vector_base<_Tp,_Alloc>::_M_throw_length_error() const
{ __stl_throw_length_error("vector"); }

template <class _Tp, class _Alloc>
void _Vector_base<_Tp, _Alloc>::_M_throw_out_of_range() const
{ __stl_throw_out_of_range("vector"); }

#if defined (_STLP_USE_PTR_SPECIALIZATIONS)
#  define vector _STLP_PTR_IMPL_NAME(vector)
#elif defined (_STLP_DEBUG)
#  define vector _STLP_NON_DBG_NAME(vector)
#else
_STLP_MOVE_TO_STD_NAMESPACE
#endif

#if defined (_STLP_NESTED_TYPE_PARAM_BUG)
#  define __iterator__  _Tp*
#else
#  define __iterator__  _STLP_TYPENAME_ON_RETURN_TYPE vector<_Tp, _Alloc>::iterator
#endif

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::reserve(size_type __n)
{
  if (capacity() < __n) {
    if (max_size() < __n) {
      this->_M_throw_length_error();
    }

    const size_type __old_size = size();
    pointer __tmp;
    if (this->_M_start) {
      __tmp = _M_allocate_and_copy(__n, this->_M_start, this->_M_finish);
      _M_clear();
    } else {
      __tmp = this->_M_end_of_storage.allocate(__n);
    }
    _M_set(__tmp, __tmp + __old_size, __tmp + __n);
  }
}

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow( pointer __pos, const _Tp& __x,
                                              const false_type& /* trivial move */,
                                              size_type __fill_len, bool __atend )
{
  size_type __len = _M_compute_next_size(__fill_len);
  pointer __new_start = this->_M_end_of_storage.allocate(__len);
  pointer __new_finish = __new_start;
  pointer old = this->_M_start;
  _STLP_TRY {
    for ( ; old != __pos; ++__new_finish, ++old ) {
      get_allocator().construct( __new_finish, _STLP_STD::move( *old ) );
    }
    // handle insertion
    for ( ; __fill_len-- > 0; ++__new_finish ) {
      get_allocator().construct( __new_finish, __x );
    }
    if (!__atend) {
      for ( ; old != this->_M_finish; ++__new_finish, ++old ) {
        get_allocator().construct( __new_finish, _STLP_STD::move( *old ) );
      }
    }
  }
  _STLP_UNWIND((_STLP_STD::detail::_Destroy_Range(__new_start,__new_finish),
               this->_M_end_of_storage.deallocate(__new_start,__len)))
  _M_clear();
  _M_set(__new_start, __new_finish, __new_start + __len);
}

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow( pointer __pos, const _Tp& __x,
                                              const true_type& /* trivial move */,
                                              size_type __fill_len, bool __atend )
{
  size_type __len = _M_compute_next_size(__fill_len);
  pointer __new_start = this->_M_end_of_storage.allocate(__len);
  pointer __new_finish = __STATIC_CAST(pointer, _STLP_PRIV __ucopy_trivial( this->_M_start, __pos, __new_start ) );
  // handle insertion
  for ( ; __fill_len-- > 0; ++__new_finish ) {
    get_allocator().construct( __new_finish, __x );
  }
  if (!__atend) {
    __new_finish = __STATIC_CAST(pointer, _STLP_PRIV __ucopy_trivial( __pos, this->_M_finish, __new_finish ) ); // copy remainder
  }
  _M_clear_after_move();
  _M_set(__new_start, __new_finish, __new_start + __len);
}

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert_aux( iterator __pos, size_type __n,
                                              const _Tp& __x, const true_type& /* trivial move */ )
{
  _STLP_PRIV __copy_trivial( __pos, this->_M_finish, __pos + __n );
  this->_M_finish += __n;
  for ( ; __n-- > 0; ++__pos ) {
    get_allocator().construct( __pos, __x );
  }
}

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert_aux( iterator __pos, size_type __n,
                                              const _Tp& __x, const false_type& /* trivial move */ )
{
  iterator src = this->_M_finish - 1;
  iterator dst = src + __n;
  for ( ; src >= __pos; --dst, --src ) {
    get_allocator().construct( dst, _STLP_STD::move( *src ) );
    get_allocator().destroy( src );
  }
  this->_M_finish += __n;
  for ( ; __n-- > 0; ++__pos ) {
    get_allocator().construct( __pos, __x );
  }
}

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert( iterator __pos, size_type __n, const _Tp& __x )
{
  if (__n != 0) {
    if ( size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n ) {
      if ( &__x >= __pos && &__x < this->_M_finish ) { // inside moved
        _Tp __x_copy( __x );
        _M_fill_insert_aux( __pos, __n, __x_copy, typename __has_trivial_move<_Tp>::type() );
      } else {
        _M_fill_insert_aux( __pos, __n, __x, typename __has_trivial_move<_Tp>::type() );
      }
    } else {
      if ( &__x >= this->_M_start && &__x < this->_M_finish ) {
        _Tp __x_copy( __x );
        _M_insert_overflow( __pos, __x_copy, typename __has_trivial_move<_Tp>::type(), __n );
      } else {
        _M_insert_overflow( __pos, __x, typename __has_trivial_move<_Tp>::type(), __n );
      }
    }
  }
}

template <class _Tp, class _Alloc>
vector<_Tp, _Alloc>& vector<_Tp, _Alloc>::operator = (const vector<_Tp, _Alloc>& __x)
{
  typedef typename is_trivially_copy_assignable<_Tp>::type _TrivialCopy;
  typedef typename is_trivially_copyable<_Tp>::type _TrivialUCopy;
  if (&__x != this) {
    const size_type __xlen = __x.size();
    if (__xlen > capacity()) {
      size_type __len = __xlen;
      pointer __tmp = _M_allocate_and_copy(__len, __CONST_CAST(const_pointer, __x._M_start) + 0,
                                                  __CONST_CAST(const_pointer, __x._M_finish) + 0);
      _M_clear();
      this->_M_start = __tmp;
      this->_M_end_of_storage._M_data = this->_M_start + __len;
    } else if (size() >= __xlen) {
      pointer __i = _STLP_PRIV __copy_ptrs(__CONST_CAST(const_pointer, __x._M_start) + 0,
                                           __CONST_CAST(const_pointer, __x._M_finish) + 0, this->_M_start, _TrivialCopy());
      _STLP_STD::detail::_Destroy_Range(__i, this->_M_finish);
    } else {
      _STLP_PRIV __copy_ptrs(__CONST_CAST(const_pointer, __x._M_start),
                             __CONST_CAST(const_pointer, __x._M_start) + size(), this->_M_start, _TrivialCopy());
      _STLP_PRIV __ucopy_ptrs(__CONST_CAST(const_pointer, __x._M_start) + size(),
                              __CONST_CAST(const_pointer, __x._M_finish) + 0, this->_M_finish, _TrivialUCopy());
    }
    this->_M_finish = this->_M_start + __xlen;
  }
  return *this;
}

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_assign(size_t __n, const _Tp& __val)
{
  if (__n > capacity()) {
    vector<_Tp, _Alloc> __tmp(__n, __val, get_allocator());
    __tmp.swap(*this);
  } else if (__n > size()) {
    fill(begin(), end(), __val);
    this->_M_finish = _STLP_PRIV __uninitialized_fill_n(this->_M_finish, __n - size(), __val);
  } else {
    erase(_STLP_PRIV __fill_n(begin(), __n, __val), end());
  }
}

template <class _Tp, class _Alloc>
__iterator__ vector<_Tp, _Alloc>::insert(iterator __pos, const _Tp& __x)
{
  size_type __n = __pos - begin();
  _M_fill_insert(__pos, 1, __x);
  return begin() + __n;
}

#undef __iterator__

#if defined (vector)
#  undef vector
_STLP_MOVE_TO_STD_NAMESPACE
#endif

_STLP_END_NAMESPACE

#endif /*  _STLP_VECTOR_C */

// Local Variables:
// mode:C++
// End:
