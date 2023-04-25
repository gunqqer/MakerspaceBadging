#pragma once

#include "SQLBridge.hpp"
#include "menu.hpp"
#include <mariadb/conncpp/SQLString.hpp>
#include <mariadb/conncpp/jdbccompat.hpp>

class Badging
{
	private:
		SQLBridge bridge;
		using menuString = std::pair<std::string, std::vector<std::string>>;
		const menuString mainMenu{"Main Menu", {"Scan ID", "Search for user", "Exit/Quit"}};
		const menuString IDExistsMenu{"ID", {"Update Info", "Scan new ID", "Cancel"}};
		const menuString IDNotExistMenu{"ID", {"Add new user", "Associate with existing", "cancel"}};
		const menuString searchMenu{"Search", {"By name", "By email", "Cancel"}};
		const menuString addUpdateMenu{"Add/Update machine training", {"Add/Update again", "Finish/Return"}};
		
		void scanID();
		void updateLastScan(std::string uuid);
		void search();
	public:
		Badging(const sql::SQLString& url, const sql::Properties& properties) : bridge(url, properties) {};
		void run();


};
