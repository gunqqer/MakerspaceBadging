#include "Badging.hpp"
#include "SQLBridge.hpp"
#include "SQLBridgeEnums.hpp"

#include <cstdint>
#include <exception>
#include <iostream>

void Badging::run()
{

	bool STOPPROGRAM = false;
	while (!STOPPROGRAM)
	{
		int userSelection = menu::selection(mainMenu.first, mainMenu.second);
		switch (userSelection)
		{
		case 0:
			scanID();
			break;
		case 1:
			search();
			break;
		case 2:
			STOPPROGRAM = true;
			break;
		}
	}

	// Do any cleanup here if needed
}

void Badging::scanID()
{
	std::optional<std::string> uuid = bridge.getUUID(menu::inputID());
	if (uuid.has_value())
	{
		updateLastScan(uuid.value());
		std::optional<SQLBridge::userData> data = bridge.getUserData(uuid.value());
		if (data.has_value()) { std::cout << data.value(); }
		else
		{
			return; // TODO have proper failure
		}

		bool validSelection = false;
		while (!validSelection)
		{
			int userSelection = menu::selection(IDExistsMenu.first, IDExistsMenu.second);
			switch (userSelection)
			{
			case -1:
				std::cout << "Invalid selection\n";
				continue;
			case 0:
				updateInfo(uuid.value());
				validSelection = true;
				break;
			case 1:
				validSelection = true;
				return;
			}
		}
	}
	else {}
}

inline void Badging::search()
{
	int userSelection = menu::selection(searchMenu.first, searchMenu.second);
}

void Badging::updateLastScan(std::string uuid)
{
	bridge.updateScanDate(uuid);
}

void Badging::updateInfo(std::string uuid)
{
	bool isDone = false;
	while (!isDone)
	{
		int selection = menu::selection(machines.first, machines.second);
		if (selection < 0)
		{
			std::cout << "Invalid selection, try again\n";
			continue;
		}
		SQLBridgeEnum::Machine machine = static_cast<SQLBridgeEnum::Machine>(selection);

		std::optional<SQLBridge::trainingData> odata = bridge.getTraining(uuid, machine);
		SQLBridge::trainingData data;
		if (odata.has_value())
		{
			std::cout << odata.value();
			data = odata.value();
		}
		else { data.uuid = uuid; } // TODO make this handle failure better

		SQLBridge::trainingData newData;
		bool validData = false;
		while (!validData)
		{
			// Set training level TODO put these strings somewhere else, maybe dynamicly generate instead of hard coding
			int trainingLevelSelection = menu::selection(
				"Select training level for " + SQLBridgeEnum::MachineToString(machine),
				{"Untrained (default)", "Partially trained", "Fully trained", "Able to train others (staff)"});
			// Set training date, default to today TODO actually default
			std::string date = menu::inputDate();

			// loop through all others
			for (int i = 0; i < data.otherInfo.size(); i++)
			{
				newData.otherInfo.at(i) = data.otherInfo.at(i); // Copy everything other we need
				newData.otherInfo.at(i).second = menu::inputBool(data.otherInfo.at(i).first).value_or(false);
			}
		}
	}
}
