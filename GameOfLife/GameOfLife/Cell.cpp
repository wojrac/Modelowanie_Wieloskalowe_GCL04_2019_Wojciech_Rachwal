#include "Cell.h"
void Cell::setCellAlive()
{
	this->state = true;
}
void Cell::setCellDead()
{
	this->state = false;
}
bool Cell::IsAlive()
{
	if ((this->state)==true)
		return true;
	else
		return false;
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

