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
int coordinateToNumber(int, int, shared_ptr<Cell>,int,int);
vector<vector<char>> decompress(vector<vector<char>>);
int coordinateToNumber(int coordinateX, int coordinateY, shared_ptr<Cell> arr,int sX,int sY)
{
	int i = 0;
	while (!(arr.get()[i].getCoordinateX() == coordinateX && arr.get()[i].getCoordinateY() == coordinateY) && i<sX*sY)
	{
		//cout << "i :" << i;
		i++;
	}
	return  i;
}
vector<string> decompressVec(vector<vector<char>> &vec)
{
	vector<string> vecSTR;
	vector<string> vecSTR2;
	vecSTR.reserve(vec.size());
	int ich,counter;
	int sizeVect;
	vector<int> tmp;
	int cnt = 0;
	int is;
	for (auto const & v : vec)
		vecSTR.emplace_back(v.begin(), v.end());  //vector char[] ->vector string
	for (int i = 0; i < vecSTR.size(); i++)
	{
		sizeVect = vecSTR[i].size();
		for (int j = 0 ; j <sizeVect; j=j++)
		{
			//counter = 0;
			if (isdigit(vecSTR[i][j]) )
			{
				if (j == sizeVect-1)
				{
					vecSTR[i].push_back(vecSTR[i][j]);
					is = stoi(string(1, vecSTR[i][j]));
					for (int l = 0;l < is-1; l++)
					{
						tmp.push_back(i);
						cnt++;
					}
				}
				else
				{
					ich = stoi(string(1, vecSTR[i][j]));
					//ich = static_cast<int>(vecSTR[0][j]);

					//cout << "ich " << ich << endl;
					//cout << "vector " << vecSTR[0][j]<<endl;
					for (int k = 0; k < ich; k++)
					{
						//vecSTR[0][j] += vecSTR[0][j + 1];

						vecSTR[i].push_back(vecSTR[i][j + 1]);
						//counter++;
					}
				}
					
			}
			else
			{
				//wchodzi i sie nie konczy
				if (j == 0 || !isdigit(vecSTR[i][j - 1]))
				{
					vecSTR[i].push_back(vecSTR[i][j]);
				}
				else {
					
				}
			}
		}
		for (int l = sizeVect, m = 0; m < vecSTR[i].size(); l++, m++) {
			if (m < vecSTR[i].size() - sizeVect)
				vecSTR[i][m] = vecSTR[i][l];
			else
				vecSTR[i][m] = 0;
		}
	}
	for (int i = 0; i < tmp.size(); i++)
	{
		vecSTR.insert(vecSTR.begin() + tmp[i]+i+1, "bbbb");
	}
	return vecSTR;
}
/*void printBoard(vector<vector<bool>>& vectorToDisp, CircleShape shapeAlive, CircleShape shapeDead ,int sizFunX,int sizFunY,RenderWindow &winFun , int cntBtn)
{
	for (int i = sizFunY- 1; i >= 0; i--)
	{
		for (int j = 0; j < sizFunX; j++)
		{
			if (vectorToDisp[cntBtn - 1][j + sizFunX*i] == true)
			{
				winFun.draw(shapeAlive);
				shapeAlive.setPosition(20 * j, 20 * i);
			}
			else
			{
				winFun.draw(shapeDead);
				shapeDead.setPosition(20 * j, 20 * i);
			}
		}
	}
}*/

int main()
{
	srand(time(NULL));
	unsigned int windowWidth = 1200;
	unsigned int windowHeight = 1000;
	RenderWindow window = { VideoMode{windowWidth,windowHeight} , "GAME OF LIFE" };
	window.setFramerateLimit(20); 
	float a = 8, b = 8;
	RectangleShape circleShape1(Vector2f(a, b));
	RectangleShape circleShape2(Vector2f(a, b));
	circleShape1.setFillColor(Color::Green);
	circleShape2.setFillColor(Color::Red);
	Font font;
	if (!font.loadFromFile("arial.ttf"))
	{
		cout << "error loading a file " << endl;
		system("pause");
	}
	Text FPSLabel;
	
	int ButtonX = windowWidth - 200;
	//int firstButtonY = 100;
	int buttonWidth = 100;
	int buttonHeight = 40;
	Button button1(ButtonX, 100, buttonWidth, buttonHeight,font, "OSCYLATOR", Color::Blue);
	Button button4(ButtonX, 200, buttonWidth, buttonHeight, font, "GLIDER", Color::Blue);
	Button button2(ButtonX, 300, buttonWidth, buttonHeight, font, "NIEZMIENNIK", Color::Blue);
	Button button3(ButtonX, 500, buttonWidth, buttonHeight, font, "FINISH", Color::Blue);
	Button button6(ButtonX, 400, buttonWidth, buttonHeight, font, "RANDOM", Color::Blue);
	Button button7(ButtonX, 0, buttonWidth, buttonHeight, font, "PLAY", Color::Blue);
	Button button8(ButtonX, 600, buttonWidth, buttonHeight, font, "STOP", Color::Blue);
	vector<string> patternFromFile;
	string iter;
	boost::property_tree::ptree pt;
	fs::path p{ "hardPattern.rle" };
	//fs::path p2{ "hardPattern2.rle" };    do ewentualnego zapisu
	std::ifstream ifs(p);
	//std::ofstream ofs(p2);
	while (!ifs.eof())
	{
		ifs >> iter;
		patternFromFile.push_back(iter);
	} //czyli mam w wektorze  caly string z pliku
	/*for (int i = 0; i < patternFromFile.size(); i++)
	{
		cout << patternFromFile[i] << endl;
	}*/
	//przydaloby sie poodzielac te stringi zeby kazda linijka planszy to byl oddzielny element tablicy
	vector<char> dividedByDollar;
	vector<vector<char>> dividedByDollar2;
	string concatenedPatternFromFile=" ";
	for (int i = 0; i < patternFromFile.size(); i++)
	{
		concatenedPatternFromFile += patternFromFile[i];
	}
	//cout << concatenedPatternFromFile << endl;
	//cout<<"rozmiar :"<<concatenedPatternFromFile.size() << endl;
	int j = 1;
	int counter;
	for (int i = 1; concatenedPatternFromFile[i]; i=i+counter+1)
	{
		
		counter = 0;
		while(concatenedPatternFromFile[j] != '$' && concatenedPatternFromFile[j] != '!' && concatenedPatternFromFile[j])
		{
			
			dividedByDollar.push_back(concatenedPatternFromFile[j]);
			j ++ ;
			
			counter++;
		}
		j++;
		dividedByDollar2.push_back(dividedByDollar);
		dividedByDollar.clear();
	}
	
	vector<string> decompressedVector = decompressVec(dividedByDollar2);   //wreszcie to mam!
	/*for (int i = 0; i < decompressedVector.size(); i++)
	{
		cout << decompressedVector[i] << endl;
	}*/
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
	shared_ptr<Cell> myBoard(new Cell[sizeX*sizeY],default_delete<Cell[]>());
	 for (int i = 0; i <sizeY; i++)
	{
		for (int j = 0; j < sizeX; j++)
		{
			myBoard.get()[j + sizeX*i].setCellDead();
			myBoard.get()[j+sizeX*i].setCoordinateX(j);
			myBoard.get()[j+sizeX*i].setCoordinateY(i);
		}
	}
	//MUSZE JAKOS TO WYPISAC
	//przed setupem
	Event event;
	
		int is;
		for (int i = 0; i < decompressedVector.size(); i++)
		{
			for (int j = 0; j < decompressedVector[i].size(); j++)
			{
				if (decompressedVector[i][j] == 'o')
				{

					cordXVec.push_back(j);
					cordYVec.push_back(i);
				}
			}
		}
			cordXs[0] = pt.get<std::string>("Section1.cordX1");
			cordYs[0] = pt.get<std::string>("Section1.cordY1");
			cordXs[1] = pt.get<std::string>("Section1.cordX2");
			cordYs[1] = pt.get<std::string>("Section1.cordY2");
			cordXs[2] = pt.get<std::string>("Section1.cordX3");
			cordYs[2] = pt.get<std::string>("Section1.cordY3");
			cordXs[3] = pt.get<std::string>("Section1.cordX4");
			cordYs[3] = pt.get<std::string>("Section1.cordY4");
			cordXs[4] = pt.get<std::string>("Section1.cordX5");
			cordYs[4] = pt.get<std::string>("Section1.cordY5");
	
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
		for (int i = 0; i < NumberOfCellInRow; i++)
		{
			smartPtr1D = make_unique<Cell[]>(8);
			DownRow1D = make_unique<Cell[]>(8);
		
			neighboursWithTopRow[i] = move(smartPtr1D);
			neighboursWithDownRow[i] = move(DownRow1D);
		}
		for (int i = 0; i < NumberOfCellInColumn; i++)
		{
			Left1D = make_unique<Cell[]>(8);
			Right1D = make_unique<Cell[]>(8);
			neighboursWithLeftColumn[i] = move(Left1D);
			neighboursWithRightColumn[i] = move(Right1D);
		}
		int NumberOfCellInTheMiddle = (sizeX - 2)*(sizeY - 2);
		unique_ptr<unique_ptr<Cell[]>[]>    neighboursMiddle;
		unique_ptr<Cell[]>                   middle1D;
		neighboursMiddle = make_unique< unique_ptr<Cell[]>[] >(NumberOfCellInTheMiddle);
		for (int i = 0; i < NumberOfCellInTheMiddle; i++)
		{
			middle1D = make_unique<Cell[]>(8);

			neighboursMiddle[i] = move(middle1D);
		}
		unique_ptr<int[]> temporaryArray;
		temporaryArray = make_unique<int[]>(NumberOfCellInTheMiddle);
		unique_ptr<int[]> arrayWithSum;
		arrayWithSum = make_unique<int[]>(sizeX*sizeY);
		bool selectorMain = true;
		int rightDownId = sizeX - 1;
		int rightTopId = sizeX*sizeY - 1;
		int leftTopId = rightTopId - sizeX + 1;
		bool isStarted = false;
		bool IsDidAnyTurn = false;
		bool selectorPlay = false;
		
		//TU BEDZIE POCZATEK PETLI Z ITERACJAMI
		while (window.isOpen())
		{
			window.clear(Color::Black);
			window.draw(button1);
			window.draw(button2);
			window.draw(button3);
			window.draw(button4);
			window.draw(button7);
			window.draw(button8);
			window.draw(button6);
			
					for (int i = sizeY - 1; i >= 0; i--)
					{
						for (int j = 0; j < sizeX; j++)
						{
							if (myBoard.get()[j + sizeX*i].IsAlive())
							{
								window.draw(circleShape1);
								circleShape1.setPosition((a+1) * j, (b+1) * i);
							}
							else
							{
								window.draw(circleShape2);
								circleShape2.setPosition((a+1) * j, (b+1) * i);
							}
						}
					}
			if (selectorPlay && isStarted)
			{
	
				neighboursWithLeftDown[0] = myBoard.get()[1];
				neighboursWithLeftDown[1] = myBoard.get()[sizeX];
				neighboursWithLeftDown[2] = myBoard.get()[sizeX + 1];
				neighboursWithLeftDown[3] = myBoard.get()[rightDownId];
				neighboursWithLeftDown[4] = myBoard.get()[rightDownId + sizeX];
				neighboursWithLeftDown[5] = myBoard.get()[leftTopId];
				neighboursWithLeftDown[6] = myBoard.get()[leftTopId+1];
				neighboursWithLeftDown[7] = myBoard.get()[rightTopId];

				neighboursWithRightDown[0] = myBoard.get()[rightDownId - 1];
				neighboursWithRightDown[1] = myBoard.get()[rightDownId + sizeX];
				neighboursWithRightDown[2] = myBoard.get()[rightDownId + sizeX - 1];
				neighboursWithRightDown[3] = myBoard.get()[0];
				neighboursWithRightDown[4] = myBoard.get()[sizeX];
				neighboursWithRightDown[5] = myBoard.get()[rightTopId];
				neighboursWithRightDown[6] = myBoard.get()[rightTopId-1];
				neighboursWithRightDown[7] = myBoard.get()[leftTopId];

				neighboursWithRightTop[0] = myBoard.get()[rightTopId - 1];
				neighboursWithRightTop[1] = myBoard.get()[rightTopId - sizeX];
				neighboursWithRightTop[2] = myBoard.get()[rightTopId - 1 - sizeX];
				neighboursWithRightTop[3] = myBoard.get()[leftTopId];
				neighboursWithRightTop[4] = myBoard.get()[leftTopId - sizeX];
				neighboursWithRightTop[5] = myBoard.get()[rightDownId];
				neighboursWithRightTop[6] = myBoard.get()[rightDownId-1];
				neighboursWithRightTop[7] = myBoard.get()[0];

				neighboursWithLeftTop[0] = myBoard.get()[leftTopId + 1];
				neighboursWithLeftTop[1] = myBoard.get()[leftTopId - sizeX];
				neighboursWithLeftTop[2] = myBoard.get()[leftTopId + 1 - sizeX];
				neighboursWithLeftTop[3] = myBoard.get()[rightTopId];
				neighboursWithLeftTop[4] = myBoard.get()[rightTopId - sizeX];
				neighboursWithLeftTop[5] = myBoard.get()[0];
				neighboursWithLeftTop[6] = myBoard.get()[1];
				neighboursWithLeftTop[7] = myBoard.get()[rightDownId];
				//CZAS NA PASY
				//najpierw musze wyznaczyc ilosc komorek w kolumnie
				for (int i = 0; i < NumberOfCellInRow; i++)
				{
					neighboursWithTopRow[i][0] = myBoard.get()[(leftTopId + i + 1) - 1];
					neighboursWithTopRow[i][1] = myBoard.get()[(leftTopId + i + 1) + 1];
					neighboursWithTopRow[i][2] = myBoard.get()[(leftTopId + i + 1) - sizeX - 1];
					neighboursWithTopRow[i][3] = myBoard.get()[(leftTopId + i + 1) - sizeX];
					neighboursWithTopRow[i][4] = myBoard.get()[(leftTopId + i + 1) - sizeX + 1];
					//nowe do przerobienia
					neighboursWithTopRow[i][5] = myBoard.get()[0 + i];
					neighboursWithTopRow[i][6] = myBoard.get()[1+i];
					neighboursWithTopRow[i][7] = myBoard.get()[2+i];

					neighboursWithDownRow[i][0] = myBoard.get()[(i + 1) - 1];
					neighboursWithDownRow[i][1] = myBoard.get()[(i + 1) + 1];
					neighboursWithDownRow[i][2] = myBoard.get()[(i + 1) + sizeX + 1];
					neighboursWithDownRow[i][3] = myBoard.get()[(i + 1) + sizeX];
					neighboursWithDownRow[i][4] = myBoard.get()[(i + 1) + sizeX - 1];
					//nowe do prerobienia
					neighboursWithDownRow[i][5] = myBoard.get()[leftTopId+i];
					neighboursWithDownRow[i][6] = myBoard.get()[leftTopId+1+i];
					neighboursWithDownRow[i][7] = myBoard.get()[leftTopId+2+i];
				}  //chyba dziala :)

				for (int i = 0; i < NumberOfCellInColumn; i++)
				{
					neighboursWithLeftColumn[i][0] = myBoard.get()[(sizeX*i + sizeX) + sizeX];
					neighboursWithLeftColumn[i][1] = myBoard.get()[(sizeX*i + sizeX) + sizeX + 1];
					neighboursWithLeftColumn[i][2] = myBoard.get()[(sizeX*i + sizeX) + 1];
					neighboursWithLeftColumn[i][3] = myBoard.get()[(sizeX*i + sizeX) - sizeX + 1];
					neighboursWithLeftColumn[i][4] = myBoard.get()[(sizeX*i + sizeX) - sizeX];
					//nowe
					neighboursWithLeftColumn[i][5] = myBoard.get()[rightDownId + sizeX*i + sizeX];
					neighboursWithLeftColumn[i][6] = myBoard.get()[(rightDownId + sizeX*(i + 1)) + sizeX];
					neighboursWithLeftColumn[i][7] = myBoard.get()[(rightDownId + sizeX*(i + 1)) - sizeX];

					neighboursWithRightColumn[i][0] = myBoard.get()[(rightDownId + sizeX*(i + 1)) + sizeX];
					neighboursWithRightColumn[i][1] = myBoard.get()[(rightDownId + sizeX*(i + 1)) + sizeX - 1];
					neighboursWithRightColumn[i][2] = myBoard.get()[(rightDownId + sizeX*(i + 1)) - 1];
					neighboursWithRightColumn[i][3] = myBoard.get()[(rightDownId + sizeX*(i + 1)) - sizeX - 1];
					neighboursWithRightColumn[i][4] = myBoard.get()[(rightDownId + sizeX*(i + 1)) - sizeX];
					//nowe
					neighboursWithRightColumn[i][5] = myBoard.get()[i*sizeX + sizeX];
					neighboursWithRightColumn[i][6] = myBoard.get()[(sizeX*i + sizeX) + sizeX];
					neighboursWithRightColumn[i][7] = myBoard.get()[(sizeX*i + sizeX) - sizeX];
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

					neighboursMiddle[i][0] = myBoard.get()[temporaryArray[i] - sizeX - 1];
					neighboursMiddle[i][1] = myBoard.get()[temporaryArray[i] - sizeX];
					neighboursMiddle[i][2] = myBoard.get()[temporaryArray[i] - sizeX + 1];
					neighboursMiddle[i][3] = myBoard.get()[temporaryArray[i] + 1];
					neighboursMiddle[i][4] = myBoard.get()[temporaryArray[i] + sizeX + 1];
					neighboursMiddle[i][5] = myBoard.get()[temporaryArray[i] + sizeX];
					neighboursMiddle[i][6] = myBoard.get()[temporaryArray[i] + sizeX - 1];
					neighboursMiddle[i][7] = myBoard.get()[temporaryArray[i] - 1];
				}
				arrayWithSum[0] = neighboursWithLeftDown[0].IsAlive() + neighboursWithLeftDown[1].IsAlive() + neighboursWithLeftDown[2].IsAlive() + neighboursWithLeftDown[3].IsAlive() + neighboursWithLeftDown[4].IsAlive() + neighboursWithLeftDown[5].IsAlive() + neighboursWithLeftDown[6].IsAlive() + neighboursWithLeftDown[7].IsAlive();
				arrayWithSum[rightDownId] = neighboursWithRightDown[0].IsAlive() + neighboursWithRightDown[1].IsAlive() + neighboursWithRightDown[2].IsAlive() + neighboursWithRightDown[3].IsAlive() + neighboursWithRightDown[4].IsAlive() + neighboursWithRightDown[5].IsAlive() + neighboursWithRightDown[6].IsAlive() + neighboursWithRightDown[7].IsAlive();
				arrayWithSum[rightTopId] = neighboursWithRightTop[0].IsAlive() + neighboursWithRightTop[1].IsAlive() + neighboursWithRightTop[2].IsAlive() + neighboursWithRightTop[3].IsAlive() + neighboursWithRightTop[4].IsAlive() + neighboursWithRightTop[5].IsAlive() + neighboursWithRightTop[6].IsAlive() + neighboursWithRightTop[7].IsAlive();
				arrayWithSum[leftTopId] = neighboursWithLeftTop[0].IsAlive() + neighboursWithLeftTop[1].IsAlive() + neighboursWithLeftTop[2].IsAlive() + neighboursWithLeftTop[3].IsAlive() + neighboursWithLeftTop[4].IsAlive() + neighboursWithLeftTop[5].IsAlive() + neighboursWithLeftTop[6].IsAlive() + neighboursWithLeftTop[7].IsAlive();  //oki to chyba dziala
				for (int i = 1, j = 0; j < NumberOfCellInRow; i++, j++)
				{
					arrayWithSum[i] = neighboursWithDownRow[j][0].IsAlive() + neighboursWithDownRow[j][1].IsAlive() + neighboursWithDownRow[j][2].IsAlive() + neighboursWithDownRow[j][3].IsAlive() + neighboursWithDownRow[j][4].IsAlive() + neighboursWithDownRow[j][5].IsAlive() + neighboursWithDownRow[j][6].IsAlive() + neighboursWithDownRow[j][7].IsAlive();
				}
				for (int i = leftTopId + 1, j = 0; j < NumberOfCellInRow; i++, j++)
				{
					arrayWithSum[i] = neighboursWithTopRow[j][0].IsAlive() + neighboursWithTopRow[j][1].IsAlive() + neighboursWithTopRow[j][2].IsAlive() + neighboursWithTopRow[j][3].IsAlive() + neighboursWithTopRow[j][4].IsAlive() + neighboursWithTopRow[j][5].IsAlive() + neighboursWithTopRow[j][6].IsAlive() + neighboursWithTopRow[j][7].IsAlive();
				}
				for (int i = sizeX, j = 0; j < NumberOfCellInColumn; i += sizeX, j++)
				{
					arrayWithSum[i] = neighboursWithLeftColumn[j][0].IsAlive() + neighboursWithLeftColumn[j][1].IsAlive() + neighboursWithLeftColumn[j][2].IsAlive() + neighboursWithLeftColumn[j][3].IsAlive() + neighboursWithLeftColumn[j][4].IsAlive() + neighboursWithLeftColumn[j][5].IsAlive() + neighboursWithLeftColumn[j][6].IsAlive() + neighboursWithLeftColumn[j][7].IsAlive();
				}
				for (int i = rightDownId + sizeX, j = 0; j < NumberOfCellInColumn; i += sizeX, j++)
				{
					arrayWithSum[i] = neighboursWithRightColumn[j][0].IsAlive() + neighboursWithRightColumn[j][1].IsAlive() + neighboursWithRightColumn[j][2].IsAlive() + neighboursWithRightColumn[j][3].IsAlive() + neighboursWithRightColumn[j][4].IsAlive() + neighboursWithRightColumn[j][5].IsAlive() + neighboursWithRightColumn[j][6].IsAlive() + neighboursWithRightColumn[j][7].IsAlive();
				}
				for (int j = 0; j < (sizeX - 2)*(sizeY - 2); j++)
				{
					arrayWithSum[temporaryArray[j]] = neighboursMiddle[j][0].IsAlive() + neighboursMiddle[j][1].IsAlive() + neighboursMiddle[j][2].IsAlive() + neighboursMiddle[j][3].IsAlive() + neighboursMiddle[j][4].IsAlive() + neighboursMiddle[j][5].IsAlive() + neighboursMiddle[j][6].IsAlive() + neighboursMiddle[j][7].IsAlive();
				}
				
				//TERAZ WARUNKI
				for (int i = 0; i < sizeX*sizeY; i++)
				{
					if (!(myBoard.get()[i].IsAlive()))
					{
						if (arrayWithSum[i] == 3)
						{
							myBoard.get()[i].setCellAlive();
						}
					}
					else
					{
						if (arrayWithSum[i] < 2 || arrayWithSum[i] > 3)
						{
							myBoard.get()[i].setCellDead();
						}
					}
				}
				
				for (int i = sizeY - 1; i >= 0; i--)
				{
					for (int j = 0; j < sizeX; j++)
					{
						if (myBoard.get()[j + sizeX*i].IsAlive())
						{
							window.draw(circleShape1);
							circleShape1.setPosition((a+1) * j, (b+1) * i);
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
				if (event.type == Event::Closed)
				{
					window.close();
				}
									
				if (event.type == Event::MouseButtonPressed)
				{
					if (event.mouseButton.button == sf::Mouse::Left)
					{
					//START
						//OSCYLATOR
					if (Mouse::getPosition(window).x > ButtonX && Mouse::getPosition(window).x < (windowWidth+buttonWidth) && Mouse::getPosition(window).y >100 && Mouse::getPosition(window).y < (100 + buttonHeight) && !isStarted)
						{
							isStarted = true;
							int it = 0;

							while (it < cordXVec.size())
							{
								numberOfCell = coordinateToNumber(cordXVec[it] + 10, cordYVec[it] + 10, myBoard, sizeX, sizeY);
								myBoard.get()[numberOfCell].setCellAlive();
								if (it == cordXVec.size() - 1)
								{
									for (int i = sizeY - 1; i >= 0; i--)
									{
										for (int j = 0; j < sizeX; j++)
										{
											if (myBoard.get()[j + sizeX*i].IsAlive())
											{
												window.draw(circleShape1);
												circleShape1.setPosition((a+1) * j, (b+1) * i);
											}
											else
											{
												window.draw(circleShape2);
												circleShape2.setPosition((a+1) * j, (b+1) * i);
											}
										}
									}
								}
								it++;
							}
						}
					//GLIDER
					if (Mouse::getPosition(window).x > ButtonX && Mouse::getPosition(window).x < (windowWidth + buttonWidth) && Mouse::getPosition(window).y >200 && Mouse::getPosition(window).y < (200 + buttonHeight)  && !isStarted)
					{
						isStarted = true;
						int it = 0;
						while (it < 5)
						{
							cordX[it] = atoi(cordXs[it].c_str());
							cordY[it] = atoi(cordYs[it].c_str());
							numberOfCell = coordinateToNumber(cordX[it], cordY[it], myBoard, sizeX, sizeY);
							myBoard.get()[numberOfCell].setCellAlive();
							if (it == 4)
							{
								for (int i = sizeY - 1; i >= 0; i--)
								{
									for (int j = 0; j < sizeX; j++)
									{
										if (myBoard.get()[j + sizeX*i].IsAlive())
										{
											window.draw(circleShape1);
											circleShape1.setPosition((a+1) * j, (b+1) * i);
										}
										else
										{
											window.draw(circleShape2);
											circleShape2.setPosition((a+1)* j, (b+1) * i);
										}
									}
								}
							}
							it++;
						}
					}
					//NIEZMIENNIK
					if (Mouse::getPosition(window).x > ButtonX && Mouse::getPosition(window).x < (windowWidth + buttonWidth) && Mouse::getPosition(window).y >300 && Mouse::getPosition(window).y < (300 + buttonHeight) && !isStarted)
					{
						isStarted = true;
						int it=0;
						cordX[0] = sizeX / 2;
						cordY[0] = sizeY / 2;
						cordX[1] = cordX[0] + 1;
						cordY[1] = cordY[0] - 1;
						cordX[2] = cordX[0] + 1;
						cordY[2] = cordY[0] - 2;
						cordX[3] = cordX[0];
						cordY[3] = cordY[0] - 3;
						cordX[4] = cordX[0] - 1;
						cordY[4] = cordY[0] - 2;
						cordX[5] = cordX[0] - 1;
						cordY[5] = cordY[0] - 1;
						while (it < 6)
						{
							
							numberOfCell = coordinateToNumber(cordX[it], cordY[it], myBoard, sizeX, sizeY);
							myBoard.get()[numberOfCell].setCellAlive();
							if (it == 5)
							{
								for (int i = sizeY - 1; i >= 0; i--)
								{
									for (int j = 0; j < sizeX; j++)
									{
										if (myBoard.get()[j + sizeX*i].IsAlive())
										{
											window.draw(circleShape1);
											circleShape1.setPosition((a+1) * j, (b+1) * i);
										}
										else
										{
											window.draw(circleShape2);
											circleShape2.setPosition((a+1) * j, (b+1) * i);
										}
									}
								}
							}
							it++;
						}
						
					}
					//RANDOM
					if (Mouse::getPosition(window).x > ButtonX && Mouse::getPosition(window).x < (windowWidth + buttonWidth) && Mouse::getPosition(window).y >400 && Mouse::getPosition(window).y < (400 + buttonHeight) && !isStarted)
					{
					
						isStarted = true;
						int NumberOfCellsToAlive = 0;
						int it=0;
						NumberOfCellsToAlive = (rand()%sizeX*sizeY ) ;
						int* cordRandX = new int[NumberOfCellsToAlive];
						int* cordRandY = new int[NumberOfCellsToAlive];
						for(int i = 0 ; i<NumberOfCellsToAlive ; i++)
						{
							cordRandX[i] = (rand() % sizeX - 1)+1;
							cordRandY[i] = (rand() % sizeY - 1)+1;
							
						}
						while (it < NumberOfCellsToAlive)
						{

							numberOfCell = coordinateToNumber(cordRandX[it], cordRandY[it], myBoard, sizeX, sizeY);
							myBoard.get()[numberOfCell].setCellAlive();
							if (it == NumberOfCellsToAlive-1)
							{
								for (int i = sizeY - 1; i >= 0; i--)
								{
									for (int j = 0; j < sizeX; j++)
									{
										if (myBoard.get()[j + sizeX*i].IsAlive())
										{
											window.draw(circleShape1);
											circleShape1.setPosition((a+1) * j, (b+1) * i);
										}
										else
										{
											window.draw(circleShape2);
											circleShape2.setPosition((a+1) * j, (b+1) * i);
										}
									}
								}
							}
							it++;
						}

					}
					//KLIKANIE
					if ( !selectorPlay && Mouse::getPosition(window).y >= 0 && Mouse::getPosition(window).y <= sizeY*(b + 1) && Mouse::getPosition(window).x >= 0 && Mouse::getPosition(window).x <= sizeX*(a+1))
					{
						int tmpX, tmpY;
				
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
									if (Mouse::getPosition(window).x > ButtonX && Mouse::getPosition(window).x < (windowWidth + buttonWidth) && Mouse::getPosition(window).y >0 && Mouse::getPosition(window).y < (0 + buttonHeight) && isStarted)
									{
										selectorPlay = true;
									}
								
								}
							}
							
							numberOfCell = coordinateToNumber(tmpX, tmpY, myBoard, sizeX, sizeY);
							if(!(myBoard.get()[numberOfCell].IsAlive()))
							myBoard.get()[numberOfCell].setCellAlive();
							else myBoard.get()[numberOfCell].setCellDead();

							
						
					}
					//PLAY
					if (Mouse::getPosition(window).x > ButtonX && Mouse::getPosition(window).x < (windowWidth + buttonWidth) && Mouse::getPosition(window).y >0 && Mouse::getPosition(window).y < (0 + buttonHeight) && isStarted)
					{
						selectorPlay = true;
						
					}
					//STOP
					if (Mouse::getPosition(window).x > ButtonX && Mouse::getPosition(window).x < (windowWidth + buttonWidth) && Mouse::getPosition(window).y >600 && Mouse::getPosition(window).y < (600 + buttonHeight) && selectorPlay)
					{
						selectorPlay = false;
						
					
					}

						//FINISH
						if (Mouse::getPosition(window).x > ButtonX && Mouse::getPosition(window).x < (windowWidth + buttonWidth) && Mouse::getPosition(window).y >500 && Mouse::getPosition(window).y < (500 + buttonHeight))
						{
							for (int i = 0; i < sizeX*sizeY; i++)
							{
								myBoard.get()[i].setCellDead();
							}
							isStarted = false;
							selectorPlay = false;
						}
					}
				}
			}
			window.display();
			
			
			
	}
	system("pause");
}