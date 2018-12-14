#pragma once
#include <Windows.h>

enum eSampVersion
{
	SAMP_000,
	SAMP_037_R2,
	SAMP_037_R3,
	SAMP_03DL
};

eSampVersion DetectSampVersion(DWORD dwSAMP);