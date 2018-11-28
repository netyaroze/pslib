
#ifndef _OBJ3D_H_
#define _OBJ3D_H_

#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include "System.h"


// GPU Attributes
#define	OBJ3D_NORMAL		0
#define OBJ3D_SEMI_TRANS	(1 << 30)

// Manager Process Flags
#define	OBJ3D_TRANSLATE		(1 << 0)
#define	OBJ3D_ROTATE		(1 << 1)


#define	Z_SHIFT		(14 - OT_LENGTH)
#define	FOG_OFF		0
#define	FOG_ON		1


typedef struct
{
	GsDOBJ2			Handler;
	GsCOORDINATE2	Coord2;
	SVECTOR			Position;
	SVECTOR			Rotation;
}	OBJ3D;


enum
{
	LIGHT_0,
	LIGHT_1,
	LIGHT_2
};


// Obj3D_Init(Near Clip, Far Clip);
void	Obj3D_Init(int nearclip, int farclip);

// Obj3D_InitView(GsRVIEW2 Camera View Pointer, Projection Distance from Screen, X,Y,Z Position to Look From, X,Y,Z Position to Look To);
void	Obj3D_InitView(GsRVIEW2 *tView, int tProjDist, int tFromX, int tFromY, int tFromZ, int tToX, int tToY, int tToZ);

// Obj3D_SetLight(GsF_LIGHT Pointer, Light Id (0-2), X,Y,Z Light Vector, R,G,B Light Color);
void	Obj3D_SetLight(GsF_LIGHT *light, u_int n, int x, int y, int z, u_char r, u_char g, u_char b);

// Obj3D_SetAmbientLight(Ambient Light R,G,B, Fog Mode);
void	Obj3D_SetAmbientLight(u_char r, u_char g, u_char b, int mode);

// Obj3D_InitTMD(Object3D Pointer, GPU Attributes, Object ID in TMD, X,Y,Z Initial Position, TMD Address);
void	Obj3D_InitTMD(OBJ3D *obj, u_long attrib, u_char id, int x, int y, int z, u_long *adrs);

// Obj3D_SetView(GsRVIEW2 Camera View Pointer);
#define	Obj3D_SetView	GsSetRefView2

// Obj3D_Process(Object3D Pointer, Process Flags);
void	Obj3D_Process(OBJ3D *obj, u_char flags);

// Obj3D_Draw(Object3D Pointer, Current OT Pointer);
void	Obj3D_Draw(OBJ3D *obj, GsOT *ot);

#endif
