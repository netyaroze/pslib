

#ifndef _DIALOG_H_
#define _DIALOG_H_

#include "System.h"
#include "Sprite.h"
#include "Prim2D.h"

#define	DIALOG_SPACING		4

// DialogBox Answers & Options
#define	DIALOG_WAITING			0x03
#define	DIALOG_BVALID			0x04
#define	DIALOG_BVALID2			0x08
#define	DIALOG_ANSWER_CENTER	0x00
#define	DIALOG_ANSWER_LEFT		0x10
#define	DIALOG_ANSWER_RIGHT		0x20

// DialogBox Answers
#define	DIALOG_OPENING		0x00
#define	DIALOG_CLOSING		0x01
#define	DIALOG_END			0x02


typedef	struct
{
	u_char		flags;
	bool		nomin;
	u_short		time;
	short		lw, lh;
	short		text_offset;
	char		*text, *text1, *text2;
	int			pad1, pad2;
	GsIMAGE		*ig;
	GsSPRITE	pic[2];
	P2D_BOX		box[2];
}	P2D_DIALOG;


// Note: Always use this Init Order: Dialog_Init/Dialog_SetText/Dialog_SetAnswer, Only Dialog_Init is mandatory.

// Dialog_Init(DIALOG Pointer, Flags, Time in second (if WAITING flag), P2D_BOX Pointer for Dialog size and style (normal/degrad));
void	Dialog_Init(P2D_DIALOG *diag, u_char flags, u_short time, P2D_BOX *initbox);

// Dialog_SetText(DIALOG Pointer, Font Image, Text, Font Letter Width/Height, Text Left Offset (Use Negative Number for Right Offset), Convert Min to Maj Flag);
void	Dialog_SetText(P2D_DIALOG *diag, GsIMAGE *textig, char *text, short lw, short lh, int text_offset, bool nomin);

// Dialog_SetAnswer(DIALOG Pointer, Answer1 Sprite/Text/PadFlag, Answer2 Sprite/Text/PadFlag, Pixel Gap between Answer1 & 2);
void	Dialog_SetAnswer(P2D_DIALOG *diag, GsSPRITE *answer1, char *text1, int pad1, GsSPRITE *answer2, char *text2, int pad2, u_short gap);

// Dialog_Draw(DIALOG Pointer, OT);
u_char	Dialog_Draw(P2D_DIALOG *diag, GsOT *ot);

#endif
