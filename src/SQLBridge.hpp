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
	public:
		struct trainingData
		{
				std::string uuid;
				SQLBridgeEnum::TrainingLevel training;
				std::string trainingDate;
				SQLBridgeEnum::Machine machine;
				std::vector<std::pair<std::string, std::string>>
					otherInfo; // Key, Value, human readable, same order as SQL table
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

	private:
		sql::Driver *driver;
		sql::SQLString url;
		sql::Properties properties;
		std::unique_ptr<sql::Connection> conn;

		// There has to be some better way of doing this :|
		// Maybe have dynamic Statements that assemble the statement from strings based on the machine type? For now
		// this will work
		std::optional<trainingData> getLaserData(std::string uuid) const;
		std::optional<trainingData> get3DPrinterData(std::string uuid) const;
		std::optional<trainingData> getHandToolData(std::string uuid) const;
		std::optional<trainingData> getWoodshopData(std::string uuid) const;
		std::optional<trainingData> getEmbroideryData(std::string uuid) const;
		std::optional<trainingData> getShopbotData(std::string uuid) const;
		std::optional<trainingData> getVinylData(std::string uuid) const;
		std::optional<trainingData> getSprayBoothData(std::string uuid) const;
		// Well at least this is common between all the tables
		inline trainingData commonData(sql::ResultSet* res) const
		{
			SQLBridge::trainingData data{static_cast<std::string>(res->getString(1)),
			                             SQLBridgeEnum::TrainingLevelFromString(res->getString(2)),
			                             static_cast<std::string>(res->getString(3))};
			return data;
		}

		bool addLaserData(trainingData &data);
		bool add3DPrinterData(trainingData &data);
		bool addHandToolData(trainingData &data);
		bool addWoodshopData(trainingData &data);
		bool addEmbroideryData(trainingData &data);
		bool addShopbotData(trainingData &data);
		bool addVinylData(trainingData &data);
		bool addSprayBoothData(trainingData &data);

	public:
		SQLBridge(sql::SQLString url, sql::Properties properties);
		~SQLBridge();

		std::optional<std::string> getUUID(uint64_t id) const;
		std::optional<std::string> findUUIDfromEmail(std::string email) const; // Emails are unique
		std::vector<std::string>
		findUUIDfromName(std::string name) const; // Names are not unique, they can return more than one UUID

		std::optional<userData> getUserData(std::string uuid) const;

		std::optional<trainingData> getTraining(std::string uuid, SQLBridgeEnum::Machine machine) const;

		std::string getNewUUID() const;
		bool addID(uint64_t id, std::string uuid);
		bool addPerson(userData &data);
		bool addTool(trainingData &data);

		// UUID may need to be updated, first arg is old UUID, if change is needed new UUID is in data
		bool updatePerson(std::string uuid, userData &data);
		// UUID comes from struct
		bool updateTraining(trainingData &data);

		bool deleteUser(std::string uuid);
};

std::ostream &operator<<(std::ostream &os, const SQLBridge::trainingData &data); // For Debug
std::ostream &operator<<(std::ostream &os, const SQLBridge::userData &data);     // For Debug
