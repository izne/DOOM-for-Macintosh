/* *	math.h glue functions (long double precision math) ... */#ifndef __FP__#include <fp.h>#endif/* *	We can't include both fp.h and math.h since their definitions of cos, etc... conflict */#ifdef cplusplusextern "C" {#endiflong double cosl(long double);long double sinl(long double);long double tanl(long double);long double acosl(long double);long double asinl(long double);long double atanl(long double);long double atan2l(long double, long double);long double coshl(long double);long double sinhl(long double);long double tanhl(long double);long double expl(long double);long double frexpl(long double, int *);long double ldexpl(long double, int);long double logl(long double);long double log10l(long double);long double modfl(long double, long double *);long double fabsl(long double);long double powl(long double, long double);long double sqrtl(long double);long double ceill(long double);long double floorl(long double);long double fmodl(long double, long double);#ifdef cplusplus}#endif/********************************************************************************                            Trigonometric functions                           ********************************************************************************/long double cosl(long double x){	return((cos)(x));}long double sinl(long double x){	return((sin)(x));}long double tanl(long double x){	return((tan)(x));}long double acosl(long double x){	return((acos)(x));}long double asinl(long double x){	return((asin)(x));}long double atanl(long double x){	return((atan)(x));}long double atan2l(long double x, long double y){	return((atan2)(x,y));}/********************************************************************************                              Hyperbolic functions                            ********************************************************************************/long double coshl(long double x){	return((cosh)(x));}long double sinhl(long double x){	return((sinh)(x));}long double tanhl(long double x){	return((tanh)(x));}/********************************************************************************                              Exponential functions                           ********************************************************************************/long double expl(long double x){	return((exp)(x));}long double frexpl(long double x, int *exponent){	return((frexp)(x,exponent));}long double ldexpl(long double x, int n){	return((ldexp)(x,n));}long double logl(long double x){	return((log)(x));}long double log10l(long double x){	return((log10)(x));}/********************************************************************************                     Power and absolute value functions                       ********************************************************************************/long double fabsl(long double x){	return((fabs)(x));}long double powl(long double x, long double y){	return((pow)(x,y));}long double sqrtl(long double x){	return((sqrt)(x));}/********************************************************************************                        Nearest integer functions                             ********************************************************************************/long double ceill(long double x){	return((ceil)(x));}long double floorl(long double x){	return((floor)(x));}/********************************************************************************                            Remainder functions                               ********************************************************************************/long double fmodl(long double x, long double y){	return((fmod)(x,y));}