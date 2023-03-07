#include <iostream>
#include <memory>

#include <argparse/argparse.hpp>

#include <mariadb/conncpp.hpp>
#include <mariadb/conncpp/ResultSet.hpp>
#include <mariadb/conncpp/jdbccompat.hpp>

int main(int argc, const char *argv[])
{
	try
	{
		// Instantiate Driver
		sql::Driver *driver = sql::mariadb::get_driver_instance();

		// Configure Connection, including initial database name "test":
		sql::SQLString url("jdbc:mariadb://localhost:3306/");

		// Use a properties map for the other connection options
		sql::Properties properties({{"user", "make"}, {"password", "make"}});

		// Establish Connection
		// Use a smart pointer for extra safety
		std::unique_ptr<sql::Connection> conn(driver->connect(url, properties));

		// Use Connection
		// ...
		std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
		std::unique_ptr<sql::ResultSet> res(stmnt->executeQuery("select * from makerspace.user_data"));

		while (res.get()->next())
		{
			std::cout << "id = " << res->getString(1);
			std::cout << ", email = " << res->getString(2);
			std::cout << ", name = " << res->getString(4);
			std::cout << ", last scan = " << res->getString(5);
		}

		// Close Connection
		conn->close();
	}

	// Catch Exceptions
	catch (sql::SQLException &e)
	{
		std::cerr << "Error Connecting to the database: " << e.what() << std::endl;

		// Exit (Failed)
		return 1;
	}

	// Exit (Success)
	return 0;
}
