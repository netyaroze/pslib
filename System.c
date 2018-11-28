#include "System.h"

GsOT		OT[2], *System_CurOT;
GsOT_TAG	OT_zSort[2][1 << OT_LENGTH];
PACKET		GpuPacket[2][PACKET_MAX * PACKET_SIZE];
u_char		ClearColor[3];
int			SysPad, SysPadT;
int			System_CurrentBufferID;
int			System_Prio;
u_long		SysMsecTimer;


// Internal VSync Callback
void	SysVSyncCounter(void)
{
	if (GetVideoMode() == MODE_PAL)
		SysMsecTimer += 5120;	// (1000/50)<<8;
	else
		SysMsecTimer += 4267;	// (1000/60)<<8;
}


// System_Init(Video Mode, 16/24bits Mode, X Resolution, Y Resolution, Interlaced Mode, Dither Mode);
void	System_Init(long vmode, u_short bpp, u_short xres, u_short yres, u_short int1, u_short dither)
{
	printf("\n\nPlayStation Lib by Jeremy [2018]\nSystem, DataManager, Sprite, Prim2D, Dialog, BG, Obj3D, Sound, Movie, BSdec.\n\n");

	ResetCallback();
	PadInit(0);

	SetGraphDebug(0);
	SetVideoMode(vmode);
	GsInitGraph(xres, yres, int1 | GsOFSGPU, dither, bpp);
	if (int1 == GsINTER)
		GsDefDispBuff(0, 0, 0, 0);		// Interlaced Mode !
	else
		GsDefDispBuff(0, 0, 0, yres);	// NonInterlaced 1st Screen Up and 2nd Screen Down
	GsClearDispArea(0, 0, 0);

	OT[0].length = OT_LENGTH;
	OT[1].length = OT_LENGTH;
	OT[0].org = OT_zSort[0];
	OT[1].org = OT_zSort[1];

	SysMsecTimer = 0;
	VSyncCallback(SysVSyncCounter);

	System_ResetPad();
	System_SetBackgroundColor(0, 0, 0);
}


// System_Exit();
void	System_Exit(void)
{
	PadStop();
	ResetGraph(0);
	StopCallback();
}


// System_ResetPad();
void	System_ResetPad(void)
{
	SysPad = 0;
	SysPadT = 0;
}


// System_ReadPad();
void	System_ReadPad(void)
{
	int	pad = PadRead(0);

	SysPadT = pad & (pad ^ SysPad);
	SysPad = pad;
}


// System_SetBackgroundColor(R, G, B);
void	System_SetBackgroundColor(u_char r, u_char g, u_char b)
{
	ClearColor[0] = r;
	ClearColor[1] = g;
	ClearColor[2] = b;
}


// System_InitFrame();	Return the Current OT Pointer
GsOT	*System_InitFrame(void)
{
	System_ReadPad();

	System_CurrentBufferID = GsGetActiveBuff();
	System_CurOT = &OT[System_CurrentBufferID];
	GsSetWorkBase(GpuPacket[System_CurrentBufferID]);
	GsClearOt(0, 0, System_CurOT);

	System_Prio = (1 << OT_LENGTH) - 1;

	return (System_CurOT);
}


// System_DrawFrame(Current OT Pointer, CPU Usage int Pointer (can be NULL), GPU Usage int Pointer (can be NULL));
void	System_DrawFrame(GsOT *ot, int *cpu, int *gpu)
{
	if (cpu != NULL)
		*cpu = VSync(1);

	DrawSync(0);

	if (gpu != NULL)
		*gpu = VSync(0);
	else
		VSync(0);

	GsSwapDispBuff();
	GsSortClear(ClearColor[0], ClearColor[1], ClearColor[2], ot);
	GsDrawOt(ot);
}


// System_LoadExecutable(CD Rom Path to the Executable);
void	System_LoadExecutable(char *path)
{
	CdInit();
	System_Exit();
	_96_init();
	LoadExec(path, 0x801ffff0, 0);
}
