#pragma once
#include<iostream>
using namespace std;
class Cell
{
private:
	bool state;
	unsigned int coordinateX;
	unsigned int coordinateY;
	static int counter;
	int iid;
public:
	Cell()
	{
		counter++;
		iid=counter;
	}
	~Cell()
	{
		counter--;
	}
	void setCellAlive();
	void setCellDead();
	bool IsAlive();
	static int getCounter()
	{
		return counter;
	}
	unsigned int getCoordinateX();
	unsigned int getCoordinateY();
	int getIid();
	void setCoordinateX(unsigned int x)
	{
		this->coordinateX = x;
	}
	void setCoordinateY(unsigned int y)
	{
		this->coordinateY = y;
	}
};
