#include "Badging.hpp"

#include <cstdint>
#include <exception>
#include <mariadb/conncpp/SQLString.hpp>
#include <mariadb/conncpp/jdbccompat.hpp>

void Badging::run()
{

	int userSelection = -1;
	bool STOPPROGRAM = false;
	while (!STOPPROGRAM)
	{
		userSelection = menu::selection(mainMenu.first, mainMenu.second);
		switch (userSelection)
		{
		case 1:
			scanID();
			break;
		case 2:
			search();
			break;
		case 3:
			STOPPROGRAM = true;
			break;
		}
	}

	// Do any cleanup here if needed
}

void Badging::scanID()
{
	std::optional<std::string> uuid = bridge.getUUID(menu::inputID());
	if (uuid.has_value()) {}
	else {}
}
