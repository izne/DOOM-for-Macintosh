/* graphics libraries:    picture library    by Cary Clark, Georgiann Delaney, Michael Fairman, Dave Good, Robert Johnson, Keith McGreggor, Oliver Steele, David Van Brink, Chris Yerga    Copyright 1987 - 1991 Apple Computer, Inc.  All rights reserved.    */#include "graphics libraries.h"#include "math routines.h"void AddToPicture(gxShape picture, gxShape newShape, gxStyle newStyle, gxInk newInk, gxTransform newTransform){    GXSetPictureParts(picture, 0, 0, 1, &newShape, &newStyle, &newInk, &newTransform);}void InsertPictureItem(gxShape picture, long index, gxShape newShape, gxStyle newStyle, gxInk newInk, gxTransform newTransform){    GXSetPictureParts(picture, index, 0, 1, &newShape, &newStyle, &newInk, &newTransform);}gxShape GetPictureItem(gxShape picture, long index, gxShape *destShape, gxStyle *destStyle, gxInk *destInk, gxTransform *destTransform){    gxShape tempShape;    if( destShape == nil )        destShape = &tempShape;    if( GXGetPictureParts(picture, index, 1, destShape, destStyle, destInk, destTransform) )        return *destShape;    return nil;}void SetPictureItem(gxShape picture, long index, gxShape newShape, gxStyle newStyle, gxInk newInk, gxTransform newTransform){    GXSetPictureParts(picture, index, 1, 1, &newShape, &newStyle, &newInk, &newTransform);}