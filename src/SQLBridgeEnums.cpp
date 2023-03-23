#include "SQLBridgeEnums.hpp"

#include <stdexcept>
#include <string>
#include <unordered_map>

SQLBridgeEnum::TrainingLevel SQLBridgeEnum::TrainingLevelFromString(std::string level)
{
	static const std::unordered_map<std::string, TrainingLevel> TrainingLevelStrToEnum = {
		{"untrained", TrainingLevel::untrained},
		{"partial", TrainingLevel::partial},
		{"fully", TrainingLevel::fully},
		{"canTrain", TrainingLevel::canTrain}};

	static auto it = TrainingLevelStrToEnum.find(level);
	if (it != TrainingLevelStrToEnum.end()) { return it->second; }
	else { throw std::invalid_argument("String Not Enum" + level); }
}

SQLBridgeEnum::PersonType SQLBridgeEnum::PersonTypeFromString(std::string type)
{
	static const std::unordered_map<std::string, PersonType> PersonTypeStrToEnum = {{"student", PersonType::student},
	                                                                                {"faculty", PersonType::faculty},
	                                                                                {"staff", PersonType::staff},
	                                                                                {"other", PersonType::other}};

	static auto it = PersonTypeStrToEnum.find(type);
	if (it != PersonTypeStrToEnum.end()) { return it->second; }
	else { return PersonType::other; }
}

SQLBridgeEnum::Machine SQLBridgeEnum::MachineFromString(std::string machine)
{
	static const std::unordered_map<std::string, Machine> MachineStrToEnum = {
		{"laser", Machine::laser},       {"d3_printers", Machine::d3_printers}, {"hand_tools", Machine::hand_tools},
		{"woodshop", Machine::woodshop}, {"embroidery", Machine::embroidery},   {"shopbot", Machine::shopbot},
		{"vinyl", Machine::vinyl}};

	static auto it = MachineStrToEnum.find(machine);
	if (it != MachineStrToEnum.end()) { return it->second; }
	else { throw std::invalid_argument("Invalid Machine: " + machine); }
}

std::string SQLBridgeEnum::TrainingLevelToString(SQLBridgeEnum::TrainingLevel level)
{
	static const std::array<std::string, 4> LevelStr = {"untrained", "partial", "fully", "canTrain"};
	return LevelStr.at(static_cast<int>(level));
}

std::string SQLBridgeEnum::PersonTypeToString(SQLBridgeEnum::PersonType type)
{
	static const std::array<std::string, 4> TypeStr = {"student", "faculty", "staff", "other"};
	return TypeStr.at(static_cast<int>(type));
}

std::string SQLBridgeEnum::MachineToString(SQLBridgeEnum::Machine machine)
{
	static const std::array<std::string, 7> MachineStr = {"laser",      "d3_printers", "hand_tools", "woodshop",
	                                                      "embroidery", "shopbot",     "vinyl"};
	return MachineStr.at(static_cast<int>(machine));
}
