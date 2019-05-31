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
int Cell::counter = 0;

