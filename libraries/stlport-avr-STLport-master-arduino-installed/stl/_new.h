/*
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

#ifndef _STLP_INTERNAL_NEW
#define _STLP_INTERNAL_NEW

#ifndef _STLP_CSTDDEF
// size_t
#  include <cstddef>
#endif

#if defined (__BORLANDC__) && (__BORLANDC__ < 0x570)
// new.h uses ::malloc ;(
#  include _STLP_NATIVE_CPP_C_HEADER(cstdlib)
using _STLP_VENDOR_CSTD::malloc;
#endif

// eMbedded Visual C++ .NET unfortunately uses _INC_NEW for both <new.h> and <new>
// we undefine the symbol to get the stuff in the SDK's <new>
#  if defined (_STLP_WCE_NET) && defined (_INC_NEW)
#    undef _INC_NEW
#  endif

#  if defined (new)
/* STLport cannot replace native Std library new header if new is a macro,
 * please define new macro after <new> header inclusion.
 */
#    error Cannot include native new header as new is a macro.
#  endif

#if defined (_STLP_USE_EXCEPTIONS)

//#  ifndef _STLP_INTERNAL_EXCEPTION
#    include <stl/_exception.h>
//#  endif

#ifdef _STLP_NO_BAD_ALLOC

_STLP_BEGIN_NAMESPACE

/*
 * STLport own bad_alloc exception to be used if the native C++ library
 * do not define it or when the new operator do not throw it to avoid
 * a useless library dependency.
 */
class bad_alloc : public exception {
public:
  bad_alloc () _STLP_NOTHROW_INHERENTLY { }
  bad_alloc(const bad_alloc&) _STLP_NOTHROW_INHERENTLY { }
  bad_alloc& operator=(const bad_alloc&) _STLP_NOTHROW_INHERENTLY {return *this;}
  ~bad_alloc () _STLP_NOTHROW_INHERENTLY { }
  const char* what() const _STLP_NOTHROW_INHERENTLY { return "bad alloc"; }
};

_STLP_END_NAMESPACE

#elif defined(_STLP_VENDOR_BAD_ALLOC)

namespace _STLP_VENDOR_STD {

class bad_alloc :
    public exception
{
  public:
    bad_alloc() throw() { }

    // This declaration is not useless:
    // http://gcc.gnu.org/onlinedocs/gcc-3.0.2/gcc_6.html#SEC118
    virtual ~bad_alloc() throw();

    // See comment in eh_exception.cc.
    virtual const char* what() const throw();
};

} // namespace _STLP_VENDOR_STD

_STLP_BEGIN_NAMESPACE

using _STLP_VENDOR_STD::bad_alloc;

_STLP_END_NAMESPACE

#endif /* _STLP_NO_BAD_ALLOC */

#endif /* _STLP_USE_EXCEPTIONS && _STLP_NO_BAD_ALLOC */

#if defined(_STLP_OPERATORS_NEW_DELETE)

_STLP_BEGIN_NAMESPACE

class bad_array_new_length;

struct nothrow_t {};

extern const nothrow_t nothrow;

typedef void (*new_handler)();

new_handler get_new_handler() noexcept;
new_handler set_new_handler(new_handler new_p) noexcept;

_STLP_END_NAMESPACE

void* operator new(_STLP_STD::size_t size) __attribute__ ((weak,visibility("default")));
void* operator new(_STLP_STD::size_t size, const _STLP_STD::nothrow_t&) noexcept __attribute__ ((weak,visibility("default")));

void operator delete(void* ptr) noexcept __attribute__ ((weak,visibility("default")));
void operator delete(void* ptr, const _STLP_STD::nothrow_t&) noexcept __attribute__ ((weak,visibility("default")));

void* operator new[](_STLP_STD::size_t size) __attribute__ ((weak,visibility("default")));
void* operator new[](_STLP_STD::size_t size, const _STLP_STD::nothrow_t&) noexcept __attribute__ ((weak,visibility("default")));

void operator delete[](void* ptr) noexcept __attribute__ ((weak,__visibility__("default")));
void operator delete[](void* ptr, const _STLP_STD::nothrow_t&) noexcept __attribute__ ((weak,visibility("default")));

inline void* operator new (_STLP_STD::size_t size, void* ptr) noexcept
{ return ptr; }
inline void* operator new[](_STLP_STD::size_t size, void* ptr) noexcept
{ return ptr; }
inline void operator delete (void* ptr, void*) noexcept
{ }
inline void operator delete[](void* ptr, void*) noexcept
{ }

_STLP_BEGIN_NAMESPACE

inline void* _STLP_CALL __stl_new(size_t __n)   { return ::operator new(__n); }
inline void  _STLP_CALL __stl_delete(void* __p) { ::operator delete(__p); }

_STLP_END_NAMESPACE

#define _STLP_THROW_BAD_ALLOC _STLP_THROW(_STLP_STD::bad_alloc())

#else /* _STLP_OPERATORS_NEW_DELETE */

#  if defined (_STLP_HAS_INCLUDE_NEXT)
#    include_next <new>
#    ifdef __ANDROID__
inline void* operator new(size_t, void* p) { return p; }
inline void* operator new[](size_t, void *p) { return p; }
#    endif
#  else
#    include _STLP_NATIVE_CPP_RUNTIME_HEADER(new)
#  endif

#if defined (_STLP_USE_OWN_NAMESPACE)

_STLP_BEGIN_NAMESPACE

#  if !defined (_STLP_NEW_DONT_THROW_BAD_ALLOC)
using _STLP_VENDOR_EXCEPT_STD::bad_alloc;
#  endif

#  if !defined (_STLP_NO_BAD_ALLOC)
using _STLP_VENDOR_EXCEPT_STD::nothrow_t;
using _STLP_VENDOR_EXCEPT_STD::nothrow;
#    if defined (_STLP_GLOBAL_NEW_HANDLER)
using ::new_handler;
using ::set_new_handler;
#    else
using _STLP_VENDOR_EXCEPT_STD::new_handler;
using _STLP_VENDOR_EXCEPT_STD::set_new_handler;
#    endif
#  endif /* !_STLP_NO_BAD_ALLOC */

_STLP_END_NAMESPACE
#endif /* _STLP_USE_OWN_NAMESPACE */

#ifndef _STLP_THROW_BAD_ALLOC
#  if !defined (_STLP_USE_EXCEPTIONS)
#    ifndef _STLP_INTERNAL_CSTDIO
#      include <stl/_cstdio.h>
#    endif
#    ifdef __ANDROID__
#      include <stdlib.h> /* for exit() in _STLP_THROW_BAD_ALLOC */
#    endif
#    define _STLP_THROW_BAD_ALLOC puts("out of memory\n"); exit(1)
#  else
#    define _STLP_THROW_BAD_ALLOC _STLP_THROW(_STLP_STD::bad_alloc())
#  endif
#endif

#if defined (_STLP_NEW_DONT_THROW_BAD_ALLOC)
#  define _STLP_CHECK_NULL_ALLOC(__x) void* __y = __x; if (__y == 0) { _STLP_THROW_BAD_ALLOC; } return __y
#else
#  define _STLP_CHECK_NULL_ALLOC(__x) return __x
#endif

_STLP_BEGIN_NAMESPACE

#if ((defined (__IBMCPP__) || defined (__OS400__) || defined (__xlC__) || defined (qTidyHeap)) && defined (_STLP_DEBUG_ALLOC))
inline void* _STLP_CALL __stl_new(size_t __n)   { _STLP_CHECK_NULL_ALLOC(::operator new(__n, __FILE__, __LINE__)); }
inline void  _STLP_CALL __stl_delete(void* __p) { ::operator delete(__p, __FILE__, __LINE__); }
#else
inline void* _STLP_CALL __stl_new(size_t __n)   { _STLP_CHECK_NULL_ALLOC(::operator new(__n)); }
inline void  _STLP_CALL __stl_delete(void* __p) { ::operator delete(__p); }
#endif
_STLP_END_NAMESPACE

#endif /* _STLP_OPERATORS_NEW_DELETE */

#endif /* _STLP_INTERNAL_NEW */

/*
 * Local Variables:
 * mode:C++
 * End:
 */
