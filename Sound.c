
#include "Sound.h"

char	SpuMallocRec[SPU_MALLOC_RECSIZ * (MAX_SOUND + 1)];
u_char	SpuChn[24];
int		CDTracks[2];


void	Sound_Init(void)
{
	SpuCommonAttr	c_attr;
	SpuVoiceAttr	s_attr;

	printf("Sound_Init();\n");

	SpuInit();
	SpuInitMalloc(MAX_SOUND, SpuMallocRec);

    c_attr.mask = (SPU_COMMON_MVOLL | SPU_COMMON_MVOLR);
    c_attr.mvol.left  = MAX_VOLUME;
    c_attr.mvol.right = MAX_VOLUME;

    SpuSetCommonAttr(&c_attr);
	SpuSetTransferMode(SpuTransByDMA);

    s_attr.mask = (	SPU_VOICE_VOLL | SPU_VOICE_VOLR |
					SPU_VOICE_ADSR_AMODE | SPU_VOICE_ADSR_SMODE | SPU_VOICE_ADSR_RMODE |
					SPU_VOICE_ADSR_AR | SPU_VOICE_ADSR_DR | SPU_VOICE_ADSR_SR | SPU_VOICE_ADSR_RR | SPU_VOICE_ADSR_SL);

    s_attr.voice		= SPU_ALLCH;
    s_attr.volume.left  = MAX_VOLUME;
    s_attr.volume.right = MAX_VOLUME;
    s_attr.a_mode       = SPU_VOICE_LINEARIncN;
    s_attr.s_mode       = SPU_VOICE_LINEARIncN;
    s_attr.r_mode       = SPU_VOICE_LINEARDecN;
    s_attr.ar           = 0x0;
    s_attr.dr           = 0x0;
    s_attr.sr           = 0x0;
    s_attr.rr           = 0x0;
    s_attr.sl           = 0xf;

    SpuSetVoiceAttr(&s_attr);

	memset(SpuChn, 0, sizeof(SpuChn));
}


void	Sound_CD_PlayXA(char *filename, u_char channel)
{
	CdlFILE		fp;
	CdlFILTER	filter;
	u_char		param[4];

	/* get file position*/
	if (CdSearchFile(&fp, filename) == 0)
		return;

	/* set ADPCM filter */
	filter.file = 1;
	filter.chan = channel;
	CdControl(CdlSetfilter, (u_char *)&filter, 0);

	param[0] = CdlModeSpeed | CdlModeRT | CdlModeSF;
	CdControlB(CdlSetmode, param, 0);
	VSync(3);

	CdControl(CdlReadS, (u_char *)&fp.pos, 0);
}


void	Sound_CD_StopXA(void)
{
	CdControl(CdlStop, 0, 0);
	CdFlush();
}


void	Sound_CD_Init(void)
{
	printf("Sound_CD_Init();\n");
//	CdInit();
	SpuSetCommonCDMix(SPU_ON);
	SpuSetCommonCDVolume(MAX_VOLUME, MAX_VOLUME);
}


int		Sound_CD_GetNumTracks(void)
{
	CdlLOC	myLOC[100];

	return (CdGetToc(myLOC));
}


void	Sound_CD_Play(int t, int mode)
{
	CDTracks[0] = t;
	CDTracks[1] = 0;

	CdPlay(mode, CDTracks, 0);
}


void	Sound_CD_Stop(void)
{
	CDTracks[0] = 0;

	CdPlay(0, CDTracks, 0);
}


int		Sound_CD_IsPlaying(void)
{
	return (CdStatus() & CdlStatPlay);
}


void	Sound_Exit(void)
{
	int	i;

	printf("Sound_Exit();\n");

	if (Sound_CD_IsPlaying())
		Sound_CD_Stop();

	SpuSetKey(SpuOff, SPU_ALLCH);

	for (i = 0; i < 24; i++)
		Sound_Free(i);

	SpuQuit();
}


void	Sound_Load(int channel, u_char *adrs)
{
	u_long	a_adrs;
	u_long	size = (adrs[12] << 24) | (adrs[13] << 16) | (adrs[14] << 8) | adrs[15];
	u_short	freq = (adrs[18] << 8) | adrs[19];

	if (SpuChn[channel])
		Sound_Free(channel);

	printf("Sound_Load(%02d, %08x); Size: %08x; Freq: %d;", channel, adrs, size, freq);
	if ((a_adrs = SpuMalloc(size)) == 0xFFFFFFFF)
	{
		printf(" SpuMalloc Error !\n");
		return;
	}
	printf(" Spu: %08x;", a_adrs);
	SpuSetTransferStartAddr(a_adrs);
	SpuWrite(&adrs[SPU_ST_VAG_HEADER_SIZE], size);
    SpuIsTransferCompleted(SPU_TRANSFER_WAIT);
	SpuSetVoiceStartAddr(channel, a_adrs);
	SpuSetVoicePitch(channel, (freq == 44100) ? S_PITCH : S_PITCH / 2);
	printf(" done.\n");

	SpuChn[channel] = 1;

	return;
}


void	Sound_Free(int channel)
{
	u_long	adrs;

	if (SpuChn[channel] == 0)
		return;

	SpuGetVoiceStartAddr(channel, &adrs);
	if (adrs >= 0x1010)
	{
		SpuFree(adrs);
		printf("Sound_Free(%d); 0x%x\n", channel, adrs);
	}

	SpuChn[channel] = 0;
}
