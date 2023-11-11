//	===========================================================================//	UEventUtils.cp				�1994 Metrowerks Inc. All rights reserved.//	===========================================================================#ifdef PowerPlant_PCH#include PowerPlant_PCH#endif#include	<PP_Prefix.h>#include	"UEventUtils.h"Boolean	UEventUtils::KeyDownInMap(Uint16 inKeyScanCode, const KeyMap inKeys){	Uint8	*p = (Uint8 *) inKeys;	Uint8	index = inKeyScanCode >> 3;			Assert_((16 > index) && (index >= 0));	Uint8	subvalue = p[index];	Uint8	subposition = inKeyScanCode & 0x07;	Uint8	mask = 0x01 << subposition;		if ((subvalue & mask) != 0)		return true;	else		return false;}short	UEventUtils::GetModifiers(void){	KeyMap		keys;	short		modifiers = Button() ? btnState : 0;	GetKeys(keys);	if (KeyDownInMap(cmdKeyCode, keys))		modifiers |= cmdKey;	if (KeyDownInMap(shiftKeyCode, keys))		modifiers |= shiftKey;	if (KeyDownInMap(alphaLockKeyCode, keys))		modifiers |= alphaLock;	if (KeyDownInMap(optionKeyCode, keys))		modifiers |= optionKey;	if (KeyDownInMap(controlKeyCode, keys))		modifiers |= controlKey;		return modifiers;}