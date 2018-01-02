// -*- C++ -*- Time-stamp: <2012-04-16 20:51:04 ptr>

/*
 * Copyright (c) 2011
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

_STLP_BEGIN_NAMESPACE

template <class T>
struct default_delete
{
    /* constexpr */ default_delete() /* _STLP_NOEXCEPT */ = default;
    template <class U, class = typename enable_if<is_convertible<U*,T*>::value>::type>
    default_delete( const default_delete<U>& ) _STLP_NOEXCEPT
      { }
    void operator()( T* p ) const
      { delete p; }
};

template <class T>
struct default_delete<T[]>
{
    /* constexpr */ default_delete() /* _STLP_NOEXCEPT */ = default;
    void operator()( T* p ) const
      { delete [] p; }
    template <class U>
    void operator()(U*) const = delete;
};

template <class T> class shared_ptr;

template <class T, class... Args>
shared_ptr<T> make_shared( Args&&... args );

template <class T, class A, class... Args>
shared_ptr<T> allocate_shared( const A& a, Args&&... args );

template <class T, class U>
shared_ptr<T> static_pointer_cast( const shared_ptr<U>& r ) _STLP_NOEXCEPT;
template <class T, class U>
shared_ptr<T> dynamic_pointer_cast( const shared_ptr<U>& r ) _STLP_NOEXCEPT;
template <class T, class U>
shared_ptr<T> const_pointer_cast( const shared_ptr<U>& r ) _STLP_NOEXCEPT;

template <class D, class T>
D* get_deleter( const shared_ptr<T>& ) _STLP_NOEXCEPT;

namespace detail {

struct __shared_ref_base
{
    virtual void unlink() = 0;
    virtual void link() = 0;
    virtual long count() = 0;

    virtual void weak_unlink() = 0;
    virtual void weak_link() = 0;

    virtual __shared_ref_base* ref() = 0;

    virtual void* get_deleter()
      { return NULL; }
};

template <class T>
class __shared_ref :
    public __shared_ref_base
{
  public:
    __shared_ref( T* p ) :
        _p( p ),
        _n( 1 ),
        _w( 0 )
      { }

    virtual void unlink()
      {
        if ( --_n == 0 ) {
          // don't refer to members after 'delete _p;', due to
          // 'this' may be destroyed during 'delete _p;'.
          if ( _w > 0 ) {
            delete _p;
          } else {
            delete _p;
            delete this;
          }
        }
      }

    virtual void link()
      { ++_n; }

    virtual long count()
      { return _n; }

    virtual void weak_unlink()
      {
        if ( (--_w == 0) && (_n == 0) ) {
          delete this;
        }
      }

    virtual void weak_link()
      { ++_w; }

    virtual __shared_ref_base* ref()
      { return this; }

  protected:
    T* _p;
    long _n;
    long _w;
};

template <class Y>
class __alias_shared_ref :
        public __shared_ref_base
{
  public:
    __alias_shared_ref( __shared_ref_base* r ) :
        _parent( r == NULL ? NULL : r->ref() )
      {
        if ( _parent != NULL ) {
          _parent->link();
        }
      }

    virtual void unlink()
      {
        if ( _parent != NULL ) {
          _parent->unlink();
        }

        delete this;
      }

    virtual void link()
      {
        if ( _parent != NULL ) {
          _parent->link();
        }
      }

    virtual long count()
      { return _parent == NULL ? 0L : _parent->count(); }

    virtual void weak_unlink()
      {
        if ( _parent != NULL ) {
          _parent->weak_unlink();
        }

        delete this;
      }

    virtual void weak_link()
      {
        if ( _parent != NULL ) {
          _parent->weak_link();
        }
      }

    virtual __shared_ref_base* ref()
      { return _parent == NULL ? NULL : _parent->ref(); }

    // virtual void* get_deleter()
    //   { return _parent == NULL ? NULL : _parent->get_deleter(); }

  protected:
    __shared_ref_base* _parent;
};

template <class T, class D>
class __shared_ref_deleter :
    public __shared_ref<T>
{
  public:
    __shared_ref_deleter( T* p, D d ) :
        __shared_ref<T>( p ),
        _d( d )
      { }

    virtual void unlink()
      {
        if ( --__shared_ref<T>::_n == 0 ) {
          if ( __shared_ref<T>::_w > 0 ) {
          __shared_ref_deleter<T,D>::_d( __shared_ref<T>::_p );
          } else {
            __shared_ref_deleter<T,D>::_d( __shared_ref<T>::_p );
            delete this;
          }
        }
      }

    virtual void weak_unlink()
      {
        if ( (--__shared_ref<T>::_w == 0) && (__shared_ref<T>::_n == 0) ) {
          delete this;
        }
      }

    virtual void* get_deleter()
      { return reinterpret_cast<void*>(this); }

    // template <DD>
    // bool check_deleter() const
    //   { return is_convertible<DD*,D*>::value; }

  private:
    template <class DD, class TT> friend DD* _STLP_STD::get_deleter( const _STLP_STD::shared_ptr<TT>& ) _STLP_NOEXCEPT;
    D _d;
};

#if 0
template <class T, class D, bool>
__shared_ref_deleter<T,D>* _deleter_helper( T* p, D d )
{
  return new detail::__shared_ref_deleter<T,D>( p, r.get_deleter() );
}

template <class T, class D>
detail::__shared_ref_deleter<T,D>* _deleter_helper<T,D,true>( T* p, D d )
{
  return new detail::__shared_ref_deleter<T,D>( p, typename remove_reference<D>::type(d) );
}
#endif

template <class T, class D, class A>
class __shared_ref_alloc :
    public __shared_ref_deleter<T,D>
{
  public:
    __shared_ref_alloc( T* p, D d, A a ) :
        __shared_ref_deleter<T,D>( p, d ),
        _a( a )
      { }

    virtual void unlink()
      {
        if ( --__shared_ref<T>::_n == 0 ) {
          if ( __shared_ref<T>::_w > 0 ) {
            __shared_ref_deleter<T,D>::_d( __shared_ref<T>::_p );
          } else {
            __shared_ref_deleter<T,D>::_d( __shared_ref<T>::_p );
            _a.deallocate( this, 1 );
          }
        }
      }

    virtual void weak_unlink()
      {
        if ( (--__shared_ref<T>::_w == 0) && (__shared_ref<T>::_n == 0) ) {
          _a.deallocate( this, 1 );
        }
      }

  private:
    A _a;
};

template <class T, class A>
class __shared_ref_intrusive :
    public __shared_ref_base
{
  private:
    typedef typename A::template rebind<__shared_ref_intrusive>::other allocator_type;

  public:
    template <class... Args >
    __shared_ref_intrusive( A a, Args ... args ) :
        _p( args... ),
        _n( 1 ),
        _w( 0 ),
        _a( _STLP_STD::move( a ) )
      { }

    virtual void unlink()
      {
        if ( --_n == 0 ) {
          if ( _w > 0 ) {
            _p.~T();
          } else {
            _p.~T();
            _a.deallocate( this, 1 );
          }
        }
      }

    virtual void link()
      { ++_n; }

    virtual long count()
      { return _n; }

    virtual void weak_unlink()
      {
        if ( (--_w == 0) && (_n == 0) ) {
          _a.deallocate( this, 1 );
        }
      }

    virtual void weak_link()
      { ++_w; }

    virtual __shared_ref_base* ref()
      { return this; }

  protected:
    friend class shared_ptr<T>;

    T _p;
    long _n;
    long _w;
    allocator_type _a;
};

} // namespace detail

template <class T> class weak_ptr;

template <class T, class D = default_delete<T> >
class unique_ptr
{
  public:
    typedef typename detail::__pointer_type2<
       is_same<true_type,
               decltype(detail::__has_type_selector::__test_p<typename remove_reference<D>::type>(0))
              >::value,
       D,T>::pointer pointer;
    typedef T element_type;
    typedef D deleter_type;

    // 20.7.1.2.1, constructors
    /* constexpr */ unique_ptr() _STLP_NOEXCEPT :
        _p( nullptr )
      { }

    explicit unique_ptr( pointer p ) _STLP_NOEXCEPT :
        _p( p )
      { }

    unique_ptr( pointer p,
                typename conditional<!is_reference<D>::value,
                                     typename add_const<typename add_lvalue_reference<D>::type>::type,
                                     D>::type d2 ) _STLP_NOEXCEPT :
        _p( p ),
        _d( d2 ) // _d( _STLP_STD::forward( d2 ) ) // copy ctor
      { }

    unique_ptr( pointer p,
                typename conditional<!is_reference<D>::value,
                                     typename remove_const<typename add_rvalue_reference<D>::type>::type,
                                     typename add_rvalue_reference<D>::type>::type d2 ) _STLP_NOEXCEPT :
        _p( p ),
        _d( _STLP_STD::move(d2) )
      { }

    unique_ptr( unique_ptr&& u ) _STLP_NOEXCEPT :
        _p( _STLP_STD::move( u._p ) ),
        _d( _STLP_STD::forward( u._d ) )
      { u._p = NULL; /* ? */ }

    /* constexpr */ unique_ptr( nullptr_t ) _STLP_NOEXCEPT :
        _p( nullptr )
      { }

    template <class U, class E,
              class = typename enable_if<
                is_convertible<typename unique_ptr<U, E>::pointer,pointer>::value &&
                !is_array<U>::value &&
                ((is_reference<D>::value && is_same<E,D>::value) ||
                 (!is_reference<D>::value && is_convertible<E,D>::value))>::type>
    unique_ptr( unique_ptr<U, E>&& u ) _STLP_NOEXCEPT :
        _p( _STLP_STD::move( u._p ) ),
        _d( _STLP_STD::forward( u._d ) )
    { u._p = nullptr; /* ? */ }

    template <class U>
    unique_ptr( auto_ptr<U>&& u ) _STLP_NOEXCEPT :
        _p( u.release() )
      { }

    // 20.7.1.2.2, destructor
    ~unique_ptr()
      { if ( _p != nullptr ) { _d(_p); } }

    // 20.7.1.2.3, assignment
    unique_ptr& operator =( unique_ptr&& u ) _STLP_NOEXCEPT
      {
        reset( u.release() );
        _d = _STLP_STD::forward<D>(u.get_deleter());
        return *this;
      }

    template <class U, class E, class = typename enable_if<
                is_convertible<typename unique_ptr<U, E>::pointer,pointer>::value &&
                !is_array<U>::value &&
                ((is_reference<D>::value && is_same<E,D>::value) ||
                 (!is_reference<D>::value && is_convertible<E,D>::value))>::type>
    unique_ptr& operator =(unique_ptr<U, E>&& u) _STLP_NOEXCEPT
      {
        reset( u.release() );
        _d = _STLP_STD::forward<D>(u.get_deleter());
        return *this;
      }

    unique_ptr& operator =( nullptr_t ) _STLP_NOEXCEPT
      {
        reset();
        return *this;
      }

    // 20.7.1.2.4, observers
    typename add_lvalue_reference<T>::type operator*() const
      { return *get(); }

    pointer operator->() const _STLP_NOEXCEPT
      { return get(); }

    pointer get() const _STLP_NOEXCEPT
      { return _p; }

    deleter_type& get_deleter() _STLP_NOEXCEPT
      { return _d; }

    const deleter_type& get_deleter() const _STLP_NOEXCEPT
      { return _d; }

    explicit operator bool() const _STLP_NOEXCEPT
      { return _p != nullptr; }

    // 20.7.1.2.5 modifiers
    pointer release() _STLP_NOEXCEPT
      {
        pointer tmp = _p;
        _p = nullptr;
        return tmp;
      }

    void reset( pointer p = pointer() ) _STLP_NOEXCEPT
      {
        pointer tmp = _p;
        _p = p;
        if ( tmp != nullptr ) {
          _d( tmp );
        }
      }

    void reset( nullptr_t ) _STLP_NOEXCEPT
      {
        if ( _p != nullptr ) {
          _d( _p );
        }
        _p = nullptr;
      }

    void swap( unique_ptr& u ) _STLP_NOEXCEPT
      { swap( _p, u._p ); swap( _d, u._d ); }

    // disable copy from lvalue
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator =( const unique_ptr& ) = delete;

  protected:
    pointer _p;
    deleter_type _d;
};

template <class T, class D>
class unique_ptr<T[], D>
{
  public:
    typedef typename detail::__pointer_type2<
       is_same<true_type,
               decltype(detail::__has_type_selector::__test_p<typename remove_reference<D>::type>(0))
              >::value,
       D,T>::pointer pointer;
    typedef T element_type;
    typedef D deleter_type;

    // 20.7.1.3.1, constructors
    /* constexpr */ unique_ptr() _STLP_NOEXCEPT :
        _p( nullptr )
      { }

    explicit unique_ptr( pointer p ) _STLP_NOEXCEPT :
        _p( p )
      { }

    template <class U>
    explicit unique_ptr( U ) = delete; /* noexcept */

    unique_ptr(pointer p, typename conditional<!is_reference<D>::value,
                                               typename add_const<typename add_lvalue_reference<D>::type>::type,
                                     D>::type d2 ) _STLP_NOEXCEPT :
        _p( p ),
        _d( d2 ) // _d( _STLP_STD::forward( d2 ) ) // copy ctor
      { }

    template <class U>
    unique_ptr(U, typename conditional<!is_reference<D>::value,
                                         typename add_const<typename add_lvalue_reference<D>::type>::type,
                                         D>::type ) = delete; /* noexcept */

    unique_ptr(pointer p, typename conditional<!is_reference<D>::value,
                                               typename remove_const<typename add_rvalue_reference<D>::type>::type,
                                               typename add_rvalue_reference<D>::type>::type d2 ) _STLP_NOEXCEPT :
        _p( p ),
        _d( _STLP_STD::move(d2) )
      { }

    template <class U>
    unique_ptr(U, typename conditional<!is_reference<D>::value,
                                         typename remove_const<typename add_rvalue_reference<D>::type>::type,
                                         typename add_rvalue_reference<D>::type>::type ) = delete; /* noexcept */

    unique_ptr( unique_ptr&& u ) _STLP_NOEXCEPT :
        _p( _STLP_STD::move( u._p ) ),
        _d( _STLP_STD::forward( u._d ) )
      { u._p = NULL; /* ? */ }

    /* constexpr */ unique_ptr( nullptr_t ) _STLP_NOEXCEPT :
        _p( nullptr )
      { }

    // destructor
    ~unique_ptr()
      { if ( _p != nullptr ) { _d(_p); } }

    // assignment
    unique_ptr& operator =( unique_ptr&& u ) _STLP_NOEXCEPT
      {
        reset( u.release() );
        _d = _STLP_STD::forward<D>(u.get_deleter());
        return *this;
      }

    unique_ptr& operator =( nullptr_t ) _STLP_NOEXCEPT
      {
        reset();
        return *this;
      }

    // 20.7.1.3.2, observers
    T& operator []( size_t i ) const
      { return _p[i]; }

    pointer get() const _STLP_NOEXCEPT
      { return _p; }

    deleter_type& get_deleter() _STLP_NOEXCEPT
      { return _d; }

    const deleter_type& get_deleter() const _STLP_NOEXCEPT
      { return _d; }

    explicit operator bool() const _STLP_NOEXCEPT
      { return _p != nullptr; }

    // 20.7.1.3.3 modifiers
    pointer release() _STLP_NOEXCEPT
      {
        pointer tmp = _p;
        _p = nullptr;
        return tmp;
      }

    void reset( pointer p = pointer() ) _STLP_NOEXCEPT
      {
        pointer tmp = _p;
        _p = p;
        if ( tmp != nullptr ) {
          _d( tmp );
        }
      }

    void reset( nullptr_t ) _STLP_NOEXCEPT
      {
        if ( _p != nullptr ) {
          _d( _p );
        }
        _p = nullptr;
      }

    template <class U>
    void reset(U) = delete;

    void swap( unique_ptr& u ) _STLP_NOEXCEPT
      { swap( _p, u._p ); swap( _d, u._d ); }

    // disable copy from lvalue
    unique_ptr( const unique_ptr& ) = delete;
    unique_ptr& operator =( const unique_ptr& ) = delete;

  protected:
    pointer _p;
    deleter_type _d;
};

template <class T, class D>
void swap( unique_ptr<T, D>& x, unique_ptr<T, D>& y ) /* noexcept */
{ x.swap(y); }

template <class T1, class D1, class T2, class D2>
bool operator ==( const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y )
{ return x.get() == y.get(); }

template <class T1, class D1, class T2, class D2>
bool operator !=( const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y )
{ return x.get() != y.get(); }

template <class T1, class D1, class T2, class D2>
bool operator <( const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y )
{ return less<typename common_type<typename unique_ptr<T1, D1>::pointer, typename unique_ptr<T2, D2>::pointer>::type>(x.get(), y.get()); }

template <class T1, class D1, class T2, class D2>
bool operator <=( const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y )
{ return !(y < x); }

template <class T1, class D1, class T2, class D2>
bool operator >( const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y )
{ return y < x; }

template <class T1, class D1, class T2, class D2>
bool operator >=( const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y )
{ return !(x < y); }

template <class T, class D>
bool operator ==(const unique_ptr<T, D>& x, nullptr_t) _STLP_NOEXCEPT
{ return !x; }

template <class T, class D>
bool operator ==(nullptr_t, const unique_ptr<T, D>& x) _STLP_NOEXCEPT
{ return !x; }

template <class T, class D>
bool operator !=(const unique_ptr<T, D>& x, nullptr_t) _STLP_NOEXCEPT
{ return (bool)x; }

template <class T, class D>
bool operator !=(nullptr_t, const unique_ptr<T, D>& x) _STLP_NOEXCEPT
{ return (bool)x; }

// Fix required: provide strict weak ordering
template <class T, class D>
bool operator <(const unique_ptr<T, D>& x, nullptr_t)
{ return less<typename unique_ptr<T, D>::pointer>()(x.get(), nullptr); }

// Fix required: provide strict weak ordering
template <class T, class D>
bool operator <(nullptr_t, const unique_ptr<T, D>& x)
{ return less<typename unique_ptr<T, D>::pointer>()(nullptr, x.get()); }

template <class T, class D>
bool operator >(const unique_ptr<T, D>& x, nullptr_t)
{ return nullptr < x; }

template <class T, class D>
bool operator >(nullptr_t, const unique_ptr<T, D>& x)
{ return x < nullptr; }

template <class T, class D>
bool operator <=(const unique_ptr<T, D>& x, nullptr_t)
{ return !(nullptr < x); }

template <class T, class D>
bool operator <=(nullptr_t, const unique_ptr<T, D>& x)
{ return !(x < nullptr); }

template <class T, class D>
bool operator >=(const unique_ptr<T, D>& x, nullptr_t)
{ return !(x < nullptr); }

template <class T, class D>
bool operator >=(nullptr_t, const unique_ptr<T, D>& x)
{ return !(nullptr < x); }


class _STLP_DECLSPEC bad_weak_ptr :
    public _STLP_STD::__Named_exception
{
  public:
    bad_weak_ptr() _STLP_NOEXCEPT;
};

template <class T> class enable_shared_from_this;

namespace detail {

template <class T,bool> struct __enable_shared_from_this;
template <class T> struct __enable_shared_from_this<T,true>;

} // namespace detail

template <class T>
class shared_ptr
{
  public:
    typedef T element_type;

    // 20.7.2.2.1, constructors:
    /* constexpr */ shared_ptr() _STLP_NOEXCEPT :
        _ref( NULL )
      { }

    template <class Y, class = typename enable_if<is_convertible<Y*,T*>::value>::type>
    explicit shared_ptr( Y* p )
      {
        try {
          _p = static_cast<T*>(p);
          _ref = new detail::__shared_ref<T>(_p);
          detail::__enable_shared_from_this<T,is_base_of<enable_shared_from_this<T>,T>::value>::pass( *this );
        }
        catch ( ... ) {
          delete p;
          _ref = NULL;
          throw;
        }
      }

    template <class Y, class D,
              class = typename enable_if<is_convertible<Y*,T*>::value>::type>
    shared_ptr( Y* p, D d )
      {
        static_assert( is_copy_constructible<D>::value, "deleter is not copy constructable" );
        try {
          _p = static_cast<T*>(p);
          _ref = new detail::__shared_ref_deleter<T,D>( _p, d );
          detail::__enable_shared_from_this<T,is_base_of<enable_shared_from_this<T>,T>::value>::pass( *this );
        }
        catch ( ... ) {
          d( p );
          _ref = NULL;
          throw;
        }
      }

    template <class Y, class D, class A,
              class = typename enable_if<is_convertible<Y*,T*>::value>::type>
    shared_ptr( Y* p, D d, A a )
      {
        static_assert( is_copy_constructible<D>::value, "deleter is not copy constructable" );

        typedef typename A::template rebind<detail::__shared_ref_alloc<T,D,A> >::other allocator_type;
        allocator_type _a( _STLP_STD::move(a) );
        void* m;

        try {
          m = _a.allocate(1);
          _p = static_cast<T*>(p);
          _ref = new (m) detail::__shared_ref_alloc<T,D,A>( _p, d, _STLP_STD::move(_a) );
          detail::__enable_shared_from_this<T,is_base_of<enable_shared_from_this<T>,T>::value>::pass( *this );
        }
        catch ( ... ) {
          d( p );
          _a.deallocate( m, 1 );
          _ref = NULL;
          throw;
        }
      }

    template <class D>
    shared_ptr( nullptr_t, D d )
      {
        static_assert( is_copy_constructible<D>::value, "deleter is not copy constructable" );
        try {
          _p = NULL;
          _ref = new detail::__shared_ref_deleter<T,D>( NULL, d );
        }
        catch ( ... ) {
          d( NULL );
          _ref = NULL;
          throw;
        }
      }

    template <class D, class A>
    shared_ptr( nullptr_t, D d, A a )
      {
        static_assert( is_copy_constructible<D>::value, "deleter is not copy constructable" );
        try {
          void* m = a.allocate(1);
          _p = NULL;
          _ref = new (m) detail::__shared_ref_alloc<T,D,A>( NULL, d, _STLP_STD::move(a) );
        }
        catch ( ... ) {
          d( NULL );
          a.deallocate();
          _ref = NULL;
          throw;
        }
      }

    template <class Y>
    shared_ptr( const shared_ptr<Y>& r, T* p ) _STLP_NOEXCEPT
      {
        try {
          _p = p;
          _ref = new detail::__alias_shared_ref<Y>( r._ref );
        }
        catch ( ... ) {
          _ref = NULL;
        }
      }

    shared_ptr( const shared_ptr& r ) _STLP_NOEXCEPT :
        _ref( r._ref == NULL ? NULL : r._ref->ref() )
      {
        if ( _ref != NULL ) {
          _p = r.get();
          _ref->link();
        } else {
          _p = NULL;
        }
      }

    template <class Y, class = typename enable_if<is_convertible<Y*,T*>::value>::type >
    shared_ptr( const shared_ptr<Y>& r ) _STLP_NOEXCEPT :
        _ref( r._ref == NULL ? NULL : r._ref->ref() )
      {
        // static_assert( is_convertible<Y*,T*>::value, "pointers not convertible" );

        if ( _ref != NULL ) {
          _p = r.get();
          _ref->link();
        } else {
          _p = NULL;
        }
      }

    shared_ptr( shared_ptr&& r ) _STLP_NOEXCEPT :
        _p( r._p ),
        _ref( r._ref )
      {
        r._p = NULL;
        r._ref = NULL;
      }

    template <class Y, class = typename enable_if<is_convertible<Y*,T*>::value>::type>
    shared_ptr( shared_ptr<Y>&& r ) _STLP_NOEXCEPT :
        _p( r._p ),
        _ref( r._ref )
      {
        r._p = NULL;
        r._ref = NULL;
      }

  private:
    template <class A>
    shared_ptr( detail::__shared_ref_intrusive<T,A>* r ) :
        _p( &r->_p ),
        _ref( r )
      { }

  public:

    template <class Y>
    explicit shared_ptr( const weak_ptr<Y>& r )
      {
        static_assert( is_convertible<Y*,T*>::value, "pointers not convertible" );

        if ( r.expired() ) {
          throw bad_weak_ptr();
        }
        _ref = r._ref->ref(); // r._ref not NULL here
        _p = r._p;
        _ref->link();
      }

    template <class Y>
    shared_ptr( auto_ptr<Y>&& r )
      {
        static_assert( is_convertible<Y*,T*>::value, "pointers not convertible" );

        try {
          _p = static_cast<T*>(r.get());
          _ref = new detail::__shared_ref<T>(_p);
          r.release();
        }
        catch ( ... ) {
          _ref = NULL;
          throw;
        }
      }

    template <class Y, class D>
    shared_ptr( unique_ptr<Y,D>&& r )
      {
        static_assert( is_convertible<typename unique_ptr<Y,D>::pointer,T*>::value, "pointers not convertible" );

        try {
          _p = static_cast<T*>(r.get());
          // if ( !is_reference<D>::value ) {
          _ref = new detail::__shared_ref_deleter<T,D>( _p, r.get_deleter() );
          // } else {
          //   _ref = new detail::__shared_ref_deleter<T,D>( _p, typename remove_reference<D>::type(r.get_deleter()) );
          // }
          r.release();
        }
        catch ( ... ) {
          _ref = NULL;
          throw;
        }
      }

    /* constexpr */ shared_ptr( nullptr_t ) _STLP_NOEXCEPT :
        _ref( NULL )
      { }

    // 20.7.2.2.2, destructor:
    ~shared_ptr()
      {
        if ( _ref != NULL ) {
          _ref->unlink();
        }
      }

    // 20.7.2.2.3, assignment:
    shared_ptr& operator =( const shared_ptr& r ) _STLP_NOEXCEPT
      { // shared_ptr(r).swap(*this);
        if ( _ref != r._ref ) {
          _p = r._p;
          if ( _ref != NULL ) {
            _ref->unlink();
          }
          _ref = r._ref;
          if ( _ref != NULL ) {
            _ref->link();
          }          
        }
        return *this;
      }

    template <class Y, class = typename enable_if<is_convertible<Y*,T*>::value>::type>
    shared_ptr& operator =(const shared_ptr<Y>& r) _STLP_NOEXCEPT
      {
        if ( _ref != r._ref ) {
          _p = static_cast<T*>(r._p);
          if ( _ref != NULL ) {
            _ref->unlink();
          }
          _ref = r._ref;
          if ( _ref != NULL ) {
            _ref->link();
          }          
        }
        return *this;
      }

    shared_ptr& operator =( shared_ptr&& r ) _STLP_NOEXCEPT
      {
        if ( _ref != NULL ) {
          _ref->unlink();
        }
        if ( _ref != r._ref ) {
          _p = r._p;
          _ref = r._ref;
        }
        r._ref = NULL;
        // r._p = NULL;
        return *this;
     }

    template<class Y, class = typename enable_if<is_convertible<Y*,T*>::value>::type>
    shared_ptr& operator =( shared_ptr<Y>&& r ) _STLP_NOEXCEPT
      {
        if ( _ref != NULL ) {
          _ref->unlink();
        }
        if ( _ref != r._ref ) {
          _p = static_cast<T*>(r._p);
          _ref = r._ref;
        }
        r._ref = NULL;
        // r._p = NULL;
        return *this;
      }

    template <class Y, class = typename enable_if<is_convertible<Y*,T*>::value>::type>
    shared_ptr& operator =( auto_ptr<Y>&& r )
      {
        static_assert( is_convertible<Y*,T*>::value, "pointers not convertible" );

        try {
          _p = static_cast<T*>(r.get());
          if ( _ref != NULL ) {
            _ref->unlink();
          }
          _ref = new detail::__shared_ref<T>(_p);
          r.release();
        }
        catch ( ... ) {
          _ref = NULL;
          throw;
        }
        return *this;
      }

    template <class Y, class D, class = typename enable_if<is_convertible<Y*,T*>::value>::type>
    shared_ptr& operator =( unique_ptr<Y, D>&& r )
      {
        if ( _ref != r._ref ) {
          _p = static_cast<T*>(r._p);
          if ( _ref != NULL ) {
            _ref->unlink();
          }
          _ref = r._ref;
          if ( _ref != NULL ) {
            _ref->link();
            _ref->weak_unlink();
          }
        } else if ( _ref != NULL ) {
          _ref->weak_unlink();
        }
        r._ref = NULL;
        // r._p = NULL;
        return *this;
      }

    // 20.7.2.2.4, modifiers:
    void swap( shared_ptr& r ) _STLP_NOEXCEPT
      {
        if ( _ref != r._ref ) {
          _STLP_STD::swap( _p, r._p );
          _STLP_STD::swap( _ref, r._ref );
        }
      }

    void reset() _STLP_NOEXCEPT
      { // shared_ptr().swap( *this );
        if ( _ref != NULL ) {
          _ref->unlink();
          _ref = NULL;
        }
      }

    template <class Y>
    void reset( Y* p )
      { shared_ptr(p).swap( *this ); }

    template <class Y, class D>
    void reset( Y* p, D d )
      { shared_ptr(p, d).swap( *this ); }

    template <class Y, class D, class A>
    void reset( Y* p, D d, A a )
      { shared_ptr(p, d, _STLP_STD::move(a)).swap( *this ); }

    // 20.7.2.2.5, observers:
    T* get() const _STLP_NOEXCEPT
      { return _p; }
    T& operator*() const _STLP_NOEXCEPT
      { return *_p; }
    T* operator->() const _STLP_NOEXCEPT
      { return get(); }
    long use_count() const _STLP_NOEXCEPT
      { return _ref == NULL ? 0L : _ref->count(); }
    bool unique() const _STLP_NOEXCEPT
      { return _ref == NULL ? false : _ref->count() == 1L; }

    explicit operator bool() const _STLP_NOEXCEPT
      { return _p != NULL; }

    template <class U>
    bool owner_before(shared_ptr<U> const& b) const;

    template <class U>
    bool owner_before(weak_ptr<U> const& b) const;

  private:
    template <class Y> friend class shared_ptr;
    template <class Y> friend class weak_ptr;
    template <class Y, class... Args> friend shared_ptr<Y> make_shared(Args&&... args);
    template <class Y, class A, class... Args> friend shared_ptr<Y> allocate_shared(const A& a, Args&&... args);
    template <class Y, class U> friend shared_ptr<Y> static_pointer_cast( const shared_ptr<U>& r ) _STLP_NOEXCEPT;
    template <class Y, class U> friend shared_ptr<Y> dynamic_pointer_cast( const shared_ptr<U>& r ) _STLP_NOEXCEPT;
    template <class Y, class U> friend shared_ptr<Y> const_pointer_cast( const shared_ptr<U>& r ) _STLP_NOEXCEPT;

    template <class D, class TT> friend D* get_deleter( const shared_ptr<TT>& p ) _STLP_NOEXCEPT;

    T* _p;
    detail::__shared_ref_base* _ref;
};

// specialization for void

template <>
class shared_ptr<void>
{
  public:
    typedef void element_type;

    // 20.7.2.2.1, constructors:
    /* constexpr */ shared_ptr() _STLP_NOEXCEPT :
        _ref( NULL )
      { }

    template <class Y>
    explicit shared_ptr( Y* p )
      {
        try {
          _p = static_cast<void*>(p);
          _ref = new detail::__shared_ref<void>(_p);
        }
        catch ( ... ) {
          delete p;
          _ref = NULL;
          throw;
        }
      }

    template <class Y, class D, class = typename enable_if<is_copy_constructible<D>::value>::type>
    shared_ptr( Y* p, D d )
      {
        try {
          _p = static_cast<void*>(p);
          _ref = new detail::__shared_ref_deleter<void,D>( _p, d );
        }
        catch ( ... ) {
          d( p );
          _ref = NULL;
          throw;
        }
      }

    template <class Y, class D, class A,
              class = typename enable_if<is_copy_constructible<D>::value>::type>
    shared_ptr( Y* p, D d, A a )
      {
        typedef typename A::template rebind<detail::__shared_ref_alloc<void,D,A> >::other allocator_type;
        allocator_type _a( _STLP_STD::move(a) );
        void* m;

        try {
          m = _a.allocate(1);
          _p = static_cast<void*>(p);
          _ref = new (m) detail::__shared_ref_alloc<void,D,A>( _p, d, _STLP_STD::move(_a) );
        }
        catch ( ... ) {
          d( p );
          _a.deallocate( m, 1 );
          _ref = NULL;
          throw;
        }
      }

    template <class D,
              class = typename enable_if<is_copy_constructible<D>::value>::type>
    shared_ptr( nullptr_t, D d )
      {
        try {
          _p = NULL;
          _ref = new detail::__shared_ref_deleter<void,D>( NULL, d );
        }
        catch ( ... ) {
          d( NULL );
          _ref = NULL;
          throw;
        }
      }

    template <class D, class A,
              class = typename enable_if<is_copy_constructible<D>::value>::type>
    shared_ptr( nullptr_t, D d, A a )
      {
        try {
          void* m = a.allocate(1);
          _p = NULL;
          _ref = new (m) detail::__shared_ref_alloc<void,D,A>( NULL, d, _STLP_STD::move(a) );
        }
        catch ( ... ) {
          d( NULL );
          a.deallocate();
          _ref = NULL;
          throw;
        }
      }

    template <class Y>
    shared_ptr( const shared_ptr<Y>& r, void* p ) _STLP_NOEXCEPT
      {
        try {
          _p = p;
          _ref = new detail::__alias_shared_ref<Y>( r._ref );
        }
        catch ( ... ) {
          _ref = NULL;
        }
      }

    shared_ptr( const shared_ptr& r ) _STLP_NOEXCEPT :
        _ref( r._ref == NULL ? NULL : r._ref->ref() )
      {
        if ( _ref != NULL ) {
          _p = r.get();
          _ref->link();
        } else {
          _p = NULL;
        }
      }

    template <class Y>
    shared_ptr( const shared_ptr<Y>& r ) _STLP_NOEXCEPT :
        _ref( r._ref == NULL ? NULL : r._ref->ref() )
      {
        // static_assert( is_convertible<Y*,T*>::value, "pointers not convertible" );

        if ( _ref != NULL ) {
          _p = r.get();
          _ref->link();
        } else {
          _p = NULL;
        }
      }

    shared_ptr( shared_ptr&& r ) _STLP_NOEXCEPT :
        _p( r._p ),
        _ref( r._ref )
      {
        r._p = NULL;
        r._ref = NULL;
      }

    template <class Y>
    shared_ptr( shared_ptr<Y>&& r ) _STLP_NOEXCEPT :
        _p( r._p ),
        _ref( r._ref )
      {
        r._p = NULL;
        r._ref = NULL;
      }

  private:
    template <class A>
    shared_ptr( detail::__shared_ref_intrusive<void,A>* r ) :
        _p( &r->_p ),
        _ref( r )
      { }

  public:

    template <class Y>
    explicit shared_ptr( const weak_ptr<Y>& r )
      {
        if ( r.expired() ) {
          throw bad_weak_ptr();
        }
        _ref = r._ref->ref(); // r._ref not NULL here
        _p = r._p;
        _ref->link();
      }

    template <class Y>
    shared_ptr( auto_ptr<Y>&& r )
      {
        try {
          _p = static_cast<void*>(r.get());
          _ref = new detail::__shared_ref<void>(_p);
          r.release();
        }
        catch ( ... ) {
          _ref = NULL;
          throw;
        }
      }

    template <class Y, class D>
    shared_ptr( unique_ptr<Y,D>&& r )
      {
        try {
          _p = static_cast<void*>(r.get());
          // if ( !is_reference<D>::value ) {
          _ref = new detail::__shared_ref_deleter<void,D>( _p, r.get_deleter() );
          // } else {
          //   _ref = new detail::__shared_ref_deleter<void,D>( _p, typename remove_reference<D>::type(r.get_deleter()) );
          // }
          r.release();
        }
        catch ( ... ) {
          _ref = NULL;
          throw;
        }
      }

    /* constexpr */ shared_ptr( nullptr_t ) _STLP_NOEXCEPT :
        _ref( NULL )
      { }
    
    // 20.7.2.2.2, destructor:
    ~shared_ptr()
      {
        if ( _ref != NULL ) {
          _ref->unlink();
        }
      }

    // 20.7.2.2.3, assignment:
    shared_ptr& operator =( const shared_ptr& r ) _STLP_NOEXCEPT
      { // shared_ptr(r).swap(*this);
        if ( _ref != r._ref ) {
          _p = r._p;
          if ( _ref != NULL ) {
            _ref->unlink();
          }
          _ref = r._ref;
          if ( _ref != NULL ) {
            _ref->link();
          }          
        }
        return *this;
      }

    template <class Y>
    shared_ptr& operator =(const shared_ptr<Y>& r) _STLP_NOEXCEPT
      {
        if ( _ref != r._ref ) {
          _p = static_cast<void*>(r._p);
          if ( _ref != NULL ) {
            _ref->unlink();
          }
          _ref = r._ref;
          if ( _ref != NULL ) {
            _ref->link();
          }          
        }
        return *this;
      }

    shared_ptr& operator =( shared_ptr&& r ) _STLP_NOEXCEPT
      {
        if ( _ref != NULL ) {
          _ref->unlink();
        }
        if ( _ref != r._ref ) {
          _p = r._p;
          _ref = r._ref;
        }
        r._ref = NULL;
        // r._p = NULL;
        return *this;
     }

    template<class Y>
    shared_ptr& operator =( shared_ptr<Y>&& r ) _STLP_NOEXCEPT
      {
        if ( _ref != NULL ) {
          _ref->unlink();
        }
        if ( _ref != r._ref ) {
          _p = static_cast<void*>(r._p);
          _ref = r._ref;
        }
        r._ref = NULL;
        // r._p = NULL;
        return *this;
      }

    template <class Y>
    shared_ptr& operator =( auto_ptr<Y>&& r )
      {
        try {
          _p = static_cast<void*>(r.get());
          if ( _ref != NULL ) {
            _ref->unlink();
          }
          _ref = new detail::__shared_ref<void>(_p);
          r.release();
        }
        catch ( ... ) {
          _ref = NULL;
          throw;
        }
        return *this;
      }

    template <class Y, class D>
    shared_ptr& operator =( unique_ptr<Y, D>&& r )
      {
        if ( _ref != r._ref ) {
          _p = static_cast<void*>(r._p);
          if ( _ref != NULL ) {
            _ref->unlink();
          }
          _ref = r._ref;
          if ( _ref != NULL ) {
            _ref->link();
            _ref->weak_unlink();
          }
        } else if ( _ref != NULL ) {
          _ref->weak_unlink();
        }
        r._ref = NULL;
        // r._p = NULL;
        return *this;
      }

    // 20.7.2.2.4, modifiers:
    void swap( shared_ptr& r ) _STLP_NOEXCEPT
      {
        if ( _ref != r._ref ) {
          _STLP_STD::swap( _p, r._p );
          _STLP_STD::swap( _ref, r._ref );
        }
      }

    void reset() _STLP_NOEXCEPT
      { // shared_ptr().swap( *this );
        if ( _ref != NULL ) {
          _ref->unlink();
          _ref = NULL;
        }
      }

    template <class Y>
    void reset( Y* p )
      { shared_ptr(p).swap( *this ); }

    template <class Y, class D>
    void reset( Y* p, D d )
      { shared_ptr(p, d).swap( *this ); }

    template <class Y, class D, class A>
    void reset( Y* p, D d, A a )
      { shared_ptr(p, d, _STLP_STD::move(a)).swap( *this ); }

    // 20.7.2.2.5, observers:
    void* get() const _STLP_NOEXCEPT
      { return _p; }
    void* operator->() const _STLP_NOEXCEPT
      { return get(); }
    long use_count() const _STLP_NOEXCEPT
      { return _ref == NULL ? 0L : _ref->count(); }
    bool unique() const _STLP_NOEXCEPT
      { return _ref == NULL ? false : _ref->count() == 1L; }

    explicit operator bool() const _STLP_NOEXCEPT
      { return _p != NULL; }

    template <class U>
    bool owner_before(shared_ptr<U> const& b) const;

    template <class U>
    bool owner_before(weak_ptr<U> const& b) const;

  private:
    template <class Y> friend class shared_ptr;
    template <class Y> friend class weak_ptr;
    template <class Y, class... Args> friend shared_ptr<Y> make_shared(Args&&... args);
    template <class Y, class A, class... Args> friend shared_ptr<Y> allocate_shared(const A& a, Args&&... args);
    template <class Y, class U> friend shared_ptr<Y> static_pointer_cast( const shared_ptr<U>& r ) _STLP_NOEXCEPT;
    template <class Y, class U> friend shared_ptr<Y> dynamic_pointer_cast( const shared_ptr<U>& r ) _STLP_NOEXCEPT;
    template <class Y, class U> friend shared_ptr<Y> const_pointer_cast( const shared_ptr<U>& r ) _STLP_NOEXCEPT;
    template <class D, class TT> friend D* get_deleter( const shared_ptr<TT>& p ) _STLP_NOEXCEPT;

    void* _p;
    detail::__shared_ref_base* _ref;
};

// 20.7.2.2.6, shared_ptr creation

template<class T, class A, class... Args>
shared_ptr<T> allocate_shared(const A& a, Args&&... args)
{
  typedef typename detail::__shared_ref_intrusive<T,A> iptr_type;
  typedef typename A::template rebind<iptr_type>::other allocator_type;

  allocator_type _a( _STLP_STD::move(a) );

  void* m = _a.allocate( 1 );
  try {
    iptr_type* ip = ::new (m) iptr_type( _a, _STLP_STD::forward<Args>(args)... );

    return shared_ptr<T>( ip );
  }
  catch ( ... ) {
    _a.deallocate( static_cast<iptr_type*>(m), 1 );
    throw;
  }
/*
  A _a( a );
  void* pv = _a.allocate( 1 );

  try {
    T* p = ::new(pv) T( _STLP_STD::forward<Args>( args ) ... );
    return shared_ptr<T>( p );
  }
  catch ( ... ) {
    _a.deallocate( 1, pv );
    throw;
  }
*/
}

template <class T, class... Args>
shared_ptr<T> make_shared(Args&&... args)
{
  typedef typename detail::__shared_ref_intrusive<T,_STLP_STD::allocator<typename _STLP_STD::remove_const<T>::type> > iptr_type;
  _STLP_STD::allocator<iptr_type> _a;

  void* m = _a.allocate( 1 );
  try {
    iptr_type* ip = ::new (m) iptr_type( _a, _STLP_STD::forward<Args>(args)... );

    return shared_ptr<T>( ip );
  }
  catch ( ... ) {
    _a.deallocate( static_cast<iptr_type*>(m), 1 );
    throw;
  }

  // return allocate_shared<T>(
  //   _STLP_STD::allocator<typename _STLP_STD::remove_const<T>::type>(),
  //   _STLP_STD::forward<Args>( args ) ... );
}

// 20.7.2.2.7, shared_ptr comparisons:
template<class T, class U>
bool operator ==(const shared_ptr<T>& a, const shared_ptr<U>& b) _STLP_NOEXCEPT
{ return a.get() == b.get(); }

template<class T, class U>
bool operator !=(const shared_ptr<T>& a, const shared_ptr<U>& b) _STLP_NOEXCEPT
{ return !(a == b); }

template<class T, class U>
bool operator <(const shared_ptr<T>& a, const shared_ptr<U>& b) _STLP_NOEXCEPT
{ return less<typename common_type<T*,U*>::type>()( a.get(), b.get() ); }

template<class T, class U>
bool operator >(const shared_ptr<T>& a, const shared_ptr<U>& b) _STLP_NOEXCEPT
{ return b < a; }

template<class T, class U>
bool operator <=(const shared_ptr<T>& a, const shared_ptr<U>& b) _STLP_NOEXCEPT
{ return !(a > b); }

template<class T, class U>
bool operator >=(const shared_ptr<T>& a, const shared_ptr<U>& b) _STLP_NOEXCEPT
{ return !(a < b); }

template <class T>
bool operator ==(const shared_ptr<T>& a, nullptr_t) _STLP_NOEXCEPT
{ return !(bool)a; }

template <class T>
bool operator ==(nullptr_t, const shared_ptr<T>& b) _STLP_NOEXCEPT
{ return !(bool)b; }

template <class T>
bool operator !=(const shared_ptr<T>& a, nullptr_t) _STLP_NOEXCEPT
{ return (bool)a; }

template <class T>
bool operator !=(nullptr_t, const shared_ptr<T>& b) _STLP_NOEXCEPT
{ return (bool)b; }

template <class T>
bool operator <(const shared_ptr<T>& a, nullptr_t) _STLP_NOEXCEPT
{ return less<T*>()( a.get(), nullptr ); }

template <class T>
bool operator <(nullptr_t, const shared_ptr<T>& b) _STLP_NOEXCEPT
{ return less<T*>()( nullptr, b.get() ); }

template <class T>
bool operator <=(const shared_ptr<T>& a, nullptr_t) _STLP_NOEXCEPT
{ return !(a > nullptr); }

template <class T>
bool operator <=(nullptr_t, const shared_ptr<T>& b) _STLP_NOEXCEPT
{ return !(nullptr > b); }

template <class T>
bool operator >(const shared_ptr<T>& a, nullptr_t) _STLP_NOEXCEPT
{ return less<T*>()( nullptr, a.get() ); }

template <class T>
bool operator >(nullptr_t, const shared_ptr<T>& b) _STLP_NOEXCEPT
{ return less<T*>()( b.get(), nullptr ); }

template <class T>
bool operator >=(const shared_ptr<T>& a, nullptr_t) _STLP_NOEXCEPT
{ return !(a < nullptr); }

template <class T>
bool operator >=(nullptr_t, const shared_ptr<T>& b) _STLP_NOEXCEPT
{ return !(nullptr < b); }

// 20.7.2.2.8, shared_ptr specialized algorithms:

template <class T>
void swap( shared_ptr<T>& a, shared_ptr<T>& b ) _STLP_NOEXCEPT
{ a.swap( b ); }

// 20.7.2.2.9, shared_ptr casts:

template <class T, class U>
shared_ptr<T> static_pointer_cast( const shared_ptr<U>& r ) _STLP_NOEXCEPT
{
  shared_ptr<T> p;

  if ( r._ref != NULL ) {
    p._p = static_cast<T*>(r.get());
    p._ref = r._ref->ref();
    p._ref->link();
  }

  return p;
}

template <class T, class U>
shared_ptr<T> dynamic_pointer_cast( const shared_ptr<U>& r ) _STLP_NOEXCEPT
{
  shared_ptr<T> p;

  if ( r._ref != NULL ) {
    p._p = dynamic_cast<T*>(r.get());
    p._ref = r._ref->ref();
    p._ref->link();
  }

  return p;
}

template <class T, class U>
shared_ptr<T> const_pointer_cast( const shared_ptr<U>& r ) _STLP_NOEXCEPT
{
  shared_ptr<T> p;

  if ( r._ref != NULL ) {
    p._p = const_cast<T*>(r.get());
    p._ref = r._ref->ref();
    p._ref->link();
  }

  return p;
}

// 20.7.2.2.10, shared_ptr get_deleter:

template <class D, class T>
D* get_deleter( const shared_ptr<T>& p ) _STLP_NOEXCEPT
{
  // try {
    if ( p._ref != NULL ) {
      void* d =  p._ref->get_deleter();
      if ( d != NULL ) {
        return &reinterpret_cast<detail::__shared_ref_deleter<T,D>*>(d)->_d;
      }
    }
    // }
  // catch ( /* const _STLP_STD::bad_cast& */ ... ) {
  // }
  return reinterpret_cast<D*>(NULL);
}

// 20.7.2.2.11, shared_ptr I/O:

template <class E, class T> class basic_ostream;

template <class E, class T, class Y>
basic_ostream<E,T>& operator <<( basic_ostream<E,T>& os, const shared_ptr<Y>& p )
{
  os << p.get();
  return os;
}

template<class T>
class weak_ptr
{
  public:
    typedef T element_type;

    // 20.7.2.3.1, constructors
    /* constexpr */ weak_ptr() _STLP_NOEXCEPT :
        _p( NULL ),
        _ref( NULL )
      { }

    template <class Y, class = typename enable_if<is_convertible<Y*,T*>::value>::type>
    weak_ptr( const shared_ptr<Y>& r ) _STLP_NOEXCEPT :
        _ref( r._ref == NULL ? NULL : r._ref->ref() )
      {
        if ( _ref != NULL ) {
          _p = r._p;
          _ref->weak_link();
        } else {
          _p = NULL;
        }
      }

    weak_ptr( const weak_ptr& r ) _STLP_NOEXCEPT :
        _ref( /* r._ref == NULL ? NULL : */ r.expired() ? NULL : r._ref->ref() )
      {
        if ( _ref != NULL ) {
          _p = r._p;
          _ref->weak_link();
        } else {
          _p = NULL;
        }
      }

    template <class Y, class = typename enable_if<is_convertible<Y*,T*>::value>::type>
    weak_ptr( const weak_ptr<Y>& r ) _STLP_NOEXCEPT :
        _ref( /* r._ref == NULL ? NULL : */ r.expired() ? NULL : r._ref->ref() )
      {
        if ( _ref != NULL ) {
          _p = r._p;
          _ref->weak_link();
        } else {
          _p = NULL;
        }
      }

    // 20.7.2.3.2, destructor
    ~weak_ptr()
      {
        if ( _ref != NULL ) {
          _ref->weak_unlink();
        }
      }

    // 20.7.2.3.3, assignment
    weak_ptr& operator =( const weak_ptr& r ) _STLP_NOEXCEPT
      { // weak_ptr(r).swap(*this);
        if ( _ref != r._ref ) {
          _p = r._p;
          if ( _ref != NULL ) {
            _ref->weak_unlink();
          }
          _ref = r._ref;
          if ( _ref != NULL ) {
            _ref->weak_link();
          }          
        }
        return *this;
      }

    template <class Y, class = typename enable_if<is_convertible<Y*,T*>::value>::type>
    weak_ptr& operator =( const weak_ptr<Y>& r ) _STLP_NOEXCEPT
      {
        if ( _ref != r._ref ) {
          _p = static_cast<T*>(r._p);
          if ( _ref != NULL ) {
            _ref->weak_unlink();
          }
          _ref = r._ref;
          if ( _ref != NULL ) {
            _ref->weak_link();
          }          
        }
        return *this;
      }

    template <class Y, class = typename enable_if<is_convertible<Y*,T*>::value>::type>
    weak_ptr& operator =( const shared_ptr<Y>& r ) _STLP_NOEXCEPT
      {
        if ( _ref != r._ref ) {
          _p = static_cast<T*>(r._p);
          if ( _ref != NULL ) {
            _ref->weak_unlink();
          }
          _ref = r._ref;
          if ( _ref != NULL ) {
            _ref->weak_link();
          }          
        }
        return *this;
      }

    // 20.7.2.3.4, modifiers
    void swap( weak_ptr& r ) _STLP_NOEXCEPT
      {
        if ( _ref != r._ref ) {
          _STLP_STD::swap( _p, r._p );
          _STLP_STD::swap( _ref, r._ref );
        }
      }

    void reset() _STLP_NOEXCEPT
      { // weak_ptr().swap( *this );
        if ( _ref != NULL ) {
          _ref->weak_unlink();
          _ref = NULL;
          // _p = NULL;
        }
      }

    // 20.7.2.3.5, observers
    long use_count() const _STLP_NOEXCEPT
      { return _ref == NULL ? 0L : _ref->count(); }

    bool expired() const _STLP_NOEXCEPT
      { return (_ref == NULL) || (_ref->count() == 0); }

    shared_ptr<T> lock() const _STLP_NOEXCEPT
      {
        if ( expired() ) {
          return shared_ptr<T>();
        }
        return shared_ptr<T>(*this);
      }

    template <class U>
    bool owner_before( shared_ptr<U> const& b );

    template <class U>
    bool owner_before( weak_ptr<U> const& b );

  private:
    template <class Y> friend class weak_ptr;
    template <class Y> friend class shared_ptr;

    T* _p;
    detail::__shared_ref_base* _ref;
};

// 20.7.2.3.6, specialized algorithms
template <class T>
void swap( weak_ptr<T>& a, weak_ptr<T>& b ) _STLP_NOEXCEPT
{ a.swap( b ); }

// 20.7.2.3.7, Class template owner_less  [util.smartptr.ownerless]

template <class T> struct owner_less;

template <class T>
struct owner_less<shared_ptr<T> >
{
    typedef bool result_type;
    typedef shared_ptr<T> first_argument_type;
    typedef shared_ptr<T> second_argument_type;

    bool operator()( shared_ptr<T> const& x, shared_ptr<T> const& y ) const
      { return x.owner_before(y); }
    bool operator()( shared_ptr<T> const& x, weak_ptr<T> const& y ) const
      { return x.owner_before(y); }
    bool operator()( weak_ptr<T> const& x, shared_ptr<T> const& y ) const
      { return x.owner_before(y); }
};

template <class T>
struct owner_less<weak_ptr<T> >
{
    typedef bool result_type;
    typedef weak_ptr<T> first_argument_type;
    typedef weak_ptr<T> second_argument_type;

    bool operator()( weak_ptr<T> const& x, weak_ptr<T> const& y ) const
      { return x.owner_before(y); }
    bool operator()( shared_ptr<T> const& x, weak_ptr<T> const& y ) const
      { return x.owner_before(y); }
    bool operator()( weak_ptr<T> const& x, shared_ptr<T> const& y ) const
      { return x.owner_before(y); }
};

namespace detail {

template <class T,bool>
struct __enable_shared_from_this
{
    static void pass( shared_ptr<T>& )
      { }
};

template <class T>
struct __enable_shared_from_this<T,true>
{
    static void pass( shared_ptr<T>& t )
      {
        static_cast<enable_shared_from_this<T>&>(*t).w = t;
      }
};

} // namespace detail

template <class T>
class enable_shared_from_this
{
  protected:
    /* constexpr */ enable_shared_from_this() _STLP_NOEXCEPT :
        w()
      { }
    enable_shared_from_this( const enable_shared_from_this&) _STLP_NOEXCEPT
      { }
    enable_shared_from_this& operator =( const enable_shared_from_this& ) _STLP_NOEXCEPT
      { return *this; }
    ~enable_shared_from_this()
      { }

  public:
    shared_ptr<T> shared_from_this()
      { return shared_ptr<T>(w); }
    shared_ptr<T const> shared_from_this() const
      { return shared_ptr<T const>(w); }

  private:
    friend class detail::__enable_shared_from_this<T,true>;
    weak_ptr<T> w;
};

_STLP_END_NAMESPACE
