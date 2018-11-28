#include "bg.h"



// Load my own map format ('MAP', nLayer.b, MapWidth.w, MapHeight.w)
// This call BG_Init for each layer, BG pointer must have enough BG structures for each layer
// BG_LoadMap(BGs Pointer, TileSet TIM, GPU Attributes (See Sprite.h), Cell Size, Map Pointer);
void	BG_LoadMap(BG *p, GsIMAGE *ig, u_long attrib, u_short cell_size, u_short *map)
{
	int		i;
	u_char	n, *ptr;
	u_short map_size_x, map_size_y;

	ptr = (u_char *)map;
	map += 2;
	if ((ptr[0] == 'M') && (ptr[1] == 'A') && (ptr[2] == 'P'))
	{
		n = ptr[3];
		map_size_x = *map++;
		map_size_y = *map++;

		printf("BG_LoadMap: %d Layers, of %dx%d\n", n, map_size_x, map_size_y);

		while (n--)
		{
			for (i = 0; i < map_size_x * map_size_y; i++)
				map[i]--;
			BG_Init(p, ig, attrib, map_size_x, map_size_y, cell_size, map);
			p++;
			map += (map_size_x * map_size_y);
		}
	}
	else
	{
		printf("BG_LoadMap: Wrong map file format !\n");
	}
}


// BG_Init(BG Pointer, TileSet TIM, GPU Attributes (See Sprite.h), Map Width, Map Height, Cell Size, Map Pointer);
void	BG_Init(BG *p, GsIMAGE *ig, u_long attrib, u_short map_size_x, u_short map_size_y, u_short cell_size, u_short *map)
{
	GsBG	*bg = &p->bg;
	GsMAP	*mp = &p->map;
	int		ncellx, ncell, i;

	ncellx = VRamToImageSize(ig->pw, ig->pmode) / cell_size;
	ncell = ncellx * (ig->ph / cell_size);

	if (ncell > MAX_CELL)
		printf("BG_Init: ERROR: Too much CELLs ! > %d\n", MAX_CELL);

	bg->attribute = ((ig->pmode & 3) << 24) | attrib;

	if ((map_size_x * cell_size) >= 320)
	{
		bg->x = 0;
		bg->w = 320;
	}
	else	// If Map is smaller than screen, then center the map !
	{
		bg->w = map_size_x * cell_size;
		bg->x = (320 - bg->w) / 2;
	}

	if ((map_size_y * cell_size) >= 240)
	{
		bg->y = 0;
		bg->h = 240;
	}
	else	// If Map is smaller than screen, then center the map !
	{
		bg->h = map_size_y * cell_size;
		bg->y = (240 - bg->h) / 2;
	}

	bg->scrollx = 0;
	bg->scrolly = 0;
	bg->r = bg->g = bg->b = 128;
	bg->mx = bg->my = 0;
	bg->scalex = bg->scaley = ONE;
	bg->rotate = 0;
	bg->map = mp;

	mp->cellw = mp->cellh = cell_size;
	mp->ncellw = map_size_x;
	mp->ncellh = map_size_y;
	mp->base = p->cell;
	mp->index = map;

	for (i = 0; i < ncell; i++)
	{
		GsCELL	*cl = &p->cell[i];
		u_short	u, v;

		u = (i % ncellx) * cell_size;
		v = (i / ncellx) * cell_size;
		cl->u = u & 255;
		cl->v = v & 255;
		cl->cba = getClut(ig->cx, ig->cy);
		cl->flag = 0;
		cl->tpage = GetTPage(ig->pmode & 3, 0, ig->px, ig->py) + ((u >> 8) * (1 << (ig->pmode & 3)));	// In case our tileset go over the texture page (256px) !
	}
}


// BG_Draw(BG Pointer, OT Pointer);
void	BG_Draw(BG *p, GsOT *ot)
{
	GsSortFastBg(&p->bg, ot, System_Prio--);
}


// BG_Scroll(BG Pointer, Scroll Direction, Number of pixel to Scroll of);
void	BG_Scroll(BG *p, u_char dir, u_char nof)
{
	if (dir & BG_UP)
	{
		if ((dir & BG_BLOCK) && (p->bg.scrolly < nof)) { }
		else p->bg.scrolly -= nof;
	}

	if (dir & BG_DOWN)
	{
		if ((dir & BG_BLOCK) && ((p->bg.scrolly + 240 + nof) >= (p->bg.map->cellh * p->bg.map->ncellh))) { }
		else p->bg.scrolly += nof;
	}

	if (dir & BG_LEFT)
	{
		if ((dir & BG_BLOCK) && (p->bg.scrollx < nof)) { }
		else p->bg.scrollx -= nof;
	}

	if (dir & BG_RIGHT)
	{
		if ((dir & BG_BLOCK) && ((p->bg.scrollx + 320 + nof) >= (p->bg.map->cellw * p->bg.map->ncellw))) { }
		else p->bg.scrollx += nof;
	}
}


// BG_ConvertMAR(Map Pointer, Map Size);	// Convert a MapWin 'MAR' map format to a common map format
void	BG_ConvertMAR(u_short *map, int size)
{
	while (size--)
	{
		*map = ((*map) << (16 - 5)) | ((*map) >> 5);
		map++;
	}
}
