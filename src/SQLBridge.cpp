#include "SQLBridge.hpp"

#include <iostream>
#include <memory>

#include <mariadb/conncpp/Connection.hpp>
#include <mariadb/conncpp/Exception.hpp>
#include <mariadb/conncpp/PreparedStatement.hpp>
#include <mariadb/conncpp/ResultSet.hpp>

#include "SQLBridgeEnums.hpp"

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
		return static_cast<std::string>(res->getString(1));
	}
	catch (sql::SQLException &e)
	{
		// Put log to error here
		return {};
	}
}

std::optional<SQLBridge::userData> SQLBridge::getUserData(std::string uuid) const
{
	static std::unique_ptr<sql::PreparedStatement> stmntGetUserData(
		conn->prepareStatement("SELECT uuid, email, type, name, last_scan, creation FROM user_data WHERE uuid = ?"));
	stmntGetUserData->setString(1, uuid);

	userData data;
	try
	{
		std::unique_ptr<sql::ResultSet> res(stmntGetUserData->executeQuery());
		res->next();

		data.uuid = res->getString(1);
		data.email = res->getString(2);
		data.type = SQLBridgeEnum::PersonTypeFromString(res->getString(3));
		data.name = res->getString(4);
		data.lastScan = res->getString(5); // Should check for null date and give something nicer
		data.creationDate = res->getString(6);
	}
	catch (sql::SQLException &e)
	{
		// Put log to error here
		return {};
	}
	return data;
}

std::ostream &operator<<(std::ostream &os, const SQLBridge::trainingData &data)
{
	os << "UUID: " << data.uuid << "\n";
	os << "Training for: " << SQLBridgeEnum::MachineToString(data.machine) << "\n";
	os << "Training Level: " << SQLBridgeEnum::TrainingLevelToString(data.training) << "\n";
	os << "Training Date: " << data.trainingDate << "\n";
	for (auto &str : data.otherInfo)
	{
		os << str << "\n";
	}
	return os;
}

std::ostream &operator<<(std::ostream &os, const SQLBridge::userData &data)
{
	os << "UUID: " << data.uuid << "\n";
	os << "Email Address: " << data.email << "\n";
	os << "Status: " << SQLBridgeEnum::PersonTypeToString(data.type) << "\n";
	os << "Name: " << data.name << "\n";
	os << "Last Scan in: " << data.lastScan << "\n";
	os << "Date profile created: " << data.creationDate << "\n";
	return os;
}
