/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     misc/otccopy.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1997-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/misc/copy.hh"
#endif
#endif

#include <OTC/misc/copy.hh>

#include <string.h>

#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif

/* ------------------------------------------------------------------------- */

// Go back to trusting that native memmove()
// operation is probably optimal. Certainly
// on Win32 it runs about 3 times as fast as
// the routine below we were using instead.

void OTC_Copy::copy(char* dst0, char const* src0, size_t length)
{
  memmove(dst0,src0,length);
}

/* ------------------------------------------------------------------------- */

#if 0
/*  $Revision: 1.3 $
**
**  This file has been modified to get it to compile more easily
**  on pre-4.4BSD systems.  Rich $alz, June 1991.
*/
#if 0
#define const /* NULL */
#define void char
#define size_t int
#define NULL 0
#define MEMCOPY
#endif

/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms are permitted provided
 * that: (1) source distributions retain this entire copyright notice and
 * comment, and (2) distributions including binaries display the following
 * acknowledgement:  ``This product includes software developed by the
 * University of California, Berkeley and its contributors'' in the
 * documentation or other materials provided with the distribution and in
 * all advertising materials mentioning features or use of this software.
 * Neither the name of the University nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#if 0
#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)bcopy.c	5.8 (Berkeley) 6/23/90";
#endif /* LIBC_SCCS and not lint */

#include <sys/stdc.h>
#include <string.h>
#endif

/*
 * sizeof(word) MUST BE A POWER OF TWO
 * SO THAT wmask BELOW IS ALL ONES
 */
typedef	int word;		/* "word" used for optimal copy speed */

#define	wsize	sizeof(word)
#define	wmask	(wsize - 1)

/*
 * Copy a block of memory, handling overlap.
 * This is the routine that actually implements
 * (the portable versions of) bcopy, memcpy, and memmove.
 */
#if 0
#ifdef MEMCOPY
void *
memcpy(dst0, src0, length)
#else
#ifdef MEMMOVE
void *
memmove(dst0, src0, length)
#else
void
bcopy(src0, dst0, length)
#endif
#endif
	char *dst0;
	const char *src0;
	register size_t length;
#endif
void OTC_Copy::copy(char* dst0, char const* src0, size_t length)
{
	register char *dst = dst0;
	register const char *src = src0;
	register size_t t;

	if (length == 0 || dst == src)		/* nothing to do */
		goto retval;

	/*
	 * Macros: loop-t-times; and loop-t-times, t>0
	 */
#define	TLOOP(s) if (t) TLOOP1(s)
#define	TLOOP1(s) do { s; } while (--t)

	if ((size_t)dst < (size_t)src) {
		/*
		 * Copy forward.
		 */
		t = (size_t)src;	/* only need low bits */
		if ((t | (size_t)dst) & wmask) {
			/*
			 * Try to align operands.  This cannot be done
			 * unless the low bits match.
			 */
			if ((t ^ (size_t)dst) & wmask || length < wsize)
				t = length;
			else
				t = wsize - (t & wmask);
			length -= t;
			TLOOP1(*dst++ = *src++);
		}
		/*
		 * Copy whole words, then mop up any trailing bytes.
		 */
		t = length / wsize;
		TLOOP(*(word *)dst = *(word *)src; src += wsize; dst += wsize);
		t = length & wmask;
		TLOOP(*dst++ = *src++);
	} else {
		/*
		 * Copy backwards.  Otherwise essentially the same.
		 * Alignment works as before, except that it takes
		 * (t&wmask) bytes to align, not wsize-(t&wmask).
		 */
		src += length;
		dst += length;
		t = (size_t)src;
		if ((t | (size_t)dst) & wmask) {
			if ((t ^ (size_t)dst) & wmask || length <= wsize)
				t = length;
			else
				t &= wmask;
			length -= t;
			TLOOP1(*--dst = *--src);
		}
		t = length / wsize;
		TLOOP(src -= wsize; dst -= wsize; *(word *)dst = *(word *)src);
		t = length & wmask;
		TLOOP(*--dst = *--src);
	}
retval:
#ifdef MEMCOPY
	return(dst0);
#else
#ifdef MEMMOVE
	return(dst0);
#else
	return;
#endif
#endif
}
#endif

/* ------------------------------------------------------------------------- */
