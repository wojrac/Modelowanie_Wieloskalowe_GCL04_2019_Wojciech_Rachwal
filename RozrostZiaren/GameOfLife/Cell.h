#pragma once
#include<iostream>
using namespace std;
class Cell
{
private:
	int state;
	unsigned int coordinateX;
	unsigned int coordinateY;
	static int counter;
	int iid;
	bool isBusy;
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
	void setCellColor(int);
	//void setCellDead();
	int getCellColor();
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
	bool IsBusy()
	{
		return isBusy;
	}
	void setBusyTrue()
	{
		isBusy = true;
	}
	void setBusyFalse()
	{
		isBusy = false;
	}
};
