#include <fstream>
#include <sstream>
#include "../include/WareHouse.h"
#include "../include/Customer.h"
#include "../include/Volunteer.h"
#include "../include/Order.h"
#include "../include/Action.h"
#include <string>
#include <vector>
#include <iostream> 
#include <sstream>
using namespace std;


WareHouse::WareHouse(const string &configFilePath) 
	: isOpen(true), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), customerCounter(0), volunteerCounter(0)
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

void WareHouse::start(){
	open();
    cout << "Warehouse is open!" << endl;
    getUserCommand();
}

void WareHouse::getUserCommand(){

    string commend;
    while(isOpen){
        std::getline(std::cin, commend);
        std::istringstream iss(commend);

        std::vector<string> words;
        string word;

        while (iss >> word)
        {
            words.push_back(word);
        }

         if (words[0] == "customer" && words.size() >= 5)
        {
            string customerName = words[1];
            string customerType = words[2];
            int distance = std::stoi(words[3]);
            int maxOrders = std::stoi(words[4]);

            AddCustomer newCustomer(customerName, customerType, distance, maxOrders);
            newCustomer.act(*this);
        }
        else if (words[0] == "order" && words.size() >= 2)
        {
            int customerId = std::stoi(words[1]);
            AddOrder newOrder(customerId);
            newOrder.act(*this);
        }
        else if (words[0] == "step" && words.size() >= 2)
        {
            int numOfStep = std::stoi(words[1]);
            SimulateStep simStep(numOfStep);
            simStep.act(*this);
        }
        else if (words[0] == "orderStatus" && words.size() >= 2)
        {
            int orderId = std::stoi(words[1]);
            PrintOrderStatus orderStatus(orderId);
            orderStatus.act(*this);
        }
        else if (words[0] == "customerStatus" && words.size() >= 2)
        {
            int customerId = std::stoi(words[1]);
            PrintCustomerStatus customerStatus(customerId);
            customerStatus.act(*this);
        }
        else if (words[0] == "volunteerStatus" && words.size() >= 2)
        {
            int volunteerId = std::stoi(words[1]);
            PrintVolunteerStatus volunteerStatus(volunteerId);
            volunteerStatus.act(*this);
        }
        else if (words[0] == "log")
        {
            PrintActionsLog log;
            log.act(*this);
        }
        else if (words[0] == "close")
        {
            Close close;
            close.act(*this);
        }
        else if (words[0] == "backup")
        {
            BackupWareHouse backup;
            backup.act(*this);
        }
        else if (words[0] == "restore")
        {
            RestoreWareHouse restore;
            restore.act(*this);
        }
        else
        {
            std::cout << "error: not a valid command" << std::endl;
            ;
        }

    }
 }

void WareHouse::addOrder(Order* order){
    if(order->getStatus()==OrderStatus::COMPLETED){
        completedOrders.push_back(order);
    }
    else if (order->getStatus()==OrderStatus::PENDING){
         pendingOrders.push_back(order);
    }
    else inProcessOrders.push_back(order);
}

void WareHouse::addAction(BaseAction* action){
    actionsLog.push_back(action);
} 

void WareHouse::addVolunteer(Volunteer* volunteer){
    volunteers.push_back(volunteer);
    volunteerCounter++;
}//new method that adds a volunteer to the warehouse

void WareHouse::addCustomer(Customer* customer){
    customers.push_back(customer);
    customerCounter++;
}; //new method that adds a customer to the warehouse

Customer &WareHouse::getCustomer(int customerId) const{ 
    for(auto customer:customers){
        if(customer->getId()==customerId){
            return *customer;
        }
    }
    return nullptr;
}

Volunteer &WareHouse::getVolunteer(int volunteerId) const{
    return *volunteers[volunteerId];
}

Order &WareHouse::getOrder(int orderId) const{
    for(auto order:pendingOrders){
        if(order->getId()==orderId){
            return *order;
        }
    }
    for(auto order:inProcessOrders){
        if(order->getId()==orderId){
            return *order;
        }
    }
   for(auto order:completedOrders){
        if(order->getId()==orderId){
            return *order;
        }
    }
    else return nullptr;
}

const vector<BaseAction*> &WareHouse::getActions() const{
    return actionsLog;
}

}

void WareHouse::close(){
    isOpen=false;
}

void WareHouse::open(){
    isOpen=true;
}

int WareHouse::getCustomerCounter(){
    return customerCounter;
}

int WareHouse::getVolunteerCounter(){
    return volunteerCounter;
}

int WareHouse::getOrderCounter(){
    return orderCounter;
}

void WareHouse::deleteAll(){
    actionLog.clear();
    vollenuteers.clear();
    pendingOrders.clear();
    inProcessOrders.clear();
    completedOrders.clear();
    customers.clear();
    isOpen=false;
}


//Rule of 5
 WareHouse::~WareHouse(){
    deleteAll();
 }

WareHouse::WareHouse(const WareHouse &other):isOpen(other.isOpen),actionsLog(),volunteers(),pendingOrders(),inProcessOrders(),completedOrders(),customers(),customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter),orderCounter(other.orderCounter){
    for(auto action:other.actionsLog){
        actionsLog.push_back(action->clone());
    }
    for(auto volunteer:other.volunteers){
        volunteers.push_back(volunteer->clone());
    }
    for(auto pendingOrder:other.pendingOrders){
        pendingOrders.push_back(pendingOrder->clone());
    }
    for(auto inProcessOrder:other.inProcessOrders){
        inProcessOrders.push_back(inProcessOrder->clone());
    }
    for(auto completedOrder:other.completedOrders){
        completedOrders.push_back(completedOrder->clone());
    }
    for(auto customer:other.customers){
        customers.push_back(customer->clone());
    }
}

WareHouse &WareHouse::operator=(const WareHouse &other){
    if(this!=&other){
        isOpen=other.isOpen;
        customerCounter=other.customerCounter;
        volunteerCounter=other.volunteerCounter;
        orderCounter=other.orderCounter;
        //delet
        actionsLog.clear();
        volunteers.clear();
        pendingOrders.clear();
        inProcessOrders.clear();
        completedOrders.clear();
        customers.clear();
        //copy
        for(auto action:other.actionsLog){
            actionsLog.push_back(action->clone());
        }
        for(auto volunteer:other.volunteers){
            volunteers.push_back(volunteer->clone());
        }
        for(auto pendingOrder:other.pendingOrders){
            pendingOrders.push_back(pendingOrder->clone());
        }
        for(auto inProcessOrder:other.inProcessOrders){
            inProcessOrders.push_back(inProcessOrder->clone());
        }
        for(auto completedOrder:other.completedOrders){
            completedOrders.push_back(completedOrder->clone());
        }
        for(auto customer:other.customers){
            customers.push_back(customer->clone());
        }
    }
    return *this;
}

WareHouse&WareHouse::operator=(WareHouse &&other){
    if(this!=&other){
        isOpen=other.isOpen;
        customerCounter=other.customerCounter;
        volunteerCounter=other.volunteerCounter;
        orderCounter=other.orderCounter;
        //delet
        actionsLog.clear();
        volunteers.clear();
        pendingOrders.clear();
        inProcessOrders.clear();
        completedOrders.clear();
        customers.clear();
        //copy
        isOpen=other.isOpen;
        customerCounter=other.customerCounter;
        volunteerCounter=other.volunteerCounter;
        orderCounter=other.orderCounter;
        for(auto action:other.actionsLog){
            actionsLog.push_back(action->clone());
        }
        for(auto volunteer:other.volunteers){
            volunteers.push_back(volunteer->clone());
        }
        for(auto pendingOrder:other.pendingOrders){
            pendingOrders.push_back(pendingOrder->clone());
        }
        for(auto inProcessOrder:other.inProcessOrders){
            inProcessOrders.push_back(inProcessOrder->clone());
        }
        for(auto completedOrder:other.completedOrders){
            completedOrders.push_back(completedOrder->clone());
        }
        for(auto customer:other.customers){
            customers.push_back(customer->clone());
        }
        //detete other
        other.isOpen=false;
        other.customerCounter=0;
        other.volunteerCounter=0;
        other.orderCounter=0;
        other.actionsLog.clear();
        other.volunteers.clear();
        other.pendingOrders.clear();
        other.inProcessOrders.clear();
        other.completedOrders.clear();
        other.customers.clear();
    }
    return *this;
}

WareHouse::WareHouse(WareHouse &&other):isOpen(other.isOpen),actionsLog(),volunteers(),pendingOrders(),inProcessOrders(),completedOrders(),customers(),customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter),orderCounter(other.orderCounter){
    for(auto action:other.actionsLog){
        actionsLog.push_back(action->clone());
    }
    for(auto volunteer:other.volunteers){
        volunteers.push_back(volunteer->clone());
    }
    for(auto pendingOrder:other.pendingOrders){
        pendingOrders.push_back(pendingOrder->clone());
    }
    for(auto inProcessOrder:other.inProcessOrders){
        inProcessOrders.push_back(inProcessOrder->clone());
    }
    for(auto completedOrder:other.completedOrders){
        completedOrders.push_back(completedOrder->clone());
    }
    for(auto customer:other.customers){
        customers.push_back(customer->clone());
    }
    //detete other
    other.isOpen=false;
    other.customerCounter=0;
    other.volunteerCounter=0;
    other.orderCounter=0;
    other.actionsLog.clear();
    other.volunteers.clear();
    other.pendingOrders.clear();
    other.inProcessOrders.clear();
    other.completedOrders.clear();
    other.customers.clear();
}////move constructor