/* stdarg.h standard header */#ifndef _STDARG#define _STDARG#ifndef _YVALS#include <yvals.h>#endif		/* type definitions */typedef _Va_list va_list;		/* macros */#define va_arg(ap, T)	\	(*(T *)(((ap) += _Bnd(T, _AUPBND)) - _Bnd(T, _ADNBND)))#define va_end(ap)		(void)0#define va_start(ap, A)	\	(void)((ap) = (char *)&(A) + _Bnd(A, _AUPBND))#define _Bnd(X, bnd)	(sizeof (X) + (bnd) & ~(bnd))#if 0 /* older version of macros ... */#define va_start(ap, A) _Va_start(ap, A)#define va_arg(ap, T) _Va_arg(ap, T)#define va_end(ap)  (void)0#endif#endif/* * Copyright (c) 1994 by P.J. Plauger.  ALL RIGHTS RESERVED.  * Consult your license regarding permissions and restrictions. *//* Change log: *94June04 PlumHall baseline *94Sept30 Applied diffs for Fri Aug 26 03:49:00 1994 */