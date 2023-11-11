/* graphics libraries	scaler library interface to the routines used to call gxFont scalers directly	by Mike Reed	Copyright 1992 - 1993 Apple Computer, Inc.  All rights reserved.*/#pragma once#ifndef scalerLibraryIncludes	#define scalerLibraryIncludes		#ifndef __COMPONENTS__		#include "Components.h"	#endif		#ifndef scalerTypeIncludes	    #include "scaler types.h"	#endif	enum {	    kScalerOpenSelect = 0,                      /* Component scaler operation selectors */	    kScalerNewFontSelect,	    kScalerNewVariationSelect,	    kScalerNewTransformationSelect,	    kScalerNewGlyphSelect,	    kScalerStreamFontSelect,	    kScalerKernGlyphsSelect,	    kScalerCloseSelect	};		#ifdef __cplusplus	extern "C" {	#endif		scalerError FontScalerOpen( scalerContext* context, scalerInfo* theInfo);	scalerError FontScalerNewFont( scalerContext* context, scalerFontInfo* info);	scalerError FontScalerNewVariation( scalerContext* context, long count, const gxFontVariation variation[]);	scalerError FontScalerNewTransform( scalerContext* context, const scalerTransform* trans, scalerTransformInfo* info);	scalerError FontScalerNewGlyph( scalerContext* context, const scalerGlyph* glyph, scalerMetrics* theMetrics, scalerBitmap* bitImage, void* outlineData);	scalerError FontScalerStreamFont( scalerContext* context, scalerStream* stream);	scalerError FontScalerKernGlyphs( scalerContext* context, scalerKerning* kerning, gxPoint deltas[], scalerKerningNote flags[], gxPoint locations[]);	scalerError FontScalerClose( scalerContext* context);	void    CloseAllOpenFontScalers(void);		#define FSCCallNow(selector,size) ComponentCallNow(selector,size)		pascal ComponentResult ComponentFontScalerOpen(ComponentInstance c, scalerContext* context, scalerInfo* theInfo)			FSCCallNow( 0 /* kScalerOpenSelect */, 8 /* sizeof(scalerContext*)+sizeof(scalerInfo*) */ );				pascal ComponentResult ComponentFontScalerNewFont(ComponentInstance c, scalerContext* context, scalerFontInfo* info)			FSCCallNow( 1 /* kScalerNewFontSelect */, 8 /* sizeof(scalerContext*)+sizeof(scalerFontInfo*) */ );				pascal ComponentResult ComponentFontScalerNewVariation(ComponentInstance c, scalerContext* context, long count, const gxFontVariation variation[])			FSCCallNow( 2 /* kScalerNewVariationSelect */, 12 /* sizeof(scalerContext*)+sizeof(long)+sizeof(gxFontVariation*) */);				pascal ComponentResult ComponentFontScalerNewTransform(ComponentInstance c, scalerContext* context, const scalerTransform* trans, scalerTransformInfo* info)			FSCCallNow( 3  /* kScalerNewTransformationSelect */, 12 /* sizeof(scalerContext*)+sizeof(scalerTransform*)+sizeof(scalerTransformInfo*) */);				pascal ComponentResult ComponentFontScalerNewGlyph(ComponentInstance c, scalerContext* context, const scalerGlyph* glyph, scalerMetrics* theMetrics, scalerBitmap* bitImage, void* outlineData)			FSCCallNow( 4 /* kScalerNewGlyphSelect */, 20 /* sizeof(scalerContext*)+sizeof(scalerGlyph*)+sizeof(scalerMetrics*)+sizeof(scalerBitmap*)+sizeof(void*) */);		pascal ComponentResult ComponentFontScalerStreamFont(ComponentInstance c, scalerContext* context, scalerStream* Stream)			FSCCallNow( 5 /* kScalerStreamFontSelect */, 8 /* sizeof(scalerContext*)+sizeof(scalerStream*) */);		pascal ComponentResult ComponentScalerKernGlyphs(ComponentInstance c, scalerContext* context, scalerKerning* kerning, gxPoint deltas[], scalerKerningNote flags[], gxPoint locations[])			FSCCallNow( 6 /* kScalerKernGlyphsSelect */, 20 /* sizeof(scalerContext*)+sizeof(scalerKerning* )+sizeof(gxPoint*)+sizeof(gxPoint*)+sizeof(unsigned short*) */);					pascal ComponentResult ComponentFontScalerClose(ComponentInstance c, scalerContext* context)			FSCCallNow( 7 /* kScalerCloseSelect */, 4 /* sizeof(scalerContext*) */);		#undef FSCCallNow	#ifdef __cplusplus	}	#endif#endif