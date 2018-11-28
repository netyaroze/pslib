

// Handle File from pc (debug) or from cdrom (final)

#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_

#include <sys/types.h>
#include <libcd.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libsn.h>


// Uncomment this for a CDROM Release, or use compiler command line -DCDROM_RELEASE
// (If commented, then a dummy loader + debug messages are used)
//#define CDROM_RELEASE


// Define this before including this header if you want to not emulate CD Rom Loading Speed
#define PC_LOAD_MAX_SPEED


// Base address used for loading datas (beware on how much memory takes your program ! bss/data/text included)
#ifndef USER_BASEADRS	// Following define can be overwritten by the compiler line command if you also define this
#define	DM_BASE_ADDRESS	0x80010000
#endif

#define	ALIGN			16	// Datas loading address Align
#define	CD_LOAD_RETRY	10
#define	PC_READ_SIZE	(1*1024)

typedef struct	s_DM_info
{
  char			*filename;	// CDROM Data Filename (NULL to terminate the datas table)
  void			*address;	// Address where the datas are loaded
  int			size;
}				DataManager_Files;


typedef	void	(*TInitCDLoading)(void);
typedef	void	(*TDoCDLoading)(void);
typedef	void	(*TErrorScreen)(char *error_type, char *filename);

#ifndef CDROM_RELEASE
#define	DataManager_Init	PCinit
#else
#define	DataManager_Init	CdInit
#endif

void	DataManager_SetScreenCallBack(TInitCDLoading icdl, TDoCDLoading dcdl, TErrorScreen es);
void	DataManager_LoadDatas(char *base_dir, DataManager_Files *datas);
int		DataManager_Load(char *filename, char *buffer);

#endif
