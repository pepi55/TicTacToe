#pragma once

#include <string>

class Player {
	public:
		Player(void);
		~Player(void);

		void setName(std::string name);
		std::string getName(void);

	private:
		std::string _name;
};
