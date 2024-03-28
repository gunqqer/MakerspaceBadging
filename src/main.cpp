#include <iostream>
#include <memory>

#include <argparse/argparse.hpp>

#include "Badging.hpp"
#include "SQLBridge.hpp"
#include "SQLBridgeEnums.hpp"
#include "menu.hpp"

int main(int argc, const char *argv[])
{
	sql::SQLString url("jdbc:mariadb://localhost:3306/makerspace");

	// Use a properties map for the other connection options
	sql::Properties properties({{"user", "make"}, {"password", "make"}});

	SQLBridge bridge(url, properties);
	bridge.updateScanDate("421a9edd-c061-11ed-a025-09fac8fb94f1");
	// Badging program(url, properties);
	// program.run();

	// std::cout << menu::selection("This is a selection thingymabob", {"test", "tester", "tested"}) << "\n";

	// std::cout << menu::inputDate() << "\n";

	// std::cout << menu::inputName() << "\n";

	// std::cout << menu::inputBool("BOOLEANS") << "\n";

	// bridge.addID(6010030000023871, "421a9edd-c061-11ed-a025-09fac8fb94f1");
	// if (bridge.getUUID(6010030000023871) == "421a9edd-c061-11ed-a025-09fac8fb94f1") { std::cout << "It works\n"; }
	// else { std::cout << "Failure!\n"; }

	// SQLBridge::userData tmp{"421a9edd-c061-11ed-a025-09fac8fb94f1",
	//"test@test.com",
	// SQLBridgeEnum::PersonType::student,
	//"Testperson",
	//"",
	//""};
	// bridge.addPerson(tmp);

	// SQLBridge::trainingData lasertest{"421a9edd-c061-11ed-a025-09fac8fb94f1", SQLBridgeEnum::TrainingLevel::fully,
	//"2023-04-19", SQLBridgeEnum::Machine::laser};
	// lasertest.otherInfo.emplace_back("rotary", "0");
	// lasertest.otherInfo.emplace_back("small_laser", "1");

	// bridge.updateTraining(lasertest);

	// std::cout << bridge.getTraining("421a9edd-c061-11ed-a025-09fac8fb94f1", SQLBridgeEnum::Machine::laser).value();
	return 0;
	}
