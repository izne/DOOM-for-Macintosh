////	CodeResource / DRVR / DA support functions //#ifndef __SETUPA4__#define __SETUPA4__#ifndef powercstatic void RememberA4(void);static asm long SetUpA4(void){	move.l	a4,d0	lea		storage,a4	move.l	(a4),a4	rtsstorage:	dc.l	0	// this storage is only referenced thru data cache	entry	static RememberA4	lea		storage,a0	move.l	a4,(a0)	rts}static long RestoreA4(long:__D0):__D0 = 0xC18C;#endif#endif