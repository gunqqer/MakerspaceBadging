#include "SQLBridge.hpp"
#include <iostream>
#include <mariadb/conncpp/Connection.hpp>
#include <mariadb/conncpp/Exception.hpp>
#include <memory>

SQLBridge::SQLBridge(sql::SQLString url, sql::Properties properties) : url(url), properties(properties)
{
	try
	{
		driver = sql::mariadb::get_driver_instance();
		conn.reset(driver->connect(url, properties));
	}
	catch (sql::SQLException &e)
	{
	}
}

SQLBridge::~SQLBridge()
{
	conn->close();
}

std::optional<std::string> SQLBridge::getUUID(uint64_t id) const
{
	static std::unique_ptr<sql::PreparedStatement> stmntGetUUID(
		conn->prepareStatement("SELECT uuid FROM `id_uuid` WHERE id = ?"));
	stmntGetUUID->setUInt64(1, id);
	try
	{
		std::unique_ptr<sql::ResultSet> res(stmntGetUUID->executeQuery());
		res->next();
		std::cout << res->getString(1) << "\n";
		auto tmp = static_cast<std::string>(res->getString(1));
		if (tmp.empty()) { return {}; }
		else { return tmp; }
	}
	catch (sql::SQLException &e)
	{
		std::cerr << e.what();
		return {};
	}
}
