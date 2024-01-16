#include <fstream>
#include <sstream>
#include "../include/WareHouse.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h"
#include "../include/Order.h"

WareHouse::WareHouse(const string &configFilePath) : isOpen(true), actionsLog(), volunteers(), pendingOrders(), vol(), completedOrders(), customers(), customerCounter(0), volunteerCounter(0) {
    ifstream configFile(configFilePath);
    string line;

    while (getline(configFile, line)) {
        istringstream iss(line);
        string type;
        iss >> type;

        if (type == "customer") {
            string name, customerType;
            int distance, maxOrders;
            iss >> name >> customerType >> distance >> maxOrders;
            customers.push_back(Customer(name, customerType, distance, maxOrders));
            customerCounter++;
        } else if (type == "volunteer") {
            string name, volunteerRole;
            int cooldown, maxDistance, distancePerStep, maxOrders;
            iss >> name >> volunteerRole >> cooldown >> maxDistance >> distancePerStep >> maxOrders;

            if (volunteerRole == "collector") {
                volunteers.push_back(make_shared<Collector>(name, cooldown));
            } else if (volunteerRole == "limited_collector") {
                volunteers.push_back(make_shared<LimitedCollector>(name, cooldown, maxOrders));
            } else if (volunteerRole == "driver") {
                volunteers.push_back(make_shared<Driver>(name, cooldown, maxDistance, distancePerStep));
            } else if (volunteerRole == "limited_driver") {
                volunteers.push_back(make_shared<LimitedDriver>(name, cooldown, maxDistance, distancePerStep, maxOrders));
            }

            volunteerCounter++;
        }
    }
}
    void WareHouse:: printActionsLogs()
    {
		for (int i = 0; i < actionsLog.size(); ++i) {
			cout << actionsLog[i]->toString() << endl;
		}
    }
