#include"Cell.h"
#include<memory>
#include "Button.h"
#include<boost\filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include<filesystem>
#include<vector>
#include<SFML\Graphics.hpp>
#include<SFML\Window.hpp>
using namespace boost::filesystem;
namespace fs= std::experimental::filesystem;
using namespace sf;
int coordinateToNumber(int coordinateX, Cell* arr, int sX, int sY)
{

	int i = 0;
	while (!(arr[i].getCoordinateX() == coordinateX ) && i<sX*sY)
	{
		//cout << "i :" << i;
		i++;
	}
	return  i;
}
int main()
{
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini("config.ini", pt);
	Event event;
	Font font;
	if (!font.loadFromFile("arial.ttf"))
	{
		cout << "error loading a file " << endl;
		system("pause");
	}
	unsigned int windowWidth = 1500;
	unsigned int windowHeight = 1000;
	int ButtonX = windowWidth - 150;
	int buttonWidth = 100;
	int buttonHeight = 40;
	RenderWindow window = { VideoMode{ windowWidth,windowHeight } , "GAME OF LIFE" };
	window.setFramerateLimit(60);
	RectangleShape drawAlive(Vector2f(8,8));
	RectangleShape drawDead(Vector2f(8,8));
	drawAlive.setFillColor(Color::Green);
	drawDead.setFillColor(Color::Red);
	Button button1(ButtonX, 100, buttonWidth, buttonHeight, font, "REGULA 90", Color::Blue);
	Button button2(ButtonX, 200, buttonWidth, buttonHeight, font, "REGULA 30", Color::Blue);
	Button button3(ButtonX, 300, buttonWidth, buttonHeight, font, "REGULA 60", Color::Blue);
	Button button4(ButtonX, 400, buttonWidth, buttonHeight, font, "REGULA 120", Color::Blue);
	Button button5(ButtonX, 500, buttonWidth, buttonHeight, font, "REGULA 225", Color::Blue);
	string sizeS, stepsS;
	sizeS = pt.get<std::string>("Section1.size");
	stepsS = pt.get<std::string>("Section1.steps");
	int size, steps;
	size= atoi(sizeS.c_str());
	steps = atoi(stepsS.c_str());
	bool is90 = false;
	bool is30 = false;
	bool is60 = false;
	bool is120 = false;
	bool is225 = false;
	int cordX, numberofCell;
	cordX = size / 2;
	Cell** array2D = new Cell*[steps];
	for (int i = 0; i < steps; i++)
	{
		array2D[i] = new Cell[size];
	}
	for (int j = 0; j < size; j++)
	{
		array2D[0][j].setCellDead();
		array2D[0][j].setCoordinateX(j);
	}
	numberofCell = coordinateToNumber(cordX, array2D[0], size, 1);
	array2D[0][numberofCell].setCellAlive();
	Cell** neighboursWithMiddle = new Cell*[size];
	for (int i = 0; i < size; i++)
	{
		neighboursWithMiddle[i] = new Cell[2];
	}
	int* arrayWithSum = new int[size];
	int* arrayWithSumLeft = new int[size];
	int* arrayWithSumRight = new int[size];
	while (window.isOpen())
	{
		window.clear(Color::Black);
		window.draw(button1);
		window.draw(button2);
		window.draw(button3);
		window.draw(button4);
		window.draw(button5);
		for (int it = 0; it < steps; it++)
		{
			for (int i = 0; i < size; i++)
			{
				array2D[it][i].setCellDead();
			}
		}
		cordX = size / 2;
		numberofCell = coordinateToNumber(cordX, array2D[0], size, 1);
		array2D[0][numberofCell].setCellAlive();

		neighboursWithMiddle[0][0] = array2D[0][size - 1];
		neighboursWithMiddle[0][1] = array2D[0][1];
		neighboursWithMiddle[size - 1][0] = array2D[0][0];
		neighboursWithMiddle[size - 1][1] = array2D[0][size - 2];
		for (int i = 1; i < size - 1; i++)
		{
			neighboursWithMiddle[i][0] = array2D[0][i - 1];
			neighboursWithMiddle[i][1] = array2D[0][i + 1];
		}


		for (int iter_time = 1; iter_time < steps; iter_time++)
		{
			neighboursWithMiddle[0][0] = array2D[iter_time - 1][size - 1];
			neighboursWithMiddle[0][1] = array2D[iter_time - 1][1];
			neighboursWithMiddle[size-1][0] = array2D[iter_time - 1][size-2];
			neighboursWithMiddle[size-1][1] = array2D[iter_time - 1][0];
			for (int i = 1; i < size - 1; i++)
			{

				neighboursWithMiddle[i][0] = array2D[iter_time - 1][i - 1];
				neighboursWithMiddle[i][1] = array2D[iter_time - 1][i + 1];

			}
			for (int i = 0; i < size; i++)
			{
				arrayWithSum[i] = neighboursWithMiddle[i][0].IsAlive() + neighboursWithMiddle[i][1].IsAlive();
			}
			for (int i = 0; i < size; i++)
			{
				arrayWithSumLeft[i] = neighboursWithMiddle[i][0].IsAlive();
			}
			for (int i = 0; i < size; i++)
			{
				arrayWithSumRight[i] = neighboursWithMiddle[i][1].IsAlive();
			}
			if (is90)
			{
				for (int i = 0; i < size; i++)
				{

					if (!(array2D[iter_time - 1][i].IsAlive()))
					{
						if (arrayWithSum[i] == 1)
						{
							array2D[iter_time][i].setCellAlive();
						}
					}
					if (array2D[iter_time - 1][i].IsAlive())
					{
						if (arrayWithSum[i] == 2 || arrayWithSum[i] == 0)
						{
							array2D[iter_time][i].setCellDead();
						}
					}
				}
			}
			if (is30)
			{
				for (int i = 0; i < size; i++)
				{
					if (!(array2D[iter_time - 1][i].IsAlive()))
					{
						if (arrayWithSum[i] == 1)
						{
							array2D[iter_time][i].setCellAlive();
						}
					}
					if (array2D[iter_time - 1][i].IsAlive())
					{
						if (arrayWithSum[i] == 2)
						{
							array2D[iter_time][i].setCellDead();
						}
						if (arrayWithSum[i] == 0)
						{
							array2D[iter_time][i].setCellAlive();
						}
						if (arrayWithSumRight[i] == 1)
						{
							array2D[iter_time][i].setCellAlive();
						}
						if (arrayWithSumLeft[i] == 1)
						{
							array2D[iter_time][i].setCellDead();
						}
					}
				}
			}
			if (is60)
			{
				for (int i = 0; i < size; i++)
				{
					if (!(array2D[iter_time - 1][i].IsAlive()))
					{
						if (arrayWithSumLeft[i] == 1)
						{
							array2D[iter_time][i].setCellAlive();
						}
					}
					if (array2D[iter_time - 1][i].IsAlive())
					{
						if (arrayWithSum[i] == 2)
						{
							array2D[iter_time][i].setCellDead();
						}
						if (arrayWithSum[i] == 0)
						{
							array2D[iter_time][i].setCellAlive();
						}
						if (arrayWithSumRight[i] == 1)
						{
							array2D[iter_time][i].setCellAlive();
						}
						if (arrayWithSumLeft[i] == 1)
						{
							array2D[iter_time][i].setCellDead();
						}
					}
				}
			}
			if (is120)
			{
				for (int i = 0; i < size; i++)
				{
					if (!(array2D[iter_time - 1][i].IsAlive()))
					{
						if (arrayWithSum[i] == 2)
						{
							array2D[iter_time][i].setCellAlive();
						}
						if (arrayWithSumLeft[i] == 1)
						{
							array2D[iter_time][i].setCellAlive();
						}
					}
					if (array2D[iter_time - 1][i].IsAlive())
					{
						if (arrayWithSum[i] == 2)
						{
							array2D[iter_time][i].setCellDead();
						}
						if (arrayWithSum[i] == 0)
						{
							array2D[iter_time][i].setCellDead();
						}
						if (arrayWithSumRight[i] == 1)
						{
							array2D[iter_time][i].setCellAlive();
						}
						if (arrayWithSumLeft[i] == 1)
						{
							array2D[iter_time][i].setCellAlive();
						}
					}
				}
			}
			if (is225)
			{
				for (int i = 0; i < size; i++)
				{
					if (!(array2D[iter_time - 1][i].IsAlive()))
					{
						if (arrayWithSum[i] == 0)
						{
							array2D[iter_time][i].setCellAlive();
						}
						if (arrayWithSum[i] == 2)
						{
							array2D[iter_time][i].setCellAlive();
						}
					}
					if (array2D[iter_time - 1][i].IsAlive())
					{
						if (arrayWithSum[i] == 2)
						{
							array2D[iter_time][i].setCellAlive();
						}
						if (arrayWithSum[i] == 0)
						{
							array2D[iter_time][i].setCellDead();
						}
						if (arrayWithSumRight[i] == 1)
						{
							array2D[iter_time][i].setCellDead();
						}
						if (arrayWithSumLeft[i] == 1)
						{
							array2D[iter_time][i].setCellAlive();
						}
					}
				}
			}
		}
		for (int cht = 0; cht < steps ; cht++)
		{
			for (int i = 0; i < size; i++)
			{
				if (array2D[cht][i].IsAlive())
				{
					window.draw(drawAlive);
					drawAlive.setPosition(9.0*i, 9.0*cht);
				}
				else
				{
					window.draw(drawDead);
					drawDead.setPosition(9.0*i, 9.0*cht);
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
					//90
					if (Mouse::getPosition(window).x > buttonWidth && Mouse::getPosition(window).x < (windowWidth + buttonWidth) && Mouse::getPosition(window).y >100 && Mouse::getPosition(window).y < (100 + buttonHeight))
					{
						is90 = true;
						is30 = false;
						is60 = false;
						is120 = false;
						is225 = false;
					}
					if (Mouse::getPosition(window).x > buttonWidth && Mouse::getPosition(window).x < (windowWidth + buttonWidth) && Mouse::getPosition(window).y >200 && Mouse::getPosition(window).y < (200 + buttonHeight))
					{
						is30 = true;
						is90 = false;
						is60 = false;
						is120 = false;
						is225 = false;
					}
					if (Mouse::getPosition(window).x > buttonWidth && Mouse::getPosition(window).x < (windowWidth + buttonWidth) && Mouse::getPosition(window).y >300 && Mouse::getPosition(window).y < (300 + buttonHeight))
					{
						is60 = true;
						is90 = false;
						is30 = false;
						is120 = false;
						is225 = false;
					}
					if (Mouse::getPosition(window).x > buttonWidth && Mouse::getPosition(window).x < (windowWidth + buttonWidth) && Mouse::getPosition(window).y >400 && Mouse::getPosition(window).y < (400 + buttonHeight))
					{
						is120 = true;
						is60 = false;
						is90 = false;
						is30 = false;
						is225 = false;
					}
					if (Mouse::getPosition(window).x > buttonWidth && Mouse::getPosition(window).x < (windowWidth + buttonWidth) && Mouse::getPosition(window).y >500 && Mouse::getPosition(window).y < (500 + buttonHeight))
					{
						is120 = false;
						is60 = false;
						is90 = false;
						is30 = false;
						is225 = true;
					}
				}
			}
		}
		window.display();
	}
	system("pause");
}