/*	File:		graphics linkage.h	Copyright:	� 1984-1994 by Apple Computer, Inc., all rights reserved.	WARNING	This file was auto generated by the interfacer tool. Modifications	must be made to the master file.*/#ifndef graphicsLinkageIncludes#define graphicsLinkageIncludes#define QuickDrawGXTrap 0xA832#define gxAnyContext 0x00#define gxNeedClient 0x10#define gxNeedStack 0x20#define gxNeedHeap 0x40#define gxEnvironmentControl 0x80#define GXParameterCount(paramEntry) ( (paramEntry) & 0x0F )#ifndef dontUseInLines#define GXTrap(x, environment) ={0x303C, x, QuickDrawGXTrap}#else#define GXTrap(x, environment)#endif#define GXInlineCode(x, environment) GXTrap(x, environment)#endif