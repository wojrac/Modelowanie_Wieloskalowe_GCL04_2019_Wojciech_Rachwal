#include"DRX.h"

long double DRX::countDensity(double sT)
{
	indexOfEXP = (-1)*(this->B)*sT;
	double ro = (this->A) / (this->B) + (1 - ((this->A) / (this->B))) * pow(this->e, indexOfEXP);
	return ro;
}