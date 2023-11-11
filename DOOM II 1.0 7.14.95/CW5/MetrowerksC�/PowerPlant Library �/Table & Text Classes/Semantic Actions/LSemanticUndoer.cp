// ===========================================================================//	LSemanticUndoer.cp			�1994 Metrowerks Inc. All rights reserved.// ===========================================================================#ifdef PowerPlant_PCH#include PowerPlant_PCH#endif#include	<PP_Prefix.h>#include	"LSemanticUndoer.h"#include	<LSemanticAction.h>#include	<LSelection.h>#include	<String_Utils.h>#include	<PP_Messages.h>#include	<PP_Resources.h>#include	<Resources.h>LSemanticUndoer::LSemanticUndoer(){	mAction = NULL;}LSemanticUndoer::~LSemanticUndoer(){	ClearAction();}void	LSemanticUndoer::PostAction(LSemanticAction *inAction, Boolean inPreRecordSelection){	ClearAction();		mAction = inAction;		if (mAction) {		if (inAction->GetSelection() && inPreRecordSelection)			inAction->GetSelection()->RecordPresentSelection();		mAction->Redo();	}}void	LSemanticUndoer::ClearAction(void){	if (mAction)		mAction->Finalize();	mAction = NULL;}Boolean	LSemanticUndoer::ObeyCommand(	CommandT	inCommand,	void		*ioParam){	switch (inCommand) {			case cmd_Undo:			Assert_(mAction);			if (mAction->CanRedo() || mAction->CanUndo()) {				if (mAction->CanUndo())					mAction->Undo();				else					mAction->Redo();			}			return true;			break;					default:			return inherited::ObeyCommand(inCommand, ioParam);			break;	}	return false;}void	LSemanticUndoer::FindCommandStatus(	CommandT	inCommand,	Boolean		&outEnabled,	Boolean		&outUsesMark,	Char16		&outMark,	Str255		outName){	switch (inCommand) {		case cmd_Undo:			outEnabled = false;			if (mAction) {				ResIDT	resID;				Int16	index;								if (mAction->CanRedo()) {					outEnabled = true;					mAction->GetDescription(&resID, &index);					GetIndString(outName, resID, index);					if (ResError() != noErr) {						GetIndString(outName, STRx_RedoGenericDescriptions, str_RedoUndo);					}					ThrowIfResError_();				} else if (mAction->CanUndo()) {					outEnabled = true;					mAction->GetDescription(&resID, &index);					GetIndString(outName, resID, index);					if (ResError() != noErr) {						GetIndString(outName, STRx_UndoGenericDescriptions, str_RedoUndo);					}					ThrowIfResError_();				}			}			if (!outEnabled) {				GetIndString(outName, STRx_Standards, str_CantUndo);				ThrowIfResError_();			}			break;		default:			inherited::FindCommandStatus(inCommand, outEnabled, outUsesMark, outMark, outName);			break;	}}