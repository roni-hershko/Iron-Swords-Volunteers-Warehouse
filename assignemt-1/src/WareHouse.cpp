#include <fstream>
#include <sstream>
#include "../include/WareHouse.h"
#include "../include/Action.h"
#include <string>
#include <vector>
#include <iostream> 
#include <sstream>
using namespace std;


WareHouse::WareHouse(const string &configFilePath) 
	: isOpen(true), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(), 
	customerCounter(0), volunteerCounter(0), 
	dummy_volunteer(-1, "dummy", 0), dummy_Customer(-1, "dummy", 0, 0), dummy_Order(-1, 0, 0)
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
                customers.push_back(new CivilianCustomer(customerCounter,name, distance, maxOrders));
            }
            else{
                customers.push_back(new SoldierCustomer(customerCounter,name, distance, maxOrders));
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
                volunteers.push_back(new CollectorVolunteer(volunteerCounter, name, cooldown));
            }
            else if (volunteerRole == "limited_collector")
            {
                iss >> cooldown >> maxOrders;
                volunteers.push_back(new LimitedCollectorVolunteer(volunteerCounter, name, cooldown, maxOrders));
            }
            else if (volunteerRole == "driver")
            {
                iss >> maxDistance >> distancePerStep;
                volunteers.push_back(new DriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep));
            }
            else if (volunteerRole == "limited_driver")
            {
                iss >> maxDistance >> distancePerStep >> maxOrders;
                volunteers.push_back(new LimitedDriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep, maxOrders));
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

Customer& WareHouse::getCustomer(int customerId) const{ 
    if (customerId == -1 || customerId>=(customers.size()))
		return *dummy_Customer;
	return *customers[customerId];
}

Volunteer &WareHouse::getVolunteer(int volunteerId) const{
    if (volunteerId == -1 || volunteerId>=volunteers.size())
		return dummy_Volunteer;
	return *volunteers[volunteerId];
}

Order &WareHouse::getOrder(int orderId) const{
	if(orderId==-1 || orderId>=orderCounter){
		return *dummy_Order;
	}
	else{
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
	}
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

void WareHouse::deleteAll(){ 
	for(auto action : actionsLog)
		delete action;
	actionsLog.clear();
	for(auto volunteer : volunteers)
		delete volunteer;
	volunteers.clear();	
	for(auto pendingOrder : pendingOrders)
		delete pendingOrder;
	pendingOrders.clear();	
	for(auto inProcessOrder : inProcessOrders)
		delete inProcessOrder;
	inProcessOrders.clear();	
	for(auto completedOrder : completedOrders)
		delete completedOrder;
	completedOrders.clear();	
	for(auto customer : customers)	
		delete customer;
	customers.clear();	
}

void WareHouse::deleteInProcessOrder(int orderid){ //chaged to iterator
    int counter=0;
    for(vector<Order*>::iterator it = pendingOrders.begin(); it != pendingOrders.end(); it++){
        Order *order = *it;
        if (order->getId() == orderid){
            delete order;
            pendingOrders.erase(pendingOrders.begin() + counter);// Move the order from pendingOrders to inProcessOrders
        }
        counter++;
    }
}

void WareHouse::deleteVolunteer(int volunteerId){ //chaged to iterator
    int counter=0;
    for(vector<Volunteer*>::iterator it = volunteers.begin(); it != volunteers.end(); it++){
        Volunteer *volunteer = *it;
        if (volunteer->getId() == volunteerId){
            delete volunteer;
            volunteers.erase(volunteers.begin() + counter);// Move the order from pendingOrders to inProcessOrders
        }
        counter++;
    }
}

//Rule of 5
 WareHouse::~WareHouse(){ //destructor
    deleteAll();
 }

WareHouse::WareHouse(const WareHouse &other){
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

		deleteAll();

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

WareHouse::WareHouse(WareHouse &&other)
    : isOpen(other.isOpen), 
      actionsLog(std::move(other.actionsLog)),
      volunteers(std::move(other.volunteers)), 
      pendingOrders(std::move(other.pendingOrders)), 
      inProcessOrders(std::move(other.inProcessOrders)), 
      completedOrders(std::move(other.completedOrders)), 
      customers(std::move(other.customers)) { //move constructorr
    customerCounter = other.customerCounter;
    volunteerCounter = other.volunteerCounter;
    orderCounter = other.orderCounter;
    
} //need to delete the other warehouse!