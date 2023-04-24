#include "Badging.hpp"

void Badging::run()
{
	// TODO use argparse to get these instead of hardcoding
	sql::SQLString url("jdbc:mariadb://localhost:3306/makerspace");
	// Use a properties map for the other connection options
	sql::Properties properties({{"user", "make"}, {"password", "make"}});

	SQLBridge bridge(url, properties);

	bool STOPPROGRAM = false;
	while (!STOPPROGRAM) {}
}
