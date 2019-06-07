#pragma once
#include<iostream>
using namespace std;
class Cell
{
private:
	int state;
	unsigned int coordinateX;
	unsigned int coordinateY;
	double centerX;
	double centerY;
	static int counter;
	int iid;
	bool isBusy;
	bool isRecristalized;
	bool isRecristalizedtmp;
	double densityOfDislocation;
	long double tmpDensity=0;
	long double tmpPackage=0;
	bool isAllLesser;
	bool isOnTheBorder;
	bool isOneNeighRec;
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
	double getCenterX();
	double getCenterY();
	void setCenterX(double  cx);
	void setCenterY(double cy);
	int getIid();
	double getDensityOfDislocation();
	void setDensityOfDislocation(double );
	bool IsRecristalized();
	bool IsRecristalizedtmp();
	void setTMPDensity(long double x)
	{
		this->tmpDensity = x;
	}
	long double getTMPDensity()
	{
		return this->tmpDensity;
	}
	bool IsAtLeastOneRecNeigh()
	{
		if (this->isOneNeighRec == true)
			return true;
		else 
			return false;
	}
	void setNeighAtLeastOneTrue()
	{
		this->isOneNeighRec = true;
	}
	void setNeighAtLeastOneFalse()
	{
		this->isOneNeighRec = false;
	}
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
	void setCellIsOnTheBorder()
	{
		this->isOnTheBorder = true;
	}
	void setCellIsOutTheBorder()
	{
		this->isOnTheBorder = false;
	}
	bool IsOnTheBorder()
	{
		if (this->isOnTheBorder == true)
			return true;
		else
			return false;
	}
	void setCellRecrystalized()
	{
		isRecristalized = true;
	}
	void setCellUnRecrystalized()
	{
		isRecristalized =false;
	}
	void setCellRecrystalizedtmp()
	{
		isRecristalizedtmp = true;
	}
	void setCellUnRecrystalizedtmp()
	{
		isRecristalizedtmp = false;
	}
	long double getTmpPackage()
	{
		return this->tmpPackage;
	}
	void setTmpPackage(long double x)
	{
		this->tmpPackage = x;
	}
	void setLessnestTrue()
	{
		this->isAllLesser = true;
	}
	void setLessnestFalse()
	{
		this->isAllLesser =false;
	}
	bool IsAllLesser()
	{
		return this->isAllLesser;
	}
};
