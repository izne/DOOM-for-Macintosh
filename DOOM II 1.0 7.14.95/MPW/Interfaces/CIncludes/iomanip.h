// 	IOStreams Package// 	Steve Teale April 1992// 	Copyright Symantec Corp 1990-1992. All Rights Reserved.////	Some portions Copyright � 1994 Apple Computer Inc.#ifndef __IOMANIP_H#define __IOMANIP_H#include <iomdefs.h>#include <iostream.h>ios &_iomanip_resetiosflags(ios&, long);inline SMANIP<long> resetiosflags(long f) 	{ return SMANIP<long>(_iomanip_resetiosflags,f); }ios &_iomanip_setfill(ios&, int);inline SMANIP<int> setfill(int filler)	{ return SMANIP<int>(_iomanip_setfill,filler); }ios &_iomanip_setiosflags(ios&, long);inline SMANIP<long> setiosflags(long f)	{ return SMANIP<long>(_iomanip_setiosflags,f); }ios &_iomanip_setprecision(ios&, int);inline SMANIP<int> setprecision(int n)	{ return SMANIP<int>(_iomanip_setprecision,n); }ios &_iomanip_setw(ios&, int);inline SMANIP<int> setw(int w)	{ return SMANIP<int>(_iomanip_setw,w); }ios &_iomanip_setbase(ios&, int);inline SMANIP<int> setbase(int n)	{ return SMANIP<int>(_iomanip_setbase,n); }#endif  // __IOMANIP_H