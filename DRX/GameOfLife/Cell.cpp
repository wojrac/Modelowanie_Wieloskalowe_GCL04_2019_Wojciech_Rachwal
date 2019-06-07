#include "Cell.h"
void Cell::setCellColor(int color)
{
	this->state = color;
}
/*void Cell::setCellDead()
{
	this->state = false;
}*/
int Cell::getCellColor()
{
	return this->state;
}
unsigned int Cell::getCoordinateX()
{
	return coordinateX;
}
unsigned int Cell::getCoordinateY()
{
	return coordinateY;
}
int Cell::getIid()
{
	return iid-1;
}
double Cell::getCenterX()
{
	return this->centerX;
}
double Cell::getCenterY()
{
	return this->centerY;
}
void Cell::setCenterX(double cx)
{
	this->centerX = cx;
}
void Cell::setCenterY(double cy)
{
	this->centerY = cy;
}
double Cell::getDensityOfDislocation()
{
	return densityOfDislocation;
}
void Cell::setDensityOfDislocation(double den)
{
	this->densityOfDislocation = den;
	this->tmpPackage += den;
	this->tmpDensity += den;
}
bool Cell::IsRecristalized()
{
	if (this->isRecristalized == true)
		return true;
	else
		return false;
	
}
bool Cell::IsRecristalizedtmp()
{
	if (this->isRecristalizedtmp == true)
		return true;
	else
		return false;

}
int Cell::counter = 0;


