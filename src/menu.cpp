#include "menu.hpp"

#include <iostream>
#include <limits>
#include <regex>
#include <string>
#include <vector>

int menu::selection(const std::string &title, const std::vector<std::string> &options)
{
	std::cout << title << std::endl;
	for (int i = 0; i < options.size(); i++)
	{
		std::cout << i << ". " << options[i] << std::endl;
	}

	std::string input;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.clear();
	std::getline(std::cin, input);

	int selection = -1;
	try
	{
		selection = std::stoi(input);
	}
	catch (...)
	{
		return -1;
	}

	// Check if the selection is valid
	if (selection >= 0 && selection < options.size()) { return selection; }
	else { return -1; }
}

int menu::inputInt(const std::string &title)
{
	std::cout << title << "\n> ";

	std::string input;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.clear();
	std::getline(std::cin, input);

	int value = -1;
	try
	{
		value = std::stoi(input);
	}
	catch (...)
	{
		return -1;
	}

	return value;
}

std::string menu::inputName()
{
	std::cout << "Input name\n> ";

	// Read the user's input
	std::string name;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.clear();
	std::getline(std::cin, name);

	return name;
}

std::string menu::inputDate()
{
	std::cout << "Input date (YYYY-MM-DD)\n> ";

	std::string date;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.clear();
	std::getline(std::cin, date);

	// Check if the input matches the expected format
	std::regex date_regex("\\d{4}-\\d{2}-\\d{2}");
	if (std::regex_match(date, date_regex)) { return date; }
	else { return ""; }
}

bool menu::inputBool(const std::string &title)
{
	std::cout << title << " [y/n]\n> ";

	std::string input;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.clear();
	std::getline(std::cin, input);

	if (input == "y" || input == "Y") { return true; }
	else if (input == "n" || input == "N") { return false; }
	else { return false; }
}

std::uint64_t menu::inputID()
{
	std::uint64_t id;
	std::string input;

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.clear();
	std::cout << "Enter an ID (or 'quit' to exit)\n> ";
	std::getline(std::cin, input);

	// Check for quit command
	std::transform(input.begin(), input.end(), input.begin(), [](unsigned char c) { return std::tolower(c); });
	if (input == "quit" || input == "q" || input == "exit") { return 0; }

	// Try to parse input as uint64_t
	try
	{
		if (input.size() != 16) { return 0; }
		id = std::stoull(input);
	}
	catch (...)
	{
		return 0;
	}

	return id;
}
