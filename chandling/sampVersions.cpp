#include "sampVersions.h"
#include "main.h"

eSampVersion DetectSampVersion(DWORD dwSAMP)
{
	switch(*(BYTE*)(dwSAMP + 0x1036))
	{
	case 0xA8:
	{
	DebugPrint("Detected SAMP version 0.3.7 R2\n");
	return SAMP_037_R2;
	}

	case 0x78:
	{
	DebugPrint("Detected SAMP version 0.3DL\n");
	return SAMP_03DL;
	}
	}

	return SAMP_000;
}