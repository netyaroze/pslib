
#include "Prim2D.h"


// Text_Draw(Font Image, Text Position X/Y, Text, Font Letter Width/Height, Font Letter Real Width, Convert Min to Maj, Text Zone Width (0 if none), Center Text in ZoneW, OT);
void	Text_Draw(GsIMAGE *image, short x, short y, char *text, short lw, short lh, short rlw, bool nomin, short zonew, bool center, GsOT *ot)
{
	GsSPRITE	letter;
	short		nletter = VRamToImageSize(image->pw, image->pmode) / lw;
	short		zone = 0, xs;

	Sprite_Init(&letter, image, SPRITE_NORMAL, 0, 0, lw, lh);

	if (center)
		x += (zonew - (strlen(text) * rlw)) / 2;

	xs = x;
	while (*text)
	{
		char	c = *text++;

		// Text Warp + Line Feed
		if (((zonew > 0) && ((zone + rlw) > zonew)) || (c == '\n'))
		{
			zone = 0;
			x = xs;
			y += lh;

			if (c == '\n')
				continue;
		}

		// Min to Maj Option
		if ((nomin) && (c >= 'a') && (c <= 'z'))
			c -= 'a' - 'A';

		// Draw Letter (if not blank space)
		c -= ' ';
		if (c)
		{
			c--;
			Sprite_Change(&letter, image, (c % nletter) * lw, (c / nletter) * lh);
			Sprite_SetPosition(&letter, x, y);
			GsSortFastSprite(&letter, ot, System_Prio);
		}

		// Next Letter
		x += rlw;
		zone += rlw;
	}

	System_Prio--;
}


// Box_Init(P2D_BOX Pointer, Box Attributes, GPU Attributes, X Position, Y Position, Width, Height, Color);
void	Box_Init(P2D_BOX *p, u_long lattrib, u_long attrib, short x, short y, u_short w, u_short h, u_long color)
{
	p->lattrib = lattrib;
	p->prim.attribute = attrib;
	p->x = p->prim.x = x;
	p->y = p->prim.y = y;
	p->w = p->prim.w = w;
	p->h = p->prim.h = h;
	p->prim.r = (color >> 16) & 0xFF;
	p->prim.g = (color >> 8) & 0xFF;
	p->prim.b = color & 0xFF;

	if (lattrib & BOX_HOPEN)
	{
		p->prim.x += (w >> 1);
		p->prim.w = 0;
		p->speedw = 1;
	}
	else if (lattrib & BOX_HCLOSE)
	{
		p->speedw = -1;
	}

	if (lattrib & BOX_VOPEN)
	{
		p->prim.y += (h >> 1);
		p->prim.h = 0;
		p->speedh = 1;
	}
	else if (lattrib & BOX_VCLOSE)
	{
		p->speedh = -1;
	}
}


// Box_InitG(P2D_BOX Pointer, Box Attributes, GPU Attributes, X Position, Y Position, Width, Height, Upper Left Gradient Color, Upper Right Gradient Color, Bottom Rigth Gradient Color, Bottom Left Gradient Color);
void	Box_InitG(P2D_BOX *p, u_long lattrib, u_long attrib, short x, short y, u_short w, u_short h, u_long color1, u_long color2, u_long color3, u_long color4)
{
	p->lattrib = lattrib | BOX_GRADIENT;
	p->prim.attribute = attrib;
	p->x = p->prim.x = x;
	p->y = p->prim.y = y;
	p->w = p->prim.w = w;
	p->h = p->prim.h = h;
	p->color = color1;
	p->color1 = color2;
	p->color2 = color3;
	p->color3 = color4;

	if (lattrib & BOX_HOPEN)
	{
		p->prim.x += (w >> 1);
		p->prim.w = 0;
		p->speedw = 1;
	}
	else if (lattrib & BOX_HCLOSE)
	{
		p->speedw = -1;
	}

	if (lattrib & BOX_VOPEN)
	{
		p->prim.y += (h >> 1);
		p->prim.h = 0;
		p->speedh = 1;
	}
	else if (lattrib & BOX_VCLOSE)
	{
		p->speedh = -1;
	}
}


// Box_SetPosition(P2D_BOX Pointer, X Position, Y Position);
void	Box_SetPosition(P2D_BOX *p, short x, short y)
{
	p->prim.x = x;
	p->prim.y = y;
}


// Box_SetOpenSpeed(P2D_BOX Pointer, Width Speed, Height Speed);
void	Box_SetOpenSpeed(P2D_BOX *p, u_char w, u_char h)
{
	p->speedw = w;
	p->speedh = h;
}


// Box_SetClose(P2D_BOX Pointer);
void	Box_SetClose(P2D_BOX *p)
{
	if (p->lattrib & BOX_OPEN)
	{
		u_char	flags = 0;

		if (p->lattrib & BOX_HOPEN)
			flags |= BOX_HCLOSE;
		if (p->lattrib & BOX_VOPEN)
			flags |= BOX_VCLOSE;

		p->lattrib = (p->lattrib & BOX_STYLEMASK) | flags;
	}
	else
	{
		p->lattrib = (p->lattrib & BOX_STYLEMASK) | BOX_CLOSE;
	}

	p->speedw = -(p->speedw);
	p->speedh = -(p->speedh);
}


// Box_IsOpened(P2D_BOX Pointer)
bool	Box_IsOpened(P2D_BOX *p)
{
	bool	h, v;

	if (p->lattrib & BOX_HOPEN)
	{
		if (p->prim.w < p->w)
			h = false;
		else
			h = true;
	}
	else
		h = true;

	if (p->lattrib & BOX_VOPEN)
	{
		if (p->prim.h < p->h)
			v = false;
		else
			v = true;
	}
	else
		v = true;

	return (h && v);
}


// Box_IsClosed(P2D_BOX Pointer)
bool	Box_IsClosed(P2D_BOX *p)
{
	bool	h, v;

	if (p->lattrib & BOX_HCLOSE)
	{
		if (p->prim.w)
			h = false;
		else
			h = true;
	}
	else
		h = true;

	if (p->lattrib & BOX_VCLOSE)
	{
		if (p->prim.h)
			v = false;
		else
			v = true;
	}
	else
		v = true;

	return (h && v);
}


// Box_Draw(P2D_BOX Pointer, OT Pointer);
void	Box_Draw(P2D_BOX *p, GsOT *ot)
{
	if (p->lattrib & BOX_HOPEN)
	{
		if (p->prim.w < p->w)
		{
			p->prim.x -= p->speedw;
			p->prim.w += p->speedw + p->speedw;
		}
		else
		{
			p->prim.x = p->x;
			p->prim.w = p->w;
		}
	}

	if (p->lattrib & BOX_HCLOSE)
	{
		if (((short)p->prim.w) > 0)
		{
			p->prim.x -= p->speedw;
			p->prim.w += p->speedw + p->speedw;
		}
		else
		{
			p->prim.w = 0;
		}
	}

	if (p->lattrib & BOX_VOPEN)
	{
		if (p->prim.h < p->h)
		{
			p->prim.y -= p->speedh;
			p->prim.h += p->speedh + p->speedh;
		}
		else
		{
			p->prim.y = p->y;
			p->prim.h = p->h;
		}
	}

	if (p->lattrib & BOX_VCLOSE)
	{
		if (((short)p->prim.h) > 0)
		{
			p->prim.y -= p->speedh;
			p->prim.h += p->speedh + p->speedh;
		}
		else
		{
			p->prim.h = 0;
		}
	}

	switch (p->lattrib & BOX_STYLEMASK)
	{
		case BOX_OUTLINED:
		{
			GsLINE	Ltmp;

			Ltmp.attribute = p->prim.attribute;
			Ltmp.r = p->prim.r;
			Ltmp.g = p->prim.g;
			Ltmp.b = p->prim.b;

			Ltmp.x0 = p->prim.x;
			Ltmp.y0 = p->prim.y;
			Ltmp.x1 = p->prim.x + p->prim.w;
			Ltmp.y1 = p->prim.y;
			GsSortLine(&Ltmp, ot, System_Prio);

			Ltmp.x0 = p->prim.x + p->prim.w;
			Ltmp.y0 = p->prim.y + p->prim.h;
			GsSortLine(&Ltmp, ot, System_Prio);

			Ltmp.x1 = p->prim.x;
			Ltmp.y1 = p->prim.y + p->prim.h;
			GsSortLine(&Ltmp, ot, System_Prio);

			Ltmp.x0 = p->prim.x;
			Ltmp.y0 = p->prim.y;
			GsSortLine(&Ltmp, ot, System_Prio);
		}
		break;

		case BOX_FILLED:
			GsSortBoxFill(&p->prim, ot, System_Prio);
		break;

		case BOX_GRADIENT:
		{
			GsGLINE	LGtmp;
			short	i, stepR0, stepG0, stepB0, stepR1, stepG1, stepB1;
			short	r0, g0, b0, r1, g1, b1;

			if (!p->prim.h)
				break;

			LGtmp.attribute = p->prim.attribute;
			LGtmp.x0 = p->prim.x;
			LGtmp.x1 = p->prim.x + p->prim.w;
			LGtmp.y0 = LGtmp.y1 = p->prim.y;

			// Compute Gradient Step
			r0 = ((p->color >> 16) & 0xFF);
			g0 = ((p->color >> 8)  & 0xFF);
			b0 =  (p->color        & 0xFF);
			r1 = ((p->color1 >> 16) & 0xFF);
			g1 = ((p->color1 >> 8)  & 0xFF);
			b1 =  (p->color1        & 0xFF);
			stepR0 = ((p->color3 >> 16) & 0xFF);
			stepG0 = ((p->color3 >> 8)  & 0xFF);
			stepB0 =  (p->color3        & 0xFF);
			stepR1 = ((p->color2 >> 16) & 0xFF);
			stepG1 = ((p->color2 >> 8)  & 0xFF);
			stepB1 =  (p->color2        & 0xFF);
			stepR0 = ((stepR0 - r0) << 4) / p->prim.h;
			stepG0 = ((stepG0 - g0) << 4) / p->prim.h;
			stepB0 = ((stepB0 - b0) << 4) / p->prim.h;
			stepR1 = ((stepR1 - r1) << 4) / p->prim.h;
			stepG1 = ((stepG1 - g1) << 4) / p->prim.h;
			stepB1 = ((stepB1 - b1) << 4) / p->prim.h;
			r0 <<= 4;
			g0 <<= 4;
			b0 <<= 4;
			r1 <<= 4;
			g1 <<= 4;
			b1 <<= 4;

			for (i = 0; i < p->prim.h; i++)
			{
				LGtmp.y0 = ++LGtmp.y1;

				LGtmp.r0 = (r0 += stepR0) >> 4;
				LGtmp.g0 = (g0 += stepG0) >> 4;
				LGtmp.b0 = (b0 += stepB0) >> 4;
				LGtmp.r1 = (r1 += stepR1) >> 4;
				LGtmp.g1 = (g1 += stepG1) >> 4;
				LGtmp.b1 = (b1 += stepB1) >> 4;

				GsSortGLine(&LGtmp, ot, System_Prio);
			}
		}
		break;
	}

	System_Prio--;
}


// Line_Draw(OT Pointer, GPU Attributes, X1 Position, Y1 Position, X2 Position, Y2 Position, Color);
void	Line_Draw(GsOT *ot, u_long attrib, short x1, short y1, short x2, short y2, u_long color)
{
	GsLINE	Ltmp;

	Ltmp.attribute = attrib;
	Ltmp.r = (color >> 16) & 0xFF;
	Ltmp.g = (color >> 8) & 0xFF;
	Ltmp.b = color & 0xFF;
	Ltmp.x0 = x1;
	Ltmp.y0 = y1;
	Ltmp.x1 = x2;
	Ltmp.y1 = y2;
	GsSortLine(&Ltmp, ot, System_Prio--);
}


// Line_DrawG(OT Pointer, GPU Attributes, X1 Position, Y1 Position, X2 Position, Y2 Position, Gradient Color Start, Gradient Color End);
void	Line_DrawG(GsOT *ot, u_long attrib, short x1, short y1, short x2, short y2, u_long color1, u_long color2)
{
	GsGLINE	Ltmp;

	Ltmp.attribute = attrib;
	Ltmp.r0 = (color1 >> 16) & 0xFF;
	Ltmp.g0 = (color1 >> 8) & 0xFF;
	Ltmp.b0 = color1 & 0xFF;
	Ltmp.r1 = (color2 >> 16) & 0xFF;
	Ltmp.g1 = (color2 >> 8) & 0xFF;
	Ltmp.b1 = color2 & 0xFF;
	Ltmp.x0 = x1;
	Ltmp.y0 = y1;
	Ltmp.x1 = x2;
	Ltmp.y1 = y2;
	GsSortGLine(&Ltmp, ot, System_Prio--);
}
