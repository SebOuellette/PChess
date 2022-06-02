#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include <cmath>

int main() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(600, 600), "Perky Chess", sf::Style::Titlebar | sf::Style::Close, settings);
	window.setFramerateLimit(1000); // Set to 1000 because it works well for the dragging audio

	const short int tileSize = 600 / 8;
	Board board(tileSize, &window);
	sf::Cursor cursor;

	// The location of the last clicked piece
	sf::Vector2f lastClickedPiece = sf::Vector2f(-1, -1);
	sf::Vector2f lastClickedPosition = sf::Vector2f(-1, -1);

	
	// Keep a copy of the previous mouse position to calculate the instantaneous velocity during the current frame
	sf::Vector2f mousePos;

	while (window.isOpen()) {

		// Update the mouse movement stuff
		sf::Vector2f mouseVelocity = mousePos; // Cache the old mousePosition

		// Get the mouse position
		mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		sf::Glsl::Vec2 shaderMousePos = sf::Glsl::Vec2(mousePos.x, window.getSize().y - mousePos.y);

		// Finish the calculation
		mouseVelocity = mousePos - mouseVelocity; // new Mouse pos - old mouse pos = vector from old mouse pos to new mouse pos

		board.pieceShader.setUniform("mousePos", shaderMousePos);

		// Get the current hovered tile
		sf::Vector2f hoveredTile = sf::Vector2f(mousePos.x / tileSize, mousePos.y / tileSize);

		// If I'm holding a piece, set the hovered tile to the center of the chess piece instead of the mouse
		if (lastClickedPiece.x >= 0) {
			// Move the selected position to the center of the held piece
			hoveredTile -= sf::Vector2f(
				std::fmod(lastClickedPosition.x * 8, 1.0) - 0.5, 
				std::fmod(lastClickedPosition.y * 8, 1.0) - 0.5);
		}

		

		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;

				case sf::Event::MouseButtonPressed:
					// Check if left click
					if (event.mouseButton.button == sf::Mouse::Left) {
						// Store the board square pointer
						Piece* clickedPiece = board.getPiece(hoveredTile);
						
						if (clickedPiece != nullptr) {
							lastClickedPiece = hoveredTile;

							// Set the cursor to drag around the piece
							if (cursor.loadFromSystem(sf::Cursor::SizeAll))
								window.setMouseCursor(cursor);

							// Set the last clicked position to the mouse position
							lastClickedPosition = sf::Vector2f(mousePos.x / window.getSize().x, mousePos.y / window.getSize().y);
							board.pieceShader.setUniform("lastClickedPos", shaderMousePos);

							board.slideSound.play();
						}

						
					}
					break;

				case sf::Event::MouseButtonReleased:
					// Check if left release
					if (event.mouseButton.button == sf::Mouse::Left) {
						Piece* oldPiece = board.getPiece(lastClickedPiece);
						Piece* newPiece = board.getPiece(hoveredTile);

						if (lastClickedPiece.x >= 0 && (newPiece == nullptr || newPiece->isPieceWhite() != oldPiece->isPieceWhite())) {
							if (newPiece != nullptr)
								// Takes pleace, play sound
								board.takeSound.play();

							board.setPiece(hoveredTile, oldPiece);

							// Clear the reference to this piece from the old position
							board.setPiece(lastClickedPiece, nullptr);
						}

						// Reset, no longer holding piece
						lastClickedPiece = sf::Vector2f(-1, -1);
						board.pieceShader.setUniform("lastClickedPos", lastClickedPiece);

						board.slideSound.stop();
					}
					break;
				default:
					break;
			} 
		}

		

		// Let the board do it's calculations for the frame
		board.doFrame(&window, lastClickedPiece, mouseVelocity, hoveredTile, &cursor, shaderMousePos);
		

		window.clear();
		window.draw(*board.getBackground());
		board.drawPieces(&window, board.getPiece(lastClickedPiece));
		window.display();
	}
	return 0;
}
