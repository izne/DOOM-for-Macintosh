// ===========================================================================//	LAttachable.h					�1994 Metrowerks Inc. All rights reserved.// ===========================================================================#pragma once#include <PP_Prefix.h>class	LAttachment;class	LList;class	LAttachable {public:					LAttachable();	virtual			~LAttachable();		virtual void	AddAttachment(LAttachment *inAttachment,							LAttachment *inBefore = nil,							Boolean inOwnsAttachment = true);	virtual void	RemoveAttachment(LAttachment *inAttachment);	virtual void	RemoveAllAttachments();		virtual Boolean	ExecuteAttachments(MessageT inMessage, void *ioParam);protected:	LList			*mAttachments;};