#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <ostream>
#include <unordered_map>

#include <mariadb/conncpp.hpp>
#include <mariadb/conncpp/ResultSet.hpp>
#include <mariadb/conncpp/SQLString.hpp>
#include <mariadb/conncpp/jdbccompat.hpp>

#include "SQLBridgeEnums.hpp"

class SQLBridge
{
	private:
		sql::Driver *driver;
		sql::SQLString url;
		sql::Properties properties;
		std::unique_ptr<sql::Connection> conn;

	public:
		struct trainingData
		{
				std::string uuid;
				SQLBridgeEnum::TrainingLevel training;
				std::string trainingDate;
				SQLBridgeEnum::Machine machine;
				std::vector<std::pair<std::string, std::string>> otherInfo; //Key, Value, human readable
		};

		struct userData
		{
				std::string uuid;
				std::string email;
				SQLBridgeEnum::PersonType type;
				std::string name;
				// Maybe these should be something from std::chrono, but for now strings will due
				std::string lastScan;
				std::string creationDate;
		};

		SQLBridge(sql::SQLString url, sql::Properties properties);
		~SQLBridge();

		std::optional<std::string> getUUID(uint64_t id) const;
		std::optional<std::string> findUUIDfromEmail(std::string email) const; // Emails are unique
		std::vector<std::string>
		findUUIDfromName(std::string name) const; // Names are not unique, they can return more than one UUID

		std::optional<userData> getUserData(std::string uuid) const;

		trainingData getTraining(std::string uuid, SQLBridgeEnum::Machine machine) const;

		std::string getNewUUID();
		bool addID(uint64_t id, std::string uuid);
		bool addPerson(userData &data);
		bool addTool(trainingData &data);
};

std::ostream &operator<<(std::ostream &os, const SQLBridge::trainingData &data); // For Debug
std::ostream &operator<<(std::ostream &os, const SQLBridge::userData &data);     // For Debug
