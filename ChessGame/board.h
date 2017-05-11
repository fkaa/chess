#pragma once

#include <SFML/Graphics.hpp>
#include "piece.h"

class ChessPiece;
enum class PieceID;

class PieceResources {
public:
	PieceResources(std::string directory);
	const sf::Texture& getTexture(PieceID id) const;
private:
	std::string directory;
	sf::Texture textures[12];
};

class Board : public sf::Drawable, public sf::Transformable {
public:
	Board();
	~Board();

	void update(sf::Window& wnd, float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool inBounds(sf::Vector2i pos);
	void putPiece(ChessPiece *piece);
	void setPiece(int x, int y, ChessPiece *piece);
	bool legalMove(ChessPiece *piece, int x, int y);
	bool kingVulnerable(TeamColor color, ChessPiece *piece, int x, int y);
	ChessPiece* get(sf::Vector2i pos);
private:
	ChessPiece *pieces[64];
	ChessPiece *selectedPiece;
	int turn;

	sf::Vector2f grabDelta;
	sf::Vector2i grabPosition;
	sf::Vector2f mousePosition;
	bool mouseDown;
	bool dragging;

	sf::Texture boardTexture;
	sf::Sprite boardSprite;
};

