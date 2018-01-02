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

#ifndef _STLP_INTERNAL_HASHTABLE_H
#define _STLP_INTERNAL_HASHTABLE_H

#  include <vector>
#  include <forward_list>

#ifndef _STLP_INTERNAL_ITERATOR_H
#  include <stl/_iterator.h>
#endif

#ifndef _STLP_INTERNAL_FUNCTION_BASE_H
#  include <stl/_function_base.h>
#endif

#ifndef _STLP_INTERNAL_ALGOBASE_H
#  include <stl/_algobase.h>
#endif

#ifndef _STLP_HASH_FUN_H
#  include <stl/_hash_fun.h>
#endif

/*
 * Hashtable class, used to implement the hashed associative containers
 * hash_set, hash_map, hash_multiset, hash_multimap,
 * unordered_set, unordered_map, unordered_multiset, unordered_multimap.
 */

_STLP_BEGIN_NAMESPACE

#if defined (_STLP_USE_TEMPLATE_EXPORT)
//Export of the classes used to represent buckets in the hashtable implementation.
#  if !defined (_STLP_USE_PTR_SPECIALIZATIONS)
//If pointer specialization is enabled vector<_Slist_node_base*> will use the void*
//storage type for which internal classes have already been exported.
_STLP_EXPORT_TEMPLATE_CLASS allocator<_STLP_PRIV _Slist_node_base*>;

_STLP_MOVE_TO_PRIV_NAMESPACE
_STLP_EXPORT_TEMPLATE_CLASS _STLP_alloc_proxy<_Slist_node_base**,
                                              allocator<_Slist_node_base*> >;
_STLP_EXPORT_TEMPLATE_CLASS _Vector_base<_Slist_node_base*,
                                         allocator<_Slist_node_base*> >;
_STLP_MOVE_TO_STD_NAMESPACE
#  endif

#  if defined (_STLP_DEBUG)
_STLP_MOVE_TO_PRIV_NAMESPACE
#    define _STLP_NON_DBG_VECTOR _STLP_NON_DBG_NAME(vector)
_STLP_EXPORT_TEMPLATE_CLASS __construct_checker<_STLP_NON_DBG_VECTOR<_Slist_node_base*, allocator<_Slist_node_base*> > >;
_STLP_EXPORT_TEMPLATE_CLASS _STLP_NON_DBG_VECTOR<_Slist_node_base*, allocator<_Slist_node_base*> >;
#    undef _STLP_NON_DBG_VECTOR
_STLP_MOVE_TO_STD_NAMESPACE
#  endif

_STLP_EXPORT_TEMPLATE_CLASS vector<_STLP_PRIV _Slist_node_base*,
                                   allocator<_STLP_PRIV _Slist_node_base*> >;
#endif

#if defined (_STLP_DEBUG)
#  define hashtable _STLP_NON_DBG_NAME(hashtable)
#endif

_STLP_MOVE_TO_PRIV_NAMESPACE

template <class _Dummy>
class _Stl_prime {
  // Returns begining of primes list and size by reference.
  static const size_t* _S_primes(size_t&);
public:
  //Returns the maximum number of buckets handled by the hashtable implementation
  static size_t _STLP_CALL _S_max_nb_buckets();

  //Returns the bucket size next to a required size
  static size_t _STLP_CALL _S_next_size(size_t);

  // Returns the bucket range containing sorted list of prime numbers <= __hint.
  static void _STLP_CALL _S_prev_sizes(size_t __hint, const size_t *&__begin, const size_t *&__end);
};

#if defined (_STLP_USE_TEMPLATE_EXPORT)
_STLP_EXPORT_TEMPLATE_CLASS _Stl_prime<bool>;
#endif

typedef _Stl_prime<bool> _Stl_prime_type;

// #if !defined (_STLP_DEBUG)
// _STLP_MOVE_TO_STD_NAMESPACE
// #endif

/*
 * Hashtables handle allocators a bit differently than other containers
 * do. If we're using standard-conforming allocators, then a hashtable
 * unconditionally has a member variable to hold its allocator, even if
 * it so happens that all instances of the allocator type are identical.
 * This is because, for hashtables, this extra storage is negligible.
 * Additionally, a base class wouldn't serve any other purposes; it
 * wouldn't, for example, simplify the exception-handling code.
 */
template <class _Val, class _Key, class _HF, class _ExK, class _EqK, class _All>
class hashtable
{
  private:
    typedef hashtable<_Val, _Key, _HF, _ExK, _EqK, _All> _Self;

  public:
    typedef _Key key_type;
    typedef _Val value_type;
    typedef _HF hasher;
    typedef _EqK key_equal;

    typedef size_t    size_type;
    typedef ptrdiff_t difference_type;

    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;

    hasher hash_funct() const { return _M_hash; }
    key_equal key_eq() const { return _M_equals; }

  private:
#if defined (_STLP_DEBUG)
    typedef _STLP_PRIV _STLP_NON_DBG_NAME(forward_list)<value_type, _All> _ElemsCont;
#else
    typedef forward_list<value_type, _All> _ElemsCont;
#endif
    typedef typename _ElemsCont::iterator _ElemsIte;
    typedef typename _ElemsCont::const_iterator _ElemsConstIte;

    typedef _STLP_PRIV _Slist_node_base _BucketType;
    typedef typename _All::template rebind<_BucketType*>::other _BucketAllocType;

  public:
    typedef _ElemsIte iterator;
    typedef _ElemsConstIte const_iterator;

  private:
  /*
   * We are going to use vector of _Slist_node_base pointers for 2 reasons:
   *  - limit code bloat, all hashtable instanciation use the same buckets representation.
   *  - avoid _STLP_DEBUG performance trouble: with a vector of iterator on forward_list the resize
   *    method would be too slow because the forward_list::splice_after method become linear on
   *    the number of iterators in the buckets rather than constant in time as the iterator
   *    has to be move from a forward_list to the other.
   */
#if defined (_STLP_DEBUG)
    typedef _STLP_PRIV _STLP_NON_DBG_NAME(vector)<_BucketType*, _BucketAllocType> _BucketVector;
#else
    typedef vector<_BucketType*, _BucketAllocType> _BucketVector;
#endif

    hasher                _M_hash;
    key_equal             _M_equals;
    _ElemsCont            _M_elems;
    _BucketVector         _M_buckets;
    size_type             _M_num_elements;
    float                 _M_max_load_factor;
    _STLP_KEY_TYPE_FOR_CONT_EXT(key_type)

    static const key_type& _M_get_key(const value_type& __val)
      {
        _ExK k;
        return k(__val);
      }
  public:
    typedef typename _ElemsCont::iterator local_iterator;
    typedef typename _ElemsCont::const_iterator const_local_iterator;

    typedef _All allocator_type;
    allocator_type get_allocator() const { return _M_elems.get_allocator(); }

    hashtable(size_type __n, const _HF& __hf, const _EqK&  __eql, const allocator_type& __a = allocator_type()) :
        _M_hash(__hf),
        _M_equals(__eql),
        _M_elems(__a),
        _M_buckets(__a),
        _M_num_elements(0),
        _M_max_load_factor(1.0f)
      { _M_initialize_buckets(__n); }

    hashtable(const _Self& __ht) :
        _M_hash(__ht._M_hash),
        _M_equals(__ht._M_equals),
        _M_elems(__ht.get_allocator()),
        _M_buckets(__ht.get_allocator()),
        _M_num_elements(0),
        _M_max_load_factor(1.0f)
      { _M_copy_from(__ht); }

#if !defined (_STLP_NO_MOVE_SEMANTIC)
    hashtable(__move_source<_Self> src) :
        _M_hash(src.get()._M_hash),
        _M_equals(src.get()._M_equals),
        _M_elems(__move_source<_ElemsCont>(src.get()._M_elems)),
        _M_buckets(__move_source<_BucketVector>(src.get()._M_buckets)),
        _M_num_elements(src.get()._M_num_elements),
        _M_max_load_factor(src.get()._M_max_load_factor)
      { }
#endif

    _Self& operator =(const _Self& __ht)
      {
        if (&__ht != this) {
          clear();
          _M_hash = __ht._M_hash;
          _M_equals = __ht._M_equals;
          _M_copy_from(__ht);
        }
        return *this;
      }

    ~hashtable()
      { clear(); }

    size_type size() const
      { return _M_num_elements; }
    size_type max_size() const
      { return size_type(-1); }
    bool empty() const
      { return size() == 0; }

    void swap(_Self& __ht)
      {
        _STLP_STD::swap(_M_hash, __ht._M_hash);
        _STLP_STD::swap(_M_equals, __ht._M_equals);
        _M_elems.swap(__ht._M_elems);
        _M_buckets.swap(__ht._M_buckets);
        _STLP_STD::swap(_M_num_elements, __ht._M_num_elements);
        _STLP_STD::swap(_M_max_load_factor, __ht._M_max_load_factor);
      }

    iterator begin()
      { return _M_elems.begin(); }
    iterator end()
      { return _M_elems.end(); }
    local_iterator begin(size_type __n)
      { return _ElemsIte(_M_buckets[__n]); }
    local_iterator end(size_type __n)
      { return _ElemsIte(_M_buckets[__n + 1]); }

    const_iterator begin() const
      { return __CONST_CAST(_ElemsCont&, _M_elems).begin(); }
    const_iterator end() const
      { return __CONST_CAST(_ElemsCont&, _M_elems).end(); }
    const_local_iterator begin(size_type __n) const
      { return _ElemsIte(_M_buckets[__n]); }
    const_local_iterator end(size_type __n) const
      { return _ElemsIte(_M_buckets[__n + 1]); }

  public:
    //The number of buckets is size() - 1 because the last bucket always contains
    //_M_elems.end() to make algo easier to implement.
    size_type bucket_count() const
      { return _M_buckets.size() - 1; }
    size_type max_bucket_count() const
      { return _STLP_PRIV _Stl_prime_type::_S_max_nb_buckets(); }
    size_type elems_in_bucket(size_type __bucket) const
      { return _STLP_STD::distance(_ElemsIte(_M_buckets[__bucket]), _ElemsIte(_M_buckets[__bucket + 1])); }

    _STLP_TEMPLATE_FOR_CONT_EXT
    size_type bucket(const _KT& __k) const
      { return _M_bkt_num_key(__k); }

    // hash policy
    float load_factor() const
      { return (float)size() / (float)bucket_count(); }
    float max_load_factor() const
      { return _M_max_load_factor; }
    void max_load_factor(float __z)
      {
        _STLP_STD::swap( _M_max_load_factor, __z );
        if ( __z > _M_max_load_factor ) { // max load was decreased
          _M_enlarge(size()); // ... have to enlarge
        } else { // We can try to reduce size
          _M_reduce();
        }
      }

    pair<iterator, bool> insert_unique(const value_type& __obj)
      {
        _M_enlarge(_M_num_elements + 1);
        return insert_unique_noresize(__obj);
      }

    iterator insert_equal(const value_type& __obj)
      {
        _M_enlarge(_M_num_elements + 1);
        return insert_equal_noresize(__obj);
      }

  protected:
    iterator _M_insert_noresize(size_type __n, const value_type& __obj);

  public:
    pair<iterator, bool> insert_unique_noresize(const value_type& __obj);
    iterator insert_equal_noresize(const value_type& __obj);

    template <class _InputIterator>
    void insert_unique(_InputIterator __f, _InputIterator __l)
      { insert_unique(__f, __l, typename iterator_traits<_InputIterator>::iterator_category()); }

    template <class _InputIterator>
    void insert_equal(_InputIterator __f, _InputIterator __l)
      { insert_equal(__f, __l, typename iterator_traits<_InputIterator>::iterator_category()); }

  private:
    template <class _InputIterator>
    void insert_unique(_InputIterator __f, _InputIterator __l, const input_iterator_tag&)
      { for ( ; __f != __l; ++__f) insert_unique(*__f); }

    template <class _InputIterator>
    void insert_equal(_InputIterator __f, _InputIterator __l, const input_iterator_tag&)
      { for ( ; __f != __l; ++__f) insert_equal(*__f); }

    template <class _ForwardIterator>
    void insert_unique(_ForwardIterator __f, _ForwardIterator __l, const forward_iterator_tag&)
      {
        size_type __n = _STLP_STD::distance(__f, __l);
        _M_enlarge(_M_num_elements + __n);
        for ( ; __n > 0; --__n, ++__f)
          insert_unique_noresize(*__f);
      }

    template <class _ForwardIterator>
    void insert_equal(_ForwardIterator __f, _ForwardIterator __l, const forward_iterator_tag&)
      {
        size_type __n = _STLP_STD::distance(__f, __l);
        _M_enlarge(_M_num_elements + __n);
        for ( ; __n > 0; --__n, ++__f)
          insert_equal_noresize(*__f);
      }

  private:
    _STLP_TEMPLATE_FOR_CONT_EXT
    _ElemsIte _M_find(const _KT& __key) const
      {
        size_type __n = _M_bkt_num_key(__key);
        _ElemsIte __first(_M_buckets[__n]);
        _ElemsIte __last(_M_buckets[__n + 1]);
        for ( ; (__first != __last) && !_M_equals(_M_get_key(*__first), __key); ++__first);
        if (__first != __last)
          return __first;
        else
          return __CONST_CAST(_ElemsCont&, _M_elems).end();
      }

    _STLP_TEMPLATE_FOR_CONT_EXT
    _ElemsIte _M_find(const _KT& __key)
      {
        size_type __n = _M_bkt_num_key(__key);
        _ElemsIte __first(_M_buckets[__n]);
        _ElemsIte __last(_M_buckets[__n + 1]);
        for ( ; (__first != __last) && !_M_equals(_M_get_key(*__first), __key); ++__first);
        if (__first != __last)
          return __first;
        else
          return __CONST_CAST(_ElemsCont&, _M_elems).end();
      }

  public:
    _STLP_TEMPLATE_FOR_CONT_EXT
    iterator find(const _KT& __key)
      { return _M_find(__key); }
    _STLP_TEMPLATE_FOR_CONT_EXT
    const_iterator find(const _KT& __key) const
      { return _M_find(__key); }

    _STLP_TEMPLATE_FOR_CONT_EXT
    size_type count(const _KT& __key) const
      {
        const size_type __n = _M_bkt_num_key(__key);

        _ElemsConstIte __cur(_M_buckets[__n]);
        _ElemsConstIte __last(_M_buckets[__n + 1]);
        for (; __cur != __last; ++__cur) {
          if (_M_equals(_M_get_key(*__cur), __key)) {
            size_type __result = 1;
            for (++__cur; __cur != __last && _M_equals(_M_get_key(*__cur), __key); ++__result, ++__cur)
              ;
            return __result;
          }
        }
        return 0;
      }

    _STLP_TEMPLATE_FOR_CONT_EXT
    pair<iterator, iterator> equal_range(const _KT& __key)
      {
        typedef pair<iterator, iterator> _Pii;
        const size_type __n = _M_bkt_num_key(__key);

        for (_ElemsIte __first(_M_buckets[__n]), __last(_M_buckets[__n + 1]); __first != __last; ++__first) {
          if (_M_equals(_M_get_key(*__first), __key)) {
            _ElemsIte __cur(__first);
            for (++__cur; (__cur != __last) && _M_equals(_M_get_key(*__cur), __key); ++__cur)
              ;
            return _Pii(__first, __cur);
          }
        }
        return _Pii(end(), end());
      }

    _STLP_TEMPLATE_FOR_CONT_EXT
    pair<const_iterator, const_iterator> equal_range(const _KT& __key) const
      {
        typedef pair<const_iterator, const_iterator> _Pii;
        const size_type __n = _M_bkt_num_key(__key);

        for (_ElemsConstIte __first(_M_buckets[__n]), __last(_M_buckets[__n + 1]); __first != __last; ++__first) {
          if (_M_equals(_M_get_key(*__first), __key)) {
            _ElemsConstIte __cur(__first);
            for (++__cur; (__cur != __last) && _M_equals(_M_get_key(*__cur), __key); ++__cur)
              ;
            return _Pii(__first, __cur);
          }
        }
        return _Pii(end(), end());
      }

    size_type erase(const key_type& __key);
    void erase(const_iterator __it);
    void erase(const_iterator __first, const_iterator __last);

  private:
    void _M_enlarge( size_type );
    void _M_reduce();
    void _M_resize();
    void _M_rehash(size_type __num_buckets);
#if defined (_STLP_DEBUG)
    void _M_check() const;
#endif

  public:
    void rehash(size_type __num_buckets_hint);
    void resize(size_type __num_buckets_hint)
      { rehash(__num_buckets_hint); }
    void clear();

    // this is for hash_map::operator[]
    reference _M_insert(const value_type& __obj);

  private:
    //__n is set to the first bucket that has to be modified if any
    //erase/insert operation is done after the returned iterator.
    iterator _M_before_begin(size_type &__n) const;

    static iterator _S_before_begin(const _ElemsCont& __elems, const _BucketVector& __buckets, size_type &__n);

    void _M_initialize_buckets(size_type __n)
      {
        const size_type __n_buckets = _STLP_PRIV _Stl_prime_type::_S_next_size(__n) + 1;
        _M_buckets.reserve(__n_buckets);
        _M_buckets.assign(__n_buckets, __STATIC_CAST(_BucketType*, 0));
      }

    _STLP_TEMPLATE_FOR_CONT_EXT
    size_type _M_bkt_num_key(const _KT& __key) const
      { return _M_bkt_num_key(__key, bucket_count()); }

    size_type _M_bkt_num(const value_type& __obj) const
      { return _M_bkt_num_key(_M_get_key(__obj)); }

    _STLP_TEMPLATE_FOR_CONT_EXT
    size_type _M_bkt_num_key(const _KT& __key, size_type __n) const
      { return _M_hash(__key) % __n; }

    size_type _M_bkt_num(const value_type& __obj, size_t __n) const
      { return _M_bkt_num_key(_M_get_key(__obj), __n); }

    void _M_copy_from(const _Self& __ht);
};

#if defined (_STLP_DEBUG)
#  undef hashtable
// _STLP_MOVE_TO_STD_NAMESPACE
#endif

_STLP_END_NAMESPACE

_STLP_END_NAMESPACE

#include <stl/_hashtable.c>

#if defined (_STLP_DEBUG)
#  include <stl/debug/_hashtable.h>
#endif

_STLP_BEGIN_NAMESPACE

template <class _Val, class _Key, class _HF, class _ExK, class _EqK, class _All>
inline
void _STLP_CALL
swap( _STLP_PRIV hashtable<_Val,_Key,_HF,_ExK,_EqK,_All>& __hm1, _STLP_PRIV hashtable<_Val,_Key,_HF,_ExK,_EqK,_All>& __hm2)
{ __hm1.swap(__hm2); }

_STLP_END_NAMESPACE

#endif /* _STLP_INTERNAL_HASHTABLE_H */

// Local Variables:
// mode:C++
// End:
