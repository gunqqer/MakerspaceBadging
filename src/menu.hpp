#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "SQLBridgeEnums.hpp"

//Purely static class to parse user input, only takes 1 line of input
class menu
{
	public:
		//Returns zero-indexed selection from vector of options, or -1 on any failures
		static int selection(const std::string& title, const std::vector<std::string>& options);
		//Returns int from user, negative values indicate failures
		static int inputInt(const std::string& title);
		//Returns string from user, unchecked as names can be anything. Returns empty string on failure
		static std::string inputName();
		//Returns string from user, checking date formating YYYY-MM-DD, returns empty on failure
		static std::string inputDate();
		static bool inputBool(const std::string& title);
		
		//Returns 16 length ID, checking for issues, returns 0 on failure
		static std::uint64_t inputID();
};
