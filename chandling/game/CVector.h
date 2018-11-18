#pragma once
/**
* CVector Structure used to store a 3D vertex.
*/
class CVector
{
public:
	float fX, fY, fZ;

	CVector()
	{
		this->fX = 0;
		this->fY = 0;
		this->fZ = 0;
	};

	CVector(float fX, float fY, float fZ)
	{
		this->fX = fX;
		this->fY = fY;
		this->fZ = fZ;
	}
};