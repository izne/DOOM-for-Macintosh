// ===========================================================================//	LUndoer.cp						�1994 Metrowerks Inc. All rights reserved.// ===========================================================================#ifdef PowerPlant_PCH#include PowerPlant_PCH#endif#include <LUndoer.h>LUndoer::LUndoer(){}LUndoer::~LUndoer(){}voidLUndoer::RedoAction(	ActionIDT	inActionID,	void		*inUndoData){	UndoAction(inActionID, inUndoData);}voidLUndoer::FinalizeAction(	ActionIDT	inActionID,	void		*inUndoData){}