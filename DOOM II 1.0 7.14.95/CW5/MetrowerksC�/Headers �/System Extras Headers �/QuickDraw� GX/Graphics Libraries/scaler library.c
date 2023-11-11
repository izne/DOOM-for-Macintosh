/***  scaler library.c    -   Routines used to call gxFont scalers directly****  Copyright 1992 Apple Computer, Inc.  All rights reserved.*/#include <Memory.h>         /* NewPtr(), DisposePtr() */#include <GestaltEqu.h>         /* For calling Gestalt() */#include "Components.h"         /* Component Manager declarations */#include "scaler types.h"           /* Scaler API declarations and definitions */#include "scaler library.h"     /* The library's decls and defs. */#define kComponentScalerType    'sclr'          /* Type-gxTag for gxFont scaler components */#ifndef __powerc#define PostScalerError(c,e) (((c)->PostErrorFunction != nil && (e) != scaler_no_problem)? (c)->PostErrorFunction((c),(e)),e: e)#else#define PostScalerError(c,e) (((c)->PostErrorFunction && (e) != scaler_no_problem) ? CallPostErrorFunctionProc((c)->PostErrorFunction,(c),(e)),(e):(e))#endiftypedef struct {    gxFontFormatTag       format;                 /* Font format the scaler supports */    Component       theScalerComponent;         /* Used to open the component scaler */    ComponentInstance   theOpenedScalerComponent;   /* Component scaler (if 0, needs opening)*/} scalerEntry, *scalerEntryPtr;                 /* Information associated with a component gxFont scaler */typedef struct {    long                oldSeed;                    /* Component Mgr's instance seed */    long                numScalers;             /* Number of scalers in the list */    scalerEntry     entries[1];             /* The array of scalers */} scalerList, *scalerListPtr;                       /* Component gxFont scaler list structure */static  scalerListPtr   theListOfScalers = nil;     /* The list (really an array) of all registered component scalers */static  Boolean     FindComponentFontScalers(void);static  scalerError FindMatchingOpenScaler(ComponentInstance *theScaler, scalerContext *theContext);static  Boolean     FindComponentFontScalers(void);static  scalerListPtr   BuildScalerList(void);static  scalerEntryPtr  FindMatchingScaler(scalerListPtr theList, gxFontFormatTag format);static  scalerListPtr   UpdateScalerList(scalerListPtr oldList);scalerError FontScalerOpen( scalerContext* context, scalerInfo* theInfo){    scalerError error;    ComponentInstance   theOpenScaler;        if (!context) return scaler_null_context;        if (theListOfScalers == nil)        if (!FindComponentFontScalers())			return PostScalerError(context,scaler_unsupported_font_format);        if ((error = FindMatchingOpenScaler(&theOpenScaler, context)) == 0) {        error = ComponentFontScalerOpen(theOpenScaler, context, theInfo);    }        return error;}scalerError FontScalerClose( scalerContext* context){    scalerError error;    ComponentInstance   theOpenScaler;        if (!context) return scaler_null_context;    if ((error = FindMatchingOpenScaler(&theOpenScaler, context)) == 0) {        error = ComponentFontScalerClose(theOpenScaler, context);    }        return error;}scalerError FontScalerNewFont( scalerContext* context, scalerFontInfo* info){    scalerError error;    ComponentInstance   theOpenScaler;        if (!context) return scaler_null_context;    if ((error = FindMatchingOpenScaler(&theOpenScaler, context)) == 0) {        error = ComponentFontScalerNewFont(theOpenScaler, context, info);    }        return error;}scalerError FontScalerNewVariation( scalerContext* context, long count, const gxFontVariation variation[]){    scalerError error;    ComponentInstance   theOpenScaler;        if (!context) return scaler_null_context;    if ((error = FindMatchingOpenScaler(&theOpenScaler, context)) == 0) {        error = ComponentFontScalerNewVariation(theOpenScaler, context, count, (gxFontVariation*)variation);    }        return error;}scalerError FontScalerNewTransform( scalerContext* context, const scalerTransform* trans, scalerTransformInfo* info){    scalerError error;    ComponentInstance   theOpenScaler;        if (!context) return scaler_null_context;    if ((error = FindMatchingOpenScaler(&theOpenScaler, context)) == 0) {        error = ComponentFontScalerNewTransform(theOpenScaler, context, trans, info);    }        return error;}scalerError FontScalerNewGlyph( scalerContext* context, const scalerGlyph* glyph, scalerMetrics* theMetrics, scalerBitmap* bitImage, void* outlineData){    scalerError error;    ComponentInstance   theOpenScaler;        if (!context) return scaler_null_context;    if ((error = FindMatchingOpenScaler(&theOpenScaler, context)) == 0) {        error = ComponentFontScalerNewGlyph(theOpenScaler, context, glyph, theMetrics, bitImage, outlineData);    }        return error;}scalerError FontScalerKernGlyphs( scalerContext* context, scalerKerning* kerning, gxPoint deltas[], scalerKerningNote flags[], gxPoint locations[]){    scalerError error;    ComponentInstance   theOpenScaler;        if (!context) return scaler_null_context;    if ((error = FindMatchingOpenScaler(&theOpenScaler, context)) == 0) {        error = ComponentScalerKernGlyphs(theOpenScaler, context,  kerning, deltas, flags, locations);    }        return error;}scalerError FontScalerStreamFont( scalerContext* context, scalerStream* stream){    scalerError error;    ComponentInstance   theOpenScaler;        if (!context) return scaler_null_context;    if ((error = FindMatchingOpenScaler(&theOpenScaler, context)) == 0) {        error = ComponentFontScalerStreamFont(theOpenScaler, context, stream);    }        return error;}/***  CloseAllOpenFontScalers()   -   Runs down the list of registered component scalers and closes the open ones.*/void    CloseAllOpenFontScalers(void){    if (theListOfScalers != nil) {        unsigned short  whichScaler;                for (whichScaler = 0; whichScaler < theListOfScalers->numScalers; whichScaler++) {            if (theListOfScalers->entries[whichScaler].theOpenedScalerComponent != nil) {                CloseComponent(theListOfScalers->entries[whichScaler].theOpenedScalerComponent);            }        }                DisposePtr((Ptr)theListOfScalers);        theListOfScalers = nil;    }}/***  FindComponentFontScalers()  -   Called during the first ScalerOpen() call, this routine first determines whether the **                          Component Manager is present.  If it is, it registers the built-in scalers and then**                          asks the Component Manager to find all of the registered scaler components.  A list**                          of these is built, and if all goes well the routine will return true, indicating that**                          component gxFont scalers are in use.  If it returns false, the built-in scalers will**                          need to be called directly.*/static  Boolean FindComponentFontScalers(){    long    dummyLong;        if (Gestalt(gestaltComponentMgr, &dummyLong) == 0) {        if ((theListOfScalers = BuildScalerList()) != 0) {            return true;        }    }    return false;}/***  FindMatchingScaler()    -   Given a gxFont format and the list of all registered component gxFont scalers, it looks for**                      a scaler (actually the first) that supports the format and returns the address of**                      its list entry.  It will return nil if no scaler is found for the format.*/static  scalerEntryPtr  FindMatchingScaler(register scalerListPtr theList, gxFontFormatTag format){    unsigned short  numScalers;    register scalerEntryPtr entry;        numScalers = theList->numScalers;    entry = theList->entries;    do {        if (entry->format == format)            return entry;        entry++;    } while (--numScalers);        return nil;}/***  UpdateScalerList()  -   Builds a new list of registered scalers in response to a changed component list**                      seed.  It copies from the old list the entries for those scalers that are in both lists.*/static  scalerListPtr   UpdateScalerList(scalerListPtr oldList){    scalerListPtr       newList;    unsigned short      whichScaler;    scalerEntryPtr      oldEntry;        newList = BuildScalerList();        /* Create an all-new scaler list */        /* We copy the entries from the old list for those scalers in both lists (they may be open)*/    for (whichScaler = 0; whichScaler < newList->numScalers; whichScaler++) {        if ((oldEntry = FindMatchingScaler(oldList, newList->entries[whichScaler].format)) != 0) {            newList->entries[whichScaler] = *oldEntry;        }    }        DisposePtr((Ptr)oldList);    theListOfScalers = newList;        return newList;}/***  BuildScalerList()   -   Queries the Component Manager to find all currently registered component scalers.**                      It allocates and builds a list of these scalers.*/static  scalerListPtr   BuildScalerList(){    long                    numScalers;    ComponentDescription    looking, theInfo;    register    scalerListPtr   newList = nil;        /* Fill in the description structure */    looking.componentType           = kComponentScalerType;    looking.componentSubType            = kAnyComponentSubType;    looking.componentManufacturer       = kAnyComponentManufacturer;    looking.componentFlags          = 0;    looking.componentFlagsMask      = 0;        /* Count the number of components and create our list based on that number*/        numScalers = CountComponents(&looking);        {   register scalerEntryPtr entry;        Component           lastComponent = 0, currentComponent;                newList = (scalerListPtr) NewPtr(sizeof(scalerList) + (numScalers - 1)*sizeof(scalerEntry));                newList->oldSeed        = GetComponentListModSeed();        newList->numScalers = numScalers;                    /* For each scaler, get its info and remember the subtype, this is the format */        entry = newList->entries;        do {            currentComponent = FindNextComponent(lastComponent, &looking);                    if (GetComponentInfo(currentComponent,&theInfo, nil, nil, nil) == 0) {                entry->format                   = theInfo.componentSubType;                entry->theScalerComponent       = currentComponent;                entry->theOpenedScalerComponent = nil;            } else                entry->format = 0;  /* unusable scaler */                            lastComponent = currentComponent;            entry++;        } while(--numScalers);    }        return newList;}/***  FindMatchingOpenScaler()    -   This routine will look for a component scaler that supports the gxFont format**                          specified in the given scalerContext.  If necessary, the list of scalers will be**                          updated.  If a matching component scaler is found and is not open, the component**                          is opened.****                          The routine returns scaler_no_problem if all went well,**                          scaler_unsupported_font_format if a component was not found or couldn't be**                          opened.****                          If scaler_no_problem is returned *theOpenScaler will contain the**                          component instance for the desired scaler, else it will be undefined.*/static  scalerError FindMatchingOpenScaler(ComponentInstance* theOpenScaler, scalerContext* theContext){    scalerError result = scaler_no_problem;    scalerEntryPtr  theScaler;        /* Check if a component was added while we were away, we may need to update the list of scalers */    if (GetComponentListModSeed() != theListOfScalers->oldSeed)        theListOfScalers = UpdateScalerList(theListOfScalers);            theScaler = FindMatchingScaler(theListOfScalers, theContext->format);        if (theScaler == nil) {         result = scaler_unsupported_font_format;    } else {        if (theScaler->theOpenedScalerComponent == nil) {            if ((theScaler->theOpenedScalerComponent = OpenComponent(theScaler->theScalerComponent)) == nil)                result = scaler_unsupported_font_format;        }                *theOpenScaler = theScaler->theOpenedScalerComponent;    }        return PostScalerError(theContext, result);}