#include "Board.hpp"
#include <functional>

Board::Board(short int tileSize, sf::Window* window) : tileSize(tileSize) {

	// Set the size of the board
	this->setSize(sf::Vector2i(tileSize * 8, tileSize * 8));

	// Generate a texture
	if (!this->backgroundTexture.create(this->getSize().x, this->getSize().y)) {
		std::cout << "Error generating background texture" << std::endl;
	}

	// Clear background
	this->backgroundTexture.clear();

	// Generate the 8 vertical
	for (int i=0;i<64;i++) {
		sf::RectangleShape shape(sf::Vector2f(this->tileSize, this->tileSize));
		shape.setPosition(this->tileSize * (i%8), this->tileSize * (i/8));
		
		
		shape.setFillColor((i + i/8)%2 == 0 ? tile1 : tile2);
		
		this->backgroundTexture.draw(shape);
	}

	// Display the stuff to the texture
	this->backgroundTexture.display();
	
	sf::Sprite sprite(this->backgroundTexture.getTexture());
	this->background = sprite;

	// Generate the pieces
	this->genPieces();

	// Create shaders used to modify pieces
	std::string vertexText = 
	"uniform sampler2D pieceOffset;"
	"uniform vec2 mousePos;"
	"uniform vec2 resolution;"
	"uniform bool isHolding;"
	"uniform vec2 position;"
	"uniform vec2 lastClickedPos;"

	"uniform vec2 texResolution;"
	"uniform vec2 boardWindowDiff;"
	"uniform vec2 mouseVelocity;"

	"void main() {"
		// Do actual movements
		"gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"

		"float unit = 1. / 4.;"

		// Draw the item that we are holding
		"vec2 localLastClicked = fract(lastClickedPos / resolution * 8.);"
		"if (isHolding) {"
			"gl_Position.xy += ((mousePos - lastClickedPos) * 2. / resolution) * boardWindowDiff;" // mousePos / resolution * 2. - position * unit - localLastClicked * unit;"
		"}"

		// If we are holding some item but this is not the item we are holding
		"if (lastClickedPos.x >= 0. && !isHolding) {"
			"gl_Position.xy += (texture2D(pieceOffset, position / 8.).xy - 0.5) / (10.0 / resolution.x * resolution.x);"
		"}"
		
		"gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;"

		"gl_FrontColor = gl_Color;"
	"}";

	
	std::string fragText = 
	"uniform bool isWhite;"
	"uniform sampler2D texture;"
	"uniform vec2 position;"
	"uniform vec2 mousePos;"
	"uniform vec2 resolution;"
	"uniform vec2 lastClickedPos;"
	"uniform sampler2D pieceOffset;"

	"void main() {"
		// Draw the piece texture
		"float unit = 1. / 8.;"
		"gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);"
		//"gl_FragColor = vec4(vec3(fract(lastClickedPos / resolution * 8.), 0.), 1.);"
		//"gl_FragColor *= texture2D(pieceOffset, position / 8.);"
		
		// If the pieces are black, draw them darker
		"if (!isWhite) {"
			"gl_FragColor.rgb *= 0.3;"
		"}"
	"}";


	// If the shader can't load, kill the program
	if (!this->pieceShader.loadFromMemory(vertexText, fragText)) {
		std::cerr << "Failed to load shader 1 or 2... Dying..." << std::endl;
		exit(1);
	}

	this->pieceShader.setUniform("resolution", sf::Glsl::Vec2(this->getSize()));
	this->pieceShader.setUniform("texture", sf::Shader::CurrentTexture);

	// Generate a texture
	if (!this->pieceOffsetTexture.create(8,8)) {
		std::cout << "Error generating offset texture" << std::endl;
	}

	this->pieceOffsetTexture.clear(sf::Color(127, 127, 0, 255));
	this->pieceOffsetTexture.display();


	std::string offsetTextureShader = 
	"uniform vec2 mousePos;"
	"uniform sampler2D texture;"
	"uniform vec2 texResolution;"
	"uniform vec2 boardResolution;"
	"uniform vec2 mouseVelocity;"
	
	"void main() {"
		"vec2 uv = gl_FragCoord.xy / texResolution;"
		"uv += uv / 16.;"
		"vec2 mouseLocal = mousePos / boardResolution + uv / 16.;"

		"vec2 offsetDir = normalize(uv - mouseLocal);"

		"float fac = 2. - 2. / (1. + pow(2.718, -pow(10. * distance(uv, mouseLocal), 6.)));"

		"float inLine = dot(normalize(mouseVelocity), offsetDir);"

		"gl_FragColor = vec4(vec3(0.5), 1.);"

		"if (fac > 0.01) {"
			"gl_FragColor = vec4(offsetDir * fac + 0.5, 0, 1.);"
		"}"
		
	"}";

	// If the shader can't load, kill the program
	if (!this->pieceOffsetShader.loadFromMemory(offsetTextureShader, sf::Shader::Fragment)) {
		std::cerr << "Failed to load shader 3... Dying..." << std::endl;
		exit(1);
	}

	this->pieceOffsetShader.setUniform("texture", sf::Shader::CurrentTexture);
	this->pieceOffsetShader.setUniform("texResolution", sf::Glsl::Vec2(pieceOffsetTexture.getSize()));
	this->pieceOffsetShader.setUniform("boardResolution", sf::Glsl::Vec2(this->getSize()));
	

	// Loading sounds
	// Load the piece sliding sound
	if (!this->slideBuffer.loadFromFile("sounds/slide.wav")) {
		std::cerr << "Failed to open slide.wav." << std::endl;
		exit(1);
	}

	this->slideSound.setBuffer(this->slideBuffer);
	this->slideSound.setLoop(true);
	this->slideSound.setPitch(0);

	// Load the piece sliding sound
	if (!this->takeBuffer.loadFromFile("sounds/take.wav")) {
		std::cerr << "Failed to open take.wav." << std::endl;
		exit(1);
	}
	this->takeSound.setBuffer(this->takeBuffer);
}

Board::~Board() {
	for (int i=0;i<32;i++) {
		if (this->getPiece(i%8, i/8) != nullptr)
			delete this->getPiece(i%8, i/8);
	}
}

sf::Sprite * Board::getBackground() {
	return &(this->background);
}

void Board::genPieces() {
	sf::Vector2f boardPos = this->getPosition();

	for (int colour=0;colour<=1;colour++) {
		int y = colour ? 6 : 1;

		// Generate pawns
		for (int x=0;x<8;x++) {
			this->pieces[x][y] = new Pawn<Board>(colour, &this->tileSize, boardPos, x, y);
		}
		
		// Set the y variable to work for the back rank pieces
		y = colour ? 7 : 0;

		// Generate Rooks
		for (int x=0;x<8;x+=7) {
			this->pieces[x][y] = new Rook<Board>(colour, &this->tileSize, boardPos, x, y);
		}

		// Generate Knights
		for (int x=1;x<8;x+=5) {
			this->pieces[x][y] = new Knight<Board>(colour, &this->tileSize, boardPos, x, y);
		}

		// Generate Bishops
		for (int x=2;x<8;x+=3) {
			this->pieces[x][y] = new Bishop<Board>(colour, &this->tileSize, boardPos, x, y);
		}

		// Generate Queens
		this->pieces[3][y] = new Queen<Board>(colour, &this->tileSize, boardPos, 3, y);

		// Generate King
		this->pieces[4][y] = new King<Board>(colour, &this->tileSize, boardPos, 4, y);
	}
}

void Board::drawPieces(sf::RenderWindow *window, Piece<Board>* holdingPiece) {
	// Get the current mouse position, in the coordinate space of a vertex shader
	//sf::Glsl::Vec2 mousePos = sf::Glsl::Vec2(window->mapPixelToCoords(sf::Mouse::getPosition(*window)));
	//mousePos.y = this->getSize().y - mousePos.y - this->getBackground()->getPosition().y;

	auto drawPiece = [&](Piece<Board>* piece, int x, int y, bool isHolding) {
		this->pieceShader.setUniform("isWhite", piece->isPieceWhite());
		this->pieceShader.setUniform("position", sf::Glsl::Vec2(x, 7.-y));
		this->pieceShader.setUniform("isHolding", isHolding);
		this->pieceShader.setUniform("pieceOffset", this->pieceOffsetTexture.getTexture());

		sf::Vector2f diff = sf::Vector2f((float)this->getSize().x / (float)window->getSize().x, (float)this->getSize().y / (float)window->getSize().y);
		
		this->pieceShader.setUniform("boardWindowDiff", sf::Glsl::Vec2(diff));

		(*window).draw(*piece->getSprite(), &this->pieceShader);
	};

	for (int i=0;i<sizeof(this->pieces)/sizeof(unsigned long /* Piece* */);i++) {
		int x = i % 8;
		int y = i / 8;

		Piece<Board>* piece = this->getPiece(x, y);

		if (piece != nullptr && holdingPiece != piece) {
			drawPiece(piece, x, y, false);
		}
	}

	// Draw the piece that the player is holding so that it draws above all the other elements
	if (holdingPiece != nullptr)
		drawPiece(holdingPiece, holdingPiece->getPosition().x, holdingPiece->getPosition().y, true);
}

Piece<Board>* Board::getPiece(int x, int y) {
	if (x < 0 || y < 0 || x > 7 || y > 7) return nullptr;

	return this->pieces[x][y];
}

Piece<Board>* Board::getPiece(sf::Vector2f pos) {
	return this->getPiece(pos.x, pos.y);
}

void Board::setPiece(int x, int y, Piece<Board> * newPiece) {
	this->pieces[x][y] = newPiece;

	if (this->pieces[x][y] != nullptr)
		this->pieces[x][y]->setPosition(x, y);
}

void Board::setPiece(sf::Vector2f pos, Piece<Board> * newPiece) {
	this->setPiece(pos.x, pos.y, newPiece);
}

void Board::doFrame(sf::RenderWindow* window, sf::Vector2f lastClickedPiece, sf::Vector2f mouseVelocity, sf::Vector2f hoveredTile, sf::Cursor* cursor, sf::Vector2f mousePos, float framerate) {
	// Get the speed the mouse is moving from the velocity
	float speed = std::log(std::hypot(mouseVelocity.x, mouseVelocity.y) + 1) / 20.;

	// First play sounds
	// If we are holding a piece, do audio stuff
	if (lastClickedPiece.x >= 0) {
		this->slideSound.setVolume(50);

		this->slideSound.setPitch(this->slideSound.getPitch() + (speed*framerate/AUDIO_SLIDE_PITCH - this->slideSound.getPitch()) / (framerate/30.f));
	}

	// Set the mouse cursor
	// Ensure hovered tile is within the bounds
	if (hoveredTile.x >= 0 && hoveredTile.x < 8 && hoveredTile.y >= 0 && hoveredTile.y < 8) {
		// If we are not holding a piece...
		if (lastClickedPiece.x < 0) {
				// Set to pointer when hovering over piece
			if (this->getPiece(hoveredTile) != nullptr) {
				if (cursor->loadFromSystem(sf::Cursor::Hand))
					window->setMouseCursor(*cursor);
			} else {
				// Set to normal arrow when no longer hovering
				if (cursor->loadFromSystem(sf::Cursor::Arrow))
					window->setMouseCursor(*cursor);
			}
		
		} else {
				// If attempting to take piece of the same colour, show a notAllowed X
			if (this->getPiece(hoveredTile) != nullptr && this->getPiece(hoveredTile)->isPieceWhite() == this->getPiece(lastClickedPiece)->isPieceWhite()) {
				if (cursor->loadFromSystem(sf::Cursor::NotAllowed))
					window->setMouseCursor(*cursor);
			} else {
				// Set to normal drag arrow
				if (cursor->loadFromSystem(sf::Cursor::SizeAll))
				window->setMouseCursor(*cursor);
			}
		}
	}

	// Draw to the pieceOffset texture
	if (lastClickedPiece.x >= 0 && speed > 0) { // If holding a piece and it's being moved
		this->pieceOffsetShader.setUniform("texture", this->pieceOffsetTexture.getTexture());
		this->pieceOffsetShader.setUniform("mousePos", mousePos);
		this->pieceOffsetShader.setUniform("mouseVelocity", mouseVelocity);

		this->pieceOffsetTexture.draw(sf::Sprite(this->pieceOffsetTexture.getTexture()), &this->pieceOffsetShader);

		this->pieceOffsetTexture.display();
	}

	
}

// Retrieve the position of the board, storedi nthe position of the sprite texture
sf::Vector2f Board::getPosition() {
	return this->getBackground()->getPosition();
}

// Move the board to a new position
void Board::setPosition(sf::Vector2f newPos) {
	this->getBackground()->setPosition(newPos);

	// Update the pieces
	for (int i=0;i<sizeof(this->pieces)/sizeof(unsigned long /* Piece* */);i++) {
		
		int x = i % 8;
		int y = i / 8;

		Piece<Board>* piece = this->getPiece(x, y);

		if (piece != nullptr) {
			piece->updateBoardPos(newPos);
		}
	}
}


sf::Vector2i Board::getSize() {
	return this->size;
}

void Board::setSize(sf::Vector2i newSize) {
	this->size = newSize;
}

sf::Vector2f Board::mapShaderCoord(sf::Vector2f windowPos) {
	sf::Vector2f result = windowPos;

	// Translate
	//result -= this->getPosition();

	// Flip
	result.y = this->getSize().y - windowPos.y;

	return result;
}

sf::Vector2f Board::mapWindowPixelToBoard(sf::Vector2f windowPos) {
	sf::Vector2f result = windowPos;

	// Translate
	result -= this->getPosition();

	return result;
}

void Board::mousePressEvent(sf::Event event, sf::Vector2f &lastClickedPiece, sf::Vector2f &hoveredTile, sf::RenderWindow& window, sf::Cursor& cursor, sf::Vector2f& lastClickedPosition, sf::Vector2f& shaderMousePos) {
	// Check if left click
	if (event.mouseButton.button == sf::Mouse::Left) {
		// Store the board square pointer
		Piece<Board>* clickedPiece = this->getPiece(hoveredTile);

		if (clickedPiece != nullptr) {
// START OF DEBUG PRINT STUFF
			// sf::Vector2f hoveredTileDebug = hoveredTile;
			// hoveredTileDebug.y += 1;
			// Piece<Board>* clickedPieceDebug = board.getPiece(hoveredTileDebug);
			// std::cout << "KQBNRF"[clickedPieceDebug->getType()] << std::endl;
			std::cout << clickedPiece->getValidSquares(this).size() << " available spaces to move to" << std::endl;
// END OF DEBUG PRINT STUFF

			lastClickedPiece = hoveredTile;

			// Set the cursor to drag around the piece
			if (cursor.loadFromSystem(sf::Cursor::SizeAll))
				window.setMouseCursor(cursor);

			// Set the last clicked position to the mouse position
			lastClickedPosition = shaderMousePos;//sf::Vector2f(mousePos.x / window.getSize().x, mousePos.y / window.getSize().y);
			this->pieceShader.setUniform("lastClickedPos", lastClickedPosition);

			this->slideSound.play();
		}

		
	}
}

void Board::mouseReleaseEvent(sf::Event event, sf::Vector2f& hoveredTile, sf::Vector2f& lastClickedPiece) {
	// Check if left release
	if (event.mouseButton.button == sf::Mouse::Left) {
		Piece<Board>* oldPiece = this->getPiece(lastClickedPiece);
		Piece<Board>* newPiece = this->getPiece(hoveredTile);

		// Get the valid squares for the held piece
		bool movingToValid = false;
		if (oldPiece != nullptr) {
			std::vector<sf::Vector2i> validSquares = oldPiece->getValidSquares(this);
			
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
				this->takeSound.play();

				// if took, print
				//gameMovementsLog += 'x';
			}

			
			this->setPiece(hoveredTile, oldPiece);

			// Clear the reference to this piece from the old position
			this->setPiece(lastClickedPiece, nullptr);

			// Print location
			// Rank
			//gameMovementsLog += "abcdefgh"[(int)(hoveredTile.x+0.2)];
			// Row
			//gameMovementsLog += std::to_string(8 - (int)(hoveredTile.y+0.2));

			//std::cout << gameMovementsLog << std::endl;

		}

		// Reset, no longer holding piece
		lastClickedPiece = sf::Vector2f(-1, -1);
		this->pieceShader.setUniform("lastClickedPos", lastClickedPiece);

		this->slideSound.stop();
	}
}