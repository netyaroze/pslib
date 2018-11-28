

#ifndef _PRIM2D_H_
#define _PRIM2D_H_

#include "System.h"
#include "Sprite.h"


// GPU Attributes
#define	PRIM2D_NORMAL					0
#define PRIM2D_SEMI_TRANS				(1 << 30)
#define PRIM2D_TRANS_50_PLUS_50			(PRIM2D_SEMI_TRANS)
#define PRIM2D_TRANS_100_PLUS_100		(PRIM2D_SEMI_TRANS | (1 << 28))
#define PRIM2D_TRANS_100_MINUS_100		(PRIM2D_SEMI_TRANS | (2 << 28))
#define PRIM2D_TRANS_100_PLUS_50		(PRIM2D_SEMI_TRANS | (3 << 28))
#define PRIM2D_NO_DISPLAY				(1 << 31)

// Box Attributes
#define	BOX_OUTLINED		0x00
#define	BOX_FILLED			0x01
#define	BOX_GRADIENT		0x02
#define	BOX_STYLEMASK		0x0F
#define	BOX_HOPEN			0x10
#define	BOX_VOPEN			0x20
#define	BOX_OPEN			(BOX_HOPEN | BOX_VOPEN)
#define	BOX_HCLOSE			0x40
#define	BOX_VCLOSE			0x80
#define	BOX_CLOSE			(BOX_HCLOSE | BOX_VCLOSE)
#define	BOX_CMDMASK			0xF0


typedef	struct
{
	u_char	lattrib;
	GsBOXF	prim;
	short	x, y;
	u_short	w, h;
	signed char	speedw, speedh;
	u_long	color;
	u_long	color1;
	u_long	color2;
	u_long	color3;
}	P2D_BOX;


// Text_Draw(Font Image, Text Position X/Y, Text, Font Letter Width/Height, Font Letter Real Width, Convert Min to Maj, Text Zone Width (0 if none), Center Text in ZoneW, OT);
void	Text_Draw(GsIMAGE *image, short x, short y, char *text, short lw, short lh, short rlw, bool nomin, short zonew, bool center, GsOT *ot);

// Box_Init(P2D_BOX Pointer, Box Attributes, GPU Attributes, X Position, Y Position, Width, Height, Color);
void	Box_Init(P2D_BOX *p, u_long lattrib, u_long attrib, short x, short y, u_short w, u_short h, u_long color);

// Box_InitG(P2D_BOX Pointer, Box Attributes, GPU Attributes, X Position, Y Position, Width, Height, Upper Left Gradient Color, Upper Right Gradient Color, Bottom Rigth Gradient Color, Bottom Left Gradient Color);
void	Box_InitG(P2D_BOX *p, u_long lattrib, u_long attrib, short x, short y, u_short w, u_short h, u_long color1, u_long color2, u_long color3, u_long color4);

// Box_SetPosition(P2D_BOX Pointer, X Position, Y Position);
void	Box_SetPosition(P2D_BOX *p, short x, short y);

// Box_SetOpenSpeed(P2D_BOX Pointer, Width Speed, Height Speed);
void	Box_SetOpenSpeed(P2D_BOX *p, u_char w, u_char h);

// Box_SetClose(P2D_BOX Pointer);	// Use this Function if the Box was Not in Close Mode at Init !
void	Box_SetClose(P2D_BOX *p);

// Box_IsOpened(P2D_BOX Pointer)
bool	Box_IsOpened(P2D_BOX *p);

// Box_IsClosed(P2D_BOX Pointer)
bool	Box_IsClosed(P2D_BOX *p);

// Box_Draw(P2D_BOX Pointer, OT Pointer);
void	Box_Draw(P2D_BOX *p, GsOT *ot);

// Line_Draw(OT Pointer, GPU Attributes, X1 Position, Y1 Position, X2 Position, Y2 Position, Color);
void	Line_Draw(GsOT *ot, u_long attrib, short x1, short y1, short x2, short y2, u_long color);

// Line_DrawG(OT Pointer, GPU Attributes, X1 Position, Y1 Position, X2 Position, Y2 Position, Gradient Color Start, Gradient Color End);
void	Line_DrawG(GsOT *ot, u_long attrib, short x1, short y1, short x2, short y2, u_long color1, u_long color2);

#endif
