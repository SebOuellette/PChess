#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cstddef>
#include <string>
#include "UI_Module.hpp"
#include "Textbox.hpp"

#define BOARD_SIZE 400 // px
#define MAX_FRAMERATE 512
#define WINDOW_TITLE "Perky Chess"

int main() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 0; // 8 (set to 0 for now cause why)

	sf::RenderWindow window(sf::VideoMode(1000, 800), WINDOW_TITLE, sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, settings);
	window.setFramerateLimit(MAX_FRAMERATE); // Set to 512 because it works best for the dragging audio

	const short int tileSize = BOARD_SIZE / 8;
	Board board(tileSize, &window);
	board.setPosition(sf::Vector2f((window.getSize().x - BOARD_SIZE) / 2, 50));
	sf::Cursor cursor;

	// The location of the last clicked piece
	sf::Vector2f lastClickedPiece = sf::Vector2f(-1, -1);
	sf::Vector2f lastClickedPosition = sf::Vector2f(-1, -1);

	// Keep a copy of the previous mouse position to calculate the instantaneous velocity during the current frame
	sf::Vector2f boardMousePos;
	sf::Vector2f mousePos;

	// Create a clock for calculating the framerate
	sf::Clock FrameClock;
	std::string gameMovementsLog = "";

	// UI Test
	UI_Module uitest(sf::Vector2f(200, 200), sf::Vector2f(750, 150));
	sf::Font** f = uitest.loadFont("./fonts/coolvetica.ttf");
	uitest.updateCanvas();
	// Child UI test
	Textbox test2(sf::Vector2f(300, 300), sf::Vector2f(100, 100));
	test2.loadFont(f);
	test2.updateCanvas();

	while (window.isOpen()) {
		
		// Calculate framerate
		float secondsSinceLastFrame = FrameClock.getElapsedTime().asSeconds();
		FrameClock.restart(); // Reset frame clock
		
		float framerate = 1. / secondsSinceLastFrame;

		// Update the mouse movement stuff
		sf::Vector2f mouseVelocity = boardMousePos; // Cache the old mousePosition

		// Get the mouse position
		mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		boardMousePos = board.mapWindowPixelToBoard(mousePos);

		// Constrain the mouse pos to the board
		auto constrain = [](int n, int s){
			int r = n;

			if (r < 0)
				r = 0;

			if (r >= s)
				r = s - 1;

			return r;
		};
		boardMousePos.x = constrain(boardMousePos.x, board.getSize().x);
		boardMousePos.y = constrain(boardMousePos.y, board.getSize().y);

		sf::Glsl::Vec2 shaderMousePos = sf::Glsl::Vec2(board.mapShaderCoord(boardMousePos));//sf::Glsl::Vec2(mousePos.x, window.getSize().y - mousePos.y);

		// Finish the calculation
		mouseVelocity = boardMousePos - mouseVelocity; // new Mouse pos - old mouse pos = vector from old mouse pos to new mouse pos

		board.pieceShader.setUniform("mousePos", shaderMousePos);

		// Get the current hovered tile
		sf::Vector2f hoveredTile = sf::Vector2f(floor(boardMousePos.x / tileSize), floor(boardMousePos.y / tileSize));

		// If I'm holding a piece, set the hovered tile to the center of the chess piece instead of the mouse
		// if (lastClickedPiece.x >= 0) {
		// 	// Move the selected position to the center of the held piece
		// 	hoveredTile -= sf::Vector2f(
		// 		std::fmod(lastClickedPosition.x * 8, 1.0) - 0.5, 
		// 		std::fmod(lastClickedPosition.y * 8, 1.0) - 0.5);
		// }

		//std::cout << hoveredTile.x << " " << hoveredTile.y << std::endl;;

		//std::cout << (shaderMousePos.x - lastClickedPosition.x) * 2 / board.getSize().x / 0.25 << " " << (shaderMousePos.y - lastClickedPosition.y) * 2 / board.getSize().y  / 0.25 << std::endl;

		

		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Resized:
					{
						sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
						window.setView(sf::View(visibleArea));

						board.setPosition(sf::Vector2f((window.getSize().x - BOARD_SIZE) / 2, 50));
					}

					break;
				case sf::Event::Closed:
					window.close();
					break;

				case sf::Event::MouseButtonPressed:
					board.mousePressEvent(event, lastClickedPiece, hoveredTile, window, cursor, lastClickedPosition, shaderMousePos);
					break;

				case sf::Event::MouseButtonReleased:
					board.mouseReleaseEvent(event, hoveredTile, lastClickedPiece);
					break;
				default:
					break;
			} 
		}
		
		
		// Let the board do it's calculations for the frame
		window.clear( sf::Color(46, 38, 31, 255));
		
		board.doFrame(&window, lastClickedPiece, mouseVelocity, hoveredTile, &cursor, shaderMousePos, framerate);
		
		window.draw(*board.getBackground());
		board.drawPieces(&window, board.getPiece(lastClickedPiece));
		window.draw(uitest.drawable());
		window.draw(test2.drawable());
		window.display();
	}
	return 0;
}
