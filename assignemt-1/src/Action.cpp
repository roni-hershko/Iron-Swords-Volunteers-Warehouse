#include "../include/Action.h"
#include <string>
#include <vector>
#include "../include/WareHouse.h"
#include "../include/Order.h"
#include "../include/Volunteer.h"
#include "../include/Customer.h"
#include "../include/Order.h"
using std::string;
using std::vector;

#include <iostream>
using namespace std;

//base action
BaseAction::BaseAction():errorMsg("<error_msg>"),status(ActionStatus::ERROR){}

ActionStatus BaseAction::getStatus() const{
	return status;
}
string BaseAction::ActionStatusToString(ActionStatus status) const{
	if(status==ActionStatus::COMPLETED)
		return "COMPLETED";
	return "ERROR";
}

void BaseAction::complete(){
    status=ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg){
    status=ActionStatus::ERROR;
    this->errorMsg=errorMsg;
}//change status to error, and print

string BaseAction::getErrorMsg() const{ 
    return errorMsg;
} 


//simulate step
SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps){} //constructor

void SimulateStep::act(WareHouse &wareHouse){
	for(int i=0; i<numOfSteps; i++){
		wareHouse.makeTheStep();
	}
	wareHouse.addAction(this);
    complete();
}

SimulateStep *SimulateStep::makeTheStep(WareHouse &wareHouse){

} 	

SimulateStep *SimulateStep::clone() const{
	return new SimulateStep(numOfSteps);
}

string SimulateStep::toString() const{
	return "SimulateStep " + std::to_string(numOfSteps) + status;
}


//AddOrder
AddOrder::AddOrder(int id) : customerId(id){}

void AddOrder::act(WareHouse &wareHouse){
	Customer &customer = wareHouse.getCustomer(customerId);
    if(customer.getId() == -1)
        error("Customer doesn't exist");
    
    else if (customer.addOrder(customerId)==-1)
        error("cannot place this order");
    else {
        Order* order = new Order(wareHouse.getOrderCounter(), customerId, (wareHouse.getCustomer(customerId)).getCustomerDistance());
		wareHouse.addOrder(order);
        complete();
    }
    wareHouse.addAction(this);
} //add order to warehouse, maybe error

AddOrder *AddOrder::clone() const{
    return new AddOrder(customerId);
}

string AddOrder::toString() const{ 
	string stringStatus= ActionStatusToString(getStatus());
	return "Order " + std::to_string(customerId) + stringStatus;
}


//add customer
AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders) :
 customerName(customerName),customerType(StringToCustomerType(customerType)), distance(distance), maxOrders(maxOrders) {} //constructor    

void AddCustomer::act(WareHouse &wareHouse){
    int customerId=wareHouse.getCustomerCounter();
    if(CustomerType(customerType)==CustomerType::Civilian){
        wareHouse.addCustomer(new CivilianCustomer(customerId ,customerName, distance, maxOrders));
    }
    else {
        wareHouse.addCustomer(new SoldierCustomer (customerId,customerName, distance, maxOrders));
    }
	complete();   
    wareHouse.addAction(this);
} //add customer to warehouse, never error

AddCustomer *AddCustomer::clone() const{
    if(customerType == CustomerType::Civilian){
       return new AddCustomer(customerName, "Civilian",  distance,  maxOrders);
    }
    return new AddCustomer(customerName, "Soldier",  distance,  maxOrders);
}

string AddCustomer::toString() const{
	string stringStatus= ActionStatusToString(getStatus());

    if(CustomerType(customerType)==CustomerType::Civilian){  
        return "AddCustomer " + customerName + ", Civilian, distance:" + std::to_string(distance) + ", maxOrder: " + std::to_string(maxOrders) + stringStatus;
    }
    return "Customer: " + customerName + ", Soldier, distance:" + std::to_string(distance) + ", maxOrder: " + std::to_string(maxOrders) + stringStatus;
}

CustomerType AddCustomer::StringToCustomerType(string customerType){ 
	if(customerType=="Civilian")
		return CustomerType::Civilian;
	return CustomerType::Soldier;
}


//PrintOrderStatus
PrintOrderStatus::PrintOrderStatus(int id) : orderId(id){}//constructor

void PrintOrderStatus::act(WareHouse &wareHouse){
	Order &order = wareHouse.getOrder(orderId);
    if(order.getId() == -1)
        error("Order doesn't exist");
    else{
        cout << "OrderId: " + std::to_string(order.getId()) << endl;
		cout << "OrderStatus: " + OrderStatusToString(order.getStatus()) << endl; 
        cout << "CustomerId: " + order.getCustomerId() << endl; 
        cout << "Collector: " + order.getCollectorId() << endl;
        cout << "Driver: " + order.getDriverId()<< endl;
    }
    complete();
    wareHouse.addAction(this);
} //print order status, maybe error 

PrintOrderStatus *PrintOrderStatus::clone() const{
    return new PrintOrderStatus(orderId);
}

string PrintOrderStatus::toString() const{ 
	string stringStatus= ActionStatusToString(getStatus());
    return "OrderStatus " + std::to_string(orderId) + stringStatus;
}   

string OrderStatusToString(OrderStatus status){
	switch(status){
		case OrderStatus::PENDING:
			return "PENDING";
		case OrderStatus::COLLECTING:
			return "COLLECTING";
		case OrderStatus::DELIVERING:
			return "DELIVERING";
		case OrderStatus::COMPLETED:
			return "COMPLETED";
	}
	return "ERROR";
}
 

//PrintCustomerStatus
PrintCustomerStatus::PrintCustomerStatus(int customerId) : customerId(customerId){}//constructor

void PrintCustomerStatus::act(WareHouse &wareHouse){
	Customer &customer = wareHouse.getCustomer(customerId);
    if(customer.getId() == -1){
        error("Customer doesn't exist");
    }
    else{
        cout<<"CustomerID: "+ customerId <<endl;
        vector<Order*> orderList = wareHouse.getCustomer(customerId).getOrdersIds();
        for(int i=0; i < wareHouse.getCustomer(customerId).getNumOrders(); i++){
            cout<< "orderID: <" + to_string((*orderList[i]).getId()) +"> /n"<<endl;
			cout<< "orderStatus: " + (*orderList[i]).OrderStatusToString()+ "/n" <<endl;
        }
        if(wareHouse.getCustomer(customerId).canMakeOrder())
            cout<<"num order left: " + to_string(wareHouse.getCustomer(customerId).getMaxOrders() - wareHouse.getCustomer(customerId).getNumOrders()) +"\n";
        else cout<<"num order left: 0 ";
        complete();
    } 
    wareHouse.addAction(this);
}

PrintCustomerStatus *PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus(customerId);
}

string PrintCustomerStatus::toString() const{ 
	string stringStatus= ActionStatusToString(getStatus());
	return "CustomerStatus " + std::to_string(customerId) + stringStatus;
}

         
//PrintVolunteerStatus
PrintVolunteerStatus :: PrintVolunteerStatus(int id) : volunteerId(id){}//constructor

void PrintVolunteerStatus ::act(WareHouse &wareHouse){
	if(wareHouse.getVolunteer(VolunteerId)==nullptr){
		error("Volunteer doesnt exist");
		error();
	}
	else {
		cout << "VolunteerID:" << VolunteerId << endl; //print volunteer id	
		cout << "isBusy: " wareHouse.getVolunteer(VolunteerId)->isBusy()<< endl; //print is busy

		if (wareHouse.getVolunteer(VolunteerId)->isBusy() == false){ //if the volunteer is not busy
			cout << "OrderID: None" << endl; 
			cout << "timeLeft: None" << endl; 
			cout << "ordersLeft: None" << endl; 
		}
		else if (wareHouse.getVolunteer(VolunteerId).isCollector()){ //if the volunteer is a collector
			cout << "OrderID:" wareHouse.getVolunteer(VolunteerId).getOrder().getId()<< endl;
			cout << "timeLeft:" << wareHouse.getVolunteer(VolunteerId).getTimeLeft() << endl; 
			if(wareHouse.getVolunteer(VolunteerId).getDistanceLeft() != nullptr)
				cout << "ordersLeft:" << wareHouse.getVolunteer(VolunteerId).getDistanceLeft() << endl;
		}
		else { //if the volunteer is a driver
			cout << "OrderID:" wareHouse.getVolunteer(VolunteerId).getOrder().getId()<< endl;
			cout << "distanceLeft:" << wareHouse.getVolunteer(VolunteerId).getDistanceLeft() << endl; 
			if (wareHouse.getVolunteer(VolunteerId).getNumOrdersLeft() != nullptr)
				cout << "ordersLeft:" << wareHouse.getVolunteer(VolunteerId).getNumOrdersLeft() << endl;
		}
		complete();
	}
    wareHouse.actionLog.addAction(this);
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const{
	return new PrintVolunteerStatus(VolunteerId);
}

string PrintVolunteerStatus::toString() const{
	return "PrintVolunteerStatus" + VolunteerId + status;
}


//PrintActionsLog
PrintActionsLog::PrintActionsLog(){}//constructor

void PrintActionsLog::act(WareHouse &wareHouse){
	for(int i=0; i<wareHouse.getActions().size(); i++)
		cout<< wareHouse.getActions()[i].toString()"\n"; 
	complete();
    wareHouse.actionLog.addAction(this);
} 

PrintActionsLog *PrintActionsLog::clone() const{
	return new PrintActionsLog();
}

string PrintActionsLog::toString() const{
	return "log" + status; 
}


//close
close::close(){}//constructor

void close::printAll(){

    for (auto order : pendingOrders)
    {
        cout << to_string(order->getId())+" "+ to_string(order->getCustomerId())+" pendeing" << endl;
    }
    cout << "/n" << endl;
    for (auto order : inProcessOrders)
    {
        cout << to_string(order->getId())+" "+ to_string(order->getCustomerId())+" inProcess" << endl;
    }
    cout << "/n" << endl;
    for (auto order : completedOrders)
    {
        cout << to_string(order->getId())+" "+ to_string(order->getCustomerId())+" complete" << endl;
    }
}

void close::act(warehouse &wareHouse){ 
    printAll();
	wareHouse.close();
}

close *close::clone() const{
	return new close();
}

string close::toString() const{
	return "close";
}


//backup warehouse
BackupWareHouse::BackupWareHouse(){}//constructor

void BackupWareHouse::act(WareHouse &wareHouse){
    Backup.deleteAll();
    Backup= new WareHouse(wareHouse);
}//copy constructor

BackupWareHouse *BackupWareHouse::clone() const {
	return new BackupWareHouse();
}

string BackupWareHouse::toString() const {
	return "BackupWareHouse";
}


//restore warehouse
RestoreWareHouse::RestoreWareHouse(){}//constructor

void RestoreWareHouse::act(WareHouse &wareHouse){
if(backup == nullptr)
	error("No backup available");
else
	wareHouse = backup; //move assignment opertor
}

RestoreWareHouse *RestoreWareHouse::clone() const{
	return new RestoreWareHouse();
}

string RestoreWareHouse::toString() const{
	return "RestoreWareHouse";
}

