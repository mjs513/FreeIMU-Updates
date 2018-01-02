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

#ifndef _STLP_INTERNAL_FUNCTION_H
#define _STLP_INTERNAL_FUNCTION_H

#include <stdint.h>

#ifndef _STLP_INTERNAL_FUNCTION_BASE_H
#  include <stl/_function_base.h>
#endif

_STLP_BEGIN_NAMESPACE

template <class _Tp>
struct not_equal_to : public binary_function<_Tp, _Tp, bool> {
  bool operator()(const _Tp& __x, const _Tp& __y) const { return __x != __y; }
};

template <class _Tp>
struct greater : public binary_function<_Tp, _Tp, bool> {
  bool operator()(const _Tp& __x, const _Tp& __y) const { return __x > __y; }
};

template <class _Tp>
struct greater_equal : public binary_function<_Tp, _Tp, bool> {
  bool operator()(const _Tp& __x, const _Tp& __y) const { return __x >= __y; }
};

template <class _Tp>
struct less_equal : public binary_function<_Tp, _Tp, bool> {
  bool operator()(const _Tp& __x, const _Tp& __y) const { return __x <= __y; }
};

template <class _Tp>
struct divides : public binary_function<_Tp, _Tp, _Tp> {
  _Tp operator()(const _Tp& __x, const _Tp& __y) const { return __x / __y; }
};

template <class _Tp>
struct modulus : public binary_function<_Tp, _Tp, _Tp> {
  _Tp operator()(const _Tp& __x, const _Tp& __y) const { return __x % __y; }
};

template <class _Tp>
struct negate : public unary_function<_Tp, _Tp> {
  _Tp operator()(const _Tp& __x) const { return -__x; }
};

template <class _Tp>
struct logical_and : public binary_function<_Tp, _Tp, bool> {
  bool operator()(const _Tp& __x, const _Tp& __y) const { return __x && __y; }
};

template <class _Tp>
struct logical_or : public binary_function<_Tp, _Tp,bool> {
  bool operator()(const _Tp& __x, const _Tp& __y) const { return __x || __y; }
};

template <class _Tp>
struct logical_not : public unary_function<_Tp, bool> {
  bool operator()(const _Tp& __x) const { return !__x; }
};

#if !defined (_STLP_NO_EXTENSIONS)
// identity_element (not part of the C++ standard).
template <class _Tp> inline _Tp identity_element(plus<_Tp>) {  return _Tp(0); }
template <class _Tp> inline _Tp identity_element(multiplies<_Tp>) { return _Tp(1); }
#endif

template <class _Predicate>
class unary_negate :
    public unary_function<typename _Predicate::argument_type, bool>
{
  private:
    typedef unary_function<typename _Predicate::argument_type, bool> _Base;
  public:
    typedef typename _Base::argument_type argument_type;
  private:
    typedef typename conditional<is_pod<argument_type>::value,argument_type,typename add_lvalue_reference<typename add_const<argument_type>::type>::type>::type _ArgParamType;
  protected:
    _Predicate _M_pred;
  public:
    explicit unary_negate(const _Predicate& __x) :
        _M_pred(__x)
      {}

    bool operator()(_ArgParamType __x) const
      { return !_M_pred(__x); }
};

template <class _Predicate>
inline unary_negate<_Predicate>
not1(const _Predicate& __pred) {
  return unary_negate<_Predicate>(__pred);
}

template <class _Predicate>
class binary_negate :
    public binary_function<typename _Predicate::first_argument_type, typename _Predicate::second_argument_type, bool>
{
  private:
    typedef binary_function<typename _Predicate::first_argument_type, typename _Predicate::second_argument_type, bool> _Base;

  public:
    typedef typename _Base::first_argument_type first_argument_type;
    typedef typename _Base::second_argument_type second_argument_type;

  private:
    typedef typename conditional<is_pod<first_argument_type>::value,first_argument_type,typename add_lvalue_reference<typename add_const<first_argument_type>::type>::type>::type _FstArgParamType;
    typedef typename conditional<is_pod<second_argument_type>::value,second_argument_type,typename add_lvalue_reference<typename add_const<second_argument_type>::type>::type>::type _SndArgParamType;

  protected:
    _Predicate _M_pred;

  public:
    explicit binary_negate(const _Predicate& __x) :
        _M_pred(__x)
      { }
    bool operator()(_FstArgParamType __x, _SndArgParamType __y) const
      { return !_M_pred(__x, __y); }
};

template <class _Predicate>
inline binary_negate<_Predicate> not2(const _Predicate& __pred)
{ return binary_negate<_Predicate>(__pred); }

template <class _Operation>
class binder1st :
    public unary_function<typename _Operation::second_argument_type, typename _Operation::result_type>
{
  private:
    typedef unary_function<typename _Operation::second_argument_type, typename _Operation::result_type> _Base;

  public:
    typedef typename _Base::argument_type argument_type;
    typedef typename _Base::result_type result_type;

  private:
    typedef typename add_lvalue_reference<typename decay<argument_type>::type>::type _ArgParamType;
    typedef typename add_lvalue_reference<typename add_const<typename decay<argument_type>::type>::type>::type _ConstArgParamType;
    typedef typename conditional<is_pod<typename _Operation::first_argument_type>::value,typename _Operation::first_argument_type,typename add_lvalue_reference<typename add_const<typename _Operation::first_argument_type>::type>::type>::type _ValueParamType;

  protected:
    //op is a Standard name (20.3.6.1), do no make it STLport naming convention compliant.
    _Operation op;
    typename _Operation::first_argument_type _M_value;

  public:
    binder1st(const _Operation& __x, _ValueParamType __y) :
        op(__x),
        _M_value(__y)
      { }

    result_type operator()( _ConstArgParamType __x) const
      { return op(_M_value, __x); }
    // DR 109 Missing binders for non-const sequence elements
    result_type operator()( _ArgParamType __x) const
      { return op(_M_value, __x); }
};

template <class _Operation, class _Tp>
inline binder1st<_Operation> bind1st(const _Operation& __fn, const _Tp& __x)
{
  typedef typename _Operation::first_argument_type _Arg1_type;
  return binder1st<_Operation>(__fn, _Arg1_type(__x));
}

template <class _Operation>
class binder2nd :
    public unary_function<typename _Operation::first_argument_type, typename _Operation::result_type>
{
  private:
    typedef unary_function<typename _Operation::first_argument_type, typename _Operation::result_type> _Base;
  public:
    typedef typename _Base::argument_type argument_type;
    typedef typename _Base::result_type result_type;

  private:
    typedef typename add_lvalue_reference<typename decay<argument_type>::type>::type _ArgParamType;
    typedef typename add_lvalue_reference<typename add_const<typename decay<argument_type>::type>::type>::type _ConstArgParamType;
    typedef typename conditional<is_pod<typename _Operation::second_argument_type>::value,typename _Operation::second_argument_type,typename add_lvalue_reference<typename add_const<typename _Operation::second_argument_type>::type>::type>::type _ValueParamType;
  protected:
    //op is a Standard name (20.3.6.3), do no make it STLport naming convention compliant.
    _Operation op;
    typename _Operation::second_argument_type value;

  public:
    binder2nd(const _Operation& __x, _ValueParamType __y) :
        op(__x),
        value(__y)
      { }

    result_type operator()(_ConstArgParamType __x) const
      { return op(__x, value); }
    // DR 109 Missing binders for non-const sequence elements
    result_type operator()(_ArgParamType __x) const
      { return op(__x, value); }
};

template <class _Operation, class _Tp>
inline binder2nd<_Operation> bind2nd(const _Operation& __fn, const _Tp& __x)
{
  typedef typename _Operation::second_argument_type _Arg2_type;
  return binder2nd<_Operation>(__fn, _Arg2_type(__x));
}

#if !defined (_STLP_NO_EXTENSIONS)
// unary_compose and binary_compose (extensions, not part of the standard).

template <class _Operation1, class _Operation2>
class unary_compose :
    public unary_function<typename _Operation2::argument_type, typename _Operation1::result_type>
{
    typedef unary_function<typename _Operation2::argument_type, typename _Operation1::result_type> _Base;
  public:
    typedef typename _Base::argument_type argument_type;
    typedef typename _Base::result_type result_type;
  private:
    typedef typename add_lvalue_reference<typename add_const<argument_type>::type>::type _ArgParamType;
  protected:
    _Operation1 _M_fn1;
    _Operation2 _M_fn2;
  public:
    unary_compose(const _Operation1& __x, const _Operation2& __y) :
        _M_fn1(__x),
        _M_fn2(__y)
      { }

    result_type operator()(_ArgParamType __x) const
      { return _M_fn1(_M_fn2(__x)); }
};

template <class _Operation1, class _Operation2>
inline unary_compose<_Operation1,_Operation2> compose1(const _Operation1& __fn1, const _Operation2& __fn2)
{ return unary_compose<_Operation1,_Operation2>(__fn1, __fn2); }

template <class _Operation1, class _Operation2, class _Operation3>
class binary_compose :
    public unary_function<typename _Operation2::argument_type, typename _Operation1::result_type>
{
    typedef unary_function<typename _Operation2::argument_type, typename _Operation1::result_type> _Base;
  public:
    typedef typename _Base::argument_type argument_type;
    typedef typename _Base::result_type result_type;
  private:
    typedef typename add_lvalue_reference<typename add_const<argument_type>::type>::type _ArgParamType;
  protected:
    _Operation1 _M_fn1;
    _Operation2 _M_fn2;
    _Operation3 _M_fn3;

  public:
    binary_compose(const _Operation1& __x, const _Operation2& __y, const _Operation3& __z) :
        _M_fn1(__x),
        _M_fn2(__y),
        _M_fn3(__z)
      { }

    result_type operator()(_ArgParamType __x) const
      { return _M_fn1(_M_fn2(__x), _M_fn3(__x)); }
};

template <class _Operation1, class _Operation2, class _Operation3>
inline binary_compose<_Operation1, _Operation2, _Operation3> compose2(const _Operation1& __fn1, const _Operation2& __fn2, const _Operation3& __fn3)
{ return binary_compose<_Operation1,_Operation2,_Operation3>(__fn1, __fn2, __fn3); }

// select1st and select2nd are extensions: they are not part of the standard.
template <class _Pair> struct select1st : public _STLP_PRIV _Select1st<_Pair> {};
template <class _Pair> struct select2nd : public _STLP_PRIV _Select2nd<_Pair> {};

template <class _Arg1, class _Arg2>
struct project1st : public _STLP_PRIV _Project1st<_Arg1, _Arg2> {};

template <class _Arg1, class _Arg2>
struct project2nd : public _STLP_PRIV _Project2nd<_Arg1, _Arg2> {};


// constant_void_fun, constant_unary_fun, and constant_binary_fun are
// extensions: they are not part of the standard.  (The same, of course,
// is true of the helper functions constant0, constant1, and constant2.)

_STLP_MOVE_TO_PRIV_NAMESPACE

template <class _Result>
struct _Constant_void_fun {
  typedef _Result result_type;
  result_type _M_val;

  _Constant_void_fun(const result_type& __v) : _M_val(__v) {}
  const result_type& operator()() const { return _M_val; }
};

_STLP_MOVE_TO_STD_NAMESPACE

template <class _Result>
struct constant_void_fun : public _STLP_PRIV _Constant_void_fun<_Result> {
  constant_void_fun(const _Result& __v)
    : _STLP_PRIV _Constant_void_fun<_Result>(__v) {}
};

template <class _Result, _STLP_DFL_TMPL_PARAM( _Argument , _Result) >
struct constant_unary_fun : public _STLP_PRIV _Constant_unary_fun<_Result, _Argument> {
  constant_unary_fun(const _Result& __v)
    : _STLP_PRIV _Constant_unary_fun<_Result, _Argument>(__v) {}
};

template <class _Result, _STLP_DFL_TMPL_PARAM( _Arg1 , _Result), _STLP_DFL_TMPL_PARAM( _Arg2 , _Arg1) >
struct constant_binary_fun
  : public _STLP_PRIV _Constant_binary_fun<_Result, _Arg1, _Arg2> {
  constant_binary_fun(const _Result& __v)
    : _STLP_PRIV _Constant_binary_fun<_Result, _Arg1, _Arg2>(__v) {}
};

template <class _Result>
inline constant_void_fun<_Result> constant0(const _Result& __val) {
  return constant_void_fun<_Result>(__val);
}

template <class _Result>
inline constant_unary_fun<_Result,_Result> constant1(const _Result& __val) {
  return constant_unary_fun<_Result,_Result>(__val);
}

template <class _Result>
inline constant_binary_fun<_Result,_Result,_Result>
constant2(const _Result& __val) {
  return constant_binary_fun<_Result,_Result,_Result>(__val);
}

// subtractive_rng is an extension: it is not part of the standard.
// Note: this code assumes that int is 32 bits.
class subtractive_rng :
    public unary_function<uint32_t,uint32_t>
{
  private:
    uint32_t _M_table[55];
    int _M_index1;
    int _M_index2;
  public:
    uint32_t operator()( uint32_t __limit )
      {
        _M_index1 = (_M_index1 + 1) % 55;
        _M_index2 = (_M_index2 + 1) % 55;
        _M_table[_M_index1] = _M_table[_M_index1] - _M_table[_M_index2];
        return _M_table[_M_index1] % __limit;
      }

    void _M_initialize( uint32_t __seed )
      {
        uint32_t __k = 1;
        _M_table[54] = __seed;
        int __ii;
        for ( int __i = 0; __i < 54; ++__i ) {
          __ii = (21 * (__i + 1) % 55) - 1;
          _M_table[__ii] = __k;
          __k = __seed - __k;
          __seed = _M_table[__ii];
        }
        for ( int __loop = 0; __loop < 4; ++__loop ) {
          for ( int __i = 0; __i < 55; ++__i ) {
            _M_table[__i] = _M_table[__i] - _M_table[(1 + __i + 30) % 55];
          }
        }
        _M_index1 = 0;
        _M_index2 = 31;
      }

    subtractive_rng( unsigned int __seed )
      { _M_initialize(__seed); }
    subtractive_rng()
      { _M_initialize( 161803398UL ); }
};

#endif /* _STLP_NO_EXTENSIONS */

_STLP_END_NAMESPACE

#include <stl/_function_adaptors.h>

#endif /* _STLP_INTERNAL_FUNCTION_H */

// Local Variables:
// mode:C++
// End:
