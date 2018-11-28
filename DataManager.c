
// Handle File from pc (debug) or from cdrom (final release)

#include "DataManager.h"

TInitCDLoading	DM_InitCDLoading	= NULL;
TDoCDLoading	DM_DoCDLoading		= NULL;
TErrorScreen	DM_ErrorScreen		= NULL;
u_long			baseaddress;


void		DataManager_SetScreenCallBack(TInitCDLoading icdl, TDoCDLoading dcdl, TErrorScreen es)
{
	DM_InitCDLoading = icdl;
	DM_DoCDLoading = dcdl;
	DM_ErrorScreen = es;
}


void		DataManager_LoadDatas(char *base_dir, DataManager_Files *datas)
{
	int		i, size;
	char	filename[64];

	if (DM_InitCDLoading != NULL)
		DM_InitCDLoading();

	baseaddress = DM_BASE_ADDRESS;

	i = 0;
	while (datas[i].filename)
	{
		datas[i].address = (void *)baseaddress;
		sprintf(filename, "%s\\%s", base_dir, datas[i].filename);
#ifndef CDROM_RELEASE
		printf("DataManager: Loading '%s' at %x\n", filename, datas[i].address);
#endif
		size = DataManager_Load(filename, datas[i].address);
		datas[i].size = size;
		baseaddress += (size - (size & (ALIGN - 1))) + ALIGN;
		i++;
	}
#ifndef CDROM_RELEASE
	printf("DataManager: done.\n");
#endif
}


int			DataManager_Load(char *filename, char *buffer)

#ifndef CDROM_RELEASE

// Load a file from PC with error handling using custom error screen and custom loading screen
{
	int		i, file, size;

	file = PCopen(filename, 0, 0);

	if (file <= 0)
	{
		if (DM_ErrorScreen == NULL)
			printf("File not found '%s'\n", filename);
		else
			DM_ErrorScreen("File not found", filename);
		return (-1);
	}

	size = PClseek(file, 0, 2);
	PClseek(file, 0, 0);

	i = size;
	while (i > PC_READ_SIZE)
	{
		PCread(file, buffer, PC_READ_SIZE);
		buffer += PC_READ_SIZE;
		i -= PC_READ_SIZE;

		if (DM_DoCDLoading != NULL)
			DM_DoCDLoading();
#ifndef	PC_LOAD_MAX_SPEED
		else
			VSync(0);
#endif
	}
	PCread(file, buffer, i);
	PCclose(file);

	return (size);
}

#else

// Load a file from CD with retry, error handling using custom error screen and custom loading screen
{
	int		i, cnt;
	CdlFILE	file;
	char	flname[64];

	sprintf(flname, "\\%s;1", filename);
	filename = flname;

	// ALWAYS UPPER CASE FILENAME FOR CD LOADING !!
	for (i = 0; i < strlen(flname); i++)
		if ((flname[i] >= 'a') && (flname[i] <= 'z'))
			flname[i] -= 'a' - 'A';

	for (i = 0; i < CD_LOAD_RETRY; i++)
	{
		if (CdSearchFile(&file, filename) != 0)
			break;
	}

	if (i == CD_LOAD_RETRY)
	{
		if (DM_ErrorScreen == NULL)
			printf("File not found '%s'\n", filename);
		else
			DM_ErrorScreen("File not found", filename);
		return (-1);
	}

	for (i = 0; i < CD_LOAD_RETRY; i++)
	{
		CdReadFile(filename, (u_long *)buffer, file.size);

		while ((cnt = CdReadSync(1, 0)) > 0)
		{
			if (DM_DoCDLoading != NULL)
				DM_DoCDLoading();
			else
				VSync(0);
		}

		if (cnt == 0)
			break;
	}

	if (i == CD_LOAD_RETRY)
	{
		if (DM_ErrorScreen == NULL)
			printf("Error while reading file '%s'\n", filename);
		else
			DM_ErrorScreen("Error while reading file", filename);
		return (-1);
	}

	return (file.size);
}

#endif
