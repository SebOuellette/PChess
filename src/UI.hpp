#ifndef UI_HPP
#define UI_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <iostream>

class UI_Module {
private:
	//std::vector<sf::Drawable> elements;

	sf::RenderTexture canvas;
	sf::Sprite canvasSprite;

public:
	UI_Module(sf::Vector2f size, sf::Vector2f position = sf::Vector2f(0, 0)) {
		// Create texture
		if (!this->canvas.create(size.x, size.y)) {
			std::cout << "Error generating background texture" << std::endl;
		}
		// Create sprite
		this->canvasSprite.setTexture(this->canvas.getTexture());
		canvasSprite.setPosition(position);


		// Test
		
		//elements.push_back(t);
	}

	void updateCanvas() {
		this->canvas.clear(sf::Color(255, 255, 255, 100));

		// Draw all the elements onto the canvas
		// for (int i=0;i<this->elements.size();i++) {
		// 	canvas.draw(elements[i]);
		// }

		sf::Font f;
		if (!f.loadFromFile("./fonts/coolvetica.ttf")) {
			std::cout << "Could not load font fonts/coolvetica.ttf" << std::endl;
		}
		sf::Text t;
		t.setFont(f);
		t.setString("This is a test");
		t.setCharacterSize(24);
		t.setFillColor(sf::Color::Red);

		this->canvas.draw(t);

		this->canvas.display();
		this->canvasSprite.setTexture(this->canvas.getTexture());
	}

	sf::Sprite& drawable() {
		return this->canvasSprite;
	}
};

#endif