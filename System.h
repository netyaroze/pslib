

#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libcd.h>

typedef		unsigned char	bool;
#define		false			0
#define		true			1

typedef	unsigned char	u8;
typedef	unsigned short	u16;
typedef	unsigned long	u32;
typedef	signed char		s8;
typedef	signed short	s16;
typedef	signed long		s32;

typedef struct
{
	int	x, y;
}		Point2D;

#define	VMODE_16BITS	0
#define	VMODE_24BITS	1

#define	PSX_RGB(r, g, b)		(((b >> 3) << 10) | ((g >> 3) << 5) | (r >> 3))
#define	PSX_ARGB(a, r, g, b)	(((a & 1) << 15) | ((b >> 3) << 10) | ((g >> 3) << 5) | (r >> 3))

#ifndef USER_SYSPARAM		// Following defines can be overwritten by the compiler line command if you also define this
							// Line command example: -DUSER_SYSPARAM -DOT_LENGTH=11 -DPACKET_MAX=300
#define OT_LENGTH	10		// Ordering Table Resolution (1-14 bits)
#define	PACKET_MAX	1000	// MAX GPU Packet (Drawing infos)
#endif

#define	PACKET_SIZE	64		// GPU Packet Size

extern	int	SysPad;
extern	int	SysPadT;
extern	int	System_Prio;	// Current Priority (used for each Sort in OT function, so the drawing can be from back to front and not the inverse as it is by default)
extern	int	System_CurrentBufferID;
extern	u_long	SysMsecTimer;
extern	GsOT	*System_CurOT;

#define	SystemGetTimeRef()	(SysMsecTimer >> 8)

#define	IsPadPress(_p_)	(SysPad & (_p_))
#define	IsPadTrig(_p_)	(SysPadT & (_p_))

#define Pad1Up			_PAD(0, PADLup)
#define Pad1Down		_PAD(0, PADLdown)
#define Pad1Left		_PAD(0, PADLleft)
#define Pad1Right		_PAD(0, PADLright)
#define Pad1Triangle	_PAD(0, PADRup)
#define Pad1Cross		_PAD(0, PADRdown)
#define Pad1Square		_PAD(0, PADRleft)
#define Pad1Circle		_PAD(0, PADRright)
#define Pad1L1			_PAD(0, PADL1)
#define Pad1L2			_PAD(0, PADL2)
#define Pad1R1			_PAD(0, PADR1)
#define Pad1R2			_PAD(0, PADR2)
#define Pad1Start		_PAD(0, PADstart)
#define Pad1Select		_PAD(0, PADselect)

#define Pad2Up			_PAD(1, PADLup)
#define Pad2Down		_PAD(1, PADLdown)
#define Pad2Left		_PAD(1, PADLleft)
#define Pad2Right		_PAD(1, PADLright)
#define Pad2Triangle	_PAD(1, PADRup)
#define Pad2Cross		_PAD(1, PADRdown)
#define Pad2Square		_PAD(1, PADRleft)
#define Pad2Circle		_PAD(1, PADRright)
#define Pad2L1			_PAD(1, PADL1)
#define Pad2L2			_PAD(1, PADL2)
#define Pad2R1			_PAD(1, PADR1)
#define Pad2R2			_PAD(1, PADR2)
#define Pad2Start		_PAD(1, PADstart)
#define Pad2Select		_PAD(1, PADselect)


// System_Init(Video Mode, 16/24bits Mode, X Resolution, Y Resolution, Interlaced Mode, Dither Mode);
void	System_Init(long vmode, u_short bpp, u_short xres, u_short yres, u_short int1, u_short dither);

// System_Exit();
void	System_Exit(void);

// System_ResetPad();
void	System_ResetPad(void);

// System_ReadPad();
void	System_ReadPad(void);

// System_LoadExecutable(CD Rom Path to the Executable);
// Ex: System_LoadExecutable("cdrom:\\MAIN.PSX;1");
void	System_LoadExecutable(char *path);

// System_SetBackgroundColor(R, G, B);
void	System_SetBackgroundColor(u_char r, u_char g, u_char b);

// System_InitFrame();	Return the Current OT Pointer
GsOT	*System_InitFrame(void);

// System_DrawFrame(Current OT Pointer, CPU Usage int Pointer (can be NULL), GPU Usage int Pointer (can be NULL));
void	System_DrawFrame(GsOT *ot, int *cpu, int *gpu);


#endif
