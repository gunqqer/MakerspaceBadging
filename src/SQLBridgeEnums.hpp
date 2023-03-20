#pragma once

#include <array>
#include <string>

#include <mariadb/conncpp/SQLString.hpp>

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

		static std::string TrainingLevelToString(TrainingLevel level);
		static std::string PersonTypeToString(PersonType type);
		static std::string MachineToString(Machine machine);

		static TrainingLevel TrainingLevelFromString(std::string level);
		inline static TrainingLevel TrainingLevelFromString(sql::SQLString level){return TrainingLevelFromString(static_cast<std::string>(level));}
		static PersonType PersonTypeFromString(std::string type);
		inline static PersonType PersonTypeFromString(sql::SQLString type){return PersonTypeFromString(static_cast<std::string>(type));}
		static Machine MachineFromString(std::string machine);
		inline static Machine MachineFromString(sql::SQLString machine){return MachineFromString(static_cast<std::string>(machine));}
};
