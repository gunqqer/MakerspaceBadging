#include "SQLBridge.hpp"

#include <algorithm>
#include <exception>
#include <iostream>
#include <memory>

#include <mariadb/conncpp/Connection.hpp>
#include <mariadb/conncpp/Exception.hpp>
#include <mariadb/conncpp/PreparedStatement.hpp>
#include <mariadb/conncpp/ResultSet.hpp>
#include <string>

#include "SQLBridgeEnums.hpp"

using Result = std::unique_ptr<sql::ResultSet>;
using Statement = std::unique_ptr<sql::PreparedStatement>;

SQLBridge::SQLBridge(sql::SQLString url, sql::Properties properties) : url(url),
	  properties(properties)
{
	try
	{
		driver = sql::mariadb::get_driver_instance();
		conn.reset(driver->connect(url, properties));
	}
	catch (sql::SQLException &e)
	{
		std::cerr << "Unable to connect to database: " << e.what() << "\n";
		std::terminate();
	}
}

SQLBridge::~SQLBridge()
{
	conn->close();
}

std::optional<std::string> SQLBridge::getUUID(uint64_t id) const
{
	static Statement stmntGetUUID(conn->prepareStatement("SELECT uuid FROM `id_uuid` WHERE id = ?"));
	stmntGetUUID->setUInt64(1, id);
	try
	{
		Result res(stmntGetUUID->executeQuery());
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
	static Statement stmntGetUserData(
		conn->prepareStatement("SELECT uuid, email, type, name, last_scan, creation FROM user_data WHERE uuid = ?"));
	stmntGetUserData->setString(1, uuid);

	userData data;
	try
	{
		Result res(stmntGetUserData->executeQuery());
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
	if (data.training == SQLBridgeEnum::TrainingLevel::untrained)
	{ // No reason to print everything out, it's all meaningless if the user is untrained
		os << "UNTRAINED\n";
		return os;
	}
	os << "Training Level: " << SQLBridgeEnum::TrainingLevelToString(data.training) << "\n";
	os << "Training Date: " << data.trainingDate << "\n";
	for (auto &pair : data.otherInfo)
	{
		os << pair.first << ": " << pair.second << "\n";
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

std::string SQLBridge::getNewUUID() const
{
	static Statement stmntGetNewUUID(conn->prepareStatement("SELECT UUID()"));

	try
	{
		Result res(stmntGetNewUUID->executeQuery());
		res->next();
		return static_cast<std::string>(res->getString(1));
	}
	catch (sql::SQLException &e)
	{
		return {};
	}
}

bool SQLBridge::addID(uint64_t id, std::string uuid)
{
	static Statement stmntAddId(conn->prepareStatement("INSERT INTO id_uuid (id, uuid) VALUES (?, ?)"));
	stmntAddId->setUInt64(1, id);
	stmntAddId->setString(2, uuid);
	try
	{
		Result tmp(stmntAddId->executeQuery());
		return true;
	}
	catch (sql::SQLException &e)
	{
		return false;
	}
}

bool SQLBridge::addPerson(userData &data)
{
	static Statement stmntAddPerson(
		conn->prepareStatement("INSERT INTO user_data (uuid, email, type, name) VALUES (?, ?, ?, ?)"));
	stmntAddPerson->setString(1, data.uuid);
	stmntAddPerson->setString(2, data.email);
	stmntAddPerson->setString(3, SQLBridgeEnum::PersonTypeToString(data.type));
	stmntAddPerson->setString(4, data.name);
	try
	{
		Result tmp(stmntAddPerson->executeQuery());
		return true;
	}
	catch (sql::SQLException &e)
	{
		return false;
	}
}

bool SQLBridge::addTool(trainingData &data)
{
	// Really there has to be some way to do this that is more sane
	switch (data.machine)
	{
	case SQLBridgeEnum::Machine::laser:
		return addLaserData(data);
		break;
	case SQLBridgeEnum::Machine::d3_printers:
		return add3DPrinterData(data);
		break;
	case SQLBridgeEnum::Machine::hand_tools:
		return addHandToolData(data);
		break;
	case SQLBridgeEnum::Machine::woodshop:
		return addWoodshopData(data);
		break;
	case SQLBridgeEnum::Machine::embroidery:
		return addEmbroideryData(data);
		break;
	case SQLBridgeEnum::Machine::shopbot:
		return addShopbotData(data);
		break;
	case SQLBridgeEnum::Machine::vinyl:
		return addVinylData(data);
		break;
	default:
		throw std::logic_error("Unknown Enum Value!");
	}
}

// For secondary data there must be some better way to do this, but I haven't figured it out yet
// At least they're all booleans

bool SQLBridge::addLaserData(trainingData &data)
{
	static Statement stmntAddLaser(
		conn->prepareStatement("INSERT INTO laser (uuid, training, rotary, small_laser) VALUES (?, ?, ?, ?)"));
	stmntAddLaser->setString(1, data.uuid);
	stmntAddLaser->setString(2, SQLBridgeEnum::TrainingLevelToString(data.training));
	stmntAddLaser->setBoolean(3, std::stoi(data.otherInfo.at(0).second));
	stmntAddLaser->setBoolean(4, std::stoi(data.otherInfo.at(1).second));
	try
	{
		Result tmp(stmntAddLaser->executeQuery());
		return true;
	}
	catch (sql::SQLException &e)
	{
		std::cerr << e.what();
		return false;
	}
}

bool SQLBridge::add3DPrinterData(trainingData &data)
{
	static Statement stmntAdd3DPrinter(conn->prepareStatement(
		"INSERT INTO 3d_printers (uuid, training, print_starting, ultimaker) VALUES (?, ?, ?, ?)"));
	stmntAdd3DPrinter->setString(1, data.uuid);
	stmntAdd3DPrinter->setString(2, SQLBridgeEnum::TrainingLevelToString(data.training));
	stmntAdd3DPrinter->setBoolean(3, std::stoi(data.otherInfo.at(0).second));
	stmntAdd3DPrinter->setBoolean(4, std::stoi(data.otherInfo.at(1).second));
	try
	{
		Result tmp(stmntAdd3DPrinter->executeQuery());
		return true;
	}
	catch (sql::SQLException &e)
	{
		std::cerr << e.what();
		return false;
	}
}

bool SQLBridge::addHandToolData(trainingData &data)
{
	static Statement stmntAdd3DPrinter(conn->prepareStatement("INSERT INTO hand_tools (uuid, training) VALUES (?, ?)"));
	stmntAdd3DPrinter->setString(1, data.uuid);
	stmntAdd3DPrinter->setString(2, SQLBridgeEnum::TrainingLevelToString(data.training));
	try
	{
		Result tmp(stmntAdd3DPrinter->executeQuery());
		return true;
	}
	catch (sql::SQLException &e)
	{
		std::cerr << e.what();
		return false;
	}
}

bool SQLBridge::addWoodshopData(trainingData &data)
{
	static Statement stmntAddWoodshop(
		conn->prepareStatement("INSERT INTO woodshop (uuid, training, waiver) VALUES (?, ?, ?)"));
	stmntAddWoodshop->setString(1, data.uuid);
	stmntAddWoodshop->setString(2, SQLBridgeEnum::TrainingLevelToString(data.training));
	stmntAddWoodshop->setBoolean(3, std::stoi(data.otherInfo.at(0).second));
	try
	{
		Result tmp(stmntAddWoodshop->executeQuery());
		return true;
	}
	catch (sql::SQLException &e)
	{
		std::cerr << e.what();
		return false;
	}
}

bool SQLBridge::addEmbroideryData(trainingData &data)
{
	static Statement stmntAddEmbroidery(
		conn->prepareStatement("INSERT INTO woodshop (uuid, training, cap) VALUES (?, ?, ?)"));
	stmntAddEmbroidery->setString(1, data.uuid);
	stmntAddEmbroidery->setString(2, SQLBridgeEnum::TrainingLevelToString(data.training));
	stmntAddEmbroidery->setBoolean(3, std::stoi(data.otherInfo.at(0).second));
	try
	{
		Result tmp(stmntAddEmbroidery->executeQuery());
		return true;
	}
	catch (sql::SQLException &e)
	{
		std::cerr << e.what();
		return false;
	}
}

bool SQLBridge::addShopbotData(trainingData &data)
{
	static Statement stmntAddShopbot(
		conn->prepareStatement("INSERT INTO woodshop (uuid, training, rotary) VALUES (?, ?, ?)"));
	stmntAddShopbot->setString(1, data.uuid);
	stmntAddShopbot->setString(2, SQLBridgeEnum::TrainingLevelToString(data.training));
	stmntAddShopbot->setBoolean(3, std::stoi(data.otherInfo.at(0).second));
	try
	{
		Result tmp(stmntAddShopbot->executeQuery());
		return true;
	}
	catch (sql::SQLException &e)
	{
		std::cerr << e.what();
		return false;
	}
}

bool SQLBridge::addVinylData(trainingData &data)
{
	static Statement stmntAddVinyl(conn->prepareStatement(
		"INSERT INTO woodshop (uuid, training, roland, cricut, graphgear, heat_press) VALUES (?, ?, ?, ?, ?, ?)"));
	stmntAddVinyl->setString(1, data.uuid);
	stmntAddVinyl->setString(2, SQLBridgeEnum::TrainingLevelToString(data.training));
	stmntAddVinyl->setBoolean(3, std::stoi(data.otherInfo.at(0).second));
	stmntAddVinyl->setBoolean(4, std::stoi(data.otherInfo.at(1).second));
	stmntAddVinyl->setBoolean(5, std::stoi(data.otherInfo.at(2).second));
	stmntAddVinyl->setBoolean(6, std::stoi(data.otherInfo.at(3).second));
	try
	{
		Result tmp(stmntAddVinyl->executeQuery());
		return true;
	}
	catch (sql::SQLException &e)
	{
		std::cerr << e.what();
		return false;
	}
}

bool SQLBridge::addSprayBoothData(trainingData &data)
{
	static Statement stmntAddSprayBooth(
		conn->prepareStatement("INSERT INTO woodshop (uuid, training, paint_sprayer) VALUES (?, ?, ?)"));
	stmntAddSprayBooth->setString(1, data.uuid);
	stmntAddSprayBooth->setString(2, SQLBridgeEnum::TrainingLevelToString(data.training));
	stmntAddSprayBooth->setBoolean(3, std::stoi(data.otherInfo.at(0).second));
	try
	{
		Result tmp(stmntAddSprayBooth->executeQuery());
		return true;
	}
	catch (sql::SQLException &e)
	{
		std::cerr << e.what();
		return false;
	}
}

std::optional<SQLBridge::trainingData> SQLBridge::getTraining(std::string uuid, SQLBridgeEnum::Machine machine) const
{
	// Really there has to be some way to do this that is more sane
	switch (machine)
	{
	case SQLBridgeEnum::Machine::laser:
		return getLaserData(uuid);
		break;
	case SQLBridgeEnum::Machine::d3_printers:
		return get3DPrinterData(uuid);
		break;
	case SQLBridgeEnum::Machine::hand_tools:
		return getHandToolData(uuid);
		break;
	case SQLBridgeEnum::Machine::woodshop:
		return getWoodshopData(uuid);
		break;
	case SQLBridgeEnum::Machine::embroidery:
		return getEmbroideryData(uuid);
		break;
	case SQLBridgeEnum::Machine::shopbot:
		return getShopbotData(uuid);
		break;
	case SQLBridgeEnum::Machine::vinyl:
		return getVinylData(uuid);
		break;
	default:
		throw std::logic_error("Unknown Enum Value!");
	}
}

std::optional<SQLBridge::trainingData> SQLBridge::getLaserData(std::string uuid) const
{
	static Statement stmntGetLaser(
		conn->prepareStatement("SELECT uuid, training, training_date, rotary, small_laser FROM laser WHERE uuid = ?"));
	stmntGetLaser->setString(1, uuid);
	try
	{
		Result res(stmntGetLaser->executeQuery());
		res->next();
		trainingData data = commonData(res.get());
		data.otherInfo.emplace_back("rotary", res->getString(4));
		data.otherInfo.emplace_back("small_laser", res->getString(5));
		return data;
	}
	catch (sql::SQLException &e)
	{
		return {};
	}
}
std::optional<SQLBridge::trainingData> SQLBridge::get3DPrinterData(std::string uuid) const
{
	static Statement stmntGet3DPrinter(conn->prepareStatement(
		"SELECT uuid, training, training_date, print_starting, ultimaker FROM 3d_printers WHERE uuid = ?"));
	stmntGet3DPrinter->setString(1, uuid);
	try
	{
		Result res(stmntGet3DPrinter->executeQuery());
		res->next();
		trainingData data{commonData(res.get())};
		data.otherInfo.emplace_back("print_starting", res->getString(4));
		data.otherInfo.emplace_back("ultimaker", res->getString(5));
		return data;
	}
	catch (sql::SQLException &e)
	{
		return {};
	}
}
std::optional<SQLBridge::trainingData> SQLBridge::getHandToolData(std::string uuid) const
{
	static Statement stmntGetHandTool(
		conn->prepareStatement("SELECT uuid, training, training_date FROM hand_tools WHERE uuid = ?"));
	stmntGetHandTool->setString(1, uuid);
	try
	{
		Result res(stmntGetHandTool->executeQuery());
		res->next();
		trainingData data{commonData(res.get())};
		return data;
	}
	catch (sql::SQLException &e)
	{
		return {};
	}
}
std::optional<SQLBridge::trainingData> SQLBridge::getWoodshopData(std::string uuid) const
{
	static Statement stmntGetWoodshop(
		conn->prepareStatement("SELECT uuid, training, training_date, waiver FROM woodshop WHERE uuid = ?"));
	stmntGetWoodshop->setString(1, uuid);
	try
	{
		Result res(stmntGetWoodshop->executeQuery());
		res->next();
		trainingData data{commonData(res.get())};
		data.otherInfo.emplace_back("waiver", res->getString(4));
		return data;
	}
	catch (sql::SQLException &e)
	{
		return {};
	}
}
std::optional<SQLBridge::trainingData> SQLBridge::getEmbroideryData(std::string uuid) const
{
	static Statement stmntGetEmbroidery(
		conn->prepareStatement("SELECT uuid, training, training_date, cap FROM embroidery WHERE uuid = ?"));
	stmntGetEmbroidery->setString(1, uuid);
	try
	{
		Result res(stmntGetEmbroidery->executeQuery());
		res->next();
		trainingData data{commonData(res.get())};
		data.otherInfo.emplace_back("cap", res->getString(4));
		return data;
	}
	catch (sql::SQLException &e)
	{
		return {};
	}
}
std::optional<SQLBridge::trainingData> SQLBridge::getShopbotData(std::string uuid) const
{
	static Statement stmntGetShopbot(
		conn->prepareStatement("SELECT uuid, training, training_date, rotary FROM shopbot WHERE uuid = ?"));
	stmntGetShopbot->setString(1, uuid);
	try
	{
		Result res(stmntGetShopbot->executeQuery());
		res->next();
		trainingData data{commonData(res.get())};
		data.otherInfo.emplace_back("rotary", res->getString(4));
		return data;
	}
	catch (sql::SQLException &e)
	{
		return {};
	}
}
std::optional<SQLBridge::trainingData> SQLBridge::getVinylData(std::string uuid) const
{
	static Statement stmntGetVinyl(conn->prepareStatement(
		"SELECT uuid, training, training_date, roland, cricut, graphgear, heat_press FROM vinyl WHERE uuid = ?"));
	stmntGetVinyl->setString(1, uuid);
	try
	{
		Result res(stmntGetVinyl->executeQuery());
		res->next();
		trainingData data{commonData(res.get())};
		data.otherInfo.emplace_back("roland", res->getString(4));
		data.otherInfo.emplace_back("cricut", res->getString(5));
		data.otherInfo.emplace_back("graphgear", res->getString(6));
		data.otherInfo.emplace_back("heat_press", res->getString(7));
		return data;
	}
	catch (sql::SQLException &e)
	{
		return {};
	}
}
std::optional<SQLBridge::trainingData> SQLBridge::getSprayBoothData(std::string uuid) const
{
	static Statement stmntGetSprayBooth(
		conn->prepareStatement("SELECT uuid, training, training_date, paint_sprayer FROM spray_booth WHERE uuid = ?"));
	stmntGetSprayBooth->setString(1, uuid);
	try
	{
		Result res(stmntGetSprayBooth->executeQuery());
		res->next();
		trainingData data{commonData(res.get())};
		data.otherInfo.emplace_back("paint_sprayer", res->getString(4));
		return data;
	}
	catch (sql::SQLException &e)
	{
		return {};
	}
}
