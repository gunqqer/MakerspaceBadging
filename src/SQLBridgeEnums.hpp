#pragma once

#include <array>
#include <string>

class SQLBridgeEnum
{
	public:
		enum class TrainingLevel
		{
			untrained,
			partial,
			fully,
			canTrain
		};

		enum class PersonType
		{
			student,
			faculty,
			staff,
			other
		};
		// Can't start it with a number so it's d3 instead of 3d
		enum class Machine
		{
			laser,
			d3_printers,
			hand_tools,
			woodshop,
			embroidery,
			shopbot,
			vinyl
		};

		static std::string TrainingLevelToString(TrainingLevel& level);
		static std::string PersonTypeToString(PersonType& type);
		static std::string MachineToString(Machine& machine);

		static TrainingLevel TrainingLevelFromString(std::string& level);
		static PersonType PersonTypeFromString(std::string& type);
		static Machine MachineFromString(std::string& machine);

	private:
		static constexpr const std::array<std::string_view, 6> MachineStr = {"laser",    "3d_printers", "hand_tools",
		                                                                     "woodshop", "embroidery",  "shopbot"};
};
