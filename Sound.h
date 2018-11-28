

#ifndef _SOUND_H_
#define _SOUND_H_

#include <sys/types.h>
#include <libcd.h>
#include <libspu.h>

#define MAX_SOUND	24
#define	MAX_VOLUME	0x3FFF
#define S_PITCH		0x1000
#define	CD_ONETIME	1
#define	CD_REPEAT	2

void	Sound_Init(void);
void	Sound_Exit(void);

// Sound_Load(Sound Channel, Sound Address); Only 44100Hz and 22050Hz supported
void	Sound_Load(int channel, u_char *adrs);
void	Sound_Free(int channel);

#define	Sound_Play(ch)				SpuSetKey(SPU_ON, SPU_VOICECH(ch))
#define	Sound_IsPlaying(ch)			(SpuGetKeyStatus(SPU_VOICECH(ch)) == SPU_ON)
#define	Sound_Stop(ch)				SpuSetKey(SPU_OFF, SPU_VOICECH(ch))
#define	Sound_SetVolume(ch, vol)	SpuSetVoiceVolume(ch, vol, vol)

void	Sound_CD_PlayXA(char *filename, u_char channel);
void	Sound_CD_StopXA(void);

void	Sound_CD_Init(void);		// Don't forget to call CdInit(); before !!
int		Sound_CD_GetNumTracks(void);
void	Sound_CD_Play(int t, int mode);
void	Sound_CD_Stop(void);
int		Sound_CD_IsPlaying(void);


#endif
