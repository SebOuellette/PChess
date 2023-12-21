#include "UI_Module.hpp"

UI_Module::UI_Module(sf::Vector2f size, sf::Vector2f position) {
	// Create texture
	if (!this->canvas.create(size.x, size.y)) {
		std::cout << "Error generating background texture" << std::endl;
	}
	// Create sprite
	this->canvasSprite.setTexture(this->canvas.getTexture());
	canvasSprite.setPosition(position);

	// Nullify any data in the font variable
	this->managingFont = false; // false so the function will trigger
	this->allocateFontDoublep();
}

UI_Module::~UI_Module() {
	this->destroyFont();

	// destroy the double pointer
	if (this->managingFont) {
		delete this->activeFont;
	}
}

void UI_Module::allocateFontDoublep() {
	// No point doing all this if it's allready been done
	if (!this->managingFont) {
		// If we are allocating the double pointer, we are managing the font
		this->managingFont = true;

		// Allocate and prepare space
		this->activeFont = new sf::Font*;
		*this->activeFont = nullptr;
	}
}

sf::Font** UI_Module::loadFont(std::string path) {
	// We are loading a font by file path, screw the pointers, allocate our own space (if we haven't already)
	this->allocateFontDoublep();
	
	// If a font is loaded, destroy it
	this->destroyFont();

	// Now allocate a new font
	*this->activeFont = new sf::Font;

	// Load the font file
	if (!(*this->activeFont)->loadFromFile(path)) {
		std::cout << "Could not load font '" << path << "'." << std::endl;
	}

	// Ensure we are returning the correct value
	return this->getFont();
}

sf::Font** UI_Module::loadFont(sf::Font** font) {
	if (*font == nullptr) {
		return this->getFont();
	}

	// We don't need to keep track of our own font anymore
	this->destroyFont();

	// Load the font doublep given
	this->activeFont = font;
	// Indicate we are not in control of this pointer
	this->managingFont = false;

	return this->getFont();
}

// Returns a pointer to the active font
//  Could be nullptr if not defined yet
sf::Font** UI_Module::getFont() {\
	if (*this->activeFont == nullptr) {
		std::cerr << "[Warning] Font is null." << std::endl;
	}

	return this->activeFont;
}

void UI_Module::destroyFont() {
	if (this->activeFont != nullptr && *this->activeFont != nullptr) {
		delete *this->activeFont;
		*this->activeFont = nullptr;
	}
}

void UI_Module::CLEAR_CANVAS(sf::Color background) {
	this->canvas.clear(background);
}

void UI_Module::DISPLAY_CANVAS() {
	this->canvas.display();
	this->canvasSprite.setTexture(this->canvas.getTexture());
}

// Draw to the canvas
void UI_Module::updateCanvas() {
	this->CLEAR_CANVAS(sf::Color(255,255,255,100));

	sf::Text t;
	sf::Font* f = *this->getFont();

	if (f) {
		t.setFont(*f);
		t.setString("I am a UI Element");
		t.setCharacterSize(24);
		t.setFillColor(sf::Color::Red);

		this->canvas.draw(t);
	}

	
	this->DISPLAY_CANVAS();
}

sf::Sprite& UI_Module::drawable() {
	return this->canvasSprite;
}


