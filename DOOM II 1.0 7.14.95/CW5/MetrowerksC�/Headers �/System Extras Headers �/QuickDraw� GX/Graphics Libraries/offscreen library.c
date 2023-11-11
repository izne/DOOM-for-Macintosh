/* graphics libraries:	offscreen support library	by Cary Clark, Georgiann Delaney, Michael Fairman, Dave Good, Robert Johnson, Keith McGreggor, Oliver Steele, David Van Brink, Chris Yerga	Copyright 1987 - 1991 Apple Computer, Inc.  All rights reserved. */	#include <Memory.h>#include "graphics libraries.h"#include "offscreen library.h"typedef struct viewPortBufferRecord {	gxViewPort	masterPort;	gxViewPort	slavePort;	gxShape		area;	gxShape		draw;	gxViewGroup	group;	long			deviceCount;	gxViewDevice	devices[1];} viewPortBufferRecord;static void ClearBytes(void *block, register long length){	register char *b = (char *) block;		do		*b++ = 0;	while (--length);}/*	Given a gxBitmap gxShape, this routine fills out the offscreen structure with the objects necessary to draw into	the offscreen and transfer the offscreen onscreen.  See the header file for details about the offscreen structure.*/void CreateOffscreen(register offscreen *os, gxShape bitShape){	NilParamReturn(os);	ClearBytes(os, sizeof(offscreen));	NilShapeReturn(bitShape);	os->group = GXNewViewGroup();	os->device = GXNewViewDevice(os->group, bitShape);	os->port = GXNewViewPort(os->group);	os->draw = GXGetViewDeviceBitmap(os->device);		/* draw this gxShape to copy the offscreen to the screen */	os->xform = GXNewTransform();				/* assign this to shapes and they will draw offscreen */	GXSetTransformViewPorts(os->xform, 1, &os->port);}/*	When you're done with the offscreen, call this routine.*/void DisposeOffscreen(offscreen *os){	NilParamReturn(os);	GXDisposeShape(os->draw);		/* dispense with gxBitmap gxShape */	GXDisposeTransform(os->xform);	/* �and gxTransform */	GXDisposeViewGroup(os->group);	/* �and gxViewGroup (which disposes gxViewPort and gxViewDevice) */}void CopyToBitmaps(gxShape dest, gxShape source){	offscreen destOff;		NilShapeReturn(dest);	NilShapeReturn(source);	CreateOffscreen(&destOff, dest);	{	long portCount = GXGetShapeViewPorts(source, nil);		gxViewPort *savedPorts = (gxViewPort *) NewPtr(sizeof(gxViewPort) * portCount);				GXGetShapeViewPorts(source, savedPorts);		GXSetShapeViewPorts(source, 1, &destOff.port);		GXDrawShape(source);		GXSetShapeViewPorts(source, portCount, savedPorts);		DisposePtr((Ptr) savedPorts);	}	DisposeOffscreen(&destOff);}typedef struct {	gxBitmap	bits;	gxPoint		offset;} areaCharacteristics;static areaCharacteristics GetAreaCharacteristics(gxShape area, gxViewDevice device, gxViewPort port){	areaCharacteristics x;	gxRectangle bounds;	gxShape bitShape;	gxMapping map;	ClearBytes(&x, sizeof(x));	ClearBytes(&bounds, sizeof(bounds));	ClearBytes(&map, sizeof(map));	bitShape = GXGetViewDeviceBitmap(device);	GXGetShapeGlobalBounds(area, port, nil, &bounds);	GXGetBitmap(bitShape, &x.bits, nil);	if (x.bits.space == gxIndexedSpace)		GXCloneColorSet(x.bits.set);	if (x.bits.profile)		GXCloneColorProfile(x.bits.profile);	GXDisposeShape(bitShape);	x.offset.x = bounds.left;	x.offset.y = bounds.top;	x.bits.width = FixedRound(bounds.right) - FixedRound(bounds.left);	x.bits.height = FixedRound(bounds.bottom) - FixedRound(bounds.top);	InvertMapping(&map, GXGetViewPortGlobalMapping(port, &map));	MapPoints(&map, 1, &x.offset);	return x;}static void DisposeAreaCharacteristics(areaCharacteristics *x){	if (x->bits.space == gxIndexedSpace)		GXDisposeColorSet(x->bits.set);	if (x->bits.profile)		GXDisposeColorProfile(x->bits.profile);}static gxViewDevice NewDeviceWithAreaCharacteristics(gxViewGroup group, areaCharacteristics *x){	gxViewDevice device;	gxShape bitShape;	gxMapping map;	NilParamReturnNil(x);	bitShape = GXNewBitmap(&x->bits, nil);	device = GXNewViewDevice(group, bitShape);	GXDisposeShape(bitShape);	ResetMapping(&map);	MoveMapping(&map, -x->offset.x, -x->offset.y);	GXSetViewDeviceMapping(device, &map);	return device;}static areaCharacteristics GetDeviceAreaCharacteristics(gxViewDevice device){	areaCharacteristics x;	gxShape bitShape;	gxMapping map;	ClearBytes(&x, sizeof(x));	ClearBytes(&map, sizeof(map));	bitShape = GXGetViewDeviceBitmap(device);	GXGetBitmap(bitShape, &x.bits, nil);	if (x.bits.space == gxIndexedSpace)		GXCloneColorSet(x.bits.set);	if (x.bits.profile)		GXCloneColorProfile(x.bits.profile);	GXDisposeShape(bitShape);	GXGetViewDeviceMapping(device, &map);	x.offset.x = -map.map[2][0];	x.offset.y = -map.map[2][1];	return x;}static void SetDeviceAreaCharacteristics(gxViewDevice device, areaCharacteristics *x){	gxShape bitShape;	gxBitmap bits;	gxMapping map;	NilParamReturn(x);	bits.width = 1;	bits.height = 1;	bits.pixelSize = 1;	bits.rowBytes = 0;	bits.image = nil;	bits.space = gxIndexedSpace;	bits.set = nil;	bits.profile = nil;	bitShape = GXNewBitmap(&bits, nil);	GXSetViewDeviceBitmap(device, bitShape);	GXDisposeShape(bitShape);	bitShape = GXNewBitmap(&x->bits, nil);	GXSetViewDeviceBitmap(device, bitShape);	GXDisposeShape(bitShape);	ResetMapping(&map);	MoveMapping(&map, -x->offset.x, -x->offset.y);	GXSetViewDeviceMapping(device, &map);}static boolean EqualAreaCharacteristics(areaCharacteristics *x, areaCharacteristics *y){	NilParamReturnNil(x);	NilParamReturnNil(y);	return x->bits.space == y->bits.space		&& x->bits.set == y->bits.set		&& x->bits.profile == y->bits.profile		&& x->offset.x == y->offset.x		&& x->offset.y == y->offset.y		&& x->bits.width == y->bits.width		&& x->bits.height == y->bits.height		&& x->bits.pixelSize == y->bits.pixelSize;}static void AddBitmapShapes(viewPortBufferRecord *buffers){	short i;	NilParamReturn(buffers);	for (i = 0; i < buffers->deviceCount; i++) 	{	gxViewDevice device = buffers->devices[i];		gxShape bitShape = GXGetViewDeviceBitmap(device);		gxMapping map;			ClearBytes(&map, sizeof(map));		GXGetViewDeviceMapping(device, &map);		GXSetShapeAttributes(bitShape, GXGetShapeAttributes(bitShape) | gxMapTransformShape);		GXMoveShape(bitShape, -map.map[2][0], -map.map[2][1]);		GXSetShapeParts(buffers->draw, 0, 0, bitShape, 0);		GXDisposeShape(bitShape);	}}viewPortBuffer NewViewPortBuffer(gxViewPort port){	viewPortBuffer buffersHandle;	viewPortBufferRecord *buffers;	gxTransform xform;	gxShape area;	long deviceCount;	short i;	NilParamReturnNil(port);	area = GXNewShape(gxFullType);	xform = GXNewTransform();	GXSetTransformViewPorts(xform, 1, &port);	GXSetShapeTransform(area, xform);	GXDisposeTransform(xform);	deviceCount = GXGetShapeGlobalViewDevices(area, port, nil);	buffersHandle = (viewPortBuffer) NewHandle(sizeof(viewPortBufferRecord) + (deviceCount - 1) * sizeof(gxViewDevice));	NilParamReturnNil(buffersHandle);	HLock((Handle) buffersHandle);	buffers = *buffersHandle;	buffers->group = GXNewViewGroup();	buffers->masterPort = port;	buffers->slavePort = GXNewViewPort(buffers->group);	buffers->area = area;	buffers->draw = GXNewShape(gxPictureType);	buffers->deviceCount = deviceCount;	GXSetViewPortDither(buffers->slavePort, GXGetViewPortDither(port));	GXGetShapeGlobalViewDevices(area, port, buffers->devices);	for (i = 0; i < deviceCount; i++) 	{	gxViewDevice device = buffers->devices[i];		areaCharacteristics x = GetAreaCharacteristics(area, device, port);			buffers->devices[i] = NewDeviceWithAreaCharacteristics(buffers->group, &x);		DisposeAreaCharacteristics(&x);	}	AddBitmapShapes(buffers);	HUnlock((Handle) buffersHandle);	return buffersHandle;}void DisposeViewPortBuffer(viewPortBuffer buffersHandle){	NilParamReturn(buffersHandle);	GXDisposeShape((*buffersHandle)->area);	GXDisposeShape((*buffersHandle)->draw);	GXDisposeViewGroup((*buffersHandle)->group);	DisposeHandle((Handle) buffersHandle);}boolean ValidViewPortBuffer(viewPortBuffer buffersHandle){	long deviceCount;	gxViewDevice *devices;	boolean valid = true;	short i;	NilParamReturnNil(buffersHandle);	deviceCount = GXGetShapeGlobalViewDevices((*buffersHandle)->area, (*buffersHandle)->masterPort, nil);	if (deviceCount != (*buffersHandle)->deviceCount)		return false;	devices = (gxViewDevice *) NewPtr(deviceCount * sizeof(gxViewDevice));	NilParamReturnNil(devices);	for (i = 0; i < deviceCount; i++) 	{	areaCharacteristics x = GetAreaCharacteristics((*buffersHandle)->area, devices[i], (*buffersHandle)->masterPort);		areaCharacteristics y = GetDeviceAreaCharacteristics((*buffersHandle)->devices[i]);			valid = EqualAreaCharacteristics(&x, &y);		DisposeAreaCharacteristics(&x);		DisposeAreaCharacteristics(&y);		if (!valid) break;	}	DisposePtr((Ptr) devices);	return valid;}boolean UpdateViewPortBuffer(viewPortBuffer buffersHandle){	viewPortBufferRecord *buffers;	short state;	long deviceCount;	gxViewDevice *devices;	boolean valid = true;	short i;	NilParamReturnNil(buffersHandle);	state = HGetState((Handle) buffersHandle);	HLock((Handle) buffersHandle);	buffers = *buffersHandle;	deviceCount = GXGetShapeGlobalViewDevices(buffers->area, buffers->masterPort, nil);	if (deviceCount != buffers->deviceCount) {		valid = false;		GXSetPicture(buffers->draw, 0, nil, nil, nil, nil);		if (deviceCount < buffers->deviceCount) {			for (i = deviceCount; i < buffers->deviceCount; i++)				GXDisposeViewDevice(buffers->devices[i]);			SetHandleSize((Handle) buffersHandle, sizeof(viewPortBufferRecord) + (deviceCount - 1) * sizeof(gxViewDevice));			buffers = *buffersHandle;		} else {			HUnlock((Handle) buffersHandle);			SetHandleSize((Handle) buffersHandle, sizeof(viewPortBufferRecord) + (deviceCount - 1) * sizeof(gxViewDevice));			HLock((Handle) buffersHandle);			buffers = *buffersHandle;			for (i = buffers->deviceCount; i < deviceCount; i++)				buffers->devices[i] = nil;		}		buffers->deviceCount = deviceCount;	}	devices = (gxViewDevice *) NewPtr(deviceCount * sizeof(gxViewDevice));	NilParamReturnNil(devices);	deviceCount = GXGetShapeGlobalViewDevices(buffers->area, buffers->masterPort, devices);	for (i = 0; i < deviceCount; i++) 	{	gxViewDevice device = buffers->devices[i];		areaCharacteristics x = GetAreaCharacteristics(buffers->area, devices[i], buffers->masterPort);			if (device) 		{	areaCharacteristics y = GetDeviceAreaCharacteristics(device);					if (EqualAreaCharacteristics(&x, &y) == false) {				valid = false;				GXSetPicture(buffers->draw, 0, nil, nil, nil, nil);				SetDeviceAreaCharacteristics(device, &x);			}			DisposeAreaCharacteristics(&y);		} else			buffers->devices[i] = NewDeviceWithAreaCharacteristics(buffers->group, &x);		DisposeAreaCharacteristics(&x);	}	DisposePtr((Ptr) devices);	if (valid == false)		AddBitmapShapes(buffers);	HSetState((Handle) buffersHandle, state);	return valid;}gxViewPort GetViewPortBufferViewPort(viewPortBuffer buffersHandle){	NilParamReturnNil(buffersHandle);	return (*buffersHandle)->slavePort;}gxShape GetViewPortBufferShape(viewPortBuffer buffersHandle){	NilParamReturnNil(buffersHandle);	return (*buffersHandle)->draw;}