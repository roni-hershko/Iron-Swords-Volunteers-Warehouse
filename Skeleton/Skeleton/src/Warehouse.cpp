#include <fstream>
#include <sstream>
#include "../include/WareHouse.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h"
#include "../include/Order.h"
#include "../include/Customer.h"
#include "../include/BaseAction.h"
#include <string>
#include <vector>
#include <iostream> 
using namespace std;

//DELETETHE PRIVIUS ONE
WareHouse::WareHouse(const string &configFilePath) : isOpen(true), actionsLog(), volunteers(), pendingOrders(), vol(), completedOrders(), customers(), customerCounter(0), volunteerCounter(0)
{
    ifstream configFile(configFilePath);
    string line;

    while (getline(configFile, line))
    {
        istringstream iss(line);
        string type;
        iss >> type;

        if (type == "customer")
        {
            string name, customerType;
            int distance, maxOrders;
            iss >> name >> customerType >> distance >> maxOrders;
            customers.push_back(Customer(name, customerType, distance, maxOrders));
            customerCounter++;
        }
        else if (type == "volunteer")
        {
            string name, volunteerRole;
            int cooldown, maxDistance, distancePerStep, maxOrders;
            iss >> name >> volunteerRole >> cooldown >> maxDistance >> distancePerStep >> maxOrders;

            if (volunteerRole == "collector")
            {
                volunteers.push_back(new CollectorVolunteer(volunteerCounter, name, cooldown));
            }
            else if (volunteerRole == "limited_collector")
            {
                volunteers.push_back(make_shared<LimitedCollector>(name, cooldown, maxOrders));
            }
            else if (volunteerRole == "driver")
            {
                volunteers.push_back(make_shared<Driver>(name, cooldown, maxDistance, distancePerStep));
            }
            else if (volunteerRole == "limited_driver")
            {
                volunteers.push_back(make_shared<LimitedDriver>(name, cooldown, maxDistance, distancePerStep, maxOrders));
            }

            volunteerCounter++;
        }
    }
}

void WareHouse::start();

const vector<BaseAction*> &WareHouse::getActionsLog() const{
    return actionsLog;
}

void WareHouse::addOrder(Order* order){
    if(order->getStatus()==OrderStatus::COMPLETED){
        completedOrders.push_back(order);
    }
    else if (order->getStatus()==OrderStatus::PENDING){
         pendingOrders.push_back(order);
    }
    else vol.push_back(order);
}

void WareHouse::addAction(BaseAction* action){
    actionsLog.push_back(action);
} //add action to warehouse

void WareHouse::addVolunteer(Volunteer* volunteer){
    volunteers.push_back(volunteer);
    volunteerCounter++;
}//new method that adds a volunteer to the warehouse

void WareHouse::addCustomer(Customer* customer){
    customers.push_back(customer);
    customerCounter++;
}; //new method that adds a customer to the warehouse

void WareHouse::printActionsLogs(){
    for(auto action:actionsLog)
        std::cout<<action->toString()<<std::endl; // Use std::cout instead of cout
}

Customer &WareHouse::getCustomer(int customerId) const{
    return *customers[customerId];
}

Volunteer &WareHouse::getVolunteer(int volunteerId) const{
    return *volunteers[volunteerId];
}

Order &WareHouse::getOrder(int orderId) const{
    if(*pendingOrders[orderId]!=NULL)
        return *pendingOrders[orderId];
    else if(*vol[orderId]!=NULL)
        return *vol[orderId];
    else return *completedOrders[orderId];
}

void WareHouse::close(){// make sure no memory leaks, that method ends the program
    isOpen=false;
for(auto penOr:pendingOrders)
        std::cout<<penOr->toString()<<std::endl; 
for(auto comOr:completedOrders)
        std::cout<<comOr->toString()<<std::endl;
}

void WareHouse::open(){
    isOpen=true;
}

int WareHouse::getCustomerCounter(){
    customerCounter++;
    return customerCounter-1;
}
int WareHouse::getVolunteerCounter(){
    volunteerCounter++;
    return volunteerCounter-1;
}
int WareHouse::getOrderCounter(){
    orderCounter++;
    return orderCounter-1;
}//new metod that returns the order counter