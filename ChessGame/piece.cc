#include "piece.h"
#include "board.h"

ChessPiece::ChessPiece(Board *const parent, sf::Vector2i position, TeamColor color)
	: parent(parent), position(position), color(color) {}


ChessPiece::~ChessPiece() {}

void ChessPiece::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(sprite);
}

void ChessPiece::update(float dt) {
	sprite.setPosition(Transformable::getPosition());
	sprite.setScale(Transformable::getScale());
}

void ChessPiece::move(sf::Vector2i to) {
	moves++;
	position = to;
}

bool ChessPiece::sameColor(ChessPiece *other) const {
	return color == other->color;
}

Board *const ChessPiece::getBoard() const {
	return parent;
}

sf::Sprite ChessPiece::getSprite() const {
	return sprite;
}

void ChessPiece::setSprite(sf::Sprite other) {
	sprite = other;
}

sf::Vector2i ChessPiece::getPosition() const {
	return position;
}

ChessPiece *ChessPiece::setPosition(Board *board, sf::Vector2i pos, bool fake)
{
	board->setPiece(position.x, position.y, nullptr);

	position = pos;
	if (!fake)
		move(pos);
	auto taken = board->get(pos);
	board->setPiece(pos.x, pos.y, this);
	return taken;
}

int ChessPiece::getMoves() const { return moves; }

TeamColor ChessPiece::getColor() const { return color; }

// Pawn

Pawn::Pawn(Board *const parent, PieceResources *const resources, sf::Vector2i position, TeamColor color)
	: ChessPiece(parent, position, color), enPassant(true) {
	setSprite(sf::Sprite(resources->getTexture(getPieceID())));
}

void Pawn::move(sf::Vector2i to) {
	sf::Vector2i position = ChessPiece::getPosition();
	sf::Vector2i d = to - position;
	enPassant = false;
	// todo: eller bara kolla # moves

	ChessPiece::move(to);
}

PieceID Pawn::getPieceID() const {
	return getColor() == TeamColor::BLACK ? PieceID::BLACK_PAWN : PieceID::WHITE_PAWN;
}

void Pawn::getPossibleMoves(std::vector<sf::Vector2i> &out) const {
	int dy = getColor() == TeamColor::WHITE ? -1 : 1;

	sf::Vector2i position = ChessPiece::getPosition();
	Board const* parent = ChessPiece::getBoard();

	sf::Vector2i forward(position.x, position.y + 1 * dy);
	if (getBoard()->inBounds(forward) && !getBoard()->get(forward)) {
		out.push_back(forward);
	}

	sf::Vector2i passade(position.x, position.y + 2 * dy);
	if (getBoard()->inBounds(passade) && !getBoard()->get(passade) && enPassant) {
		out.push_back(passade);
	}

	sf::Vector2i attackLeft(position.x - 1, position.y + 1 * dy);
	auto al = getBoard()->get(attackLeft);
	if (getBoard()->inBounds(attackLeft) && al && !sameColor(al)) {
		out.push_back(attackLeft);
	}

	sf::Vector2i attackRight(position.x + 1, position.y + 1 * dy);
	auto ar = getBoard()->get(attackRight);
	if (getBoard()->inBounds(attackRight) && ar && !sameColor(ar)) {
		out.push_back(attackRight);
	}
}

// Rook

Rook::Rook(Board *const parent, PieceResources *const resources, sf::Vector2i position, TeamColor color)
: ChessPiece(parent, position, color) {
	setSprite(sf::Sprite(resources->getTexture(getPieceID())));
}

PieceID Rook::getPieceID() const {
	return getColor() == TeamColor::BLACK ? PieceID::BLACK_ROOK : PieceID::WHITE_ROOK;
}

void Rook::getPossibleMoves(std::vector<sf::Vector2i> &out) const {
	const sf::Vector2i directions[4] = {
		{ 1, 0 },
		{ 0, 1 },
		{ -1, 0 },
		{ 0, -1 },
	};

	for (auto dir : directions) {
		sf::Vector2i pos = getPosition() + dir;
		while (getBoard()->inBounds(pos)) {
			if (getBoard()->get(pos)) {
				if (!sameColor(getBoard()->get(pos))) {
					out.push_back(pos);
				}
				break;
			}
			else {
				out.push_back(pos);
			}
			pos += dir;
		}
	}
}

// Knight

Knight::Knight(Board *const parent, PieceResources *const resources, sf::Vector2i position, TeamColor color)
: ChessPiece(parent, position, color) {
	setSprite(sf::Sprite(resources->getTexture(getPieceID())));
}

PieceID Knight::getPieceID() const {
	return getColor() == TeamColor::BLACK ? PieceID::BLACK_KNIGHT : PieceID::WHITE_KNIGHT;
}

void Knight::getPossibleMoves(std::vector<sf::Vector2i> &out) const {
	const sf::Vector2i positions[8] = {
		{  1,  2 },
		{ -1,  2 },
		
		{  2,  1 },
		{  2, -1 },

		{  1, -2 },
		{ -1, -2 },

		{ -2,  1 },
		{ -2, -1 },
	};

	for (int i = 0; i < 8; ++i) {
		sf::Vector2i vec = positions[i] + getPosition();

		if (getBoard()->inBounds(vec)) {
			if (getBoard()->get(vec)) {
				if (!sameColor(getBoard()->get(vec))) {
					out.push_back(vec);
				}
			}
			else {
				out.push_back(vec);
			}
		}
	}
}

// Bishop

Bishop::Bishop(Board *const parent, PieceResources *const resources, sf::Vector2i position, TeamColor color)
: ChessPiece(parent, position, color) {
	setSprite(sf::Sprite(resources->getTexture(getPieceID())));
}

PieceID Bishop::getPieceID() const {
	return getColor() == TeamColor::BLACK ? PieceID::BLACK_BISHOP : PieceID::WHITE_BISHOP;
}

void Bishop::getPossibleMoves(std::vector<sf::Vector2i> &out) const {
	const sf::Vector2i directions[4] = {
		{  1,  1 },
		{ -1,  1 },
		{ -1, -1 },
		{  1, -1 },
	};
	
	for (auto dir : directions) {
		sf::Vector2i pos = getPosition() + dir;
		while (getBoard()->inBounds(pos)) {
			if (getBoard()->get(pos)) {
				if (!sameColor(getBoard()->get(pos))) {
					out.push_back(pos);
				}
				break;
			}
			else {
				out.push_back(pos);
			}
			pos += dir;
		}
	}
}

// Queen

Queen::Queen(Board *const parent, PieceResources *const resources, sf::Vector2i position, TeamColor color)
: ChessPiece(parent, position, color) {
	setSprite(sf::Sprite(resources->getTexture(getPieceID())));
}

PieceID Queen::getPieceID() const {
	return getColor() == TeamColor::BLACK ? PieceID::BLACK_QUEEN : PieceID::WHITE_QUEEN;
}

void Queen::getPossibleMoves(std::vector<sf::Vector2i> &out) const {
	const sf::Vector2i directions[8] = {
		{  1,  0 },
		{  0,  1 },
		{ -1,  0 },
		{  0, -1 },
		{  1,  1 },
		{ -1,  1 },
		{ -1, -1 },
		{  1, -1 },
	};

	for (auto dir : directions) {
		sf::Vector2i pos = getPosition() + dir;
		while (getBoard()->inBounds(pos)) {
			if (getBoard()->get(pos)) {
				if (!sameColor(getBoard()->get(pos))) {
					out.push_back(pos);
				}
				break;
			}
			else {
				out.push_back(pos);
			}
			pos += dir;
		}
	}
}

// King

King::King(Board *const parent, PieceResources *const resources, sf::Vector2i position, TeamColor color)
: ChessPiece(parent, position, color) {
	setSprite(sf::Sprite(resources->getTexture(getPieceID())));
}

PieceID King::getPieceID() const {
	return getColor() == TeamColor::BLACK ? PieceID::BLACK_KING : PieceID::WHITE_KING;
}

void King::getPossibleMoves(std::vector<sf::Vector2i> &out) const {
	const sf::Vector2i directions[8] = {
		{  1,  0 },
		{  0,  1 },
		{ -1,  0 },
		{  0, -1 },
		{  1,  1 },
		{ -1,  1 },
		{ -1, -1 },
		{  1, -1 },
	};

	for (auto dir : directions) {
		sf::Vector2i pos = getPosition() + dir;
		if (getBoard()->inBounds(pos)) {
			if (getBoard()->get(pos)) {
				if (!sameColor(getBoard()->get(pos))) {
					out.push_back(pos);
				}
			}
			else {
				out.push_back(pos);
			}
		}
		pos += dir;
	}
}

