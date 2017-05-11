#include "board.h"
#include "piece.h"

#include <iostream>

PieceResources::PieceResources(std::string directory) : directory(directory) {
	textures[(int) PieceID::WHITE_PAWN].loadFromFile(directory + "pawnw.png");
	textures[(int) PieceID::BLACK_PAWN].loadFromFile(directory + "pawnb.png");
	textures[(int) PieceID::WHITE_ROOK].loadFromFile(directory + "rookw.png");
	textures[(int) PieceID::BLACK_ROOK].loadFromFile(directory + "rookb.png");
	textures[(int) PieceID::WHITE_KNIGHT].loadFromFile(directory + "knightw.png");
	textures[(int) PieceID::BLACK_KNIGHT].loadFromFile(directory + "knightb.png");
	textures[(int) PieceID::WHITE_BISHOP].loadFromFile(directory + "bishopw.png");
	textures[(int) PieceID::BLACK_BISHOP].loadFromFile(directory + "bishopb.png");
	textures[(int) PieceID::WHITE_QUEEN].loadFromFile(directory + "queenw.png");
	textures[(int) PieceID::BLACK_QUEEN].loadFromFile(directory + "queenb.png");
	textures[(int) PieceID::WHITE_KING].loadFromFile(directory + "kingw.png");
	textures[(int) PieceID::BLACK_KING].loadFromFile(directory + "kingb.png");
}

const sf::Texture& PieceResources::getTexture(PieceID id) const {
	return textures[(int) id];
}

Board::Board() : mouseDown(false), turn(0) {
	sf::Image img;

#define W 0xFFA4C8E0
#define B 0xFF5277A4
#define ROW(a, b) a, b, a, b, a, b, a, b,

	static const int BOARD_TEXTURE[64] = {
		ROW(W, B)
		ROW(B, W)
		ROW(W, B)
		ROW(B, W)
		ROW(W, B)
		ROW(B, W)
		ROW(W, B)
		ROW(B, W)
	};

#undef ROW
#undef B
#undef W

	img.create(8, 8, (unsigned char *)BOARD_TEXTURE);
	boardTexture.loadFromImage(img);
	boardTexture.setSmooth(false);
	boardSprite.setTexture(boardTexture);
	boardSprite.setScale(640 / 8, 480 / 8);
}


Board::~Board() {
	for (int i = 0; i < 64; ++i) {
		ChessPiece *p = pieces[i];
		if (p) delete p;
	}
}

void Board::update(sf::Window& wnd, float dt) {
	boardSprite.setPosition(Transformable::getPosition());

	boardSprite.setScale(Transformable::getScale().x / 8,
						 Transformable::getScale().y / 8);

	sf::Vector2i mouse = sf::Mouse::getPosition(wnd);
	sf::Vector2f scale = getScale();
	sf::Vector2f pos = getPosition();

	using namespace std;

	if (mouse.x > pos.x && mouse.x < pos.x + scale.x &&
		mouse.y > pos.y && mouse.y < pos.y + scale.y) {

		float rx = mouse.x - pos.x;
		float ry = mouse.y - pos.y;

		float nx = rx / scale.x;
		float ny = ry / scale.y;

		mousePosition = sf::Vector2f(nx * scale.x, ny * scale.y);

		int x = static_cast<int>(nx * 8);
		int y = static_cast<int>(ny * 8);
		sf::Vector2i tile = sf::Vector2i(x, y);

		if (selectedPiece && tile != grabPosition) {
			dragging = true;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (!mouseDown) {
				auto piece = get(tile);

				if (piece && (int)piece->getColor() == turn % 2) {
					selectedPiece = piece;
					grabPosition = tile;
					grabDelta = sf::Vector2f((nx * 8.f - x) * scale.x / 8.f,
									     	 (ny * 8.f - y) * scale.y / 8.f);
				}
			}
			mouseDown = true;
		}
		else {
			if (mouseDown) {
				cout << "drop? " << "x: " << x << ", y: " << y << endl;
				if (dragging) {
					if (selectedPiece) {
						if (legalMove(selectedPiece, x, y)) {
							turn++;
							selectedPiece->setPosition(this, sf::Vector2i(x, y), false);
						}
					}
					
					// todo: släpp pjäs, försök drag
					selectedPiece = nullptr;
					dragging = false;
				}
				else {
					dragging = true;
				}
			}
			mouseDown = false;
		}
	}

	for (int i = 0; i < 64; ++i) {
		ChessPiece *piece = pieces[i];
		
		if (piece) piece->update(dt);
	}
}

void Board::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	// stuff
	target.draw(boardSprite);

	if (selectedPiece) {
		std::vector<sf::Vector2i> possibleMoves;
		selectedPiece->getPossibleMoves(possibleMoves);
		
		for (auto tile : possibleMoves) {
			using namespace std;
			//cout << tile.x << ", " << tile.y << endl;

			sf::RectangleShape rect(sf::Vector2f(1, 1));
			rect.setFillColor(sf::Color(255, 45, 45, 160));
			rect.setScale(getScale() / 8.f);

			sf::Vector2f pos = sf::Vector2f(getScale().x * tile.x,
											getScale().y * tile.y) / 8.f;

			rect.setPosition(getPosition() + pos);

			target.draw(rect);
		}
	}

	for (int i = 0; i < 64; ++i) {
		ChessPiece *piece = pieces[i];
		if (piece) {
			int x = i % 8;
			int y = i / 8;

			if (piece == selectedPiece) {
				((sf::Transformable*)piece)->setPosition(getPosition() + mousePosition - grabDelta);
			}
			else {
				sf::Vector2f pos = sf::Vector2f(getScale().x * x,
												getScale().y * y) / 8.f;

				((sf::Transformable*)piece)->setPosition(getPosition() + pos);
			}

			piece->setScale(getScale() / 8.f / 60.f);
			piece->draw(target, states);
		}
	}
}

bool Board::inBounds(sf::Vector2i pos) {
	return pos.x >= 0 && pos.x < 8 && pos.y >= 0 && pos.y < 8;
}

void Board::putPiece(ChessPiece *piece) {
	ChessPiece *p = get(piece->getPosition());
	if (p) {
		if (p == selectedPiece) selectedPiece = nullptr;

		delete p;
	}

	pieces[piece->getPosition().x + 8 * piece->getPosition().y] = piece;
}

void Board::setPiece(int x, int y, ChessPiece *piece)
{
	pieces[x + 8 * y] = piece;
}

bool Board::legalMove(ChessPiece *piece, int x, int y)
{
	std::vector<sf::Vector2i> moves;
	piece->getPossibleMoves(moves);

	bool posLegal = false;
	for (auto pos : moves) {
		if (sf::Vector2i(x, y) == pos) {
			posLegal = true;
			break;
		}
	}

	if (posLegal) {
		return !kingVulnerable(piece->getColor(), piece, x, y);
	}
	else {
		return false;
	}
}

bool Board::kingVulnerable(TeamColor color, ChessPiece *piece, int x, int y)
{
	ChessPiece *king = nullptr;
	for (int i = 0; i < 64; ++i) {
		ChessPiece *ppiece = pieces[i];
		if (ppiece && ppiece->getColor() == color && (ppiece->getPieceID() == PieceID::WHITE_KING || ppiece->getPieceID() == PieceID::BLACK_KING)) {
			king = ppiece;
			break;
		}
	}

	if (!king)
		return false;

	auto oldpos = piece->getPosition();
	auto taken = piece->setPosition(this, sf::Vector2i(x, y));

	std::vector<sf::Vector2i> moves;
	for (int i = 0; i < 64; ++i) {
		ChessPiece *ppiece = pieces[i];
		if (ppiece && ppiece->getColor() != color) {
			moves.clear();
			ppiece->getPossibleMoves(moves);

			for (auto pos : moves) {
				if (pos == king->getPosition()) {
					piece->setPosition(this, oldpos);
					if (taken)
						taken->setPosition(this, sf::Vector2i(x, y));

					return true;
				}
			}
		}
	}

	piece->setPosition(this, oldpos);
	if (taken)
		taken->setPosition(this, sf::Vector2i(x, y));

	return false;
}

ChessPiece *Board::get(sf::Vector2i pos) {
	return pieces[pos.x + 8 * pos.y];
}