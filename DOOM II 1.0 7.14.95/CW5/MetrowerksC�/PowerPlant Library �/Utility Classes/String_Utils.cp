// ===========================================================================//	String_Utils.cp				 	�1993 Metrowerks Inc. All rights reserved.// ===========================================================================////	String manipulation utility functions#ifdef PowerPlant_PCH#include PowerPlant_PCH#endif#include <String_Utils.h>#ifndef __MEMORY__#include <Memory.h>#endif// ---------------------------------------------------------------------------//		� CopyPStr// ---------------------------------------------------------------------------//	Copy a Pascal string////		Returns a pointer to the copied string////		inDestSize specifies the maximum size of the destination (INCLUDING//			the length byte). The default is sizeof Str255.////		By calling this function as:////			CopyPStr(source, dest, sizeof(dest));////		you can change the declaration of dest without having to//		remember to change the call.StringPtrCopyPStr(	ConstStr255Param	inSourceStr,	StringPtr			outDestStr,	Int16				inDestSize){	Size	dataLen = StrLength(inSourceStr) + 1;	if (dataLen > inDestSize) {		dataLen = inDestSize;	}		::BlockMoveData(inSourceStr, outDestStr, dataLen);	return outDestStr;}// ---------------------------------------------------------------------------//		� ConcatPStr// ---------------------------------------------------------------------------//	Concatenate two Pascal strings. The first string becomes the combination//	of the first and second strings.////		Returns a pointer to the concatenated string////		inDestSize specifies the maximum size of the destination (INCLUDING//			the length byte). The default is sizeof Str255.////		By calling this function as:////			ConcatPStr(io, dest, sizeof(io));////		you can change the declaration of dest without having to//		remember to change the call.StringPtrConcatPStr(	Str255				ioFirstStr,	ConstStr255Param	inSecondStr,	Int16				inDestSize){								// Limit combined string to 255 chars    Int16	charsToCopy = inSecondStr[0];    if (StrLength(ioFirstStr) + charsToCopy > 255) {    	charsToCopy = 255 - StrLength(ioFirstStr);    }        if (charsToCopy > inDestSize - 1) {    	charsToCopy = inDestSize - 1;    }								// Copy second to end of first string    ::BlockMoveData(inSecondStr + 1,  ioFirstStr + StrLength(ioFirstStr) + 1,    					charsToCopy);    							// Set length of combined string    ioFirstStr[0] += charsToCopy;        return ioFirstStr;}// ---------------------------------------------------------------------------//		� OSTypeToPStr// ---------------------------------------------------------------------------//	Convert an OSType to a Pascal string and return a pointer to the string////	outString must point to a buffer (usually a string) that can hold//	the resulting string, including the length byte. Therefore, the buffer//	must be at least 5-bytes large. Usually, you will allocate an array//	of 5 unsigned char's.StringPtrOSTypeToPStr(	OSType		inOSType,	StringPtr	outString){	::BlockMoveData(&inOSType, outString + 1, sizeof(OSType));	outString[0] = sizeof(OSType);	return outString;}// ---------------------------------------------------------------------------//		� PStrToOSType// ---------------------------------------------------------------------------//	Convert an Pascal string to an OSTypevoidPStrToOSType(	const StringPtr	inString,	OSType			&outOSType){	::BlockMoveData(inString + 1, &outOSType, sizeof(OSType));}