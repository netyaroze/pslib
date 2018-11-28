

#include "BSdec.h"

DECDCTTAB	BStable;

int	BS_ClipW = 1024, BS_ClipH = 512;


// Set Clipping Area
void		BS_SetClipping(int width, int height)
{
	BS_ClipW = width;
	BS_ClipH = height;
}

// Display a Raw Image. BS_Display(image data, x position, y position, image width, image height, image output mode);
void		BS_Display(u_long *addr, int x, int y, int width, int height, int mode, bool effect)
{
	char	neg;
	RECT	rect;

	rect.y = y;
	rect.w = 16 + (mode << 3);	// *8
	rect.h = height;

	if ((x + width) <= 0)
		return;

	if ((width + x) > BS_ClipW)
		width = BS_ClipW - x;

	if (width <= 0)
		return;

	neg = 0;
	if (x < 0)
	{
		neg = 1;
		x = -x;
	}

	// 24/16bits Mode Pixel Adjust (24bits must be multiple of 3)
	x = (x >> 1) * (2 + mode);
	width = (width >> 1) * (2 + mode);

	if (neg)
	{
		width -= x;
		addr += (8 + (mode << 2)) * height * (x / rect.w);
		x = 0;
	}

	for (rect.x = x; rect.x < x + width; rect.x += rect.w, addr += (8 + (mode << 2)) * height)
	{
		if (effect)
		{
			DrawSync(0);
			VSync(0);
		}
		LoadImage(&rect, addr);
	}
}


// Decode a BS Stream to a Raw Image. BS_Decode(bs data, image width, image height, image output mode);
u_long	*BS_Decode(u_long *mdec_bs, u_long *mdec_rl, u_long *mdec_image, int width, int height, int mode)
{
	int	size, total;

	DecDCTReset(0);

	DecDCTvlcBuild(BStable);
	size = (DecDCTBufSize(mdec_bs) * sizeof(u_long)) + sizeof(u_short);
	if (size > (128 * 1024))
	{
		return (NULL);
	}
	total = width * height * (2 + mode);

	DecDCTvlc2(mdec_bs, mdec_rl, BStable);
	DecDCTin(mdec_rl, mode);
	DecDCTout(mdec_image, total >> 2);
	DecDCToutSync(0);

	return (mdec_image);
}
