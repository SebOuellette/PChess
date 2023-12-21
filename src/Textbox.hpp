#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP

#include "UI_Module.hpp"

class Textbox : public UI_Module {
private:
public:
	Textbox(sf::Vector2f size = sf::Vector2f(0, 0), sf::Vector2f position = sf::Vector2f(0, 0));
};

#endif