#include "SQLBridgeEnums.hpp"

#include <stdexcept>
#include <unordered_map>

SQLBridgeEnum::TrainingLevel SQLBridgeEnum::TrainingLevelFromString(std::string &level)
{
	static const std::unordered_map<std::string_view, TrainingLevel> TrainingLevelStrToEnum = {
		{"untrained", TrainingLevel::untrained},
		{"partial", TrainingLevel::partial},
		{"fully", TrainingLevel::fully},
		{"canTrain", TrainingLevel::canTrain}};

	auto it = TrainingLevelStrToEnum.find(level);
	if (it != TrainingLevelStrToEnum.end()) { return it->second; }
	else { throw std::invalid_argument("String Not Enum" + level); }
}

SQLBridgeEnum::PersonType SQLBridgeEnum::PersonTypeFromString(std::string &type)
{
	static const std::unordered_map<std::string_view, PersonType> PersonTypeStrToEnum = {
		{"student", PersonType::student},
		{"faculty", PersonType::faculty},
		{"staff", PersonType::staff},
		{"other", PersonType::other}};

	auto it = PersonTypeStrToEnum.find(type);
	if (it != PersonTypeStrToEnum.end()) { return it->second; }
	else { return PersonType::other; }
}

SQLBridgeEnum::Machine SQLBridgeEnum::MachineFromString(std::string &machine)
{
	static const std::unordered_map<std::string_view, Machine> MachineStrToEnum = {
		{"laser", Machine::laser},       {"d3_printers", Machine::d3_printers}, {"hand_tools", Machine::hand_tools},
		{"woodshop", Machine::woodshop}, {"embroidery", Machine::embroidery},   {"shopbot", Machine::shopbot},
		{"vinyl", Machine::vinyl}};

	auto it = MachineStrToEnum.find(machine);
	if (it != MachineStrToEnum.end()) { return it->second; }
	else { throw std::invalid_argument("Invalid Machine: " + machine); }
}
