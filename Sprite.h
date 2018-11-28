

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "System.h"


#define	SPRITE_NORMAL					0
#define SPRITE_BRIGHTNESS				(1 << 6)
//#define SPRITE_ROTATION					(1 << 27)	// Not needed
#define SPRITE_SEMI_TRANS				(1 << 30)
#define SPRITE_TRANS_50_PLUS_50			(SPRITE_SEMI_TRANS)
#define SPRITE_TRANS_100_PLUS_100		(SPRITE_SEMI_TRANS | (1 << 28))
#define SPRITE_TRANS_100_MINUS_100		(SPRITE_SEMI_TRANS | (2 << 28))
#define SPRITE_TRANS_100_PLUS_25		(SPRITE_SEMI_TRANS | (3 << 28))
#define SPRITE_NO_DISPLAY				(1 << 31)

#define	SPRITE_ATTRIB_MODE_MASK			0x03000000

enum
{
	Tim_4BITS,
	Tim_8BITS,
	Tim_16BITS,
	Tim_24BITS
};


// These macro are only valid for 4/8/16 bits mode

// Get the vram image size using bitplane info (because vram position is in 16bits mode only)
#define	ImageToVRamSize(size, mode)			((size) / (1 << (2 - ((mode) & 3))))
#define	VRamToImageSize(size, mode)			((size) * (1 << (2 - ((mode) & 3))))


// Tim_Load(GsIMAGE Pointer, Tim Address);
void	Tim_Load(GsIMAGE *image, u_long *adrs);

// Sprite_Init(GsSPRITE Pointer, GsIMAGE Pointer, Sprite Attribute, U Coordinate, V Coordinate, Width, Height);
// Note: The image can be anywhere in the VRAM (not necessarily at 0,0 of a tpage, and can be larger than 256 pixels, this function take care of this for 4/8/16 bits mode sprites)
void	Sprite_Init(GsSPRITE *spr, GsIMAGE *ig, u_long attrib, u_short ix, u_short iy, u_short xs, u_short ys);

// Sprite_InitFromXY_16bits(GsSPRITE Pointer, Sprite Attribute, Vram U Coordinate, Vram V Coordinate, Width, Height);
void	Sprite_InitFromXY_16bits(GsSPRITE *spr, u_long attrib, u_short ix, u_short iy, u_short xs, u_short ys);

// Sprite_SetPosition(GsSPRITE Pointer, X Position, Y Position);
void	Sprite_SetPosition(GsSPRITE *spr, short xpos, short ypos);

// Sprite_SetCenter(GsSPRITE Pointer, X Position, Y Position);
void	Sprite_SetCenter(GsSPRITE *spr, short xpos, short ypos);

// Sprite_SetRotation(GsSPRITE Pointer, Angle);
void	Sprite_SetRotation(GsSPRITE *spr, long rot);

// Sprite_RotationAdd(GsSPRITE Pointer, Angle);
void	Sprite_RotationAdd(GsSPRITE *spr, long rot);

// Sprite_Draw(GsSPRITE Pointer, OT Pointer);
#define	Sprite_Draw(spr, ot)					GsSortSprite(spr, ot, System_Prio--)

// Sprite_DrawFast(GsSPRITE Pointer, OT Pointer);
#define	Sprite_DrawFast(spr, ot)				GsSortFastSprite(spr, ot, System_Prio--)

// Sprite_SetAttribute(GsSPRITE Structure, Attribute);
#define	Sprite_SetAttribute(spr, attrib)		spr.attribute = ((spr.attribute & SPRITE_ATTRIB_MODE_MASK) | attrib)

// Sprite_Change(GsSPRITE Pointer, GsIMAGE Pointer, U Coordinate, V Coordinate);
void	Sprite_Change(GsSPRITE *spr, GsIMAGE *ig, u_short ix, u_short iy);

// Sprite_Fade(GsSPRITE Pointer, Fade Indice (0-128));
void	Sprite_Fade(GsSPRITE *spr, u_short fidx);

// Sprite_InitAutoFadeIn(GsSPRITE Pointer);
void	Sprite_InitAutoFadeIn(GsSPRITE *spr);

// Sprite_InitAutoFadeOut(GsSPRITE Pointer);
void	Sprite_InitAutoFadeOut(GsSPRITE *spr);

// Sprite_AutoFadeIn(GsSPRITE Pointer, Fade Speed);
void	Sprite_AutoFadeIn(GsSPRITE *spr, int fspeed);

// Sprite_AutoFadeOut(GsSPRITE Pointer, Fade Speed);
void	Sprite_AutoFadeOut(GsSPRITE *spr, int fspeed);

#endif
