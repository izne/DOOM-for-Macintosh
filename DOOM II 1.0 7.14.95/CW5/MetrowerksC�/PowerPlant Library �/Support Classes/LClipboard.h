// ===========================================================================//	LClipboard.h					�1993 Metrowerks Inc. All rights reserved.// ===========================================================================#pragma once#include <PP_Prefix.h>class	LClipboard {public:					LClipboard();	virtual			~LClipboard();		virtual void	SetData(ResType inDataType, Handle inDataH,								Boolean inReset = true);	virtual void	SetData(ResType inDataType, Ptr inDataPtr,							Int32 inDataLength, Boolean inReset = true);								virtual Int32	GetData(ResType inDataType, Handle ioData);		virtual void	Resume();	virtual void	Suspend();		static LClipboard*	GetClipboard();protected:	static LClipboard	*sClipboard;		Boolean			mImportPending;	Boolean			mExportPending;		virtual void	SetDataSelf(ResType inDataType, Ptr inDataPtr,							Int32 inDataLength, Boolean inReset);	virtual Int32	GetDataSelf(ResType inData, Handle ioDataH);		virtual void	ImportSelf();	virtual void	ExportSelf();};