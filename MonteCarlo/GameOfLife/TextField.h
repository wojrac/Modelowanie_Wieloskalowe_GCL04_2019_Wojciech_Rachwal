#pragma once
#include"Button.h"
class TextField : public sf::Transformable, public sf::Drawable {
private:
	unsigned int m_size;
	sf::Font m_font;
	std::string m_text;
	sf::RectangleShape m_rect;
	//Text text;
	bool m_hasfocus;
public:
	TextField(unsigned int maxChars) :
		m_size(maxChars),
		m_rect(sf::Vector2f(15 * m_size, 16)), // 15 pixels per char, 20 pixels height, you can tweak
		m_hasfocus(false)
	{
		m_font.loadFromFile("arial.ttf"); 
		m_rect.setOutlineThickness(2);
		m_rect.setFillColor(sf::Color::White);
		m_rect.setOutlineColor(sf::Color(127, 127, 127));
		m_rect.setPosition(this->getPosition());
		/*this->text.setString(m_text);
		this->text.setFillColor(Color::Yellow);
		this->text.setFont(m_font);
		this->text.setCharacterSize(4);*/
	}
	const string getText() const {
		return m_text;
	}
	void setPosition(float x, float y) {
		sf::Transformable::setPosition(x, y);
		m_rect.setPosition(x, y);
	}
	bool contains(sf::Vector2f point) const {
		return m_rect.getGlobalBounds().contains(point);
	}
	void setFocus(bool focus) {
		m_hasfocus = focus;
		if (focus) {
			m_rect.setOutlineColor(sf::Color::Blue);
		}
		else {
			m_rect.setOutlineColor(sf::Color(127, 127, 127)); // Gray color
		}
	}
	void handleInput(sf::Event e) {
		/*if (!m_hasfocus || e.type != sf::Event::TextEntered)
		{
			cout << "return " << endl;
			return;
		}*/

		if (e.text.unicode == 8) {   // Delete key
			m_text = m_text.substr(0, m_text.size() - 1);
			cout << "delete" << endl;
		}
		else if (m_text.size() < m_size) {
			cout << "pisanie" << endl;
			m_text += e.text.unicode;
		}
	}
	virtual void draw(RenderTarget& target, RenderStates states) const
	{
		target.draw(m_rect);
		//target.draw(text);
	};
};