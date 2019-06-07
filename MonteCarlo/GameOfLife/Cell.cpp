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
int Cell::counter = 0;


