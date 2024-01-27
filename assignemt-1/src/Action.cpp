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
        AddCustomer* newCcus= new AddCustomer(customerName, "Civilian",  distance,  maxOrders);
        newCcus->customerId=customerId; //מה הכוונה?
        return newCcus;
    }
    AddCustomer* newScus= new AddCustomer(customerName, "Soldier",  distance,  maxOrders);
    newScus->customerId=customerId; //מה הכוונה?
    return newScus;
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
        cout<<"customerId: "+ customerId <<endl;
        vector<Order*> orderList = wareHouse.getCustomer(customerId)->getOrderList();
        for(int i=0; i < orderList.size(); i++){
            cout<< "orderId: " + (*orderList[i]).getId() <<endl;
			cout<< "orderStatus: " + OrderStatus((*orderList[i]).getStatus()) <<endl;
        }
        if(wareHouse.getCustomer(customerId).canMakeOrder())
            cout<<"\n num order left: " + wareHouse.getCustomer(customerId).getMaxOrders() - wareHouse.getCustomer(customerId).getNumOrders() +"\n";
        else cout<<"\n num order left: 0 ";
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
PrintVolunteerStatus :: PrintVolunteerStatus(int id) : volunteerId(id){} //constructor

void PrintVolunteerStatus::act(WareHouse &wareHouse){
	
	Volunteer &volunteer = wareHouse.getVolunteer(volunteerId);

	if(volunteer.getId() == -1){
		error("Volunteer doesnt exist");
	}
	else {
		cout << "VolunteerID:" << std::to_string(volunteerId) << endl; 
		cout << "isBusy: " << volunteer.isBusy() << endl; //check if bool can be printed

		if (volunteer.isBusy() == false){ //if the volunteer is not busy
			cout << "OrderID: None" << endl; 
			cout << "timeLeft: None" << endl; 
			cout << "ordersLeft: None" << endl; 
		}
		else if (volunteer.isCollector()){ //if the volunteer is a collector
			CollectorVolunteer &collector = dynamic_cast<CollectorVolunteer&>(volunteer);
			cout << "OrderID: " + std::to_string(volunteer.getActiveOrderId())<< endl; //לודא שאכן מתעגכן האקטיב
			cout << "timeLeft: " + std::to_string(collector.getTimeLeft()) << endl; 
			if(collector.isLimited()){
				LimitedCollectorVolunteer &limitedCollector = dynamic_cast<LimitedCollectorVolunteer&>(volunteer);
				cout << "ordersLeft: " + std::to_string(limitedCollector.getMaxOrders()) << endl;
			}
		}	
		else { //if the volunteer is a driver
			DriverVolunteer &driver = dynamic_cast<DriverVolunteer&>(volunteer);
			cout << "OrderID: " + std::to_string(volunteer.getActiveOrderId())<< endl;
			cout << "distanceLeft: " + std::to_string(driver.getDistanceLeft()) << endl; 
			if (driver.isLimited()){
				LimitedDriverVolunteer &limitedDriver = dynamic_cast<LimitedDriverVolunteer&>(volunteer);
				cout << "ordersLeft: " + std::to_string(limitedDriver.getMaxOrders()) << endl;
				}
		}
		complete();
	}
    wareHouse.addAction(this);
}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const{
	return new PrintVolunteerStatus(volunteerId);
}

string PrintVolunteerStatus::toString() const{
	string stringStatus = ActionStatusToString(getStatus());
	return "PrintVolunteerStatus" + std::to_string(volunteerId) + stringStatus;
}

//PrintActionsLog
PrintActionsLog::PrintActionsLog(){}//constructor

void PrintActionsLog::act(WareHouse &wareHouse){
	for(int i=0; i<wareHouse.getActions().size(); i++)
		cout<< (wareHouse.getActions()[i])->toString()<<endl; 
	complete();
    wareHouse.addAction(this);
} 

PrintActionsLog *PrintActionsLog::clone() const{
	return new PrintActionsLog();
}

string PrintActionsLog::toString() const{
	string stringStatus = ActionStatusToString(getStatus());
	return "log" + stringStatus; 
}


//close
close::close(){}//constructor

void close::act(warehouse &wareHouse){ 
	wareHouse.close();
	//print all actionlog
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
    if(backup!=wareHouse){
        delete backup;
        backup = wareHouse;
    }
}//copy assignment operator

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

