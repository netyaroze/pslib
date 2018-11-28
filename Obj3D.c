


#include "Obj3D.h"


// Obj3D_Init(Near Clip, Far Clip);
void	Obj3D_Init(int nearclip, int farclip)
{
	printf("Obj3D_Init();\n");
	GsInit3D();
	GsSetNearClip(nearclip);
	GsSetFarClip(farclip);
}


// Obj3D_InitView(GsRVIEW2 Camera View Pointer, Projection Distance from Screen, X,Y,Z Position to Look From, X,Y,Z Position to Look To);
void	Obj3D_InitView(GsRVIEW2 *tView, int tProjDist, int tFromX, int tFromY, int tFromZ, int tToX, int tToY, int tToZ)
{
	GsSetProjection(tProjDist);
	tView->vpx = tFromX;
	tView->vpy = tFromY;
	tView->vpz = tFromZ;
	tView->vrx = tToX;
	tView->vry = tToY;
	tView->vrz = tToZ;
	tView->rz = 0;
	tView->super = WORLD;
	GsSetRefView2(tView);
}


// Obj3D_SetLight(GsF_LIGHT Pointer, Light Id (0-2), X,Y,Z Light Vector, R,G,B Light Color);
void	Obj3D_SetLight(GsF_LIGHT *light, u_int n, int x, int y, int z, u_char r, u_char g, u_char b)
{
	light->vx = x;
	light->vy = y;
	light->vz = z;
	light->r = r;
	light->g = g;
	light->b = b;
	GsSetFlatLight(n, light);
}


// Obj3D_SetAmbientLight(Ambient Light R,G,B, Fog Mode);
void	Obj3D_SetAmbientLight(u_char r, u_char g, u_char b, int mode)
{
	GsSetAmbient(r << 4, g << 4, b << 4);
	GsSetLightMode(mode);
}


// Obj3D_InitTMD(Object3D Pointer, GPU Attributes, X,Y,Z Initial Position, TMD Address);
void	Obj3D_InitTMD(OBJ3D *obj, u_long attrib, u_char id, int x, int y, int z, u_long *adrs)
{
	adrs++;
	GsMapModelingData(adrs);
	GsInitCoordinate2(WORLD, &obj->Coord2);
	adrs++;
	if (id >= *adrs)
	{
		printf("Obj3D_InitTMD: ID %d too high ! TMD only have %d objects.\n", id, *adrs);
		id = 0;
	}
	adrs++;
	GsLinkObject4((u_long)adrs, &obj->Handler, id);
	obj->Handler.coord2 = &obj->Coord2;
	obj->Handler.attribute |= attrib;
	obj->Position.vx = obj->Coord2.coord.t[0] = x;
	obj->Position.vy = obj->Coord2.coord.t[1] = y;
	obj->Position.vz = obj->Coord2.coord.t[2] = z;
	obj->Coord2.flg = 0;
}


// Obj3D_Draw(Object3D Pointer, Current OT Pointer);
void		Obj3D_Draw(OBJ3D *obj, GsOT *ot)
{
	MATRIX	tLocalWorld, tLocalScreen;

	GsGetLws(obj->Handler.coord2, &tLocalWorld, &tLocalScreen);
	GsSetLsMatrix(&tLocalScreen);
	GsSetLightMatrix(&tLocalWorld);
	GsSortObject4(&obj->Handler, ot, Z_SHIFT, (u_long *)getScratchAddr(0));
}


// Obj3D_Process(Object3D Pointer, Process Flags);
void	Obj3D_Process(OBJ3D *obj, u_char flags)
{
	if (flags & OBJ3D_TRANSLATE)
	{
		obj->Coord2.coord.t[0] = obj->Position.vx;
		obj->Coord2.coord.t[1] = obj->Position.vy;
		obj->Coord2.coord.t[2] = obj->Position.vz;
		obj->Coord2.flg = 0;
	}

	if (flags & OBJ3D_ROTATE)
	{
		RotMatrix(&obj->Rotation, &obj->Handler.coord2->coord);
		obj->Coord2.flg = 0;
	}
}
