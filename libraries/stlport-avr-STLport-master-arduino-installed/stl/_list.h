/*
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
 * Copyright (c) 2011,2012
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

#ifndef _STLP_INTERNAL_LIST_IMPL_H
#define _STLP_INTERNAL_LIST_IMPL_H

#ifndef _STLP_INTERNAL_ALGOBASE_H
#  include <stl/_algobase.h>
#endif

#ifndef _STLP_INTERNAL_ALLOC_H
#  include <stl/_alloc.h>
#endif

#ifndef _STLP_INTERNAL_ITERATOR_H
#  include <stl/_iterator.h>
#endif

#ifndef _STLP_INTERNAL_FUNCTION_BASE_H
#  include <stl/_function_base.h>
#endif

_STLP_BEGIN_NAMESPACE

template <class _Tp, class _Alloc> class list;

_STLP_MOVE_TO_PRIV_NAMESPACE

struct _List_node_base
{
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Dummy>
class _List_global {
public:
  typedef _List_node_base _Node_base;
  static void  _STLP_CALL _Transfer(_Node_base* __pos,
                                    _Node_base* __first, _Node_base* __last);
};

#if defined (_STLP_USE_TEMPLATE_EXPORT)
_STLP_EXPORT_TEMPLATE_CLASS _List_global<bool>;
#endif
typedef _List_global<bool> _List_global_inst;

template <class _Tp>
struct _List_node :
    public _List_node_base
{
    _List_node() = default;
    _List_node( const _List_node& ) = default;
    _List_node( _List_node&& ) = default;
    _List_node& operator =( const _List_node& ) = default;
    ~_List_node() = default;

    _Tp _M_data;
};

template <class _Tp> class _List_iterator;
template <class _Tp> class _List_iterator<_Tp const>;

template <class _Tp>
class _List_iterator
{
  public:
    typedef typename remove_const<_Tp>::type value_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef bidirectional_iterator_tag iterator_category;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

  private:
    typedef _List_iterator<value_type> _Self;
    typedef _List_node<value_type> _Node;

  public:
    _List_iterator() = default;
    _List_iterator(const _List_iterator&) = default;
    _List_iterator(_List_iterator&&) = default;

    explicit _List_iterator(_List_node_base* __x) :
        _M_node(static_cast<_Node*>(__x))
      { }

    _List_iterator& operator =( const _List_iterator& ) = default;

    reference operator *() const
      { return _M_node->_M_data; }

    const pointer operator ->() const
      { return &_M_node->_M_data; }

    _Self& operator ++()
      {
        _M_incr();
        return *this;
      }
    _Self operator ++(int)
      {
        _Self __tmp = *this;
        _M_incr();
        return __tmp;
      }
    _Self& operator --()
      {
        _M_decr();
        return *this;
      }
    _Self operator --(int)
      {
        _Self __tmp = *this;
        _M_decr();
        return __tmp;
      }

    bool operator ==(const _Self& __y ) const
      { return _M_node == __y._M_node; }
    bool operator ==(const _List_iterator<_Tp const>& __y ) const
      { return _M_node == reinterpret_cast<_Node*>(__y._M_node); }
    bool operator !=(const _Self& __y ) const
      { return _M_node != __y._M_node; }
    bool operator !=(const _List_iterator<_Tp const>& __y ) const
      { return _M_node != reinterpret_cast<_Node*>(__y._M_node); }

  private:
    void _M_incr()
      { _M_node = static_cast<_Node*>(_M_node->_M_next); }
    void _M_decr()
      { _M_node = static_cast<_Node*>(_M_node->_M_prev); }

    _Node* _M_node;

    template <class _TTp, class _Alloc>
    friend class _STLP_STD::list;

    friend class _List_iterator<_Tp const>;
};

template <class _Tp>
class _List_iterator<_Tp const>
{
  public:
    typedef _Tp value_type;
    typedef value_type const* pointer;
    typedef value_type const& reference;
    typedef bidirectional_iterator_tag iterator_category;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

  private:
    typedef _List_iterator<value_type const> _Self;
    typedef _List_node<value_type const> _Node;

  public:
    _List_iterator() = default;
    _List_iterator(const _List_iterator&) = default;
    _List_iterator(_List_iterator&&) = default;
    _List_iterator(const _List_iterator<value_type>& x) :
        _M_node( reinterpret_cast<_Node*>(x._M_node) )
      { }

    explicit _List_iterator(_List_node_base* __x) :
        _M_node(static_cast<_Node*>(__x))
      { }

    _List_iterator& operator =( const _List_iterator& ) = default;
    _List_iterator& operator =( const _List_iterator<value_type>& x )
      {
        _M_node = reinterpret_cast<_Node*>(x._M_node);
        return *this;
      }

    reference operator *() const
      { return _M_node->_M_data; }

    const pointer operator ->() const
      { return &_M_node->_M_data; }

    _Self& operator ++()
      {
        _M_incr();
        return *this;
      }
    _Self operator ++(int)
      {
        _Self __tmp = *this;
        _M_incr();
        return __tmp;
      }
    _Self& operator --()
      {
        _M_decr();
        return *this;
      }
    _Self operator --(int)
      {
        _Self __tmp = *this;
        _M_decr();
        return __tmp;
      }

    bool operator ==(const _Self& __y ) const
      { return _M_node == __y._M_node; }
    bool operator ==(const _List_iterator<_Tp>& __y ) const
      { return _M_node == reinterpret_cast<_Node*>(__y._M_node); }
    bool operator !=(const _Self& __y ) const
      { return _M_node != __y._M_node; }
    bool operator !=(const _List_iterator<_Tp>& __y ) const
      { return _M_node != reinterpret_cast<_Node*>(__y._M_node); }

  private:
    void _M_incr()
      { _M_node = static_cast<_Node*>(_M_node->_M_next); }
    void _M_decr()
      { _M_node = static_cast<_Node*>(_M_node->_M_prev); }

    _Node* _M_node;

    template <class _TTp, class _Alloc>
    friend class _STLP_STD::list;

    friend class _List_iterator<_Tp>;
};

#if defined (_STLP_USE_PTR_SPECIALIZATIONS)
#  define list _STLP_PTR_IMPL_NAME(list)
#elif defined (_STLP_DEBUG)
#  define list _STLP_NON_DBG_NAME(list)
#else
_STLP_MOVE_TO_STD_NAMESPACE
#endif

template <class _Tp, class _Alloc = allocator<_Tp> >
class list
{
  private:
    typedef _STLP_PRIV _List_node<_Tp> _Node;
    typedef typename _Alloc::template rebind<_Node>::other _M_node_allocator_type;
    typedef _STLP_PRIV _List_node_base _Node_base;

    typedef _STLP_PRIV _STLP_alloc_proxy<_Node_base, _M_node_allocator_type> _AllocProxy;
    typedef list<_Tp,_Alloc> _Self;

  public:
    typedef typename remove_const<_Tp>::type value_type;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
    typedef _STLP_PRIV _List_iterator<typename remove_const<_Tp>::type> iterator;
    typedef _STLP_PRIV _List_iterator<typename add_const<_Tp>::type>  const_iterator;
    typedef size_t            size_type;
    typedef ptrdiff_t         difference_type;

    typedef _Alloc            allocator_type;
    typedef typename allocator_traits<allocator_type>::pointer pointer;
    typedef typename allocator_traits<allocator_type>::const_pointer const_pointer;

    typedef _STLP_STD::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef _STLP_STD::reverse_iterator<iterator> reverse_iterator;

  protected:
    _Node* _M_create_node(const value_type& __x)
      {
        _Node* __p = _M_head.allocate(1);
        _STLP_TRY {
          _Self::get_allocator().construct( &__p->_M_data, __x );
        }
        _STLP_UNWIND(_M_head.deallocate(__p, 1))
          return __p;
      }

    _Node* _M_create_node(value_type&& __x)
      {
        _Node* __p = _M_head.allocate(1);
        _STLP_TRY {
          _Self::get_allocator().construct( &__p->_M_data, _STLP_STD::move(__x) );
        }
        _STLP_UNWIND(_M_head.deallocate(__p, 1))
          return __p;
      }

    _Node* _M_create_node(size_type n, const value_type& __x)
      {
        _Node* __p = _M_head.allocate(n);
        _Node* __cur = __p;
        _STLP_TRY {
          while ( n > 0 ) {
            _Self::get_allocator().construct( &__cur->_M_data, __x );
            --n;
            ++__cur;
          }
        }
        _STLP_UNWIND(_M_head.deallocate(__p, n)) // ToDo: destroy [__p, __cur)
          return __p;
      }

    void _M_empty_initialize()
      { _M_head._M_data._M_prev = _M_head._M_data._M_next = &_M_head._M_data; }

  public:
    allocator_type get_allocator() const
      { return (const _M_node_allocator_type&)_M_head; }

    explicit list(const allocator_type& __a = allocator_type()) :
        _M_head(__a)
      { _M_empty_initialize(); }

    explicit list(size_type __n) :
        _M_head(allocator_type())
      {
        _M_empty_initialize();
        this->insert(begin(), __n, _STLP_DEFAULT_CONSTRUCTED(value_type));
      }

    list(size_type __n, const value_type& __val, const allocator_type& __a = allocator_type()) :
        _M_head(__a /* , _STLP_PRIV _List_node_base() */ )
      {
        _M_empty_initialize();
        this->insert(begin(), __n, __val);
      }

    // We don't need any dispatching tricks here, because insert does all of
    // that anyway.
    template <class _InputIterator>
    list(_InputIterator __first, _InputIterator __last, const allocator_type& __a = allocator_type()) :
        _M_head(__a /* , _STLP_PRIV _List_node_base() */ )
      {
        _M_empty_initialize();
        _M_insert(begin(), __first, __last);
      }

    list(const _Self& __x) :
        _M_head(__x.get_allocator() /* , _STLP_PRIV _List_node_base() */ )
      {
        _M_empty_initialize();
        _M_insert(begin(), __x.begin(), __x.end());
      }

    list(_Self&& __x) :
        _M_head(__x.get_allocator() /* , _STLP_PRIV _List_node_base() */ )
      {
        _Node_base& xnode = __x._M_head._M_data;
        _Node_base& node = _M_head._M_data;

        if (xnode._M_next == &xnode) { // __x.empty()
          //We force this to empty.
          node._M_next = node._M_prev = &node; // _M_empty_initialize();
        } else {
          node._M_next = xnode._M_next;
          node._M_prev = xnode._M_prev;
          node._M_next->_M_prev = node._M_prev->_M_next = &node;
          xnode._M_next = xnode._M_prev = &xnode; // __x._M_empty_initialize();
        }
     }

    list(const _Self& __x, const allocator_type& __a) :
        _M_head(__a /* , _STLP_PRIV _List_node_base() */ )
      {
        _M_empty_initialize();
        _M_insert(begin(), __x.begin(), __x.end());
      }

    list(_Self&& __x, const allocator_type& __a) :
        _M_head(__a /* , _STLP_PRIV _List_node_base() */ )
      {
        _Node_base& xnode = __x._M_head._M_data;
        _Node_base& node = _M_head._M_data;

        if (xnode._M_next == &xnode) { // __x.empty()
          //We force this to empty.
          node._M_next = node._M_prev = &node; // _M_empty_initialize();
        } else if (__a == __x.get_allocator()) { // check allocators equality?
          node._M_next = xnode._M_next;
          node._M_prev = xnode._M_prev;
          node._M_next->_M_prev = node._M_prev->_M_next = &node;
          xnode._M_next = xnode._M_prev = &xnode; // __x._M_empty_initialize();
        } else {
          node._M_next = node._M_prev = &node; // _M_empty_initialize();
          _M_insert(begin(), __x.begin(), __x.end());
          __x.clear();
        }
      }

    _Self& operator =(const _Self& __x);

    ~list()
      { clear(); }

    void clear();
    bool empty() const
      { return _M_head._M_data._M_next == &_M_head._M_data; }

    iterator begin()
      { return iterator(_M_head._M_data._M_next); }
    const_iterator begin() const
      { return const_iterator(_M_head._M_data._M_next); }

    iterator end()
      { return iterator(&_M_head._M_data); }
    const_iterator end() const
      { return const_iterator(const_cast<_STLP_PRIV _List_node_base*>(&_M_head._M_data)); }

    const_iterator cbegin() const
      { return const_iterator(_M_head._M_data._M_next); }
    const_iterator cend() const
      { return const_iterator(const_cast<_STLP_PRIV _List_node_base*>(&_M_head._M_data)); }

    reverse_iterator rbegin()
      { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const
      { return const_reverse_iterator(end()); }

    reverse_iterator rend()
      { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const
      { return const_reverse_iterator(begin()); }

    size_type size() const
      {
        size_type __result = _STLP_STD::distance(begin(), end());
        return __result;
      }
    size_type max_size() const
      { return size_type(-1); }

    reference front()
      { return *begin(); }
    const_reference front() const
      { return *cbegin(); }
    reference back()
      { return *(--end()); }
    const_reference back() const
      { return *(--cend()); }

    void swap(_Self& __x)
      {
        if ( get_allocator() != __x.get_allocator() ) {
          _STLP_STD::swap(static_cast<_M_node_allocator_type&>(_M_head),
                          static_cast<_M_node_allocator_type&>(__x._M_head));
        }
        _Node_base& xnode = __x._M_head._M_data;
        _Node_base& node = _M_head._M_data;
        if (xnode._M_next == &xnode) { // __x.empty()
          if (node._M_next == &node) { // this->empty()
            // Was not changed really, nothing to do.
            // node._M_prev = node._M_next = &node;
            // xnode._M_prev = xnode._M_next = &xnode;
            return;
          }
          xnode._M_next = _M_head._M_data._M_next;
          xnode._M_prev = _M_head._M_data._M_prev;
          xnode._M_next->_M_prev = xnode._M_prev->_M_next = &xnode;
          node._M_next = node._M_prev = &node; // _M_empty_initialize();
        } else if (node._M_next == &node) { // this->empty()
          node._M_next = xnode._M_next;
          node._M_prev = xnode._M_prev;
          node._M_next->_M_prev = node._M_prev->_M_next = &node;
          xnode._M_next = xnode._M_prev = &xnode; // __x._M_empty_initialize();
        } else {
          _STLP_STD::swap(node._M_next, xnode._M_next);
          _STLP_STD::swap(node._M_prev, xnode._M_prev);
          node._M_prev->_M_next = node._M_next->_M_prev = &node;
          xnode._M_prev->_M_next = xnode._M_next->_M_prev = &xnode;
        }
      }

    iterator insert(iterator __pos, const value_type& __x)
      {
        _Node_base* __tmp = _M_create_node(__x);
        _Node_base* __n = __pos._M_node;
        _Node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
      }

  private:
    template <class _InputIterator>
    void _M_insert(iterator __pos, _InputIterator __first, _InputIterator __last)
      {
        typedef typename is_integral<_InputIterator>::type _Integral;
        _M_insert_dispatch(__pos, __first, __last, _Integral());
      }

    // Check whether it's an integral type.  If so, it's not an iterator.
    template<class _Integer>
    void _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __x, const true_type& /*_IsIntegral*/)
      { _M_fill_insert(__pos, __n, __x); }
    template <class _InputIter>
    void _M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last,
                            const false_type& /*_IsIntegral*/)
      {
        //We use a temporary list to avoid the auto reference troubles (infinite loop)
        for (; __first != __last; ++__first) {
          insert(__pos, *__first);
        }
      }

  public:
    template <class _InputIterator>
    void insert(iterator __pos, _InputIterator __first, _InputIterator __last)
      {
        typedef typename is_integral<_InputIterator>::type _Integral;
        _M_splice_insert_dispatch(__pos, __first, __last, _Integral());
      }

  private:
    // Check whether it's an integral type.  If so, it's not an iterator.
    template<class _Integer>
    void _M_splice_insert_dispatch(iterator __pos, _Integer __n, _Integer __x, const true_type& /*_IsIntegral*/)
      { _M_fill_insert(__pos, __n, __x); }
    template <class _InputIter>
    void _M_splice_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last,
                                   const false_type& /*_IsIntegral*/)
      {
        //We use a temporary list to avoid the auto reference troubles (infinite loop)
        _Self __tmp(__first, __last, this->get_allocator());
        splice(__pos, __tmp);
      }

  public:
    void insert(iterator __pos, size_type __n, const value_type& __x)
      { _M_fill_insert(__pos, __n, __x); }

  private:
    void _M_fill_insert(iterator __pos, size_type __n, const_reference __x)
      {
        for ( ; __n > 0; --__n) {
          insert(__pos, __x);
        }
      }

  public:
    void push_front(const_reference __x)
      { insert(begin(), __x); }
    void push_back (const_reference __x)
      { insert(end(), __x); }

    iterator erase(iterator __pos)
      {
        _Node_base* __next_node = __pos._M_node->_M_next;
        _Node_base* __prev_node = __pos._M_node->_M_prev;
        _Node* __n = __STATIC_CAST(_Node*, __pos._M_node);
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        get_allocator().destroy( &__n->_M_data );
        _M_head.deallocate(__n, 1);
        return iterator(__next_node);
      }

    iterator erase(iterator __first, iterator __last)
      {
        while (__first != __last)
          erase(__first++);
        return __last;
      }

    void resize(size_type __new_size, const_reference __x);
    void resize(size_type __new_size)
      { this->resize(__new_size, _STLP_DEFAULT_CONSTRUCTED(value_type)); }

    void pop_front()
      { erase(begin()); }
    void pop_back()
      {
        iterator __tmp = end();
        erase(--__tmp);
      }

    // assign(), a generalized assignment member function.  Two
    // versions: one that takes a count, and one that takes a range.
    // The range version is a member template, so we dispatch on whether
    // or not the type is an integer.

    void assign(size_type __n, const_reference __val)
      { _M_fill_assign(__n, __val); }

    void _M_fill_assign(size_type __n, const_reference __val);

    template <class _InputIterator>
    void assign(_InputIterator __first, _InputIterator __last)
      {
        typedef typename is_integral<_InputIterator>::type _Integral;
        _M_assign_dispatch(__first, __last, _Integral());
      }

  private:
    template <class _Integer>
    void _M_assign_dispatch(_Integer __n, _Integer __val, const true_type& /*_IsIntegral*/)
      { _M_fill_assign(__n, __val); }

    template <class _InputIterator>
    void _M_assign_dispatch(_InputIterator __first2, _InputIterator __last2,
                            const false_type& /*_IsIntegral*/)
      {
        iterator __first1 = begin();
        iterator __last1 = end();
        for ( ; __first1 != __last1 && __first2 != __last2; ++__first1, ++__first2)
          *__first1 = *__first2;
        if (__first2 == __last2)
          erase(__first1, __last1);
        else
          insert(__last1, __first2, __last2);
      }

  public:
    void splice(iterator __pos, _Self& __x)
      {
        if (!__x.empty()) {
          if (this->get_allocator() == __x.get_allocator()) {
            _STLP_PRIV _List_global_inst::_Transfer(__pos._M_node, __x.begin()._M_node, __x.end()._M_node);
          } else {
            insert(__pos, __x.begin(), __x.end());
            __x.clear();
          }
        }
      }
    void splice(iterator __pos, _Self& __x, iterator __i)
      {
        iterator __j = __i;
        ++__j;
        if (__pos == __i || __pos == __j) return;
        if (this->get_allocator() == __x.get_allocator()) {
          _STLP_PRIV _List_global_inst::_Transfer(__pos._M_node, __i._M_node, __j._M_node);
        } else {
          insert(__pos, *__i);
          __x.erase(__i);
        }
      }
    void splice(iterator __pos, _Self& __x, iterator __first, iterator __last)
      {
        if (__first != __last) {
          if (this->get_allocator() == __x.get_allocator()) {
            _STLP_PRIV _List_global_inst::_Transfer(__pos._M_node, __first._M_node, __last._M_node);
          } else {
            insert(__pos, __first, __last);
            __x.erase(__first, __last);
          }
        }
      }

    void remove(const_reference __val)
      {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
          iterator __next = __first;
          ++__next;
          if (__val == *__first) erase(__first);
          __first = __next;
        }
      }

    void unique()
      { _Self::unique(equal_to<value_type>()); }

    void merge(_Self& __x)
      { _Self::merge(__x, less<value_type>()); }

    void reverse()
      {
        _Node_base* __p = &_M_head._M_data;
        _Node_base* __tmp = __p;
        do {
          _STLP_STD::swap(__tmp->_M_next, __tmp->_M_prev);
          __tmp = __tmp->_M_prev;     // Old next node is now prev.
        } while (__tmp != __p);
      }

    void sort()
      { _Self::sort(less<value_type>()); }

    template <class _Predicate>
    void remove_if(_Predicate __pred);

    template <class _BinaryPredicate>
    void unique(_BinaryPredicate __binary_pred);

    template <class _StrictWeakOrdering>
    void merge(_Self& __x, _StrictWeakOrdering __comp);

    template <class _StrictWeakOrdering>
    void sort(_StrictWeakOrdering __comp);

  private:
    _AllocProxy _M_head;
};

#if defined (list)
_STLP_MOVE_TO_STD_NAMESPACE
#  undef list
#else // list
#endif // list

_STLP_END_NAMESPACE

#include <stl/_list.c>

#if defined (_STLP_USE_PTR_SPECIALIZATIONS)
#  include <stl/pointers/_list.h>
#endif

#if defined (_STLP_DEBUG)
#  include <stl/debug/_list.h>
#endif

_STLP_BEGIN_NAMESPACE

template <class _Tp1, class _Alloc1, class _Tp2, class _Alloc2>
inline
bool _STLP_CALL operator ==(const list<_Tp1,_Alloc1>& _SL1, const list<_Tp2,_Alloc2>& _SL2)
{
  typedef typename list<_Tp1,_Alloc1>::const_iterator const_iterator1;
  typedef typename list<_Tp2,_Alloc2>::const_iterator const_iterator2;
  const_iterator1 __end1 = _SL1.end();
  const_iterator2 __end2 = _SL2.end();

  const_iterator1 __i1 = _SL1.begin();
  const_iterator2 __i2 = _SL2.begin();
  while (__i1 != __end1 && __i2 != __end2 && *__i1 == *__i2) {
    ++__i1;
    ++__i2;
  }
  return __i1 == __end1 && __i2 == __end2;
}

template <class _Tp1, class _Alloc1, class _Tp2, class _Alloc2>
inline
bool  _STLP_CALL operator <(const list<_Tp1,_Alloc1>& x, const list<_Tp2,_Alloc2>& y)
{ return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end()); }

template <class _Tp, class _Alloc>
inline void _STLP_CALL swap( list<_Tp,_Alloc>& x, list<_Tp,_Alloc>& y )
{ x.swap( y ); }

_STLP_END_NAMESPACE

#endif /* _STLP_INTERNAL_LIST_IMPL_H */

// Local Variables:
// mode:C++
// End:
