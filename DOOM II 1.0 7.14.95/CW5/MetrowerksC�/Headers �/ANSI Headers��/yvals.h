/* yvals.h values header */#ifndef _YVALS#define _YVALS#if __MWERKS__#pragma options align=mac68k#endif  /* determine target */#if defined(__TINY__)||defined(__SMALL__)||defined(__COMPACT__)||\    defined(__MEDIUM__)||defined(__LARGE__)||defined(__HUGE__) #define _INTELx86 1 #define _MSDOS_C_LIB 1 #if defined(_MSC) #elif __TURBOC__ == 0x0452  #define _BORLAND40 1 #else  #define _BORLAND31 1 #endif#elif defined(sun) #define _INTELx86 0 #define _POSIX_C_LIB 1#elif defined(__MWERKS__) #define _INTELx86 0 #define _POSIX_C_LIB 0 #define _C_LIB_IS_EXTERN_C 1#else #define _INTELx86 0 #define _POSIX_C_LIB 1 #define _PHI_ANALYZER 1#endif  /* errno properties */#define _EDOM 33#define _ERANGE 34#define _EFPOS 35#define _EILSEQ 36#define _ERRMAX 37  /* float properties */#define _DBIAS 0x3fe#define _DOFF 4#define _FBIAS 0x7e#define _FOFF 7#define _FRND 1#if _INTELx86 /* little-endian, big long doubles */ #define _D0 3 #define _DLONG 1 #define _LBIAS 0x3ffe #define _LOFF 15#elif defined(sun) /* big-endian, funny long doubles */ #define _LONG_DOUBLE_HAS_HIDDEN_BIT 1 #define _D0 0 #define _DLONG 1 #define _LBIAS 0x3ffe #define _LOFF 15#else /* big-endian, small long doubles */ #define _LONG_DOUBLE_HAS_HIDDEN_BIT 1 #define _D0 0 #define _DLONG 0 #define _LBIAS 0x3fe #define _LOFF 4#endif  /* integer properties */#define _BITS_BYTE 8#define _C2   1#define _CSIGN  1#define _MBMAX  8typedef int _Wintt;#define _WCMIN  0#define _WCMAX  0xffff#if _INTELx86 /* 16-bit int */ #define _ILONG 0#elif __MWERKS__ #define _ILONG __fourbyteints__#else /* 32-bit int */ #define _ILONG 1#endif#define _MAX_EXP_DIG 8 /* for parsing numerics */#define _MAX_INT_DIG 32#define _MAX_SIG_DIG 36#if defined(_BORLAND40) && defined(__cplusplus) #define _WCHART  /* wchar_t already built-in */ typedef wchar_t _Wchart;#elif __MWERKS__ typedef char _Wchart;#else typedef unsigned short   _Wchart;#endif  /* pointer properties */#if defined(_BORLAND31) || defined(_BORLAND40) #include <_null.h>  /* rules are too complicated to recite here */#endif#if defined(__HUGE__)||defined(__COMPACT__)||defined(__LARGE__) #define _NULL 0L #if defined(_BORLAND31) || defined(_BORLAND40)  typedef long _Ptrdifft; #else  typedef int _Ptrdifft; #endif typedef unsigned int _Sizet;#elif __MWERKS__ #define _NULL 0L typedef long _Ptrdifft; typedef unsigned long _Sizet;#else #define _NULL 0 typedef int _Ptrdifft; typedef unsigned int _Sizet;#endif  /* setjmp properties */#if _INTELx86 #define _NSETJMP 10#else #define _NSETJMP 80#endif#if _HAS_NO_SETJMP /* should rarely be true */ #if _USE_EXISTING_SYSTEM_NAMES  #define _Setjmp setjmp #endif _EXTERN_C int _Setjmp(int *); _END_EXTERN_C#endif /*0*/  /* signal properties */#if _INTELx86 #define _SIGABRT 22#else #define _SIGABRT 6#endif#define _SIGMAX  32  /* stdarg properties */#if SPARCompiler typedef char *_Va_list; #ifdef __cplusplus  extern "C" char __builtin_va_alist;  extern "C" void *__builtin_va_arg_incr(...); #else /* !__cplusplus */  extern char __builtin_va_alist;  extern void *__builtin_va_arg_incr(); #endif  /* __cplusplus */ #define _Va_start(ap, A) (void)(ap = (char *)&__builtin_va_alist) #define _Va_arg(ap, T) ((T *)__builtin_va_arg_incr((T *)ap))[0]#elif defined(_BORLAND40) typedef void __far *_Va_list;/* and use their <stdarg.h> */#elif __MWERKS__ typedef char *_Va_list; #define _Va_start_addr(A)		(va_list)(&A+1) #define _Va_start(ap,A)		(ap=_Va_start_addr(A)) #define _Va_arg(ap,T)			(*(*(T **)&ap)++)#else typedef char *_Va_list; #define _Bnd(X, bnd) (sizeof (X) + (bnd) & ~(bnd)) #define _Va_arg(ap, T) \ (*(T *)(((ap) += _Bnd(T, _AUPBND)) - _Bnd(T, _ADNBND))) #define _Va_start(ap, A) \ (void)((ap) = (char *)&(A) + _Bnd(A, _AUPBND))#endif  /* stdlib properties */#define _EXFAIL 1typedef struct _Mbstatet { _Wchart _Wchar; char _State; } _Mbstatet;  /* stdio properties */#define _FNAMAX 64//MW BB Change ...//#define _FOPMAX 16#define _FOPMAX 64#define _TNAMAX 16typedef struct _Fpost { unsigned long _Off; /* can be system dependent */ _Mbstatet _Wstate; } _Fpost;  /* storage alignment properties */#if SPARCompiler || defined(_PHI_ANALYZER) #define _AUPBND 4U /* sixteen-byte boundaries (2^^4) */ #define _ADNBND 4U #define _MEMBND 4U#else #define _AUPBND 1U /* even-byte boundaries (2^^1) */ #define _ADNBND 1U #define _MEMBND 1U#endif  /* time properties */#if __MWERKS__ #define _CPS 60 #define _TBIAS ((4 * 365LU + 1) * 86400)#else #define _CPS 1 #define _TBIAS ((70 * 365LU + 17) * 86400)#endif  /* naming properties */#define _USE_EXISTING_SYSTEM_NAMES 1 /* _Open => open etc. */#ifdef __cplusplus #define _EXTERN_C extern "C" { #define _END_EXTERN_C }#else #define _EXTERN_C #define _END_EXTERN_C#endif /* __cplusplus */#if defined(__cplusplus) && defined(_C_LIB_IS_EXTERN_C) #define _C_LIB_DECL extern "C" { #define _END_C_LIB_DECL }#else #define _C_LIB_DECL #define _END_C_LIB_DECL#endif /* __cplusplus && _C_LIB_IS_EXTERN_C */  /* handle botched widechar constants as need be */#if defined(sun)||defined(__CENTERLINE__)||defined(_BORLAND31) \    || defined(__MWERKS__) #define _L(c) (c)#else #define _L(c) L##c#endif  /* handle botched auto aggregate inits as need be */#if defined(__CENTERLINE__) #define _Mbstinit(x) \ static mbstate_t _Mbst0; mbstate_t x; x = _Mbst0#else #define _Mbstinit(x)  mbstate_t x = {0}#endif  /* handle botched signed char type as need be */#if defined(_PH_ANALYZER)||defined(_BORLAND40) || defined(__MWERKS__) #define _HAS_SIGNED_CHAR 1#endif  /* handle botched array-new as need be */#if defined(_BORLAND40)  /* i.e. implements array-new */ #define _HAS_ARRAY_NEW 1#endif  /* handle botched exceptions as need be */#if defined(_BORLAND40)  /* i.e. implements exceptions */ #define _HAS_EXCEPTIONS 1#endif#if __MWERKS__#pragma options align=reset#endif#endif /* _YVALS *//* * Copyright (c) 1994 by P.J. Plauger.  ALL RIGHTS RESERVED.  * Consult your license regarding permissions and restrictions. *//* Change log: *94Oct03 Version received from PlumHall *94Oct08 Inserted MW changes. *94Oct11MM Added MW definition of _HAS_SIGNED_CHAR */