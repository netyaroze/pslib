


// Tim_Load(GsIMAGE Pointer, Tim Address);
void		Tim_Load(GsIMAGE *image, u_long *adrs)
{
	RECT	rect;

	GsGetTimInfo(adrs + 1, image);

	rect.x = image->px;
	rect.y = image->py;
	rect.w = image->pw;
	rect.h = image->ph;
	LoadImage(&rect, image->pixel);

	if ((image->pmode & 3) < Tim_16BITS)
	{
		rect.x = image->cx;
		rect.y = image->cy;
		rect.w = image->cw;
		rect.h = image->ch;
		LoadImage(&rect, image->clut);
	}
}


// Sprite_Init(GsSPRITE Pointer, GsIMAGE Pointer, Sprite Attribute, U Coordinate, V Coordinate, Width, Height);
void	Sprite_Init(GsSPRITE *spr, GsIMAGE *ig, u_long attrib, u_short ix, u_short iy, u_short xs, u_short ys)
{
	// Note: The image can be anywhere in the VRAM (not necessarily at 0,0 of a tpage, and can be larger than 256 pixels, this function take care of this for 4/8/16 bits mode sprites)
	u_short	vrx = (ig->px + ImageToVRamSize(ix, ig->pmode)) & 0x3C0;
	u_short	vry = (ig->py + iy) & 0x100;

	spr->attribute = attrib | ((ig->pmode & 3) << 24);
	spr->w = xs;
	spr->h = ys;
	spr->tpage = getTPage(ig->pmode, 0, vrx, vry);
	spr->u = (VRamToImageSize(ig->px - vrx, ig->pmode) + ix) & 255;
	spr->v = (ig->py + iy) & 255;
	spr->cx = ig->cx;
	spr->cy = ig->cy;
	spr->r = spr->g = spr->b = 128;
	spr->mx = 0;
	spr->my = 0;
	spr->scalex = ONE;
	spr->scaley = ONE;
	spr->rotate = 0;
}


// Sprite_InitFromXY_16bits(GsSPRITE Pointer, Sprite Attribute, Vram U Coordinate, Vram V Coordinate, Width, Height);
void	Sprite_InitFromXY_16bits(GsSPRITE *spr, u_long attrib, u_short ix, u_short iy, u_short xs, u_short ys)
{
	u_short	vrx = ix & 0x3C0;
	u_short	vry = iy & 0x100;

	spr->attribute = attrib | (Tim_16BITS << 24);
	spr->w = xs;
	spr->h = ys;
	spr->tpage = getTPage(Tim_16BITS, 0, vrx, vry);
	spr->u = (ix - vrx) & 255;
	spr->v = iy & 255;
	spr->r = spr->g = spr->b = 128;
	spr->mx = 0;
	spr->my = 0;
	spr->scalex = ONE;
	spr->scaley = ONE;
	spr->rotate = 0;
}


// Sprite_SetPosition(GsSPRITE Pointer, X Position, Y Position);
void	Sprite_SetPosition(GsSPRITE *spr, short xpos, short ypos)
{
	spr->x = xpos;
	spr->y = ypos;
}


// Sprite_SetCenter(GsSPRITE Pointer, X Position, Y Position);
void	Sprite_SetCenter(GsSPRITE *spr, short xpos, short ypos)
{
	spr->mx = xpos;
	spr->my = ypos;
}


// Sprite_SetRotation(GsSPRITE Pointer, Angle);
void	Sprite_SetRotation(GsSPRITE *spr, long rot)
{
	spr->rotate = rot * ONE;
}


// Sprite_RotationAdd(GsSPRITE Pointer, Angle);
void	Sprite_RotationAdd(GsSPRITE *spr, long rot)
{
	spr->rotate += rot * ONE;
}


// Sprite_Change(GsSPRITE Pointer, GsIMAGE Pointer, U Coordinate, V Coordinate);
void	Sprite_Change(GsSPRITE *spr, GsIMAGE *ig, u_short ix, u_short iy)
{
	u_short	vrx = (ig->px + ImageToVRamSize(ix, ig->pmode)) & 0x3C0;
	u_short	vry = (ig->py + iy) & 0x100;

	spr->tpage = getTPage(ig->pmode, 0, vrx, vry);
	spr->u = (VRamToImageSize(ig->px - vrx, ig->pmode) + ix) & 255;
	spr->v = (ig->py + iy) & 255;
}


// Sprite_Fade(GsSPRITE Pointer, Fade Indice (0-128));
void	Sprite_Fade(GsSPRITE *spr, u_short fidx)
{
	spr->r = spr->g = spr->b = fidx;
}


// Sprite_InitAutoFadeIn(GsSPRITE Pointer);
void	Sprite_InitAutoFadeIn(GsSPRITE *spr)
{
	spr->r = spr->g = spr->b = 0;
}


// Sprite_InitAutoFadeOut(GsSPRITE Pointer);
void	Sprite_InitAutoFadeOut(GsSPRITE *spr)
{
	spr->r = spr->g = spr->b = 0x80;
}


// Sprite_AutoFadeIn(GsSPRITE Pointer, Fade Speed);
void	Sprite_AutoFadeIn(GsSPRITE *spr, int fspeed)
{
	if (spr->b != 0x80)
		spr->r = spr->g = spr->b += fspeed;
}


// Sprite_AutoFadeOut(GsSPRITE Pointer, Fade Speed);
void	Sprite_AutoFadeOut(GsSPRITE *spr, int fspeed)
{
	if (spr->b != 0)
		spr->r = spr->g = spr->b -= fspeed;
}
