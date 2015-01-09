#include "Player.hpp"

Player::Player(void) {
	_name = "Player";
}

Player::~Player(void) {
}

void Player::setName(std::string name) {
	_name = name;
}

std::string Player::getName(void) {
	return _name;
}
