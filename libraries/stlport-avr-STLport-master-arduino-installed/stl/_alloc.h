/*
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

#ifndef _STLP_INTERNAL_ALLOC_H
#define _STLP_INTERNAL_ALLOC_H

#include <stdint.h>

#ifndef _STLP_CSTDDEF
#  include <cstddef>
#endif

#ifndef _STLP_INTERNAL_CSTDLIB
#  include <stl/_cstdlib.h>
#endif

#ifndef _STLP_CSTRING
#  include <cstring>
#endif

#include <new>

#ifndef __STLP_TYPE_TRAITS
#  include <type_traits>
#endif

#ifndef _STLP_UTILITY
#  include <utility>
#endif

#ifndef _STLP_INTERNAL_ITERATOR_BASE_H
#  include <stl/_iterator_base.h>
#endif

#include <limits>

#ifdef _STLP_AVR
#  undef max
#endif

_STLP_BEGIN_NAMESPACE

template <class T>
inline T* addressof(T& r) noexcept
{ return reinterpret_cast<T*>( &const_cast<char&>( reinterpret_cast<const volatile char&>(r) ) ); }

// 20.6.3, pointer traits
template <class Ptr> struct pointer_traits;
// template <class T> struct pointer_traits<T*>;

namespace detail {

struct __has_type_selector
{
    // T::element_type?
#ifdef __clang__
    template <class T>
    static typename remove_reference<decltype( sizeof(typename T::element_type*), declval<true_type>())>::type __test( int );
#else
    template <class T>
    static typename remove_reference<decltype( declval<typename T::element_type>(), declval<true_type>())>::type __test( int );
#endif

    template <class>
    static false_type __test( ... );

    // T::difference_type?
    template <class T>
    static typename T::difference_type __test_d( int );

    template <class>
    static typename _STLP_STD::ptrdiff_t __test_d( ... );

    // T::rebind<U>?
    template <class T, class U>
    static typename remove_reference<decltype( sizeof(typename T::template rebind<U>::type*), declval<true_type>())>::type __test_r( int );

    template <class, class>
    static false_type __test_r( ... );

    // T::allocator_type?
    template <class T>
    static typename remove_reference<decltype( declval<typename T::allocator_type>(), declval<true_type>())>::type __test_a( int );

    template <class>
    static false_type __test_a( ... );

    // T::pointer?
#ifdef __clang__
    template <class T>
    static typename remove_reference<decltype( sizeof(typename T::pointer), declval<true_type>())>::type __test_p( int );
#else
    template <class T>
    static typename remove_reference<decltype(T::pointer(), declval<true_type>())>::type __test_p( int );
#endif

    template <class>
    static false_type __test_p( ... );

    // T::const_pointer?
#ifdef __clang__
    template <class T>
    static typename remove_reference<decltype( sizeof(typename T::const_pointer), declval<true_type>())>::type __test_cp( int );
#else
    template <class T>
    static typename remove_reference<decltype(T::const_pointer(), declval<true_type>())>::type __test_cp( int );
#endif

    template <class>
    static false_type __test_cp( ... );

    // T::void_pointer?
#ifdef __clang__
    template <class T>
    static typename remove_reference<decltype( sizeof(typename T::void_pointer), declval<true_type>())>::type __test_vp( int );
#else
    template <class T>
    static typename remove_reference<decltype( declval<typename T::void_pointer>(), declval<true_type>())>::type __test_vp( int );
#endif

    template <class>
    static false_type __test_vp( ... );

    // T::const_void_pointer?
#ifdef __clang__
    template <class T>
    static typename remove_reference<decltype( sizeof(typename T::const_void_pointer), declval<true_type>())>::type __test_cvp( int );
#else
    template <class T>
    static typename remove_reference<decltype( declval<typename T::const_void_pointer>(), declval<true_type>())>::type __test_cvp( int );
#endif

    template <class>
    static false_type __test_cvp( ... );

    // T::difference_type, another?
    template <class T>
    static typename remove_reference<decltype( declval<typename T::difference_type>(), declval<true_type>())>::type __test_dp( int );

    template <class>
    static false_type __test_dp( ... );

    // T::size_type?
    template <class T>
    static typename remove_reference<decltype( declval<typename T::size_type>(), declval<true_type>())>::type __test_sz( int );

    template <class>
    static false_type __test_sz( ... );

    // T::propagate_on_container_copy_assignment?
    template <class T>
    static decltype( declval<typename T::propagate_on_container_copy_assignment>() ) __test_pcca( int );

    template <class>
    static false_type __test_pcca( ... );

    // T::propagate_on_container_move_assignment?
    template <class T>
    static typename remove_reference<decltype( declval<typename T::propagate_on_container_move_assignment>() )>::type __test_pcma( int );

    template <class>
    static false_type __test_pcma( ... );

    // T::propagate_on_container_swap?
    template <class T>
    static typename remove_reference<decltype( declval<typename T::propagate_on_container_swap>() )>::type __test_pcs( int );

    template <class>
    static false_type __test_pcs( ... );

    // T::rebind<U>::other?
    template <class T, class U>
    static typename remove_reference<decltype( declval<typename T::template rebind<U>::other>(), declval<true_type>())>::type __test_ro( int );

    template <class, class>
    static false_type __test_ro( ... );

    // a.allocate( 0, NULL )?
    template <class A>
    static typename remove_reference<decltype( declval<A>().allocate(0,NULL), declval<true_type>())>::type __test_ah( int );

    template <class>
    static false_type __test_ah( ... );

    // a.construct( p, args... )?
    template <class A, class T, class ... Args>
    static typename remove_reference<decltype( declval<A>().construct( declval<T*>(), declval<Args>()...), declval<true_type>())>::type __test_construct( int );

    template <class, class, class ...>
    static false_type __test_construct( ... );

    // a.destroy( p )?
    template <class A, class T>
    static decltype( declval<A>().destroy( declval<T*>() ), declval<true_type>()) __test_destroy( int );

    template <class, class>
    static false_type __test_destroy( ... );

    // a.max_size()?
    template <class A>
    static typename remove_reference<decltype( declval<A>().max_size(), declval<true_type>())>::type __test_max_size( int );

    template <class, class>
    static false_type __test_max_size( ... );

    // a.select_on_container_copy_construction()?
    template <class A>
    static typename remove_reference<decltype( declval<A>().select_on_container_copy_construction(), declval<true_type>())>::type __test_soccc( int );

    template <class, class>
    static false_type __test_soccc( ... );
};

template <bool, class T>
struct __element_type
{
};

template <class T>
struct __element_type<true,T>
{
    typedef typename T::element_type element_type;
};

template <bool, class T, class U>
struct __rebind_type
{
};

template <class T, class U>
struct __rebind_type<true,T,U>
{
    typedef typename T::template rebind<U>::type type;
};

template <bool, class P, class S, class V, class Alloc>
struct __allocate_hint
{
    static P allocate( Alloc& a, S s, V hint )
      { return a.allocate( s ); }
};

template <class P, class S, class V, class Alloc>
struct __allocate_hint<true,P,S,V,Alloc>
{
    static P allocate( Alloc& a, S s, V hint )
      { return a.allocate( s, hint ); }
};

template <bool, class Alloc, class T, class ... Args>
struct __construct_check
{
    static void construct( Alloc&, T* p, Args&&... args )
      { ::new (static_cast<void*>(p)) T( _STLP_STD::forward<Args>(args) ... ); }
};

template <class Alloc, class T, class ... Args>
struct __construct_check<true,Alloc,T,Args...>
{
    static void construct( Alloc& a, T* p, Args&&... args )
      { a.construct( p, _STLP_STD::forward<Args>(args) ... ); }
};

template <bool, class Alloc, class T>
struct __destroy_check
{
    static void destroy( Alloc&, T* p )
      { p->~T(); }
};

template <class Alloc, class T>
struct __destroy_check<true,Alloc,T>
{
    static void destroy( Alloc& a, T* p )
      { a.destroy( p ); }
};

template <bool, class Alloc, class S>
struct __max_size_check
{
    static S max_size( Alloc& )
      { return _STLP_STD::numeric_limits<S>::max(); }
};

template <class Alloc, class S>
struct __max_size_check<true,Alloc,S>
{
    static S max_size( Alloc& a )
      { return a.max_size(); }
};

template <bool, class Alloc>
struct __soccc_check
{
    static Alloc soccc( Alloc& a )
      { return a; }
};

template <class Alloc>
struct __soccc_check<true,Alloc>
{
    static Alloc soccc( Alloc& a )
      { return a.select_on_container_copy_construction(); }
};

} // detail

template <class Ptr>
struct pointer_traits
{
    typedef Ptr pointer;
    typedef typename detail::__element_type<is_same<true_type,decltype(detail::__has_type_selector::__test<Ptr>(0))>::value,Ptr>::element_type element_type;
    typedef decltype(detail::__has_type_selector::__test_d<Ptr>(0)) difference_type;
#ifndef _STLP_NO_ALIAS_TEMPLATES
    template <class U> using rebind = typename detail::__rebind_type<is_same<true_type,decltype(detail::__has_type_selector::__test_r<Ptr,U>(0))>::value,Ptr,U>::type;
#else // _STLP_NO_ALIAS_TEMPLATES
    template <class U>
    struct rebind
    {
        typedef typename detail::__rebind_type<is_same<true_type,decltype(detail::__has_type_selector::__test_r<Ptr,U>(0))>::value,Ptr,U>::type type;
    };
#endif // _STLP_NO_ALIAS_TEMPLATES

    static pointer pointer_to( element_type& r )
      { return Ptr::pointer_to(r); }
};

template <class T>
struct pointer_traits<T*>
{
    typedef T* pointer;
    typedef T element_type;
    typedef ptrdiff_t difference_type;
#ifndef _STLP_NO_ALIAS_TEMPLATES
    template <class U> using rebind = U*;
#else // _STLP_NO_ALIAS_TEMPLATES
    template <class U>
    struct rebind
    {
        typedef U* type;
    };
#endif // _STLP_NO_ALIAS_TEMPLATES

    static pointer pointer_to( T& r ) noexcept
      { return _STLP_STD::addressof(r); }
};

template <>
struct pointer_traits<void*>
{
    typedef void* pointer;
    typedef void element_type;
    typedef ptrdiff_t difference_type;
#ifndef _STLP_NO_ALIAS_TEMPLATES
    template <class U> using rebind = U*;
#else // _STLP_NO_ALIAS_TEMPLATES
    template <class U>
    struct rebind
    {
        typedef U* type;
    };
#endif // _STLP_NO_ALIAS_TEMPLATES

    // static pointer pointer_to( void ) /* noexcept */ // unspecified
    //  { return NULL; }
};

enum class pointer_safety
{ relaxed, preferred, strict };

_STLP_DECLSPEC void declare_reachable(void *p);

namespace detail {

_STLP_DECLSPEC void* __undeclare_reachable( void* ) noexcept;

} // detail

template <class T>
T *undeclare_reachable( T* p ) noexcept
{ return reinterpret_cast<T*>( detail::__undeclare_reachable(reinterpret_cast<void*>(p)) ); }

_STLP_DECLSPEC void declare_no_pointers(char *p, size_t n) noexcept;
_STLP_DECLSPEC void undeclare_no_pointers(char *p, size_t n) noexcept;
_STLP_DECLSPEC pointer_safety get_pointer_safety() noexcept;

_STLP_DECLSPEC void *align( _STLP_STD::size_t alignment, _STLP_STD::size_t size, void *&ptr, _STLP_STD::size_t& space );

namespace detail {

//template <class T>
//struct __has_allocator_type :
//        integral_constant<bool,is_same<true_type,decltype(detail::__has_type_selector::__test_a<T>(0))>::value>
//{
//};

template <class T, class Alloc, bool>
struct __uses_allocator_aux :
    public false_type
{
};

template <class T, class Alloc>
struct __uses_allocator_aux<T,Alloc,true> :
    public integral_constant<bool,is_convertible<Alloc,typename T::allocator_type>::value>
{
};

template <bool, class Alloc>
struct __pointer_type
{
    typedef typename Alloc::value_type* pointer;
};

template <class Alloc>
struct __pointer_type<true,Alloc>
{
    typedef typename Alloc::pointer pointer;
};

template <bool, class D, class T>
struct __pointer_type2
{
    typedef T* pointer;
};

template <class D, class T>
struct __pointer_type2<true,D,T>
{
    typedef typename D::pointer pointer;
};

template <bool, class Alloc>
struct __const_pointer_type
{
#ifndef _STLP_NO_ALIAS_TEMPLATES
    typedef typename pointer_traits<typename __pointer_type<is_same<true_type,decltype(detail::__has_type_selector::__test_p<Alloc>(0))>::value,Alloc>::pointer>::template rebind<typename add_const<typename Alloc::value_type>::type> const_pointer;
#else // _STLP_NO_ALIAS_TEMPLATES
    typedef typename pointer_traits<typename __pointer_type<is_same<true_type,decltype(detail::__has_type_selector::__test_p<Alloc>(0))>::value,Alloc>::pointer>::template rebind<typename add_const<typename Alloc::value_type>::type>::type const_pointer;
#endif // _STLP_NO_ALIAS_TEMPLATES
};

template <class Alloc>
struct __const_pointer_type<true,Alloc>
{
    typedef typename Alloc::const_pointer const_pointer;
};

template <bool, class Alloc>
struct __void_pointer_type
{
#ifndef _STLP_NO_ALIAS_TEMPLATES
    typedef typename pointer_traits<typename __pointer_type<is_same<true_type,decltype(detail::__has_type_selector::__test_p<Alloc>(0))>::value,Alloc>::pointer>::template rebind<void> void_pointer;
#else // _STLP_NO_ALIAS_TEMPLATES
    typedef typename pointer_traits<typename __pointer_type<is_same<true_type,decltype(detail::__has_type_selector::__test_p<Alloc>(0))>::value,Alloc>::pointer>::template rebind<void>::type void_pointer;
#endif // _STLP_NO_ALIAS_TEMPLATES
};

template <class Alloc>
struct __void_pointer_type<true,Alloc>
{
    typedef typename Alloc::void_pointer void_pointer;
};

template <bool, class Alloc>
struct __const_void_pointer_type
{
#ifndef _STLP_NO_ALIAS_TEMPLATES
   typedef typename pointer_traits<typename __pointer_type<is_same<true_type,decltype(detail::__has_type_selector::__test_p<Alloc>(0))>::value,Alloc>::pointer>::template rebind<const void> const_void_pointer;
#else // _STLP_NO_ALIAS_TEMPLATES
   typedef typename pointer_traits<typename __pointer_type<is_same<true_type,decltype(detail::__has_type_selector::__test_p<Alloc>(0))>::value,Alloc>::pointer>::template rebind<const void>::type const_void_pointer;
#endif // _STLP_NO_ALIAS_TEMPLATES
};

template <class Alloc>
struct __const_void_pointer_type<true,Alloc>
{
    typedef typename Alloc::const_void_pointer const_void_pointer;
};

template <bool, class Alloc>
struct __difference_pointer_type
{
    typedef typename pointer_traits<typename __pointer_type<is_same<true_type,decltype(detail::__has_type_selector::__test_p<Alloc>(0))>::value,Alloc>::pointer>::difference_type difference_type;
};

template <class Alloc>
struct __difference_pointer_type<true,Alloc>
{
    typedef typename Alloc::difference_type difference_type;
};

template <bool, class Alloc>
struct __pointer_size_type
{
    typedef typename make_unsigned<typename detail::__difference_pointer_type<is_same<true_type,decltype(detail::__has_type_selector::__test_dp<Alloc>(0))>::value,Alloc>::difference_type>::type size_type;
};

template <class Alloc>
struct __pointer_size_type<true,Alloc>
{
    typedef typename Alloc::size_type size_type;
};

template <class ToF, class T, class Alloc>
class __rebind_other_type
{
};

template <class T, template <class U, class ... Args> class Alloc, class U, class ... Args>
struct __rebind_other_type<false_type,T,Alloc<U,Args...> >
{
    typedef Alloc<T,Args...> type;
};

template <class T, class Alloc>
struct __rebind_other_type<true_type,T,Alloc>
{
    typedef typename Alloc::template rebind<T>::other type;
};

} // namespace detail

template <class T, class Alloc>
struct uses_allocator :
    public detail::__uses_allocator_aux<T,Alloc,is_same<true_type,decltype(detail::__has_type_selector::__test_a<T>(0))>::value>
{
};

struct allocator_arg_t
{ };

constexpr allocator_arg_t allocator_arg = allocator_arg_t();

template <class Alloc>
struct allocator_traits
{
    typedef Alloc allocator_type;
    typedef typename Alloc::value_type value_type;
    typedef typename detail::__pointer_type<is_same<true_type,decltype(detail::__has_type_selector::__test_p<Alloc>(0))>::value,Alloc>::pointer pointer;
    typedef typename detail::__const_pointer_type<is_same<true_type,decltype(detail::__has_type_selector::__test_cp<Alloc>(0))>::value,Alloc>::const_pointer const_pointer;
    typedef typename detail::__void_pointer_type<is_same<true_type,decltype(detail::__has_type_selector::__test_vp<Alloc>(0))>::value,Alloc>::void_pointer void_pointer;
    typedef typename detail::__const_void_pointer_type<is_same<true_type,decltype(detail::__has_type_selector::__test_cvp<Alloc>(0))>::value,Alloc>::const_void_pointer const_void_pointer;

    typedef typename detail::__difference_pointer_type<is_same<true_type,decltype(detail::__has_type_selector::__test_dp<Alloc>(0))>::value,Alloc>::difference_type difference_type;
    typedef typename detail::__pointer_size_type<is_same<true_type,decltype(detail::__has_type_selector::__test_sz<Alloc>(0))>::value,Alloc>::size_type size_type;
    typedef decltype(detail::__has_type_selector::__test_pcca<Alloc>(0)) propagate_on_container_copy_assignment;
    typedef decltype(detail::__has_type_selector::__test_pcma<Alloc>(0)) propagate_on_container_move_assignment;
    typedef decltype(detail::__has_type_selector::__test_pcs<Alloc>(0)) propagate_on_container_swap;

#ifndef _STLP_NO_ALIAS_TEMPLATES
    template <class T> using rebind_alloc = typename detail::__rebind_other_type<decltype(detail::__has_type_selector::__test_ro<Alloc,T>(0)),T,Alloc>::type;
#else // _STLP_NO_ALIAS_TEMPLATES
    template <class T>
    struct rebind_alloc
    {
        typedef typename detail::__rebind_other_type<decltype(detail::__has_type_selector::__test_ro<Alloc,T>(0)),T,Alloc>::type type;
    };
#endif // _STLP_NO_ALIAS_TEMPLATES

#ifndef _STLP_NO_ALIAS_TEMPLATES
    template <class T> using rebind_traits = allocator_traits<rebind_alloc<T> >;
#else // _STLP_NO_ALIAS_TEMPLATES
    template <class T>
    struct rebind_traits
    {
        typedef allocator_traits<typename rebind_alloc<T>::type> type;
    };
#endif // _STLP_NO_ALIAS_TEMPLATES

    static pointer allocate( Alloc& a, size_type n )
      { return a.allocate( n ); }
    static pointer allocate( Alloc& a, size_type n, const_void_pointer hint )
      {
        typedef detail::__allocate_hint<is_same<true_type,decltype(detail::__has_type_selector::__test_ah<Alloc>(0))>::value,pointer,size_type,const_void_pointer,Alloc> a_type;

        return a_type::allocate( a, n, hint );
      }
    static void deallocate( Alloc& a, pointer p, size_type n )
      { a.deallocate( p, n ); }


    template <class T, class... Args>
    static void construct( Alloc& a, T* p, Args&&... args )
      {
        typedef detail::__construct_check<is_same<true_type,decltype(detail::__has_type_selector::__test_construct<Alloc,T,Args...>(0))>::value,Alloc,T,Args...> a_type;

        a_type::construct( a, p, _STLP_STD::forward<Args>(args) ... );
      }

    template <class T>
    static void destroy( Alloc& a, T* p )
      {
        typedef detail::__destroy_check<is_same<true_type,decltype(detail::__has_type_selector::__test_destroy<Alloc,T>(0))>::value,Alloc,T> a_type;

        a_type::destroy( a, p );
      }

    static size_type max_size( const Alloc& a )
      {
        typedef detail::__max_size_check<is_same<true_type,decltype(detail::__has_type_selector::__test_max_size<Alloc,size_type>(0))>::value,Alloc,size_type> a_type;

        return a_type::max_size();
      }
    static Alloc select_on_container_copy_construction( const Alloc& rhs )
      {
        typedef detail::__soccc_check<is_same<true_type,decltype(detail::__has_type_selector::__test_soccc<Alloc>(0))>::value,Alloc> a_type;

        return a_type::soccc();
      }
};

namespace detail {

template <bool>
struct __destroy_selector
{
    template <class U>
    static void destroy( U* p )
      {
        p->~U();
#if defined (_STLP_DEBUG_UNINITIALIZED)
        memset( reinterpret_cast<char*>(p), _STLP_SHRED_BYTE, sizeof(U) );
#endif
      }

    template <class _ForwardIterator>
    static void destroy( _ForwardIterator __first, _ForwardIterator __last )
      {
#ifndef _STLP_LAMBDA_PAR_BUG
        _STLP_STD::for_each( __first, __last, []( typename iterator_traits<_ForwardIterator>::value_type& i ){
            _STLP_STD::detail::__destroy_selector<false>::destroy( &i );
          } );
#else
        for ( ; __first != __last; ++__first ) {
          _STLP_STD::detail::__destroy_selector<false>::destroy( &*__first );
        }
#endif
      }
};

template <>
struct __destroy_selector<true>
{
    template <class U>
    static void destroy( U* p )
      {
#if defined (_STLP_DEBUG_UNINITIALIZED)
        memset( reinterpret_cast<char*>(p), _STLP_SHRED_BYTE, sizeof(U) );
#endif
      }

    template <class _ForwardIterator>
    static void destroy( _ForwardIterator __first, _ForwardIterator __last )
      {
#if defined (_STLP_DEBUG_UNINITIALIZED)
        for_each( __first, __last, []( typename iterator_traits<_ForwardIterator>::value_type& i ){
            destroy( &i );
          } );
#endif
      }
};

template <class _ForwardIterator>
inline void _Destroy_Range(_ForwardIterator __first, _ForwardIterator __last)
{
  _STLP_STD::detail::__destroy_selector<is_trivially_destructible<typename iterator_traits<_ForwardIterator>::value_type>::value>::destroy( __first, __last );
}

} // namespace detail

#if defined (new)
#  define _STLP_NEW_REDEFINE new
#  undef new
#endif

#if defined(_STLP_NEW_REDEFINE)
#  if defined (DEBUG_NEW)
#    define new DEBUG_NEW
#  endif
#  undef _STLP_NEW_REDEFINE
#endif

#if defined (_STLP_DEF_CONST_DEF_PARAM_BUG)
// Those adaptors are here to fix common compiler bug regarding builtins:
// expressions like int k = int() should initialize k to 0
template <class _Tp>
inline _Tp __default_constructed_aux( _Tp*, const false_type& )
{ return _Tp(); }
template <class _Tp>
inline _Tp __default_constructed_aux( _Tp*, const true_type& )
{ return _Tp(0); }

template <class _Tp>
inline _Tp __default_constructed( _Tp* __p )
{ return __default_constructed_aux(__p, _HasDefaultZeroValue(__p)._Answer()); }

#  define _STLP_DEFAULT_CONSTRUCTED(_TTp) __default_constructed((_TTp*)0)
#else
#  define _STLP_DEFAULT_CONSTRUCTED(_TTp) _TTp()
#endif /* _STLP_DEF_CONST_DEF_PARAM_BUG */

// Malloc-based allocator.

typedef void (* __oom_handler_type)();

class _STLP_CLASS_DECLSPEC __malloc_alloc
{
  public:
    // this one is needed for proper simple_alloc wrapping
    typedef char value_type;
    static void* _STLP_CALL allocate(size_t __n);
    static void _STLP_CALL deallocate(void* __p, size_t /* __n */)
      { free((char*)__p); }
    static __oom_handler_type _STLP_CALL set_malloc_handler(__oom_handler_type __f);
};

// New-based allocator.

class _STLP_CLASS_DECLSPEC __new_alloc
{
  public:
    // this one is needed for proper simple_alloc wrapping
    typedef char value_type;
    static void* _STLP_CALL allocate(size_t __n)
      { return __stl_new(__n); }
    static void _STLP_CALL deallocate(void* __p, size_t)
      { __stl_delete(__p); }
};

// Allocator adaptor to check size arguments for debugging.
// Reports errors using assert.  Checking can be disabled with
// NDEBUG, but it's far better to just use the underlying allocator
// instead when no checking is desired.
// There is some evidence that this can confuse Purify.
// This adaptor can only be applied to raw allocators

template <class _Alloc>
class __debug_alloc :
    public _Alloc
{
  public:
    typedef _Alloc __allocator_type;
    typedef typename _Alloc::value_type value_type;

  private:
    struct __alloc_header
    {
        uint32_t __magic:16;
        uint32_t __type_size:16;
        uint32_t _M_size;
    }; // that is 8 bytes for sure

    // Sunpro CC has bug on enums, so extra_before/after set explicitly
    enum
    {
      __pad = 8,
      __magic = 0xdeba,
      __deleted_magic = 0xdebd,
      __shred_byte = _STLP_SHRED_BYTE
    };

    enum
    {
      __extra_before = 16,
      __extra_after = 8
    };

    // Size of space used to store size.  Note
    // that this must be large enough to preserve alignment.
    static size_t _STLP_CALL __extra_before_chunk()
      {
        return (long)__extra_before / sizeof(value_type) +
          (size_t)((long)__extra_before % sizeof(value_type) > 0);
      }
    static size_t _STLP_CALL __extra_after_chunk()
      {
        return (long)__extra_after / sizeof(value_type) +
          (size_t)((long)__extra_after % sizeof(value_type) > 0);
      }

  public:
    __debug_alloc()
      { }
    ~__debug_alloc()
      { }

    static void* _STLP_CALL allocate(size_t);
    static void _STLP_CALL deallocate(void *, size_t);
};

#  if defined (__OS400__)
// dums 02/05/2007: is it really necessary ?
enum { _MAX_BYTES = 256 };
#  else
enum { _MAX_BYTES = 32 * sizeof(void*) };
#  endif

// node allocator.

class _STLP_CLASS_DECLSPEC __node_alloc
{
    static void * _STLP_CALL _M_allocate(size_t& __n);
    /* __p may not be 0 */
    static void _STLP_CALL _M_deallocate(void *__p, size_t __n);

  public:
    // this one is needed for proper simple_alloc wrapping
    typedef char value_type;
    /* __n must be > 0      */
    static void* _STLP_CALL allocate(size_t& __n)
      { return (__n > (size_t)_MAX_BYTES) ? __stl_new(__n) : _M_allocate(__n); }
    /* __p may not be 0 */
    static void _STLP_CALL deallocate(void *__p, size_t __n)
      { if (__n > (size_t)_MAX_BYTES) __stl_delete(__p); else _M_deallocate(__p, __n); }
};

#  if defined (_STLP_USE_TEMPLATE_EXPORT)
_STLP_EXPORT_TEMPLATE_CLASS __debug_alloc<__node_alloc>;
#  endif

#if defined (_STLP_USE_TEMPLATE_EXPORT)
_STLP_EXPORT_TEMPLATE_CLASS __debug_alloc<__new_alloc>;
_STLP_EXPORT_TEMPLATE_CLASS __debug_alloc<__malloc_alloc>;
#endif

#if defined (_STLP_USE_PERTHREAD_ALLOC)
_STLP_END_NAMESPACE
#  include <stl/_pthread_alloc.h>
_STLP_BEGIN_NAMESPACE
#endif

// This implements allocators as specified in the C++ standard.
//
// Note that standard-conforming allocators use many language features
// that are not yet widely implemented.  In particular, they rely on
// member templates, partial specialization, partial ordering of function
// templates, the typename keyword, and the use of the template keyword
// to refer to a template member of a dependent type.

template <class _Tp> class allocator;

_STLP_TEMPLATE_NULL
class _STLP_CLASS_DECLSPEC allocator<void>
{
  public:
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;
    typedef void*       pointer;
    typedef const void* const_pointer;
#if defined (_STLP_CLASS_PARTIAL_SPECIALIZATION)
    typedef void        value_type;
#endif
    template <class _Tp1>
    struct rebind
    {
        typedef allocator<_Tp1> other;
    };
};

template <class _Tp>
class allocator
{
  private:
    // underlying allocator implementation
#if defined (_STLP_USE_PERTHREAD_ALLOC)
#ifdef _STLP_DEBUG_ALLOC
    typedef __debug_alloc<__pthread_alloc> __alloc_type;
#else
    typedef __pthread_alloc __alloc_type;
#endif

#elif defined (_STLP_USE_NEWALLOC)

#ifdef _STLP_DEBUG_ALLOC
    typedef __debug_alloc<__new_alloc> __alloc_type;
#else
    typedef __new_alloc __alloc_type;
#endif

#elif defined (_STLP_USE_MALLOC)

#ifdef _STLP_DEBUG_ALLOC
    typedef __debug_alloc<__malloc_alloc> __alloc_type;
#else
    typedef __malloc_alloc __alloc_type;
#endif

#else // then use __node_alloc

#ifdef _STLP_DEBUG_ALLOC
    typedef __debug_alloc<__node_alloc> __alloc_type;
#else
    typedef __node_alloc __alloc_type;
#endif

#endif

  public:
    typedef _Tp        value_type;
    typedef _Tp*       pointer;
    typedef const _Tp* const_pointer;
    typedef _Tp&       reference;
    typedef const _Tp& const_reference;
    typedef size_t     size_type;
    typedef ptrdiff_t  difference_type;

    template <class _Tp1>
    struct rebind
    {
        typedef allocator<_Tp1> other;
    };

    allocator() noexcept
      { }
    template <class _Tp1>
    allocator(const allocator<_Tp1>&) noexcept
      { }
    allocator(const allocator<_Tp>&) noexcept
      { }
    ~allocator()
      { }
    pointer address( reference __x ) const noexcept
      { return addressof(__x); }
    const_pointer address( const_reference __x ) const noexcept
      { return addressof(__x); }
    // __n is permitted to be 0.  The C++ standard says nothing about what the return value is when __n == 0.
    pointer allocate(size_type __n, allocator<void>::const_pointer /* hint */ = 0)
      {
        if (__n > max_size()) {
          _STLP_THROW_BAD_ALLOC;
        }
        if (__n != 0) {
          size_type __buf_size = __n * sizeof(value_type);
          _Tp* __ret = __REINTERPRET_CAST(_Tp*, __alloc_type::allocate(__buf_size));
#if defined (_STLP_DEBUG_UNINITIALIZED) && !defined (_STLP_DEBUG_ALLOC)
          memset((char*)__ret, _STLP_SHRED_BYTE, __buf_size);
#endif
          return __ret;
        }

    return 0;
  }

  // __p is permitted to be a null pointer, only if n==0.
    void deallocate(pointer __p, size_type __n)
      {
        _STLP_ASSERT( (__p == 0) == (__n == 0) )
          if (__p != 0) {
#if defined (_STLP_DEBUG_UNINITIALIZED) && !defined (_STLP_DEBUG_ALLOC)
            memset((char*)__p, _STLP_SHRED_BYTE, __n * sizeof(value_type));
#endif
            __alloc_type::deallocate((void*)__p, __n * sizeof(value_type));
          }
      }
#if !defined (_STLP_NO_ANACHRONISMS)
    // backwards compatibility
    void deallocate(pointer __p) const
      {
        if (__p != 0)
          __alloc_type::deallocate((void*)__p, sizeof(value_type));
      }
#endif

    size_type max_size() const noexcept
      { return sizeof(value_type) ? _STLP_STD::numeric_limits<size_type>::max() / sizeof(value_type) : _STLP_STD::numeric_limits<size_type>::max(); }

    template<class U, class... Args>
    void construct( U* p, Args&&... args )
      { ::new((void*)p) U( _STLP_STD::forward<Args>(args)... ); }

    template <class U>
    void destroy( U* p )
      { _STLP_STD::detail::__destroy_selector<is_trivially_destructible<_Tp>::value>::destroy( p ); }
};

template <class _T1, class _T2>
inline bool _STLP_CALL operator ==(const allocator<_T1>&, const allocator<_T2>&) noexcept
{ return true; }

template <class _T1, class _T2>
inline bool _STLP_CALL operator !=(const allocator<_T1>&, const allocator<_T2>&) noexcept
{ return false; }

#if defined (_STLP_USE_TEMPLATE_EXPORT)
_STLP_EXPORT_TEMPLATE_CLASS allocator<char>;
#  if defined (_STLP_HAS_WCHAR_T)
_STLP_EXPORT_TEMPLATE_CLASS allocator<wchar_t>;
#  endif
#  if defined (_STLP_USE_PTR_SPECIALIZATIONS)
_STLP_EXPORT_TEMPLATE_CLASS allocator<void*>;
#  endif
#endif

_STLP_MOVE_TO_PRIV_NAMESPACE

_STLP_MOVE_TO_STD_NAMESPACE

#if defined (_STLP_CLASS_PARTIAL_SPECIALIZATION)

template <class T>
struct __is_stateless_alloc :
    public integral_constant<bool,is_trivial<T>::value || is_empty<T>::value>
{ };

#endif

template <class _Tp> void swap(_Tp&, _Tp&);

template <class _Tp>
inline void swap(allocator<_Tp>&, allocator<_Tp>&)
{ }

_STLP_MOVE_TO_PRIV_NAMESPACE

// inheritance is being used for EBO optimization
template <class _Value, class _MaybeReboundAlloc>
class _STLP_alloc_proxy :
    public _MaybeReboundAlloc
{
  private:
    typedef _MaybeReboundAlloc _Base;
    typedef typename _Base::size_type size_type;
    typedef _STLP_alloc_proxy<_Value, _MaybeReboundAlloc> _Self;

  public:
    _Value _M_data;

    explicit _STLP_alloc_proxy (const _MaybeReboundAlloc& __a) :
        _MaybeReboundAlloc(__a)
      { }

    template <class...Args>
    _STLP_alloc_proxy (const _MaybeReboundAlloc& __a, Args... __p) :
        _MaybeReboundAlloc(__a),
        _M_data(_STLP_STD::forward<Args>(__p)...)
      { }

    void swap( _Self& r )
      {
        _STLP_STD::swap( static_cast<_Base&>(*this), static_cast<_Base&>(r) );
        _STLP_STD::swap( _M_data, r._M_data );
      }
};

#if defined (_STLP_USE_TEMPLATE_EXPORT)
_STLP_EXPORT_TEMPLATE_CLASS _STLP_alloc_proxy<char*, allocator<char> >;
#  if defined (_STLP_HAS_WCHAR_T)
_STLP_EXPORT_TEMPLATE_CLASS _STLP_alloc_proxy<wchar_t*, allocator<wchar_t> >;
#  endif
#  if defined (_STLP_USE_PTR_SPECIALIZATIONS)
_STLP_EXPORT_TEMPLATE_CLASS _STLP_alloc_proxy<void**, allocator<void*> >;
#  endif
#endif

_STLP_MOVE_TO_STD_NAMESPACE

template <class _Value, class _MaybeReboundAlloc>
inline void swap( _STLP_PRIV _STLP_alloc_proxy<_Value,_MaybeReboundAlloc>& __a, _STLP_PRIV _STLP_alloc_proxy<_Value,_MaybeReboundAlloc>& __b)
{ __a.swap( __b ); }

_STLP_END_NAMESPACE

#if defined (_STLP_EXPOSE_GLOBALS_IMPLEMENTATION)
#  include <stl/_alloc.c>
#endif

#endif /* _STLP_INTERNAL_ALLOC_H */

// Local Variables:
// mode:C++
// End:

