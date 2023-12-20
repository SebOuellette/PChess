#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cstddef>
#include <string>

int main() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	sf::RenderWindow window(sf::VideoMode(900, 800), "Perky Chess", sf::Style::Titlebar | sf::Style::Close, settings);
	window.setFramerateLimit(512); // Set to 512 because it works best for the dragging audio

	const short int tileSize = 600 / 8;
	Board board(tileSize, &window);
	board.setPosition(sf::Vector2f(100, 100));
	sf::Cursor cursor;

	// The location of the last clicked piece
	sf::Vector2f lastClickedPiece = sf::Vector2f(-1, -1);
	sf::Vector2f lastClickedPosition = sf::Vector2f(-1, -1);

	
	// Keep a copy of the previous mouse position to calculate the instantaneous velocity during the current frame
	sf::Vector2f mousePos;

	// Create a clock for calculating the framerate
	sf::Clock FrameClock;

	std::string gameMovementsLog = "";

	while (window.isOpen()) {
		
		// Calculate framerate
		float secondsSinceLastFrame = FrameClock.getElapsedTime().asSeconds();
		FrameClock.restart(); // Reset frame clock
		
		float framerate = 1. / secondsSinceLastFrame;

		// Update the mouse movement stuff
		sf::Vector2f mouseVelocity = mousePos; // Cache the old mousePosition

		// Get the mouse position
		mousePos = board.mapWindowPixelToBoard(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		sf::Glsl::Vec2 shaderMousePos = sf::Glsl::Vec2(board.mapShaderCoord(mousePos));//sf::Glsl::Vec2(mousePos.x, window.getSize().y - mousePos.y);

		// Finish the calculation
		mouseVelocity = mousePos - mouseVelocity; // new Mouse pos - old mouse pos = vector from old mouse pos to new mouse pos

		board.pieceShader.setUniform("mousePos", shaderMousePos);

		// Get the current hovered tile
		sf::Vector2f hoveredTile = sf::Vector2f(floor(mousePos.x / tileSize), floor(mousePos.y / tileSize));

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
				case sf::Event::Closed:
					window.close();
					break;

				case sf::Event::MouseButtonPressed:
					// Check if left click
					if (event.mouseButton.button == sf::Mouse::Left) {
						// Store the board square pointer
						_Piece<Board>* clickedPiece = board.getPiece(hoveredTile);

						if (clickedPiece != nullptr) {
// START OF DEBUG PRINT STUFF
							// sf::Vector2f hoveredTileDebug = hoveredTile;
							// hoveredTileDebug.y += 1;
							// _Piece<Board>* clickedPieceDebug = board.getPiece(hoveredTileDebug);
							// std::cout << "KQBNRF"[clickedPieceDebug->getType()] << std::endl;
							std::cout << clickedPiece->getValidSquares(&board).size() << " available spaces to move to" << std::endl;
// END OF DEBUG PRINT STUFF

							lastClickedPiece = hoveredTile;

							// Set the cursor to drag around the piece
							if (cursor.loadFromSystem(sf::Cursor::SizeAll))
								window.setMouseCursor(cursor);

							// Set the last clicked position to the mouse position
							lastClickedPosition = shaderMousePos;//sf::Vector2f(mousePos.x / window.getSize().x, mousePos.y / window.getSize().y);
							board.pieceShader.setUniform("lastClickedPos", lastClickedPosition);

							board.slideSound.play();
						}

						
					}
					break;

				case sf::Event::MouseButtonReleased:
					// Check if left release
					if (event.mouseButton.button == sf::Mouse::Left) {
						_Piece<Board>* oldPiece = board.getPiece(lastClickedPiece);
						_Piece<Board>* newPiece = board.getPiece(hoveredTile);

						// Get the valid squares for the held piece
						bool movingToValid = false;
						if (oldPiece != nullptr) {
							std::vector<sf::Vector2i> validSquares = oldPiece->getValidSquares(&board);
							
							// Loop through all valid positions, check if they equal the hovered position
							for (sf::Vector2i validPos : validSquares) {
								if (validPos.x == (int)hoveredTile.x && validPos.y == (int)hoveredTile.y)
									movingToValid = true;
							}
						}

						if (lastClickedPiece.x >= 0 && (newPiece == nullptr || newPiece->isPieceWhite() != oldPiece->isPieceWhite()) && movingToValid) {

							//gameMovementsLog += '/';
							
							// Print piece letter
							//gameMovementsLog += "KQBNRF"[oldPiece->getType()];
							// Print location
							// Rank
							//gameMovementsLog += "abcdefgh"[(int)(lastClickedPiece.x)];
							// Row
							//gameMovementsLog += std::to_string(8 - (int)(lastClickedPiece.y));

							if (newPiece != nullptr) {
								// Takes pleace, play sound
								board.takeSound.play();

								// if took, print
								//gameMovementsLog += 'x';
							}

							
							board.setPiece(hoveredTile, oldPiece);

							// Clear the reference to this piece from the old position
							board.setPiece(lastClickedPiece, nullptr);

							// Print location
							// Rank
							//gameMovementsLog += "abcdefgh"[(int)(hoveredTile.x+0.2)];
							// Row
							//gameMovementsLog += std::to_string(8 - (int)(hoveredTile.y+0.2));

							//std::cout << gameMovementsLog << std::endl;

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
		window.clear();

		//std::cout << lastClickedPosition.x / tileSize << " " << lastClickedPosition.y / tileSize << " | " << shaderMousePos.x / tileSize << " " << shaderMousePos.y / tileSize << std::endl; 
		
		board.doFrame(&window, lastClickedPiece, mouseVelocity, hoveredTile, &cursor, shaderMousePos, framerate);
		
		window.draw(*board.getBackground());
		board.drawPieces(&window, board.getPiece(lastClickedPiece));
		window.display();
	}
	return 0;
}
