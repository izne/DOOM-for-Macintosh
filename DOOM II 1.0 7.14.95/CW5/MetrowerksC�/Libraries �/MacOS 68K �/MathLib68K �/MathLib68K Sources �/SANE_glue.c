/* *	SANE Glue .... */#ifndef __SANE__#include <SANE.h>#endif#ifndef __TRAPS__#include <Traps.h>#endif/********************************************************************************                            Format Conversion Routines                        ********************************************************************************/asm void x96tox80(extended96 *x96,extended80 *x){	move.l	4(sp),a0	move.l	8(sp),a1	move.w	(a0)+,(a1)+	addq.w	#2,a0	move.l	(a0)+,(a1)+	move.l	(a0),(a1)	rts}asm void x80tox96(extended80 *x80,extended96 *x96){	move.l	4(sp),a0	move.l	8(sp),a1	move.w	(a0)+,(a1)+	clr.w	(a1)+	move.l	(a0)+,(a1)+	move.l	(a0),(a1)	rts}/********************************************************************************                            Remainder functions                               ********************************************************************************/asm void Remainder(extended80 *source,extended80 *dest,short *quot){	move.l	4(sp),-(sp)	move.l	12(sp),-(sp)	move.w	#0x000C,-(sp)	/* FREMX */	op _FP68K	move.l	12(sp),a0	move.w	d0,(a0)	rts}/********************************************************************************                              Relational operator                             ********************************************************************************/asm RelOp Relation(extended80 *source,extended80 *dest){	move.l	4(sp),-(sp)	move.l	12(sp),-(sp)	move.w	#0x000A,-(sp)	/* FCPXX */	op _FP68K	bne.s	L0	moveq	#EqualTo,d0	rtsL0:	bvc.s	L1	moveq	#Unordered,d0	rtsL1:	bmi.s	L2	moveq	#GreaterThan,d0	rtsL2:	moveq	#LessThan,d0	rts}/********************************************************************************                              Environment Functions                           ********************************************************************************/asm short SignNum(extended80 x){	pea		zeroword	pea		8(sp)	move.w	#0x2008,-(sp)	/* FCMPIX */	op _FP68K	bne.s	L0	moveq	#0,d0	rtszeroword:	dc.w	0L0:	bmi.s	L1	moveq	#1,d0	rtsL1:	moveq	#-1,d0	rts}#if	__MC68881__asm RoundDir GetRound(void){	DC.W	0xF200,0xB000	//	FMOVE	FPCR,D0	ANDI.W	#0x0030,D0	ROR.B	#4,D0	MOVE.B	L0(D0.W),D0	RTSL0:	DC.W	0x0003,0x0201}asm void SetRound(RoundDir r){	MOVEQ	#0,D0	MOVE.W	4(sp),D0	ANDI.W	#0x0003,D0	DC.W	0xF201,0xB000	//	FMOVE	FPCR,D1	ANDI.B	#0xCF,D1	OR.B	L0(D0.W),D1	DC.W	0xF201,0x9000	//	FMOVE	D1,FPCR	RTSL0:	DC.W	0x0030,0x2010}asm RoundPre GetPrecision(void){	DC.W	0xF200,0xB000	//	FMOVE	FPCR,D0	ANDI.W	#0x00C0,D0	ROR.B	#6,D0	MOVE.B	L0(D0.W),D0	RTSL0:	DC.W	0x0002,0x0103}asm void SetPrecision(RoundPre p){	MOVEQ	#0,D0	MOVE.W	4(sp),D0	ANDI.W	#0x0003,D0	DC.W	0xF201,0xB000	//	FMOVE	FPCR,D1	ANDI.B	#0x3F,D1	OR.B	L0(D0.W),D1	DC.W	0xF201,0x9000	//	FMOVE	D1,FPCR	RTSL0:	DC.W	0x0080,0x80C0}asm extended NaN(short n){	move.w	4(sp),d0			//	n	bne.s	L0	moveq	#15,d0L0:	addi.w	#0x4000,d0	clr.l	-(sp)				//	push _nan(n) onto stack	clr.w	-(sp)	move.w	d0,-(sp)	move.l	#0x7FFF7FFF,-(sp)	dc.w	0xF21F,0x4800		//	FMOVE.X	(SP)+,FP0	rts}/*static asm void SetGetHalt(void){	dc.w	0x0C40,0x0000,0x6712,0x0C40,0x0001,0x673C,0x0C40,0x0002,	dc.w	0x674A,0x303C,0xFE16,0xA9C9,0xF280,0x0000,0x2208,0xF327,	dc.w	0xF200,0xA800,0x8041,0xF200,0x8800,0xF200,0xB000,0xC041,	dc.w	0x0280,0x0000,0xFF00,0x670C,0x7000,0x102F,0x0001,0x08B7,	dc.w	0x0003,0x0000,0xF35F,0x4E75,0x327C,0x00C0,0x20D9,0x20D9,	dc.w	0x20D9,0x20D9,0x20D9,0x20D9,0x20D9,0x4E75,0x327C,0x00C0,	dc.w	0x22D8,0x22D8,0x22D8,0x22D8,0x22D8,0x22D8,0x22D8,0x4E75,	dc.w	0x48E7,0x80C0,0x303C,0xA097,0xA346,0x2248,0x303C,0xA89F,	dc.w	0xA346,0xB3C8,0x6618,0x7070,0xA51E,0x6B1A,0x2248,0x41FA,	dc.w	0xFF70,0x7070,0xA02E,0x2049,0x303C,0xA097,0xA247,0x4CDF,	dc.w	0x0301,0xA097,0x4E75,0x303C,0xFE16,0xA9C9}*/asm void GetTrapVector(TrapVector *tv){	lea		0x00c0,a0	move.l	4(sp),a1	moveq	#7-1,d0l0:	move.l	(a0)+,(a1)+	dbf		d0,l0	rts}asm void SetTrapVector(TrapVector *tv){	move.l	4(sp),a0	lea		0x00c0,a1	moveq	#7-1,d0l0:	move.l	(a0)+,(a1)+	dbf		d0,l0	rts}#elsevoid SetRound(RoundDir r){	Environment ev;	GetEnvironment(&ev);	ev=(ev&0xFF9F)|((r&3)<<12);	SetEnvironment(&ev);}RoundDir GetRound(void){	Environment ev;	GetEnvironment(&ev);	return((ev>>12)&3);}void SetPrecision(RoundPre r){	Environment ev;	GetEnvironment(&ev);	ev=(ev&0xFF9F)|((r&3)<<5);	SetEnvironment(&ev);}RoundPre GetPrecision(void){	Environment ev;	GetEnvironment(&ev);	return((ev>>5)&3);}asm extended NaN(short n){	move.w	8(sp),d0		/* n */	bne.s	L0	moveq	#15,d0L0:	addi.w	#0x4000,d0	move.l	4(sp),a0		/* address of return extended */	move.w	#0x7FFF,(a0)+	move.w	d0,(a0)+	clr.l	(a0)+	clr.w	(a0)+	rts}#endif