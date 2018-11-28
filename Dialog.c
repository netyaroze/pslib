
#include "Dialog.h"

// Dialog_Init(DIALOG Pointer, Flags, Time in second (if WAITING flag), P2D_BOX Pointer for Dialog size and style (normal/degrad));
void	Dialog_Init(P2D_DIALOG *diag, u_char flags, u_short time, P2D_BOX *initbox)
{
	diag->flags = flags;

	if (time)
		diag->time = time * 50;	// PAL only for the moment
	else
		diag->time = 1;

	diag->text = NULL;

	// Use initbox Parameters
	memcpy(&diag->box[0], initbox, sizeof(P2D_BOX));
	memcpy(&diag->box[1], initbox, sizeof(P2D_BOX));

	diag->box[1].lattrib = (diag->box[1].lattrib & BOX_CMDMASK) | BOX_OUTLINED;
	diag->box[1].prim.r = 0xFF;
	diag->box[1].prim.g = 0xFF;
	diag->box[1].prim.b = 0xFF;
}


// Dialog_SetText(DIALOG Pointer, Font Image, Text, Font Letter Width/Height, Text Left Offset (Use Negative Number for Right Offset), Convert Min to Maj Flag);
void	Dialog_SetText(P2D_DIALOG *diag, GsIMAGE *textig, char *text, short lw, short lh, int text_offset, bool nomin)
{
	diag->nomin = nomin;
	diag->lw = lw;
	diag->lh = lh;
	diag->text_offset = text_offset;
	diag->text = text;
	diag->ig = textig;
}


// Dialog_SetAnswer(DIALOG Pointer, Answer1 Sprite/Text/PadFlag, Answer2 Sprite/Text/PadFlag, Pixel Gap between Answer1 & 2);
void	Dialog_SetAnswer(P2D_DIALOG *diag, GsSPRITE *answer1, char *text1, int pad1, GsSPRITE *answer2, char *text2, int pad2, u_short gap)
{
	short	x, y;
	u_short	width;
	bool	one = false, two = false;

	if (answer1 && (diag->flags & DIALOG_BVALID))
	{
		memcpy(&diag->pic[0], answer1, sizeof(GsSPRITE));
		diag->text1 = text1;
		diag->pad1 = pad1;
		one = true;

		if (answer2 && (diag->flags & DIALOG_BVALID2))
		{
			memcpy(&diag->pic[1], answer2, sizeof(GsSPRITE));
			diag->text2 = text2;
			diag->pad2 = pad2;
			two = true;
		}
	}

	if (two)
	{
		width = answer1->w + answer2->w + gap;	// Total Size of Sprites + Gap
		if (diag->text && text1)
			width += strlen(text1) * diag->lw;	// + Text1
		if (diag->text && text2)
			width += strlen(text2) * diag->lw;	// + Text2

		if (diag->flags & DIALOG_ANSWER_LEFT)
			x = diag->box[0].x + DIALOG_SPACING;
		else if (diag->flags & DIALOG_ANSWER_RIGHT)
			x = diag->box[0].x + ((diag->box[0].w - DIALOG_SPACING) - width);
		else
			x = diag->box[0].x + (((diag->box[0].w - (DIALOG_SPACING*2)) - width) >> 1);	// Center

		y = (diag->box[0].y + diag->box[0].h) - (DIALOG_SPACING + answer1->h);	// Y Down
		Sprite_SetPosition(&diag->pic[0], x, y);
		x += answer1->w + gap;
		if (diag->text && text1)
			x += strlen(text1) * diag->lw;
		Sprite_SetPosition(&diag->pic[1], x, y);
	}
	else if (one)
	{
		width = answer1->w;
		if (diag->text && text1)
			width += strlen(text1) * diag->lw;

		if (diag->flags & DIALOG_ANSWER_LEFT)
			x = diag->box[0].x + DIALOG_SPACING;
		else if (diag->flags & DIALOG_ANSWER_RIGHT)
			x = diag->box[0].x + ((diag->box[0].w - DIALOG_SPACING) - width);
		else
			x = diag->box[0].x + (((diag->box[0].w - (DIALOG_SPACING*2)) - width) >> 1);

		y = (diag->box[0].y + diag->box[0].h) - (DIALOG_SPACING + answer1->h);
		Sprite_SetPosition(&diag->pic[0], x, y);
	}
}


// Dialog_Draw(DIALOG Pointer, OT);
u_char	Dialog_Draw(P2D_DIALOG *diag, GsOT *ot)
{
	static	u_char	answer = 0;

	if (diag->time != 0xFFFF)
	{
		Box_Draw(&diag->box[0], ot);
		Box_Draw(&diag->box[1], ot);

		if (diag->time)
		{
			if (Box_IsOpened(&diag->box[0]))
			{
				u_short	zone;
				short	x, y;

				if (diag->text)
				{
					if (diag->text_offset > 0)
					{
						zone = (diag->box[0].prim.w - (DIALOG_SPACING*2)) - diag->text_offset;
						x = diag->box[0].prim.x + DIALOG_SPACING + diag->text_offset;
						y = diag->box[0].prim.y + DIALOG_SPACING;
					}
					else
					{
						zone = (diag->box[0].prim.w - (DIALOG_SPACING*2)) + diag->text_offset;
						x = diag->box[0].prim.x + DIALOG_SPACING;
						y = diag->box[0].prim.y + DIALOG_SPACING;
					}

					Text_Draw(diag->ig, x, y, diag->text, diag->lw, diag->lh, diag->lw, diag->nomin, zone, false, ot);
				}

				if (diag->flags == DIALOG_WAITING)
				{
					if (diag->time)
						diag->time--;
					if (!diag->time)
					{
						Box_SetClose(&diag->box[0]);
						Box_SetClose(&diag->box[1]);
					}
				}
				else if (diag->flags & (DIALOG_BVALID | DIALOG_BVALID2))
				{
					if (diag->flags & DIALOG_BVALID)
					{
						x = diag->pic[0].x + diag->pic[0].w;
						y = diag->pic[0].y + ((diag->pic[0].h - diag->lh) >> 1);
						Sprite_DrawFast(&diag->pic[0], ot);
						if (diag->text && diag->text1)
							Text_Draw(diag->ig, x, y, diag->text1, diag->lw, diag->lh, diag->lw, diag->nomin, 0, false, ot);

						if (diag->flags & DIALOG_BVALID2)
						{
							x = diag->pic[1].x + diag->pic[1].w;
							Sprite_DrawFast(&diag->pic[1], ot);
							if (diag->text && diag->text2)
								Text_Draw(diag->ig, x, y, diag->text2, diag->lw, diag->lh, diag->lw, diag->nomin, 0, false, ot);
						}
					}

					if (diag->flags & DIALOG_BVALID)
					{
						if (IsPadTrig(diag->pad1))
						{
							diag->time = 0;
							Box_SetClose(&diag->box[0]);
							Box_SetClose(&diag->box[1]);
							answer = DIALOG_BVALID;
						}
					}

					if (diag->flags & DIALOG_BVALID2)
					{
						if (IsPadTrig(diag->pad2))
						{
							diag->time = 0;
							Box_SetClose(&diag->box[0]);
							Box_SetClose(&diag->box[1]);
							answer = DIALOG_BVALID2;
						}
					}

					return (DIALOG_WAITING);
				}

				return (DIALOG_WAITING);
			}
			else
				return (DIALOG_OPENING);
		}
		else
		{
			if (Box_IsClosed(&diag->box[0]))
				diag->time = 0xFFFF;
			else
				return (DIALOG_CLOSING);
		}
	}

	if (diag->flags & (DIALOG_BVALID | DIALOG_BVALID2))
		return (answer);
	return (DIALOG_END);
}
