#ifndef OTC_THREAD_ATOMIC_HH
#define OTC_THREAD_ATOMIC_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     thread/atomic.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OTC/thread/nrmutex.hh>

/* ------------------------------------------------------------------------- */

// These functions are specific to use within
// this C++ library. It is recommended they
// aren't used directly, especially since they
// don't do anything special when threading support
// isn't compiled into the library.

// Note that <OTC_Atomic> may resolved to an
// unsigned value so do not assume it can handle
// negative values.

#if defined(OSE_WITH_THREADS)

#if defined(__GNUC__) && (defined(__i386) || defined(__i386__))

typedef int OTC_Atomic;

static inline OTC_Atomic
__attribute__ ((__unused__))
otclib_exchange_and_add (volatile OTC_Atomic* __mem, int __val)
{
  register OTC_Atomic __result;
  __asm__ __volatile__ ("lock; xaddl %0,%2"
                        : "=r" (__result)
                        : "0" (__val), "m" (*__mem)
                        : "memory");
  return __result;
}

inline OTC_Atomic otclib_atomic_inc(OTC_Atomic& value, OTC_NRMutex&)
{ return otclib_exchange_and_add(&value,1)+1; }
inline OTC_Atomic otclib_atomic_dec(OTC_Atomic& value, OTC_NRMutex&)
{ return otclib_exchange_and_add(&value,-1)-1; }
inline OTC_Atomic otclib_atomic_get(OTC_Atomic& value, OTC_NRMutex&)
{ return otclib_exchange_and_add(&value,0); }

#else

#if defined(__sun__) && defined(__SUNPRO_CC)

#include <sys/atomic.h>

typedef uint32_t OTC_Atomic;

inline OTC_Atomic otclib_atomic_inc(OTC_Atomic& value, OTC_NRMutex&)
{ return atomic_add_32_nv(&value,1); }
inline OTC_Atomic otclib_atomic_dec(OTC_Atomic& value, OTC_NRMutex&)
{ return atomic_add_32_nv(&value,-1); }
inline OTC_Atomic otclib_atomic_get(OTC_Atomic& value, OTC_NRMutex&)
{ return atomic_add_32_nv(&value,0); }

#else

#if defined(__GNUC__) && defined(__ppc__)

typedef int OTC_Atomic;

static inline OTC_Atomic
__attribute__ ((__unused__))      
otclib_atomic_add_return(volatile OTC_Atomic* __mem, int __val)
{
  OTC_Atomic __tmp;       
  __asm__ __volatile__ (
        "/* Inline atomic add */\n"    
        "0:\t"
        "lwarx    %0,0,%1 \n\t"
        "add%I2   %0,%0,%2 \n\t"
        "stwcx.   %0,0,%1 \n\t"
        "bne-     0b \n\t"
        "/* End atomic add */"    
        : "=&b"(__tmp)
        : "r" (__mem), "Ir"(__val)
        : "cr0", "memory");
  return __tmp;
}

inline OTC_Atomic otclib_atomic_inc(OTC_Atomic& value, OTC_NRMutex&)
{ return otclib_atomic_add_return(&value,1); }
inline OTC_Atomic otclib_atomic_dec(OTC_Atomic& value, OTC_NRMutex&)
{ return otclib_atomic_add_return(&value,-1); }
inline OTC_Atomic otclib_atomic_get(OTC_Atomic& value, OTC_NRMutex&)
{ return otclib_atomic_add_return(&value,0); }

#else
#if defined(_MSC_VER)

#include <winsock2.h>
#include <windows.h>

typedef LONG OTC_Atomic;

inline OTC_Atomic otclib_atomic_inc(OTC_Atomic& value, OTC_NRMutex&)
{ return InterlockedIncrement(&value); }
inline OTC_Atomic otclib_atomic_dec(OTC_Atomic& value, OTC_NRMutex&)
{ return InterlockedDecrement(&value); }
inline OTC_Atomic otclib_atomic_get(OTC_Atomic& value, OTC_NRMutex&)
{ return InterlockedExchangeAdd(&value,0); }

#else

typedef int OTC_Atomic;

inline OTC_Atomic otclib_atomic_inc(OTC_Atomic& value, OTC_NRMutex& mutex)
{ OTC_Atomic i; mutex.lock(); i = (++value); mutex.unlock(); return i; }
inline OTC_Atomic otclib_atomic_dec(OTC_Atomic& value, OTC_NRMutex& mutex)
{ OTC_Atomic i; mutex.lock(); i = (--value); mutex.unlock(); return i; }   
inline OTC_Atomic otclib_atomic_get(OTC_Atomic& value, OTC_NRMutex& mutex)
{ OTC_Atomic i; mutex.lock(); i = value; mutex.unlock(); return i; }

#endif

#endif

#endif

#endif

#else

typedef int OTC_Atomic;

inline OTC_Atomic otclib_atomic_inc(OTC_Atomic& value, OTC_NRMutex& mutex)
{ return (++value); }
inline OTC_Atomic otclib_atomic_dec(OTC_Atomic& value, OTC_NRMutex& mutex)
{ return (--value); }   
inline OTC_Atomic otclib_atomic_get(OTC_Atomic& value, OTC_NRMutex& mutex)
{ return value; }

#endif

/* ------------------------------------------------------------------------- */

#endif /* OTC_THREAD_ATOMIC_HH */
