#ifndef _BSDEC_H_
#define _BSDEC_H_

#include <sys/types.h>
#include <libpress.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>

#include "System.h"

#define	BS_RLE_BUFFER_SIZE		(128*1024)
#define	BS_OUTPUT_16BITS		0
#define	BS_OUTPUT_24BITS		1

// Set Clipping Area
void	BS_SetClipping(int width, int height);

// Decode a BS Stream to a Raw Image. BS_Decode(bs data, 128K RLE Free Buffer, Raw Image Output Buffer, image width, image height, image output mode);
// Note: In case of 24bits, X must be multiple of 3
// In case of negative X, clipping is done BUT X must be multiple of 16 or 24 !
u_long	*BS_Decode(u_long *mdec_bs, u_long *mdec_rl, u_long *mdec_image, int width, int height, int mode);

// Display a Raw Image. BS_Display(image data, x position, y position, image width, image height, image output mode, draw effect);
void	BS_Display(u_long *addr, int x, int y, int width, int height, int mode, bool effect);

#endif
