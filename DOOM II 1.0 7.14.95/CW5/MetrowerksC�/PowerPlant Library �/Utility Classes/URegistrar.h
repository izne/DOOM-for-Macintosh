// ===========================================================================//	URegistrar.h					�1993 Metrowerks Inc. All rights reserved.// ===========================================================================#pragma once#include <PP_Prefix.h>class	LStream;typedef void*	(*ClassCreatorFunc)(LStream *);typedef struct SClassTable {	ClassIDT			classID;	ClassCreatorFunc	creatorFunc;} SClassTable, *SClassTableP, **SClassTableH;class	URegistrar {public:	static void		RegisterClass(ClassIDT inClassID,								ClassCreatorFunc inCreatorFunc);									static void*	CreateObject(ClassIDT inClassID, LStream *inStream);	protected:	static Int16		sClassCount;	static SClassTableH	sTableH;		static Int16	FetchClassIndex(ClassIDT inClassID);};