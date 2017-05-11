#pragma once

#include <SFML/Graphics.hpp>

class Board;
class PieceResources;

enum class TeamColor {
	WHITE = 0,
	BLACK
};

// för serialisering senare
enum class PieceID {
	WHITE_PAWN = 0,
	BLACK_PAWN,
	WHITE_ROOK,
	BLACK_ROOK,
	WHITE_KNIGHT,
	BLACK_KNIGHT,
	WHITE_BISHOP,
	BLACK_BISHOP,
	WHITE_QUEEN,
	BLACK_QUEEN,
	WHITE_KING,
	BLACK_KING
};

class ChessPiece : public sf::Drawable, public sf::Transformable {
public:
	ChessPiece(Board *const parent, sf::Vector2i position, TeamColor color);
	virtual ~ChessPiece();

	virtual void update(float dt);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void getPossibleMoves(std::vector<sf::Vector2i> &out) const = 0;
	virtual PieceID getPieceID() const = 0;

	virtual void move(sf::Vector2i to);

	bool sameColor(ChessPiece *other) const;
	Board *const getBoard() const;
	sf::Sprite getSprite() const;
	void setSprite(sf::Sprite sprite);
	sf::Vector2i getPosition() const;
	ChessPiece *setPosition(Board *board, sf::Vector2i pos, bool fake = true);
	int getMoves() const;
	TeamColor getColor() const;
private:
	Board *const parent;
	sf::Sprite sprite;
	sf::Vector2i position;
	int moves;
	TeamColor color;
};

class Pawn : public ChessPiece {
public:
	Pawn(Board *const parent, PieceResources *const resources, sf::Vector2i position, TeamColor color);

	void move(sf::Vector2i to) override;
	void getPossibleMoves(std::vector<sf::Vector2i> &out) const override;
	PieceID getPieceID() const override;
private:
	bool enPassant;
};

class Rook : public ChessPiece {
public:
	Rook(Board *const parent, PieceResources *const resources, sf::Vector2i position, TeamColor color);

	void getPossibleMoves(std::vector<sf::Vector2i> &out) const override;
	PieceID getPieceID() const override;
};

class Knight : public ChessPiece {
public:
	Knight(Board *const parent, PieceResources *const resources, sf::Vector2i position, TeamColor color);

	void getPossibleMoves(std::vector<sf::Vector2i> &out) const override;
	PieceID getPieceID() const override;
};

class Bishop : public ChessPiece {
public:
	Bishop(Board *const parent, PieceResources *const resources, sf::Vector2i position, TeamColor color);

	void getPossibleMoves(std::vector<sf::Vector2i> &out) const override;
	PieceID getPieceID() const override;
};

class Queen : public ChessPiece {
public:
	Queen(Board *const parent, PieceResources *const resources, sf::Vector2i position, TeamColor color);

	void getPossibleMoves(std::vector<sf::Vector2i> &out) const override;
	PieceID getPieceID() const override;
};

class King : public ChessPiece {
public:
	King(Board *const parent, PieceResources *const resources, sf::Vector2i position, TeamColor color);

	void getPossibleMoves(std::vector<sf::Vector2i> &out) const override;
	PieceID getPieceID() const override;
};
