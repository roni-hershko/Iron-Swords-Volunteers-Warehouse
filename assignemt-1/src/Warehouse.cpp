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
            if(customerType=="Civilian"){
                customers.push_back(&CivilianCustomer(customerCounter,name, distance, maxOrders));
            }
            else{
                customers.push_back(&SoldierCustomer(customerCounter,name, distance, maxOrders));
            }
            customerCounter++;
        }
        else if (type == "volunteer")
        {
            string name, volunteerRole;
            int cooldown, maxDistance, distancePerStep, maxOrders;
            iss >> name >> volunteerRole;
            if (volunteerRole == "collector")
            {
                iss >> cooldown;
                volunteers.push_back(&CollectorVolunteer(volunteerCounter, name, cooldown));
            }
            else if (volunteerRole == "limited_collector")
            {
                iss >> cooldown >> maxOrders;
                volunteers.push_back(&LimitedCollectorVolunteer(volunteerCounter, name, cooldown, maxOrders));
            }
            else if (volunteerRole == "driver")
            {
                iss >> maxDistance >> distancePerStep;
                volunteers.push_back(&DriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep));
            }
            else if (volunteerRole == "limited_driver")
            {
                iss >> maxDistance >> distancePerStep >> maxOrders;
                volunteers.push_back(&LimitedDriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep, maxOrders));
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
	pendingOrders.push_back(order);
	orderCounter++;
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
    return nullptr;
}

const vector<BaseAction*> &WareHouse::getActions() const{
    return actionsLog;
}

vector<Order*> &WareHouse::getPendingOrders(){
	return pendingOrders;
}

vector<Order*> &WareHouse::getCompletedOrders(){
	return completedOrders;
}

vector<Order*> &WareHouse::getinProcessOrders(){
	return inProcessOrders;
}

vector<Customer*> &WareHouse::getCustomers(){
	return customers;
}

vector<Volunteer*> &WareHouse::getVolunteers(){
	return volunteers;
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

void WareHouse::deleteAll(){ //delete the vector itself?

    actionsLog.clear();
    volunteers.clear();
    pendingOrders.clear();
    inProcessOrders.clear();
    completedOrders.clear();
    customers.clear();
    isOpen=false;
}

void wareHouse::printForClosing(){
	cout << "Pending orders:" << endl;
	for (auto order : pendingOrders)
	{
		cout << *order << endl;
	}
	cout << "In process orders:" << endl;
	for (auto order : inProcessOrders)
	{
		cout << *order << endl;
	}
	cout << "Completed orders:" << endl;
	for (auto order : completedOrders)
	{
		cout << *order << endl;
	}
	cout << "Volunteers:" << endl;
	for (auto volunteer : volunteers)
	{
		cout << *volunteer << endl;
	}
	cout << "Customers:" << endl;
	for (auto customer : customers)
	{
		cout << *customer << endl;
	}
}


void WareHouse::deleteInProcessOrder(int orderid)
{
    for (int i = 0; i < inProcessOrders.size(); ++i)
    {
        Order *order = inProcessOrders[i];
        if (order->getId() == orderid)
        {
            delete order;
            inProcessOrders.erase(inProcessOrders.begin() + i);
            return;
        }
    }
}

void WareHouse::deleteVolunteer(int volunteerId){
    for (int i = 0; i < volunteers.size(); ++i){
        Volunteer *volunteer = volunteers[i];
        if (volunteer->getId() == volunteerId){
            delete volunteer;
            volunteers.erase(volunteers.begin() + i);
            return;
        }
    }
}


//Rule of 5
 WareHouse::~WareHouse(){ //destructor
    deleteAll();
 }

WareHouse::WareHouse(const WareHouse &other) //copy constructor
	: isOpen(other.isOpen),actionsLog(),volunteers(),pendingOrders(),inProcessOrders(),completedOrders(),customers(),customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter),orderCounter(other.orderCounter){
   
    isOpen = other.isOpen;
    customerCounter = other.customerCounter;
    volunteerCounter = other.volunteerCounter;
	orderCounter = other.orderCounter;
	
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

WareHouse &WareHouse::operator=(const WareHouse &other){ //copy assignment operator
    if(this!=&other){
        isOpen=other.isOpen;
        customerCounter=other.customerCounter;
        volunteerCounter=other.volunteerCounter;
        orderCounter=other.orderCounter;

        //delete
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

WareHouse &WareHouse::operator=(WareHouse &&other){ //move assignment operator
    if(this!=&other){
        isOpen=other.isOpen;
        customerCounter=other.customerCounter;
        volunteerCounter=other.volunteerCounter;
        orderCounter=other.orderCounter;

        //delete
        actionsLog.clear();
        volunteers.clear();
        pendingOrders.clear();
        inProcessOrders.clear();
        completedOrders.clear();
        customers.clear();

		//move
		actionsLog = std::move(other.actionsLog);
		volunteers = std::move(other.volunteers);
		pendingOrders = std::move(other.pendingOrders);
		inProcessOrders = std::move(other.inProcessOrders);
		completedOrders = std::move(other.completedOrders);
		customers = std::move(other.customers);
    }
    return *this;
}

WareHouse::WareHouse(WareHouse &&other) //move constructor
	: isOpen(other.isOpen),actionsLog(),volunteers(),pendingOrders(),inProcessOrders(),completedOrders(),customers(),customerCounter(other.customerCounter),volunteerCounter(other.volunteerCounter),orderCounter(other.orderCounter){
	
	isOpen = other.isOpen;
	customerCounter = other.customerCounter;
	volunteerCounter = other.volunteerCounter;
	orderCounter = other.orderCounter;

	actionsLog = std::move(other.actionsLog);
	volunteers = std::move(other.volunteers);
	pendingOrders = std::move(other.pendingOrders);
	inProcessOrders = std::move(other.inProcessOrders);
	completedOrders = std::move(other.completedOrders);
	customers = std::move(other.customers);
}