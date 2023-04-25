#include "Badging.hpp"

#include <cstdint>
#include <exception>
#include <mariadb/conncpp/SQLString.hpp>
#include <mariadb/conncpp/jdbccompat.hpp>

void Badging::run()
{

	bool STOPPROGRAM = false;
	while (!STOPPROGRAM)
	{
		int userSelection = menu::selection(mainMenu.first, mainMenu.second);
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
	if (uuid.has_value()) { updateLastScan(uuid.value()); }
	else {}
}

void Badging::search()
{
	int userSelection = menu::selection(searchMenu.first, searchMenu.second);
}
