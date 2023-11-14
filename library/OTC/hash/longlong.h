#include <OSE/OSE.h>

#if defined(SIZEOF_LONG_LONG) && SIZEOF_LONG_LONG != 0
#define HAVE_64BIT_LONG_LONG 1
#endif

#define fnv_32_buf otclib_fnv_32_buf
#define fnv_32_str otclib_fnv_32_str
#define fnv_64_buf otclib_fnv_64_buf
#define fnv_64_str otclib_fnv_64_str
#define fnv_32a_buf otclib_fnv_32a_buf
#define fnv_32a_str otclib_fnv_32a_str
#define fnv_64a_buf otclib_fnv_64a_buf
#define fnv_64a_str otclib_fnv_64a_str

