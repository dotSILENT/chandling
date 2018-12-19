#include "sampVersions.h"
#include "main.h"

eSampVersion DetectSampVersion(DWORD dwSAMP)
{
	BYTE bVer = *(BYTE*)(dwSAMP + 0x1036);
	switch(bVer)
	{
	case 0xA8:
		DebugPrint("Detected SAMP version 0.3.7 R2");
		return SAMP_037_R2;

	case 0x90:
		DebugPrint("Detected SAMP version 0.3.7 R3");
		return SAMP_037_R3;

	case 0x78:
		DebugPrint("Detected SAMP version 0.3DL");
		return SAMP_03DL;
	}
	
	DebugPrint("Couldn't detect SAMP version (version byte: 0x%x)", bVer);
	LogError("Couldn't detect SAMP version (version byte: 0x%x)", bVer);
	return SAMP_000;
}