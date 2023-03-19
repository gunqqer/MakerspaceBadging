#pragma once

#include <cstdint>
#include <memory>

#include <mariadb/conncpp.hpp>
#include <mariadb/conncpp/SQLString.hpp>
#include <mariadb/conncpp/ResultSet.hpp>
#include <mariadb/conncpp/jdbccompat.hpp>

class SQLBridge
{
	private:
		sql::Driver *driver;
		sql::SQLString url;
		sql::Properties properties;
		std::unique_ptr<sql::Connection> conn;

	public:
		enum class TrainingLevel {untrained, partial, fully, canTrain};
		constexpr static std::string stringifyTrainingLevel(TrainingLevel level);

		enum class PersonType {student, faculty, staff, other};
		constexpr static std::string stringifyPersonType(PersonType type);

		//Can't start it with a number so it's d3 instead of 3d
		enum class Machine {laser, d3_printers, hand_tools, woodshop, embroidery, shopbot, vinyl};
		//Sigh, I would much rather use a static constexpr std::vector<std::string> but it isn't that easy
		static constexpr const char* machineName[] = {"laser", "3d_printers", "hand_tools", "woodshop", "embroidery", "shopbot"};

		SQLBridge(sql::SQLString url, sql::Properties properties);
		~SQLBridge() = default;

		std::string getUUID(uint64_t id) const;
		std::string findUUIDfromEmail(std::string email) const;
		std::vector<std::string> findUUIDfromName(std::string name) const; //Names are not unique, they can return more than one UUID

		std::string getTraining(std::string uuid, Machine machine) const;
};
