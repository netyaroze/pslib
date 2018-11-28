#ifndef _BG_H_
#define _BG_H_

#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include "sprite.h"

#define	MAX_CELL	(32 * 16)

#define	BG_CELL_16	16
#define	BG_CELL_32	32

#define	BG_UP		0x01
#define	BG_DOWN		0x02
#define	BG_LEFT		0x04
#define	BG_RIGHT	0x08
#define	BG_BLOCK	0x10


#define	BgWorkSize(ScreenW, ScreenH, CellW, CellH)	(((ScreenW/CellW+1)*(ScreenH/CellH+1+1)*6+4)*2+2)


typedef	struct
{
	GsBG		bg;
	GsMAP		map;
	GsCELL		cell[MAX_CELL];
}	BG;


// Load my own map format ('MAP', nLayer.b, MapWidth.w, MapHeight.w)
// This call BG_Init for each layer, BG pointer must have enough BG structures for each layer
// BG_LoadMap(BGs Pointer, TileSet TIM, GPU Attributes (See Sprite.h), Cell Size, Map Pointer);
void	BG_LoadMap(BG *p, GsIMAGE *ig, u_long attrib, u_short cell_size, u_short *map);

// BG_Init(BG Pointer, TileSet TIM, GPU Attributes (See Sprite.h), Map Width, Map Height, Cell Size, Map Pointer);
// Note: A blank tile should be 0xFFFF in map
void	BG_Init(BG *p, GsIMAGE *ig, u_long attrib, u_short map_size_x, u_short map_size_y, u_short cell_size, u_short *map);

// BG_Draw(BG Pointer, OT Pointer);
void	BG_Draw(BG *p, GsOT *ot);

// BG_Scroll(BG Pointer, Scroll Direction, Number of pixel to Scroll of);
void	BG_Scroll(BG *p, u_char dir, u_char nof);

// BG_ConvertMAR(Map Pointer, Map Size);	// Convert a MapWin 'MAR' map format to a common map format
void	BG_ConvertMAR(u_short *map, int size);

#endif
