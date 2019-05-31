#include"Cell.h"
#include<memory>
#include "Button.h"
#include"TextField.h"
#include<boost\filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include<filesystem>
#include<vector>
#include<SFML\Graphics.hpp>
#include<SFML\Window.hpp>
#include<ctime>
#include<cstdlib>
using namespace boost::filesystem;
namespace fs= std::experimental::filesystem;
using namespace sf;
int coordinateToNumber(int, int, unique_ptr<Cell[]>&,int,int);
vector<vector<char>> decompress(vector<vector<char>>);
int coordinateToNumber(int coordinateX, int coordinateY, unique_ptr<Cell[]>& arr,int sX,int sY)
{
	int i = 0;
	while (!(arr.get()[i].getCoordinateX() == coordinateX && arr.get()[i].getCoordinateY() == coordinateY) && i<sX*sY)
	{
		i++;
	}
	return  i;
}


int mostOftenOccured(vector<int>& vec)
{
	int sum=  0;
	int tmp = 0;
	for (int i = 0; i < vec.size(); i++)
	{
		sum += vec[i];
	}
	if (sum == 0)
	{
		
		return 0;
	}
	else
	{
		
		int maxL = 0, L = 0,maxW=0;
		for (int i = 0; i < vec.size(); i++)
		{
			
			tmp = vec[i];
			
			L = 0;
			for (int j = 0; j < vec.size(); j++)
			{
				if (vec[j] == tmp) L++;
			}

			if (L > maxL)
			{
				if (tmp != 0)
				{
					maxL = L;
					maxW = tmp;
				}
			}
		}
		return maxW;
	}
}
bool isInside(int* tabX, int*tabY, int permX, int permY,int sizeTab)
{
	bool sel = false ;
	for (int i = 0; i < sizeTab; i++)
	{
		if (tabX[i] == permX && tabY[i]==permY)
		{
			sel = true;
		}
	}
	if (sel)
		return true;
	else
		return false;
}

int main()
{
	srand(time(NULL));
	unsigned int windowWidth = 1200;
	unsigned int windowHeight = 1000;
	RenderWindow window = { VideoMode{windowWidth,windowHeight} , "GAME OF LIFE" };
	window.setFramerateLimit(20); 
	float a = 8, b = 8;
	RectangleShape circleShape2(Vector2f(a, b));
	circleShape2.setFillColor(Color::White);
	Font font;
	if (!font.loadFromFile("arial.ttf"))
	{
		cout << "error loading a file " << endl;
		system("pause");
	}
	
	int ButtonX = windowWidth - 200;
	//int firstButtonY = 100;
	int buttonWidth = 100;
	int buttonHeight = 40;
	Button button1(ButtonX, 100, buttonWidth, buttonHeight,font, "JEDNORODNE", Color::Blue);
	Button button4(ButtonX, 200, buttonWidth, buttonHeight, font, "PROMIEN", Color::Blue);
	Button button2(ButtonX, 300, buttonWidth, buttonHeight, font, "ZAPASOWY", Color::Blue);
	Button button3(ButtonX, 500, buttonWidth, buttonHeight, font, "FINISH", Color::Blue);
	Button button6(ButtonX, 400, buttonWidth, buttonHeight, font, "RANDOM", Color::Blue);
	Button button7(ButtonX, 0, buttonWidth, buttonHeight, font, "PLAY", Color::Blue);
	Button button8(ButtonX, 600, buttonWidth, buttonHeight, font, "STOP", Color::Blue);
	Button buttonPeriodic(ButtonX, 700, buttonWidth+60, buttonHeight, font, "WARUNKI PERIODYCZNE", Color::Yellow);
	Button buttonAbsorb(ButtonX, 800, buttonWidth + 60, buttonHeight, font, "WARUNKI ABSORBUJACE", Color::Yellow);
	Button buttonSizePlus(ButtonX+175, 0, buttonWidth/4, buttonHeight/2, font, "X+", Color::Green);
	Button buttonSizeMinus(ButtonX + 175, 20, buttonWidth / 4, buttonHeight / 2, font, "X-", Color::Red);
	Button buttonSizePlusY(ButtonX + 175, 40, buttonWidth / 4, buttonHeight / 2, font, "Y+", Color::Green);
	Button buttonSizeMinusY(ButtonX + 175, 60, buttonWidth / 4, buttonHeight / 2, font, "Y-", Color::Red);
	vector<string> patternFromFile;
	string iter;
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini("config.ini", pt);
	//TABLICE SASIADOW DLA SKRAJNYCH KOMOREK
	// dla rogow zwieksza z 3->8
	Cell neighboursWithLeftTop[8];
	Cell neighboursWithLeftDown[8];
	Cell neighboursWithRightTop[8];
	Cell neighboursWithRightDown[8];
	string sizeXs, sizeYs,cordXs[8],cordYs[5];
	int sizeX, sizeY,cordX[5],cordY[5],cordX1,cordY1, numberOfCell;
	vector<int> cordXVec;
	vector<int> cordYVec;
	try {
		
		sizeXs = pt.get<std::string>("Section1.sizeX");
		sizeYs = pt.get<std::string>("Section1.sizeY");
		sizeX = atoi(sizeXs.c_str());
		sizeY = atoi(sizeYs.c_str());
		if (sizeX <= 0 || sizeY <=0)
			throw 99;
	}
	catch (int x)
	{
		cout << "cordinates must not be less or equal than 0, ERROR NR: " << x<< endl;
		cout << "it is necesary to change this to 1 " << endl;
		if(sizeX<=0)
		sizeX = 1;
		if(sizeY<=0)
		sizeY = 1;
	}
	int actuallNumberOfStates; 
	int elementsToRand;
	int statesInColumn, statesInRow;   
	
	int numberOfMaxStates = (sizeX) * (sizeY);
	int* arrayOfStates = new int[numberOfMaxStates];
	RectangleShape* arrayOfShapes = new RectangleShape[numberOfMaxStates];     
	for (int i = 0; i < numberOfMaxStates; i++)
	{
		arrayOfStates[i] = 0;
		arrayOfShapes[i] = circleShape2;     //stan pusty
	}
	
	//shared_ptr<Cell> myBoard(new Cell[sizeX*sizeY],default_delete<Cell[]>());
	unique_ptr<Cell[]> myBoard;
	myBoard = make_unique<Cell[]>(sizeX*sizeY);
	for (int i = 0; i <sizeY; i++)
	{
		for (int j = 0; j < sizeX; j++)
		{
			myBoard.get()[j + sizeX*i].setCellColor(0);
			myBoard.get()[j+sizeX*i].setCoordinateX(j);
			myBoard.get()[j+sizeX*i].setCoordinateY(i);
		}
	}
	
		Event event;
		int NumberOfCellInRow = sizeX - 2;
		int NumberOfCellInColumn = sizeY - 2;

		unique_ptr<unique_ptr<Cell[]>[]>    neighboursWithTopRow;
		unique_ptr<unique_ptr<Cell[]>[]>    neighboursWithDownRow;
		//5->8
		unique_ptr<unique_ptr<Cell[]>[]>    neighboursWithLeftColumn;
		unique_ptr<unique_ptr<Cell[]>[]>    neighboursWithRightColumn;
		unique_ptr<Cell[]>                   smartPtr1D;
		unique_ptr<Cell[]>                   DownRow1D;
		unique_ptr<Cell[]>					 Left1D;
		unique_ptr<Cell[]>                   Right1D;

		neighboursWithTopRow = make_unique< unique_ptr<Cell[]>[] >(NumberOfCellInRow);
		neighboursWithDownRow = make_unique< unique_ptr<Cell[]>[] >(NumberOfCellInRow);
		neighboursWithLeftColumn = make_unique< unique_ptr<Cell[]>[] >(NumberOfCellInColumn);
		neighboursWithRightColumn = make_unique< unique_ptr<Cell[]>[] >(NumberOfCellInColumn);
		int NumberOfCellInTheMiddle = (sizeX - 2)*(sizeY - 2);
		unique_ptr<unique_ptr<Cell[]>[]>    neighboursMiddle;
		unique_ptr<Cell[]>                   middle1D;
		unique_ptr<int[]> temporaryArray;
		temporaryArray = make_unique<int[]>(NumberOfCellInTheMiddle);
		unique_ptr<int[]> arrayWithSum;
		arrayWithSum = make_unique<int[]>(sizeX*sizeY);
		Text FPSLabel;
		Text LabelRow;
		Text LabelColumn;
		Text RadiusLabel;
		FPSLabel.setString("Podaj liczbe stanow do wylosowania: ");
		FPSLabel.setFont(font);
		FPSLabel.setCharacterSize(14);
		FPSLabel.setPosition(windowWidth - 500, windowHeight - 50);
		FPSLabel.setFillColor(Color::White);
		LabelRow.setString("Podaj liczbe stanow w wierszu: ");
		LabelRow.setFont(font);
		LabelRow.setCharacterSize(14);
		LabelRow.setPosition(windowWidth - 500, windowHeight - 80);
		LabelRow.setFillColor(Color::White);
		LabelColumn.setString("Podaj liczbe stanow w kolumnie: ");
		LabelColumn.setFont(font);
		LabelColumn.setCharacterSize(14);
		LabelColumn.setPosition(windowWidth - 500, windowHeight - 110);
		LabelColumn.setFillColor(Color::White);
		RadiusLabel.setString("Podaj promien ");
		RadiusLabel.setFont(font);
		RadiusLabel.setCharacterSize(14);
		RadiusLabel.setPosition(windowWidth - 500, windowHeight - 20);
		RadiusLabel.setFillColor(Color::White);
		bool selectorMain = true;
		int rightDownId = sizeX - 1;
		int rightTopId = sizeX*sizeY - 1;
		int leftTopId = rightTopId - sizeX + 1;
		bool isStarted = false;
		bool IsDidAnyTurn = false;
		bool selectorPlay = false;
		vector<int> vectorZeroId;
		vector<int> vectorRightDownId;
		vector<int> vectorRightTopId;
		vector<int> vectorLeftTopId;
		vector<int> vectorWithDownRow1D;
		vector<int> vectorWithTopRow1D;
		vector<int> vectorWithLeftColumn1D;
		vector<int> vectorWithRightColumn1D;
		vector<int> vectorMiddle1D;
		bool wasStartedAtLeastOne = false;
		int counterOfZero = 0;
		bool isPeriodic = false;
		bool isAbsorb = false;
		TextField tf(8);
		tf.setPosition(windowWidth-200, windowHeight-50);
		TextField tfRow(8);
		tfRow.setPosition(windowWidth - 200, windowHeight - 80);
		TextField tfColumn(8);
		tfColumn.setPosition(windowWidth - 200, windowHeight - 110);
		TextField tfRadius(8);
		tfRadius.setPosition(windowWidth - 300, windowHeight - 20);
		string actuallNumberOfStatesfromTextField;
		Text ACTTEXT;
		string tmpTF;
		string statesInRowsFromTF;
		Text ROWTEXT;
		string tmpTFRow;
		string statesInColumnFromTF;
		Text COLUMNTEXT;
		string tmpTFColumn;
		Text RADIUSTEXT;
		string tmpTFRadius;
		string radiusFromTF;
		ACTTEXT.setPosition(windowWidth-200+30, windowHeight-50);
		ACTTEXT.setFillColor(sf::Color::Black);
		ACTTEXT.setCharacterSize(14);
		ACTTEXT.setFont(font);
		ROWTEXT.setPosition(windowWidth - 200 + 30, windowHeight - 80);
		ROWTEXT.setFillColor(sf::Color::Black);
		ROWTEXT.setCharacterSize(14);
		ROWTEXT.setFont(font);
		COLUMNTEXT.setPosition(windowWidth - 200 + 30, windowHeight - 110);
		COLUMNTEXT.setFillColor(sf::Color::Black);
		COLUMNTEXT.setCharacterSize(14);
		COLUMNTEXT.setFont(font);
		RADIUSTEXT.setPosition(windowWidth - 300 + 30, windowHeight - 20);
		RADIUSTEXT.setFillColor(sf::Color::Black);
		RADIUSTEXT.setCharacterSize(14);
		RADIUSTEXT.setFont(font);
		int maxR ;
		int cntClick=0;
		Text FPSInfo;
		bool isFinished = true;
		//TU BEDZIE POCZATEK PETLI Z ITERACJAMI
		while (window.isOpen() )
		{
			window.clear(Color::Black);
			window.draw(button1);
			window.draw(button2);
			window.draw(button3);
			window.draw(button4);
			window.draw(button7);
			window.draw(button8);
			window.draw(button6);
			window.draw(buttonPeriodic);
			window.draw(buttonAbsorb);
			window.draw(FPSLabel);
			window.draw(LabelRow);
			window.draw(LabelColumn);
			window.draw(tf);
			window.draw(tfRow);
			window.draw(tfColumn);
			window.draw(ACTTEXT);
			window.draw(ROWTEXT);
			window.draw(COLUMNTEXT);
			window.draw(RadiusLabel);
			window.draw(tfRadius);
			window.draw(RADIUSTEXT);
			//window.draw(FPSInfo);
			window.draw(buttonSizePlus);
			window.draw(buttonSizeMinus);
			window.draw(buttonSizePlusY);
			window.draw(buttonSizeMinusY);
			
			
					for (int i = sizeY - 1; i >= 0; i--)
					{
						for (int j = 0; j < sizeX; j++)
						{
							if (myBoard.get()[j + sizeX*i].getCellColor())
							{
								int tmpColor = myBoard.get()[j + sizeX*i].getCellColor();
								window.draw(arrayOfShapes[tmpColor-1]);
								arrayOfShapes[tmpColor-1].setPosition((a + 1) * j, (b + 1) * i);
							}
							else
							{
								window.draw(circleShape2);
								circleShape2.setPosition((a+1) * j, (b+1) * i);
							}
						}
					}
					//cout << sizeX;
			if (selectorPlay && isStarted)
			{
				//cout << sizeX<<" ";
				for (int i = 0; i < sizeX*sizeY; i++)
				{
					if (myBoard.get()[i].getCellColor() == 0)
					{
					
						counterOfZero++;
					}
				}
				if (counterOfZero == 0)
				{
					isStarted = false;
					wasStartedAtLeastOne = true;
				}
				counterOfZero = 0;
				//cout << "gra " << " ";   <- ani razu sie nie wypisalo
				if (isPeriodic == true)
				{
					for (int i = 0; i < NumberOfCellInRow; i++)
					{
						smartPtr1D = make_unique<Cell[]>(4);
						DownRow1D = make_unique<Cell[]>(4);

						neighboursWithTopRow[i] = move(smartPtr1D);
						neighboursWithDownRow[i] = move(DownRow1D);
					}
					for (int i = 0; i < NumberOfCellInColumn; i++)
					{
						Left1D = make_unique<Cell[]>(4);
						Right1D = make_unique<Cell[]>(4);
						neighboursWithLeftColumn[i] = move(Left1D);
						neighboursWithRightColumn[i] = move(Right1D);
					}
					NumberOfCellInTheMiddle = (sizeX - 2)*(sizeY - 2);
					//unique_ptr<unique_ptr<Cell[]>[]>    neighboursMiddle;
					//unique_ptr<Cell[]>                   middle1D;
					neighboursMiddle = make_unique< unique_ptr<Cell[]>[] >(NumberOfCellInTheMiddle);
					for (int i = 0; i < NumberOfCellInTheMiddle; i++)
					{
						middle1D = make_unique<Cell[]>(4);

						neighboursMiddle[i] = move(middle1D);
					}
					neighboursWithLeftDown[0] = myBoard.get()[1];
					neighboursWithLeftDown[1] = myBoard.get()[sizeX];
					//neighboursWithLeftDown[2] = myBoard.get()[sizeX + 1];
					neighboursWithLeftDown[2] = myBoard.get()[rightDownId];
					//neighboursWithLeftDown[4] = myBoard.get()[rightDownId + sizeX];
					neighboursWithLeftDown[3] = myBoard.get()[leftTopId];
					//neighboursWithLeftDown[6] = myBoard.get()[leftTopId + 1];
					//neighboursWithLeftDown[7] = myBoard.get()[rightTopId];

					neighboursWithRightDown[0] = myBoard.get()[rightDownId - 1];
					neighboursWithRightDown[1] = myBoard.get()[rightDownId + sizeX];
					//neighboursWithRightDown[2] = myBoard.get()[rightDownId + sizeX - 1];
					neighboursWithRightDown[2] = myBoard.get()[0];
					//neighboursWithRightDown[4] = myBoard.get()[sizeX];
					neighboursWithRightDown[3] = myBoard.get()[rightTopId];
					//neighboursWithRightDown[6] = myBoard.get()[rightTopId - 1];
					//neighboursWithRightDown[7] = myBoard.get()[leftTopId];

					neighboursWithRightTop[0] = myBoard.get()[rightTopId - 1];
					neighboursWithRightTop[1] = myBoard.get()[rightTopId - sizeX];
					//neighboursWithRightTop[2] = myBoard.get()[rightTopId - 1 - sizeX];
					neighboursWithRightTop[2] = myBoard.get()[leftTopId];
					//neighboursWithRightTop[4] = myBoard.get()[leftTopId - sizeX];
					neighboursWithRightTop[3] = myBoard.get()[rightDownId];
					//neighboursWithRightTop[6] = myBoard.get()[rightDownId - 1];
					//neighboursWithRightTop[7] = myBoard.get()[0];

					neighboursWithLeftTop[0] = myBoard.get()[leftTopId + 1];
					neighboursWithLeftTop[1] = myBoard.get()[leftTopId - sizeX];
					//neighboursWithLeftTop[2] = myBoard.get()[leftTopId + 1 - sizeX];
					neighboursWithLeftTop[2] = myBoard.get()[rightTopId];
					//neighboursWithLeftTop[4] = myBoard.get()[rightTopId - sizeX];
					neighboursWithLeftTop[3] = myBoard.get()[0];
					//neighboursWithLeftTop[6] = myBoard.get()[1];
					//neighboursWithLeftTop[7] = myBoard.get()[rightDownId];
					//CZAS NA PASY
					//najpierw musze wyznaczyc ilosc komorek w kolumnie
					for (int i = 0; i < NumberOfCellInRow; i++)
					{
						neighboursWithTopRow[i][0] = myBoard.get()[(leftTopId + i + 1) - 1];
						neighboursWithTopRow[i][1] = myBoard.get()[(leftTopId + i + 1) + 1];
						//neighboursWithTopRow[i][2] = myBoard.get()[(leftTopId + i + 1) - sizeX - 1];
						neighboursWithTopRow[i][2] = myBoard.get()[(leftTopId + i + 1) - sizeX];
						//neighboursWithTopRow[i][4] = myBoard.get()[(leftTopId + i + 1) - sizeX + 1];
						//nowe do przerobienia
						//neighboursWithTopRow[i][5] = myBoard.get()[0 + i];
						neighboursWithTopRow[i][3] = myBoard.get()[1 + i];
						//neighboursWithTopRow[i][7] = myBoard.get()[2 + i];

						neighboursWithDownRow[i][0] = myBoard.get()[(i + 1) - 1];
						neighboursWithDownRow[i][1] = myBoard.get()[(i + 1) + 1];
						//neighboursWithDownRow[i][2] = myBoard.get()[(i + 1) + sizeX + 1];
						neighboursWithDownRow[i][2] = myBoard.get()[(i + 1) + sizeX];
						//neighboursWithDownRow[i][4] = myBoard.get()[(i + 1) + sizeX - 1];
						//nowe do prerobienia
						//neighboursWithDownRow[i][5] = myBoard.get()[leftTopId + i];
						neighboursWithDownRow[i][3] = myBoard.get()[leftTopId + 1 + i];
						//neighboursWithDownRow[i][7] = myBoard.get()[leftTopId + 2 + i];
					}  //chyba dziala :)

					for (int i = 0; i < NumberOfCellInColumn; i++)
					{
						neighboursWithLeftColumn[i][0] = myBoard.get()[(sizeX*i + sizeX) + sizeX];
						//neighboursWithLeftColumn[i][1] = myBoard.get()[(sizeX*i + sizeX) + sizeX + 1];
						neighboursWithLeftColumn[i][1] = myBoard.get()[(sizeX*i + sizeX) + 1];
						//neighboursWithLeftColumn[i][3] = myBoard.get()[(sizeX*i + sizeX) - sizeX + 1];
						neighboursWithLeftColumn[i][2] = myBoard.get()[(sizeX*i + sizeX) - sizeX];
						//nowe
						neighboursWithLeftColumn[i][3] = myBoard.get()[rightDownId + sizeX*i + sizeX];
						//neighboursWithLeftColumn[i][6] = myBoard.get()[(rightDownId + sizeX*(i + 1)) + sizeX];
						//neighboursWithLeftColumn[i][7] = myBoard.get()[(rightDownId + sizeX*(i + 1)) - sizeX];

						neighboursWithRightColumn[i][0] = myBoard.get()[(rightDownId + sizeX*(i + 1)) + sizeX];
						//neighboursWithRightColumn[i][1] = myBoard.get()[(rightDownId + sizeX*(i + 1)) + sizeX - 1];
						neighboursWithRightColumn[i][1] = myBoard.get()[(rightDownId + sizeX*(i + 1)) - 1];
						//neighboursWithRightColumn[i][3] = myBoard.get()[(rightDownId + sizeX*(i + 1)) - sizeX - 1];
						neighboursWithRightColumn[i][2] = myBoard.get()[(rightDownId + sizeX*(i + 1)) - sizeX];
						//nowe
						neighboursWithRightColumn[i][3] = myBoard.get()[i*sizeX + sizeX];
						//neighboursWithRightColumn[i][6] = myBoard.get()[(sizeX*i + sizeX) + sizeX];
						//neighboursWithRightColumn[i][7] = myBoard.get()[(sizeX*i + sizeX) - sizeX];
					}//wyglada ze dziala
					for (int i = 0, l = 0; l < sizeY - 2; i += sizeX, l++)
					{
						for (int j = sizeX + 1 + i, k = 0; k < sizeX - 2; j++, k++)
						{
							temporaryArray[k + l*(sizeX - 2)] = j;
						}
					}
					for (int i = 0; i < NumberOfCellInTheMiddle; i++)
					{

						//neighboursMiddle[i][0] = myBoard.get()[temporaryArray[i] - sizeX - 1];
						neighboursMiddle[i][0] = myBoard.get()[temporaryArray[i] - sizeX];
						//neighboursMiddle[i][2] = myBoard.get()[temporaryArray[i] - sizeX + 1];
						neighboursMiddle[i][1] = myBoard.get()[temporaryArray[i] + 1];
						//neighboursMiddle[i][4] = myBoard.get()[temporaryArray[i] + sizeX + 1];
						neighboursMiddle[i][2] = myBoard.get()[temporaryArray[i] + sizeX];
						//neighboursMiddle[i][6] = myBoard.get()[temporaryArray[i] + sizeX - 1];
						neighboursMiddle[i][3] = myBoard.get()[temporaryArray[i] - 1];
					}
					for (int i = 0; i <4; i++)
					{
						vectorZeroId.push_back(neighboursWithLeftDown[i].getCellColor());
						vectorRightDownId.push_back(neighboursWithRightDown[i].getCellColor());
						vectorRightTopId.push_back(neighboursWithRightTop[i].getCellColor());
						vectorLeftTopId.push_back(neighboursWithLeftTop[i].getCellColor());
					}

					arrayWithSum[0] = mostOftenOccured(vectorZeroId);
					arrayWithSum[rightTopId] = mostOftenOccured(vectorRightTopId);
					arrayWithSum[leftTopId] = mostOftenOccured(vectorLeftTopId);
					arrayWithSum[rightDownId] = mostOftenOccured(vectorRightDownId);
					for (int i = 1, j = 0; j < NumberOfCellInRow; i++, j++)
					{
						for (int k = 0; k < 4; k++)
							vectorWithDownRow1D.push_back(neighboursWithDownRow[j][k].getCellColor());
						arrayWithSum[i] = mostOftenOccured(vectorWithDownRow1D);
						vectorWithDownRow1D.clear();
					}
					for (int i = leftTopId + 1, j = 0; j < NumberOfCellInRow; i++, j++)
					{
						for (int k = 0; k < 4; k++)
							vectorWithTopRow1D.push_back(neighboursWithTopRow[j][k].getCellColor());
						arrayWithSum[i] = mostOftenOccured(vectorWithTopRow1D);
						vectorWithTopRow1D.clear();
					}
					for (int i = sizeX, j = 0; j < NumberOfCellInColumn; i += sizeX, j++)
					{
						for (int k = 0; k < 4; k++)
							vectorWithLeftColumn1D.push_back(neighboursWithLeftColumn[j][k].getCellColor());
						arrayWithSum[i] = mostOftenOccured(vectorWithLeftColumn1D);
						vectorWithLeftColumn1D.clear();
					}
					for (int i = rightDownId + sizeX, j = 0; j < NumberOfCellInColumn; i += sizeX, j++)
					{
						for (int k = 0; k < 4; k++)
							vectorWithRightColumn1D.push_back(neighboursWithRightColumn[j][k].getCellColor());
						arrayWithSum[i] = mostOftenOccured(vectorWithRightColumn1D);
						vectorWithRightColumn1D.clear();
					}
					for (int j = 0; j < (sizeX - 2)*(sizeY - 2); j++)
					{
						for (int k = 0; k < 4; k++)
							vectorMiddle1D.push_back(neighboursMiddle[j][k].getCellColor());
						arrayWithSum[temporaryArray[j]] = mostOftenOccured(vectorMiddle1D);
						vectorMiddle1D.clear();
					}

					vectorZeroId.clear();
					vectorRightDownId.clear();
					vectorRightTopId.clear();
					vectorLeftTopId.clear();
					for (int i = 0; i < sizeX*sizeY; i++)
					{
						if (myBoard.get()[i].getCellColor() == 0)
						{
							myBoard.get()[i].setCellColor(arrayWithSum[i]);
						}
					}
				}
				if (isAbsorb)
				{
					for (int i = 0; i < NumberOfCellInRow; i++)
					{
						smartPtr1D = make_unique<Cell[]>(3);
						DownRow1D = make_unique<Cell[]>(3);

						neighboursWithTopRow[i] = move(smartPtr1D);
						neighboursWithDownRow[i] = move(DownRow1D);
					}
					for (int i = 0; i < NumberOfCellInColumn; i++)
					{
						Left1D = make_unique<Cell[]>(3);
						Right1D = make_unique<Cell[]>(3);
						neighboursWithLeftColumn[i] = move(Left1D);
						neighboursWithRightColumn[i] = move(Right1D);
					}
					int NumberOfCellInTheMiddle = (sizeX - 2)*(sizeY - 2);
					unique_ptr<unique_ptr<Cell[]>[]>    neighboursMiddle;
					unique_ptr<Cell[]>                   middle1D;
					neighboursMiddle = make_unique< unique_ptr<Cell[]>[] >(NumberOfCellInTheMiddle);
					for (int i = 0; i < NumberOfCellInTheMiddle; i++)
					{
						middle1D = make_unique<Cell[]>(4);

						neighboursMiddle[i] = move(middle1D);
					}
					neighboursWithLeftDown[0] = myBoard.get()[1];
					neighboursWithLeftDown[1] = myBoard.get()[sizeX];
					//neighboursWithLeftDown[2] = myBoard.get()[sizeX + 1];
				

					neighboursWithRightDown[0] = myBoard.get()[rightDownId - 1];
					neighboursWithRightDown[1] = myBoard.get()[rightDownId + sizeX];
					//neighboursWithRightDown[2] = myBoard.get()[rightDownId + sizeX - 1];
				

					neighboursWithRightTop[0] = myBoard.get()[rightTopId - 1];
					neighboursWithRightTop[1] = myBoard.get()[rightTopId - sizeX];
					//neighboursWithRightTop[2] = myBoard.get()[rightTopId - 1 - sizeX];
				

					neighboursWithLeftTop[0] = myBoard.get()[leftTopId + 1];
					neighboursWithLeftTop[1] = myBoard.get()[leftTopId - sizeX];
					//neighboursWithLeftTop[2] = myBoard.get()[leftTopId + 1 - sizeX];
				
					//CZAS NA PASY
					//najpierw musze wyznaczyc ilosc komorek w kolumnie
					for (int i = 0; i < NumberOfCellInRow; i++)
					{
						neighboursWithTopRow[i][0] = myBoard.get()[(leftTopId + i + 1) - 1];
						neighboursWithTopRow[i][1] = myBoard.get()[(leftTopId + i + 1) + 1];
						//neighboursWithTopRow[i][2] = myBoard.get()[(leftTopId + i + 1) - sizeX - 1];
						neighboursWithTopRow[i][2] = myBoard.get()[(leftTopId + i + 1) - sizeX];
						//neighboursWithTopRow[i][4] = myBoard.get()[(leftTopId + i + 1) - sizeX + 1];
						

						neighboursWithDownRow[i][0] = myBoard.get()[(i + 1) - 1];
						neighboursWithDownRow[i][1] = myBoard.get()[(i + 1) + 1];
						//neighboursWithDownRow[i][2] = myBoard.get()[(i + 1) + sizeX + 1];
						neighboursWithDownRow[i][2] = myBoard.get()[(i + 1) + sizeX];
						//neighboursWithDownRow[i][4] = myBoard.get()[(i + 1) + sizeX - 1];
					
					}  //chyba dziala :)

					for (int i = 0; i < NumberOfCellInColumn; i++)
					{
						neighboursWithLeftColumn[i][0] = myBoard.get()[(sizeX*i + sizeX) + sizeX];
						//neighboursWithLeftColumn[i][1] = myBoard.get()[(sizeX*i + sizeX) + sizeX + 1];
						neighboursWithLeftColumn[i][1] = myBoard.get()[(sizeX*i + sizeX) + 1];
						//neighboursWithLeftColumn[i][3] = myBoard.get()[(sizeX*i + sizeX) - sizeX + 1];
						neighboursWithLeftColumn[i][2] = myBoard.get()[(sizeX*i + sizeX) - sizeX];
				

						neighboursWithRightColumn[i][0] = myBoard.get()[(rightDownId + sizeX*(i + 1)) + sizeX];
						//neighboursWithRightColumn[i][1] = myBoard.get()[(rightDownId + sizeX*(i + 1)) + sizeX - 1];
						neighboursWithRightColumn[i][1] = myBoard.get()[(rightDownId + sizeX*(i + 1)) - 1];
						//neighboursWithRightColumn[i][3] = myBoard.get()[(rightDownId + sizeX*(i + 1)) - sizeX - 1];
						neighboursWithRightColumn[i][2] = myBoard.get()[(rightDownId + sizeX*(i + 1)) - sizeX];
					
					}//wyglada ze dziala
					for (int i = 0, l = 0; l < sizeY - 2; i += sizeX, l++)
					{
						for (int j = sizeX + 1 + i, k = 0; k < sizeX - 2; j++, k++)
						{
							temporaryArray[k + l*(sizeX - 2)] = j;
						}
					}
					for (int i = 0; i < NumberOfCellInTheMiddle; i++)
					{

						//neighboursMiddle[i][0] = myBoard.get()[temporaryArray[i] - sizeX - 1];
						neighboursMiddle[i][0] = myBoard.get()[temporaryArray[i] - sizeX];
						//neighboursMiddle[i][2] = myBoard.get()[temporaryArray[i] - sizeX + 1];
						neighboursMiddle[i][1] = myBoard.get()[temporaryArray[i] + 1];
						//neighboursMiddle[i][4] = myBoard.get()[temporaryArray[i] + sizeX + 1];
						neighboursMiddle[i][2] = myBoard.get()[temporaryArray[i] + sizeX];
						//neighboursMiddle[i][6] = myBoard.get()[temporaryArray[i] + sizeX - 1];
						neighboursMiddle[i][3] = myBoard.get()[temporaryArray[i] - 1];
					}
					for (int i = 0; i < 2; i++)
					{
						vectorZeroId.push_back(neighboursWithLeftDown[i].getCellColor());
						vectorRightDownId.push_back(neighboursWithRightDown[i].getCellColor());
						vectorRightTopId.push_back(neighboursWithRightTop[i].getCellColor());
						vectorLeftTopId.push_back(neighboursWithLeftTop[i].getCellColor());
					}

					arrayWithSum[0] = mostOftenOccured(vectorZeroId);
					arrayWithSum[rightTopId] = mostOftenOccured(vectorRightTopId);
					arrayWithSum[leftTopId] = mostOftenOccured(vectorLeftTopId);
					arrayWithSum[rightDownId] = mostOftenOccured(vectorRightDownId);
					for (int i = 1, j = 0; j < NumberOfCellInRow; i++, j++)
					{
						for (int k = 0; k < 3; k++)
							vectorWithDownRow1D.push_back(neighboursWithDownRow[j][k].getCellColor());
						arrayWithSum[i] = mostOftenOccured(vectorWithDownRow1D);
						vectorWithDownRow1D.clear();
					}
					for (int i = leftTopId + 1, j = 0; j < NumberOfCellInRow; i++, j++)
					{
						for (int k = 0; k < 3; k++)
							vectorWithTopRow1D.push_back(neighboursWithTopRow[j][k].getCellColor());
						arrayWithSum[i] = mostOftenOccured(vectorWithTopRow1D);
						vectorWithTopRow1D.clear();
					}
					for (int i = sizeX, j = 0; j < NumberOfCellInColumn; i += sizeX, j++)
					{
						for (int k = 0; k < 3; k++)
							vectorWithLeftColumn1D.push_back(neighboursWithLeftColumn[j][k].getCellColor());
						arrayWithSum[i] = mostOftenOccured(vectorWithLeftColumn1D);
						vectorWithLeftColumn1D.clear();
					}
					for (int i = rightDownId + sizeX, j = 0; j < NumberOfCellInColumn; i += sizeX, j++)
					{
						for (int k = 0; k < 3; k++)
							vectorWithRightColumn1D.push_back(neighboursWithRightColumn[j][k].getCellColor());
						arrayWithSum[i] = mostOftenOccured(vectorWithRightColumn1D);
						vectorWithRightColumn1D.clear();
					}
					for (int j = 0; j < (sizeX - 2)*(sizeY - 2); j++)
					{
						for (int k = 0; k < 4; k++)
							vectorMiddle1D.push_back(neighboursMiddle[j][k].getCellColor());
						arrayWithSum[temporaryArray[j]] = mostOftenOccured(vectorMiddle1D);
						vectorMiddle1D.clear();
					}

					vectorZeroId.clear();
					vectorRightDownId.clear();
					vectorRightTopId.clear();
					vectorLeftTopId.clear();
					for (int i = 0; i < sizeX*sizeY; i++)
					{
						if (myBoard.get()[i].getCellColor() == 0)
						{
							myBoard.get()[i].setCellColor(arrayWithSum[i]);
						}
					}
				}
				int cntST = 0;
				for (int i = sizeY - 1; i >= 0; i--)
				{
					for (int j = 0; j < sizeX; j++)
					{
						if (myBoard.get()[j + sizeX*i].getCellColor())
						{
							int tmpColor = myBoard.get()[j + sizeX*i].getCellColor();
							window.draw(arrayOfShapes[tmpColor-1]);    //tu pokazuje przy breaku
							arrayOfShapes[tmpColor-1].setPosition((a + 1) * j, (b + 1) * i);
							cntST++;
						}
						else
						{
							window.draw(circleShape2);
							circleShape2.setPosition((a+1) * j, (b+1) * i);
						}
					}
				}
			}
			while (window.pollEvent(event))
			{
				/*cout << "x: " << Mouse::getPosition(window).x << endl;
				cout << "y: " << Mouse::getPosition(window).y << endl;*/
				if (event.type == sf::Event::KeyPressed)
				{
				}
				if (event.type == Event::Closed)
				{
					window.close();
					cntClick = 0;
				}
				if (event.type == sf::Event::TextEntered  && Mouse::getPosition(window).x>=1000 && Mouse::getPosition(window).x<=1120 && Mouse::getPosition(window).y >= 950 && Mouse::getPosition(window).y <= 965)
				{
					if (event.text.unicode == 8) {   // Delete key
						actuallNumberOfStatesfromTextField = actuallNumberOfStatesfromTextField.substr(0, actuallNumberOfStatesfromTextField.size() - 1);
						ACTTEXT.setString(actuallNumberOfStatesfromTextField);
						tmpTF = actuallNumberOfStatesfromTextField;
					}
					else {

						actuallNumberOfStatesfromTextField += event.text.unicode;
						ACTTEXT.setString(actuallNumberOfStatesfromTextField);
						tmpTF = actuallNumberOfStatesfromTextField;
						isFinished = false;
					}
					
				}
				if (event.type == sf::Event::TextEntered  && Mouse::getPosition(window).x >= 1000 && Mouse::getPosition(window).x <= 1120 && Mouse::getPosition(window).y >= 920 && Mouse::getPosition(window).y <= 935)
				{
					if (event.text.unicode == 8) {   // Delete key
						statesInRowsFromTF = statesInRowsFromTF.substr(0, statesInRowsFromTF.size() - 1);
						ROWTEXT.setString(statesInRowsFromTF);
						tmpTFRow = statesInRowsFromTF;
						//cout<<tmpTFRow.size();
				
					}
					else {

						statesInRowsFromTF += event.text.unicode;
						ROWTEXT.setString(statesInRowsFromTF);
						tmpTFRow = statesInRowsFromTF; 
						isFinished = false;
						//cout << tmpTFRow.size();
				}

				}
				if (event.type == sf::Event::TextEntered  && Mouse::getPosition(window).x >= 1000 && Mouse::getPosition(window).x <= 1120 && Mouse::getPosition(window).y >= 890 && Mouse::getPosition(window).y <= 905)
				{
					if (event.text.unicode == 8) {   // Delete key
						
						statesInColumnFromTF = statesInColumnFromTF.substr(0, statesInColumnFromTF.size() - 1);
						COLUMNTEXT.setString(statesInColumnFromTF);
						tmpTFColumn = statesInColumnFromTF;
					}
					else {
						statesInColumnFromTF += event.text.unicode;
						COLUMNTEXT.setString(statesInColumnFromTF);
						tmpTFColumn = statesInColumnFromTF;
						isFinished = false;

					}

				}
				if (event.type == sf::Event::TextEntered  && Mouse::getPosition(window).x >= 900 && Mouse::getPosition(window).x <= 1000 && Mouse::getPosition(window).y >= 978 && Mouse::getPosition(window).y <= 993)
				{
					if (event.text.unicode == 8) {   // Delete key

						radiusFromTF= statesInColumnFromTF.substr(0, radiusFromTF.size() - 1);
						RADIUSTEXT.setString(radiusFromTF);
						tmpTFRadius = radiusFromTF;
					}
					else {
						radiusFromTF += event.text.unicode;
						RADIUSTEXT.setString(radiusFromTF);
						tmpTFRadius = radiusFromTF;
						isFinished = false;

					}

				}
				if (event.type == Event::Closed)
				{
					window.close();
				}
									
				if (event.type == Event::MouseButtonPressed)
				{
					if (event.mouseButton.button == sf::Mouse::Left)
					{
					//START
					//JEDNORODNE
					if (Mouse::getPosition(window).x > ButtonX && Mouse::getPosition(window).x < (windowWidth-buttonWidth) && Mouse::getPosition(window).y >100 && Mouse::getPosition(window).y < (100 + buttonHeight) && !isStarted && (isPeriodic || isAbsorb) && tmpTFColumn.size()>0 && tmpTFRow.size()>0)
					{
						//isFinished = false;
						statesInRow = atoi(tmpTFRow.c_str());
						statesInColumn = atoi(tmpTFColumn.c_str());
						if (statesInColumn <= 0 || statesInRow <= 0)
						{
							cout << "liczba stanow w  kolumnie i wierszu musi byc dodatnia!!!";
						}
						else
						{
							cntClick = statesInColumn*statesInRow + 1;
							int gapRow, gapColumn;
							if (statesInColumn == 1 && statesInRow == 1)
							{
								gapRow = (sizeX / statesInRow) / 2;
								gapColumn = (sizeY / statesInColumn) / 2;
							}
							else
							{
								gapRow = (sizeX / statesInRow);
								gapColumn = (sizeY / statesInColumn);
							}
							isStarted = true;
							vector<int> tmp;
							for (int i = 0; i < statesInColumn*statesInRow; i++)
							{
								arrayOfShapes[i].setFillColor(Color::Color(255 - i * 10, (i + 1) * 15, (i + 1) * 17));
								arrayOfShapes[i].setSize(Vector2f(a, b));
								arrayOfStates[i] = i + 1;
							}
							for (int i = gapColumn / 2, sc = 0; sc < statesInColumn; i += gapColumn, sc++)
							{
								for (int j = gapRow / 2, sr = 0; sr < statesInRow; j += gapRow, sr++)
								{

									numberOfCell = coordinateToNumber(j, i, myBoard, sizeX, sizeY);
									tmp.push_back(numberOfCell);
								}
							}
							for (int i = 0; i < tmp.size(); i++)
							{
								myBoard.get()[tmp[i]].setCellColor(arrayOfStates[i]);
							}
						}
					}
					//PROMIEN
					if (Mouse::getPosition(window).x > ButtonX && Mouse::getPosition(window).x < (windowWidth - buttonWidth) && Mouse::getPosition(window).y >200 && Mouse::getPosition(window).y < (200 + buttonHeight) && !isStarted && (isPeriodic || isAbsorb) && tmpTF.size()>0)
					{
						
						//isFinished = false;
						int randX;
						int randY;
						isStarted = true;
						vector<int> tmp;
						int cnt = 0;
						int sizeR = (int)sqrt(sizeX*sizeY);
						int actuallRadius =atoi(tmpTFRadius.c_str());
						bool isIn = false;
						elementsToRand =  atoi(tmpTF.c_str());
						int* cordRandX = new int[elementsToRand];
						int* cordRandY = new int[elementsToRand];
						if (elementsToRand <= 0)
						{
							cout << "liczba stanow musi byc dodatnia !!!";
						}
						
						else
						{
							//cout << elementsToRand<<" ";
							cntClick = elementsToRand + 1;
							int maxPr, maxCells;
							int* arrayOfPossibleSizeCircles = new int[sizeR];

							for (int i = 0, j = 1; i < sizeR; i++, j += 2)
							{
								arrayOfPossibleSizeCircles[i] = j*j;
							}
							maxPr = (sizeX*sizeY) / elementsToRand;
							/*for (int i = 0; i < sizeR; i++)
							{
								if (arrayOfPossibleSizeCircles[i] < maxPr)
									cnt++;
							}

							if (cnt != 0)
							{
								maxCells = arrayOfPossibleSizeCircles[cnt - 1];

								maxR = sqrt(maxCells) - cnt - 1;

							}

							else
							{
								maxR = 0;

							}*/

							for (int i = 0; i < elementsToRand; i++)
							{
								arrayOfShapes[i].setFillColor(Color::Color(255 - i * 10, (i + 1) * 15, (i + 1) * 17));
								arrayOfShapes[i].setSize(Vector2f(a, b));
								arrayOfStates[i] = i + 1;
							}
						
							bool canGiveState = true;
							vector<int> coordinatesX;
							vector<int> coordinatesY;
							for (int i = 0; i < sizeY; i++)
							{
								for (int j = 0; j < sizeX; j++)
								{
									myBoard[j + sizeX*i].setBusyFalse();
								}
							}
							int numberOfRandom = 0;
							vector<int> distanceBetweenPoints;
							for (int i = 0; i < sizeX; i++)
							{
								coordinatesX.push_back(myBoard[i].getCoordinateX());
								coordinatesY.push_back(myBoard[i].getCoordinateY());
							}
							for (int i = rightDownId; i < rightTopId+1; i += sizeX)
							{
								coordinatesX.push_back(myBoard[i].getCoordinateX());
								coordinatesY.push_back(myBoard[i].getCoordinateY());
							}
							for (int i = 0; i < leftTopId; i += sizeX)
							{
								coordinatesX.push_back(myBoard[i].getCoordinateX());
								coordinatesY.push_back(myBoard[i].getCoordinateY());
							}
							for (int i = leftTopId; i < rightTopId+1; i++)
							{
								coordinatesX.push_back(myBoard[i].getCoordinateX());
								coordinatesY.push_back(myBoard[i].getCoordinateY());
							}
							//cout << coordinatesX.size() << " ";            279  
							for (int it = 0; it < elementsToRand && numberOfRandom<(sizeX*sizeY*2); it++)
							{  //zdarzy sie nie wykorzystac pelnego miejsca ale jest akceptowalne jak najbardziej 
								canGiveState = true;
								numberOfRandom++;
								//cout << "it "<<it<<endl;
								randX = (rand() % sizeX - 1) + 1;
								randY = (rand() % sizeY - 1) + 1;
								//if (it > 0)
								//	isIn = isInside(cordRandX, cordRandY, randX, randY, elementsToRand);
								//if (isIn == false)
								//{
									//cordRandX[it] = randX;
									//cordRandY[it] = randY;
									//cout << "X: "<< cordRandX[it] << endl;
									//cout << "Y: " << cordRandY[it] << endl;
									numberOfCell = coordinateToNumber(randX, randY, myBoard, sizeX, sizeY);
									coordinatesX.push_back(myBoard[numberOfCell].getCoordinateX());
									coordinatesY.push_back(myBoard[numberOfCell].getCoordinateY());
									for (int i = 0; i < coordinatesX.size() - 1; i++)
									{
										distanceBetweenPoints.push_back(sqrt(pow((coordinatesX[coordinatesX.size() - 1] - coordinatesX[i]), 2) +
											pow((coordinatesY[coordinatesY.size() - 1] - coordinatesY[i]), 2)));  //odleglosci miedzy punktem ostatnio wylosowanym a reszta
									}
									//cout<<coordinatesX.size()<<" "<<endl;
									for (int i = 0; i < distanceBetweenPoints.size(); i++)
									{
										if (distanceBetweenPoints[i] < actuallRadius+1)
										{
											canGiveState = false;
										}
									}
									if (canGiveState) {
										myBoard.get()[numberOfCell].setCellColor(arrayOfStates[it]);
									}
									else
									{
										it--;
										coordinatesX.pop_back();
										coordinatesY.pop_back();
										
									}
									distanceBetweenPoints.clear();
								//}
								//else it--;
								
							}
							coordinatesX.clear();
							coordinatesY.clear();
							
						}
						
					}
					
					//WYKLIKANIE
					if ( (isPeriodic || isAbsorb)&&  Mouse::getPosition(window).y >= 0 && Mouse::getPosition(window).y <= sizeY*(b + 1) && Mouse::getPosition(window).x >= 0 && Mouse::getPosition(window).x <= sizeX*(a + 1))
					{
						
						//isFinished = false;
						int tmpX, tmpY;
						
						cntClick++;
						

							arrayOfShapes[cntClick].setFillColor(Color::Color(255 - cntClick * 10, (cntClick + 1) * 15, (cntClick + 1) * 17));
							//arrayOfShapes[i].setFillColor(colors[i]);
							arrayOfShapes[cntClick].setSize(Vector2f(a, b));
							arrayOfStates[cntClick] = cntClick + 1;
						
						for (int i = 1; i <= sizeY*(b + 1); i = i + b + 1)
						{
							for (int j = 1; j <= sizeX*(a + 1); j = j + a + 1)
							{
								if (Mouse::getPosition(window).x >= j && Mouse::getPosition(window).x <= j + a + 1 && Mouse::getPosition(window).y >= i && Mouse::getPosition(window).y <= i + b + 1)
								{
									isStarted = true;
									tmpX = j / (a + 1);
									tmpY = i / (b + 1);
									
									
								}
								/*if (Mouse::getPosition(window).x > ButtonX && Mouse::getPosition(window).x < (windowWidth + buttonWidth) && Mouse::getPosition(window).y >0 && Mouse::getPosition(window).y < (0 + buttonHeight) && isStarted)
								{
									selectorPlay = true;
								}*/

							}
						}
						
						numberOfCell = coordinateToNumber(tmpX, tmpY, myBoard, sizeX, sizeY);
						if (!(myBoard.get()[numberOfCell].getCellColor()))
							myBoard.get()[numberOfCell].setCellColor(arrayOfStates[cntClick]);
						else myBoard.get()[numberOfCell].setCellColor(0);
						
						
						
					}
					//RANDOM
					if (Mouse::getPosition(window).x > ButtonX && Mouse::getPosition(window).x < (windowWidth -buttonWidth) && Mouse::getPosition(window).y >400 && Mouse::getPosition(window).y < (400 + buttonHeight) && !isStarted && !wasStartedAtLeastOne && (isPeriodic || isAbsorb))
					{
						//isFinished = false;
						isStarted = true;
						int NumberOfCellsToAlive = 0;
						int it=0;
						int isPermX, isPermY;
						bool isIn = false;
						
						actuallNumberOfStates = atoi(tmpTF.c_str());
						if (actuallNumberOfStates <= 0)
						{
							cout << "liczba stanow musi byc dodatnia!!!";
						}
						else
						{
							cntClick = actuallNumberOfStates + 1;
							for (int i = 0; i < actuallNumberOfStates; i++)

							{

								arrayOfShapes[i].setFillColor(Color::Color(255 - i * 10, (i + 1) * 15, (i + 1) * 17));
								//arrayOfShapes[i].setFillColor(colors[i]);
								arrayOfShapes[i].setSize(Vector2f(a, b));
								arrayOfStates[i] = i + 1;
							}
							NumberOfCellsToAlive = actuallNumberOfStates;
							int* cordRandX = new int[NumberOfCellsToAlive];
							int* cordRandY = new int[NumberOfCellsToAlive];
							for (int i = 0; i < NumberOfCellsToAlive; i++)
							{
								isPermX = (rand() % sizeX - 1) + 1;
								isPermY = (rand() % sizeY - 1) + 1;
								if (i > 0)
									isIn = isInside(cordRandX, cordRandY, isPermX, isPermY, NumberOfCellsToAlive);
								if (isIn == false)
								{
									cordRandX[i] = isPermX;
									cordRandY[i] = isPermY;
									
								}
								else
								{
									i--;   
								}

							}
							while (it < NumberOfCellsToAlive)
							{
								//for (int i = 0; i < actuallNumberOfStates; i++)

								numberOfCell = coordinateToNumber(cordRandX[it], cordRandY[it], myBoard, sizeX, sizeY);
								//cout << arrayOfStates[it] << " ";
								myBoard.get()[numberOfCell].setCellColor(arrayOfStates[it]);
								//cout<<myBoard.get()[numberOfCell].getCellColor()<<" ";


								if (it == NumberOfCellsToAlive - 1)
								{
									int cntST = 0;
									for (int i = sizeY - 1; i >= 0; i--)
									{
										for (int j = 0; j < sizeX; j++)
										{

											if (myBoard.get()[j + sizeX*i].getCellColor())
											{
												//cout << cntST << " ";
												int tmpColor = myBoard.get()[j + sizeX*i].getCellColor();

												window.draw(arrayOfShapes[tmpColor - 1]);
												arrayOfShapes[tmpColor - 1].setPosition((a + 1) * j, (b + 1) * i);
												//cout << myBoard.get()[j + sizeX*i].getCellColor() << " ";
												cntST++;

											}
											else
											{
												window.draw(circleShape2);
												circleShape2.setPosition((a + 1) * j, (b + 1) * i);
											}
										}
									}
								}
								it++;
							}
						}
						

					}
					//PERODYCZNE
					if (Mouse::getPosition(window).x > ButtonX && Mouse::getPosition(window).x < (windowWidth - buttonWidth+60) && Mouse::getPosition(window).y >700 && Mouse::getPosition(window).y < (700 + buttonHeight) )
					{
						isFinished = false;
						isPeriodic = true;
					}
					//ABSORBUJACE
					if (Mouse::getPosition(window).x >ButtonX && Mouse::getPosition(window).x < (windowWidth - buttonWidth+60) && Mouse::getPosition(window).y >800 && Mouse::getPosition(window).y < (800 + buttonHeight) )
					{
						isFinished = false;
						isAbsorb = true;

					}
					//PLAY
					if (Mouse::getPosition(window).x > ButtonX && Mouse::getPosition(window).x < (windowWidth - buttonWidth) && Mouse::getPosition(window).y >0 && Mouse::getPosition(window).y < (0 + buttonHeight) && isStarted && (isPeriodic || isAbsorb))
					{
						
						selectorPlay = true;
						
					}
					//PLUS SIZE X
					if (Mouse::getPosition(window).x > ButtonX+175 && Mouse::getPosition(window).x < (windowWidth) && Mouse::getPosition(window).y >0 && Mouse::getPosition(window).y < (0 + (buttonHeight/2) *4)  && isFinished)
					{
						if (Mouse::getPosition(window).x > ButtonX + 175 && Mouse::getPosition(window).x < (windowWidth) && Mouse::getPosition(window).y >0 && Mouse::getPosition(window).y < (0 + buttonHeight / 2))
							sizeX++;
						if (Mouse::getPosition(window).x > ButtonX + 175 && Mouse::getPosition(window).x < (windowWidth) && Mouse::getPosition(window).y >0 + 20 && Mouse::getPosition(window).y < (0 + buttonHeight / 2) + 20)
							sizeX--;
						if (Mouse::getPosition(window).x > ButtonX + 175 && Mouse::getPosition(window).x < (windowWidth) && Mouse::getPosition(window).y >+40 && Mouse::getPosition(window).y < (0 + buttonHeight / 2 + 40))
							sizeY++;
						if (Mouse::getPosition(window).x > ButtonX + 175 && Mouse::getPosition(window).x < (windowWidth) && Mouse::getPosition(window).y >0 + 60 && Mouse::getPosition(window).y < (0 + buttonHeight / 2) + 60)
							sizeY--;
						numberOfMaxStates = (sizeX) * (sizeY);
						arrayOfStates = new int[numberOfMaxStates];
						arrayOfShapes = new RectangleShape[numberOfMaxStates];
						for (int i = 0; i < numberOfMaxStates; i++)
						{
							arrayOfStates[i] = 0;
							arrayOfShapes[i] = circleShape2;     //stan pusty
						}

						myBoard = make_unique<Cell[]>(sizeX*sizeY);


						for (int i = 0; i <sizeY; i++)
						{
							for (int j = 0; j < sizeX; j++)
							{
								myBoard.get()[j + sizeX*i].setCellColor(0);
								myBoard.get()[j + sizeX*i].setCoordinateX(j);
								myBoard.get()[j + sizeX*i].setCoordinateY(i);
							}
						}
						NumberOfCellInRow = sizeX - 2;
						NumberOfCellInColumn = sizeY - 2;
						neighboursWithTopRow = make_unique< unique_ptr<Cell[]>[] >(NumberOfCellInRow);
						neighboursWithDownRow = make_unique< unique_ptr<Cell[]>[] >(NumberOfCellInRow);
						neighboursWithLeftColumn = make_unique< unique_ptr<Cell[]>[] >(NumberOfCellInColumn);
						neighboursWithRightColumn = make_unique< unique_ptr<Cell[]>[] >(NumberOfCellInColumn);
						NumberOfCellInTheMiddle = (sizeX - 2)*(sizeY - 2);
						
					
						neighboursMiddle = make_unique< unique_ptr<Cell[]>[] >(NumberOfCellInTheMiddle);
						temporaryArray = make_unique<int[]>(NumberOfCellInTheMiddle);
						arrayWithSum = make_unique<int[]>(sizeX*sizeY);
						rightDownId = sizeX - 1;
						rightTopId = sizeX*sizeY - 1;
						leftTopId = rightTopId - sizeX + 1;

					}
					//STOP
					if (Mouse::getPosition(window).x > ButtonX && Mouse::getPosition(window).x < (windowWidth - buttonWidth) && Mouse::getPosition(window).y >600 && Mouse::getPosition(window).y < (600 + buttonHeight) && selectorPlay)
					{
						selectorPlay = false;
						}

						//FINISH
						if (Mouse::getPosition(window).x > ButtonX && Mouse::getPosition(window).x < (windowWidth -buttonWidth) && Mouse::getPosition(window).y >500 && Mouse::getPosition(window).y < (500 + buttonHeight))
						{
							for (int i = 0; i < sizeX*sizeY; i++)
							{
								myBoard.get()[i].setCellColor(0);
							}
							isStarted = false;
							selectorPlay = false;
							wasStartedAtLeastOne = false;
							isPeriodic = false;
							isAbsorb = false;
							cntClick = 0;
							isFinished = true;
							
						}
					}
				}
			}
			window.display();
	}
	system("pause");
}