#pragma once
#include<math.h>
class DRX
{
private:
	long double A = 86710969050178.5;
	long double B = 9.41268203527779;
	long double indexOfEXP;
	long double e = 2.71828182846;

public:
	DRX()
	{

	}
	~DRX()
	{

	}
	long double countDensity(double);
	
};