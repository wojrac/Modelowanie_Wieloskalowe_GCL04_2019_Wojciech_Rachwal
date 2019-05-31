#pragma once
#include<SFML\Graphics.hpp>
#include<SFML\Window.hpp>
#include<string>
#include<memory>
using namespace std;
using namespace sf;
class Button:public Drawable
{
	RectangleShape buttonShape;
	Text buttontText;
	Color buttonColor;
public:
	Button(float, float, float, float,Font& ,string, Color);
	virtual void draw(RenderTarget& target, RenderStates states) const
	{
		target.draw(buttonShape);
		target.draw(buttontText);
	};
	RectangleShape getButtonShape()
	{
		
		return buttonShape;
	}
};
