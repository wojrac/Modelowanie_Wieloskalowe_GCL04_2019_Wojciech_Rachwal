#include"Button.h"
Button::Button(float x, float y, float width, float height, Font &font, string text, Color color)
{
	this->buttonShape.setPosition(Vector2f(x, y));
	this->buttonShape.setSize(Vector2f(width, height));
	this->buttontText.setString(text);
	this->buttontText.setFillColor(Color::Black);
	this->buttontText.setFont(font);
	this->buttontText.setCharacterSize(12);
	this->buttontText.setPosition(this->buttonShape.getPosition().x + (this->buttonShape.getGlobalBounds().width/2.f)-this->buttontText.getGlobalBounds().width/2.f,
	this->buttonShape.getPosition().y + (this->buttonShape.getGlobalBounds().height / 2.f) - this->buttontText.getGlobalBounds().height / 2.f);
	this->buttonColor = color;
	this->buttonShape.setFillColor(this->buttonColor);
}
