/*	StdDef.h -- Common definitions		Copyright Apple Computer,Inc.	1987-1990, 1994	All rights reserved.*/#ifndef __STDDEF__#define __STDDEF__typedef int ptrdiff_t;#ifndef __size_t__#define __size_t__#ifdef	__xlCtypedef unsigned long size_t;#else	/* __xlC */typedef unsigned int size_t;#endif	/* __xlC */#endif	/* __size_t__ */#ifndef __wchar_t__#define __wchar_t__#ifdef	__xlCtypedef unsigned short wchar_t;#else	/* __xlC */typedef short wchar_t;#endif	/* __xlC */#endif	/* __wchar_t__ */#ifndef NULL#define NULL 0#endif#define offsetof(structure,field) ((size_t)&((structure *) 0)->field)#endif