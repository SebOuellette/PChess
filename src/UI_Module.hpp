#ifndef UI_MODULE_HPP
#define UI_MODULE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <iostream>

class UI_Module {
private:
	sf::Sprite canvasSprite;
	bool managingFont;
	void allocateFontDoublep();
	
protected:
	sf::RenderTexture canvas;
	// Store a pointer to the active font (or load a font into heap using the included methods)
	sf::Font** activeFont;
	

	// Used by updateCanvas to first clear the canvas
	void CLEAR_CANVAS(sf::Color background = sf::Color(0, 0, 0, 0));
	// Used by updateCanvas to finalize the canvas once we're done.
	void DISPLAY_CANVAS(); 
	

public:
	// Constructor(s) / Destructor
	UI_Module(sf::Vector2f size = sf::Vector2f(0, 0), sf::Vector2f position = sf::Vector2f(0, 0));
	~UI_Module();

	// Font management
	sf::Font** loadFont(std::string path);
	sf::Font** loadFont(sf::Font** font);
	sf::Font** getFont();
	void destroyFont();

	// Canvas Management
	virtual void updateCanvas(); // Draw to the canvas
	sf::Sprite& drawable(); // Get sprite

};

#endif