/*
 * Copyright (c) 2003
 * Francois Dumont
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

#ifndef _STLP_SPECIALIZED_SLIST_H
#define _STLP_SPECIALIZED_SLIST_H

#ifndef _STLP_POINTERS_SPEC_TOOLS_H
#  include <stl/pointers/_tools.h>
#endif

_STLP_BEGIN_NAMESPACE

#define SLIST_IMPL _STLP_PTR_IMPL_NAME(forward_list)

#if defined (_STLP_USE_TEMPLATE_EXPORT) && !defined (_STLP_USE_MSVC6_MEM_T_BUG_WORKAROUND)
_STLP_MOVE_TO_PRIV_NAMESPACE

_STLP_EXPORT_TEMPLATE_CLASS _Slist_node<void*>;
typedef _Slist_node<void*> _VoidPtrSNode;
_STLP_EXPORT_TEMPLATE_CLASS _STLP_alloc_proxy<_Slist_node_base, allocator<_VoidPtrSNode> >;
_STLP_EXPORT_TEMPLATE_CLASS _Slist_base<void*, allocator<void*> >;
_STLP_EXPORT_TEMPLATE_CLASS SLIST_IMPL<void*, allocator<void*> >;

_STLP_MOVE_TO_STD_NAMESPACE
#endif

#if defined (_STLP_DEBUG)
#  define forward_list _STLP_NON_DBG_NAME(forward_list)
_STLP_MOVE_TO_PRIV_NAMESPACE
#endif

template <class _Tp, _STLP_DFL_TMPL_PARAM(_Alloc, allocator<_Tp>) >
class forward_list
#if defined (_STLP_USE_PARTIAL_SPEC_WORKAROUND) && !defined (forward_list)
             : public __stlport_class<forward_list<_Tp, _Alloc> >
#endif
{
  typedef _STLP_TYPENAME _STLP_PRIV _StorageType<_Tp>::_Type _StorageType;
  typedef typename _Alloc_traits<_StorageType, _Alloc>::allocator_type _StorageTypeAlloc;
  typedef _STLP_PRIV SLIST_IMPL<_StorageType, _StorageTypeAlloc> _Base;
  typedef typename _Base::iterator _BaseIte;
  typedef typename _Base::const_iterator _BaseConstIte;
  typedef forward_list<_Tp, _Alloc> _Self;
  typedef _STLP_PRIV _CastTraits<_StorageType, _Tp> cast_traits;
  typedef _STLP_PRIV _Slist_node_base _Node_base;

public:
  typedef _Tp               value_type;
  typedef value_type*       pointer;
  typedef const value_type* const_pointer;
  typedef value_type&       reference;
  typedef const value_type& const_reference;
  typedef size_t            size_type;
  typedef ptrdiff_t         difference_type;
  typedef forward_iterator_tag _Iterator_category;

  typedef _STLP_PRIV _Slist_iterator<value_type, _Nonconst_traits<value_type> >  iterator;
  typedef _STLP_PRIV _Slist_iterator<value_type, _Const_traits<value_type> >     const_iterator;

  typedef typename _Alloc_traits<value_type, _Alloc>::allocator_type allocator_type;

public:
  allocator_type get_allocator() const
  { return _M_impl.get_allocator(); }

  explicit forward_list(const allocator_type& __a = allocator_type())
    : _M_impl(__a) {}

  forward_list(size_type __n, const value_type& __x, const allocator_type& __a =  allocator_type())
    : _M_impl(__n, cast_traits::to_storage_type_cref(__x), __a) {}

  explicit forward_list(size_type __n) : _M_impl(__n) {}

  // We don't need any dispatching tricks here, because _M_insert_after_range
  // already does them.
  template <class _InputIterator>
  forward_list(_InputIterator __first, _InputIterator __last,
               const allocator_type& __a = allocator_type())
#  if !defined (_STLP_USE_ITERATOR_WRAPPER)
    : _M_impl(__first, __last, __a) {}
#  else
    : _M_impl(__a) {
    insert_after(before_begin(), __first, __last);
  }
#  endif

    forward_list(const _Self& __x) :
        _M_impl(__x._M_impl)
      { }

    forward_list(_Self&& __x) :
        _M_impl(_STLP_STD::move(__x._M_impl))
      { }

    forward_list(const _Self& __x, const allocator_type& __a) :
        _M_impl(__x._M_impl, __a)
      { }

    forward_list(_Self&& __x, const allocator_type& __a) :
        _M_impl(_STLP_STD::move(__x._M_impl), __a)
      { }

#if !defined (_STLP_NO_MOVE_SEMANTIC)
  forward_list(__move_source<_Self> src)
    : _M_impl(__move_source<_Base>(src.get()._M_impl)) {}
#endif

  _Self& operator= (const _Self& __x) { _M_impl = __x._M_impl; return *this; }

  void assign(size_type __n, const value_type& __val)
  { _M_impl.assign(__n, cast_traits::to_storage_type_cref(__val)); }

#  if defined (_STLP_USE_ITERATOR_WRAPPER)
private:
  template <class _Integer>
  void _M_assign_dispatch(_Integer __n, _Integer __val,
                          const __true_type&)
  { _M_impl.assign(__n, __val); }

  template <class _InputIterator>
  void _M_assign_dispatch(_InputIterator __first, _InputIterator __last,
                          const __false_type&) {
    _M_impl.assign(_STLP_TYPENAME _STLP_PRIV _IteWrapper<_StorageType, _Tp, _InputIterator>::_Ite(__first),
                   _STLP_TYPENAME _STLP_PRIV _IteWrapper<_StorageType, _Tp, _InputIterator>::_Ite(__last));
  }

public:
#  endif /* _STLP_USE_ITERATOR_WRAPPER */

  template <class _InputIterator>
  void assign(_InputIterator __first, _InputIterator __last) {
#  if defined (_STLP_USE_ITERATOR_WRAPPER)
    typedef typename _IsIntegral<_InputIterator>::_Ret _Integral;
    _M_assign_dispatch(__first, __last, _Integral());
#  else
    _M_impl.assign(__first, __last);
#  endif
  }

  iterator before_begin()             { return iterator(_M_impl.before_begin()._M_node); }
  const_iterator before_begin() const { return const_iterator(const_cast<_Node_base*>(_M_impl.before_begin()._M_node)); }

  iterator begin()                    { return iterator(_M_impl.begin()._M_node); }
  const_iterator begin() const        { return const_iterator(const_cast<_Node_base*>(_M_impl.begin()._M_node));}

  iterator end()                      { return iterator(_M_impl.end()._M_node); }
  const_iterator end() const          { return iterator(_M_impl.end()._M_node); }

  size_type size() const      { return _M_impl.size(); }
  size_type max_size() const  { return _M_impl.max_size(); }
  bool empty() const          { return _M_impl.empty(); }

  void swap(_Self& __x) { _M_impl.swap(__x._M_impl); }
#if defined (_STLP_USE_PARTIAL_SPEC_WORKAROUND) && !defined (_STLP_FUNCTION_TMPL_PARTIAL_ORDER)
  void _M_swap_workaround(_Self& __x) { swap(__x); }
#endif

public:
  reference front()             { return *begin(); }
  const_reference front() const { return *begin(); }
  void push_front(const value_type& __x)
  { _M_impl.push_front(cast_traits::to_storage_type_cref(__x)); }

  void pop_front() { _M_impl.pop_front(); }

  iterator previous(const_iterator __pos)
  { return iterator(_M_impl.previous(_BaseConstIte(__pos._M_node))._M_node); }
  const_iterator previous(const_iterator __pos) const
  { return const_iterator(const_cast<_Node_base*>(_M_impl.previous(_BaseConstIte(__pos._M_node))._M_node)); }

  iterator insert_after(iterator __pos, const value_type& __x)
  { return iterator(_M_impl.insert_after(_BaseIte(__pos._M_node),
                                         cast_traits::to_storage_type_cref(__x))._M_node); }

  void insert_after(iterator __pos, size_type __n, const value_type& __x)
  { _M_impl.insert_after(_BaseIte(__pos._M_node), __n, cast_traits::to_storage_type_cref(__x)); }

#  if defined (_STLP_USE_ITERATOR_WRAPPER)
private:
  template <class _Integer>
  void _M_insert_after_dispatch(iterator __pos, _Integer __n, _Integer __val,
                                const __true_type&) {
    _M_impl.insert_after(_BaseIte(__pos._M_node), __n, __val);
  }

  template <class _InputIterator>
  void _M_insert_after_dispatch(iterator __pos,
                                _InputIterator __first, _InputIterator __last,
                                const __false_type&) {
    _M_impl.insert_after(_BaseIte(__pos._M_node),
                         _STLP_TYPENAME _STLP_PRIV _IteWrapper<_StorageType, _Tp, _InputIterator>::_Ite(__first),
                         _STLP_TYPENAME _STLP_PRIV _IteWrapper<_StorageType, _Tp, _InputIterator>::_Ite(__last));
  }

public:
#  endif

  template <class _InputIterator>
  void insert_after(iterator __pos, _InputIterator __first, _InputIterator __last) {
#  if defined (_STLP_USE_ITERATOR_WRAPPER)
    // Check whether it's an integral type.  If so, it's not an iterator.
    typedef typename _IsIntegral<_InputIterator>::_Ret _Integral;
    _M_insert_after_dispatch(__pos, __first, __last, _Integral());
#  else
    _M_impl.insert_after(_BaseIte(__pos._M_node), __first, __last);
#  endif
  }

#  if defined (_STLP_USE_ITERATOR_WRAPPER)
private:
  template <class _Integer>
  void _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __val,
                          const __true_type&) {
    _M_impl.insert(_BaseIte(__pos._M_node), __n, __val);
  }

  template <class _InputIterator>
  void _M_insert_dispatch(iterator __pos,
                          _InputIterator __first, _InputIterator __last,
                          const __false_type&) {
    _M_impl.insert(_BaseIte(__pos._M_node), _STLP_TYPENAME _STLP_PRIV _IteWrapper<_StorageType, _Tp, _InputIterator>::_Ite(__first),
                                            _STLP_TYPENAME _STLP_PRIV _IteWrapper<_StorageType, _Tp, _InputIterator>::_Ite(__last));
  }

public:
#  endif

  iterator erase_after(iterator __pos)
  { return iterator(_M_impl.erase_after(_BaseIte(__pos._M_node))._M_node); }
  iterator erase_after(iterator __before_first, iterator __last)
  { return iterator(_M_impl.erase_after(_BaseIte(__before_first._M_node),
                                        _BaseIte(__last._M_node))._M_node); }

  void resize(size_type __new_size, const value_type& __x)
  { _M_impl.resize(__new_size, cast_traits::to_storage_type_cref(__x));}

  void resize(size_type __new_size) { _M_impl.resize(__new_size); }

  void clear() { _M_impl.clear(); }

  void splice_after(iterator __pos, _Self& __x,
                    iterator __before_first, iterator __before_last)
  { _M_impl.splice_after(_BaseIte(__pos._M_node), __x._M_impl,
                         _BaseIte(__before_first._M_node), _BaseIte(__before_last._M_node)); }
  void splice_after(iterator __pos, _Self& __x, iterator __prev)
  { _M_impl.splice_after(_BaseIte(__pos._M_node), __x._M_impl, _BaseIte(__prev._M_node)); }
  void splice_after(iterator __pos, _Self& __x)
  { _M_impl.splice_after(_BaseIte(__pos._M_node), __x._M_impl); }

  void reverse() { _M_impl.reverse(); }

  void remove(const value_type& __val) { _M_impl.remove(cast_traits::to_storage_type_cref(__val)); }
  void unique()                 { _M_impl.unique(); }
  void merge(_Self& __x)        { _M_impl.merge(__x._M_impl); }
  void sort()                   {_M_impl.sort(); }

  template <class _Predicate>
  void remove_if(_Predicate __pred)
  { _M_impl.remove_if(_STLP_PRIV _UnaryPredWrapper<_StorageType, _Tp, _Predicate>(__pred)); }

  template <class _BinaryPredicate>
  void unique(_BinaryPredicate __pred)
  { _M_impl.unique(_STLP_PRIV _BinaryPredWrapper<_StorageType, _Tp, _BinaryPredicate>(__pred)); }

  template <class _StrictWeakOrdering>
  void merge(_Self& __x, _StrictWeakOrdering __comp)
  { _M_impl.merge(__x._M_impl, _STLP_PRIV _BinaryPredWrapper<_StorageType, _Tp, _StrictWeakOrdering>(__comp)); }

  template <class _StrictWeakOrdering>
  void sort(_StrictWeakOrdering __comp)
  { _M_impl.sort(_STLP_PRIV _BinaryPredWrapper<_StorageType, _Tp, _StrictWeakOrdering>(__comp)); }

private:
  _Base _M_impl;
};

#if defined (forward_list)
#  undef forward_list
_STLP_MOVE_TO_STD_NAMESPACE
#endif

#undef SLIST_IMPL

_STLP_END_NAMESPACE

#endif /* _STLP_SPECIALIZED_SLIST_H */

// Local Variables:
// mode:C++
// End:
