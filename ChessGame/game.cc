#include "game.h"
#include "piece.h"

Game::Game() {
	board.setPosition(20, 20);
	board.setScale(480, 480);

	resources = new PieceResources("Resources/");

	board.putPiece(new King(&board, resources, sf::Vector2i(4, 1), TeamColor::BLACK));
	board.putPiece(new Queen(&board, resources, sf::Vector2i(5, 4), TeamColor::BLACK));
	board.putPiece(new Knight(&board, resources, sf::Vector2i(3, 3), TeamColor::BLACK));
	board.putPiece(new Rook(&board, resources, sf::Vector2i(5, 2), TeamColor::BLACK));
	
	board.putPiece(new King(&board, resources, sf::Vector2i(3, 7), TeamColor::WHITE));
	board.putPiece(new Bishop(&board, resources, sf::Vector2i(2, 4), TeamColor::WHITE));
	board.putPiece(new Pawn(&board, resources, sf::Vector2i(0, 6), TeamColor::WHITE));
	board.putPiece(new Pawn(&board, resources, sf::Vector2i(1, 6), TeamColor::WHITE));
	board.putPiece(new Pawn(&board, resources, sf::Vector2i(2, 6), TeamColor::WHITE));
	board.putPiece(new Pawn(&board, resources, sf::Vector2i(3, 6), TeamColor::WHITE));
	board.putPiece(new Pawn(&board, resources, sf::Vector2i(4, 6), TeamColor::WHITE));
	board.putPiece(new Pawn(&board, resources, sf::Vector2i(5, 6), TeamColor::WHITE));
	board.putPiece(new Pawn(&board, resources, sf::Vector2i(6, 6), TeamColor::WHITE));
	board.putPiece(new Pawn(&board, resources, sf::Vector2i(7, 6), TeamColor::WHITE));
}


Game::~Game() {
	delete resources;
}

void Game::update(sf::Window& wnd, float dt) {
	board.update(wnd, dt);
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	board.draw(target, states);
}

void Game::resize(sf::Event::SizeEvent& evt) {
	int size = evt.height - 40;
	
	board.setScale(size, size);
}